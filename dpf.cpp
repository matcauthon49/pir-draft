#include "dpf.h"

using namespace osuCrypto;

struct dpf_layer {
    size_t size;
    size_t level;
    block *nodes;
    uint8_t *prevt;
    uint8_t *currt;
    block zs[2];
    uint8_t zt[2];
};

void free_dpf_layer(dpf_layer *dpfl) {
    free(dpfl->nodes);
    free(dpfl->prevt);
    free(dpfl->currt);
    free(dpfl->zs);
    free(dpfl->zt);
    free(dpfl);
};

void convert(const int inp_bitwidth, 
             const int out_bitwidth, 
             const int no_of_group_elements, 
             const block inp_block, 
             GroupElement *out_group_element)
             {};

void prg_eval_all_and_xor(dpf_layer *dpfl, block *ct, const block *pt) {

    block* output_nodes = (block*)malloc(2*dpfl->size*sizeof(block));
    uint8_t* output_t = (uint8_t*)malloc(2*dpfl->size*sizeof(uint8_t));

    if (dpfl->level==0) {
        dpfl->zs[0] = ZeroBlock;
        dpfl->zs[1] = ZeroBlock;
        dpfl->zt[0] = 0;
        dpfl->zt[1] = 0;
    }

    #pragma omp parallel for
    for (int i = 0; i < (dpfl->size); i++) {

        // set key
        block k = dpfl->nodes[i];
        AES aes_key(k);

        // encrypt
        aes_key.ecbEncFourBlocks(pt, ct);
        output_nodes[2*i] = ct[0];
        output_nodes[2*i+1] = ct[2];
        output_t[2*i] = lsb(ct[1]);
        output_t[2*i+1] = lsb(ct[3]);

        // set xor
        dpfl->zs[0] = dpfl->zs[0] ^ ct[0];
        dpfl->zs[1] = dpfl->zs[1] ^ ct[2];
        dpfl->zt[0] = dpfl->zt[0] ^ lsb(ct[1]);
        dpfl->zt[1] = dpfl->zt[1] ^ lsb(ct[3]);
    }

    free(dpfl->nodes);
    dpfl->nodes = output_nodes;

    free(dpfl->prevt);
    dpfl->prevt = dpfl->currt;
    dpfl->currt = output_t;

    dpfl->size *= 2;
    dpfl->level += 1;
};

std::pair<dpf_key, dpf_key> dpf_keygen(int height, 
                                       const int group_bitwidth, 
                                       int no_of_group_elements,
                                       GroupElement *alpha, 
                                       GroupElement *index)
{
    // sample hat{S}_i^{0,0}, the PRG keys
    auto keys = prng.get<std::array<block, 2>>();

    // set beta
    GroupElement beta[2][2] = {{0, alpha[0]}, {1, alpha[1]}};

    // initiate outputs
    const static block s[2] = {keys[0], keys[1]};

    block t[2] = {
        toBlock(0, 0), 
        toBlock(0, 1)
    };

    block sigma[height];
    block tau0[height];
    block tau1[height];

    // set AES Plaintext
    static const block TwoBlock = toBlock(0, 2);
    static const block ThreeBlock = toBlock(0, 3);

    const static block pt[4] = {ZeroBlock, OneBlock, TwoBlock, ThreeBlock};

    // set AES ciphertext
    block ct[4];

#ifdef DPF_PROFILE_
    printf("START DPF LEVEL 0 %d\n", current_timestamp());
#endif

    // set dpf_layers
    dpf_layer *dpfl0;
    dpf_layer *dpfl1;

    dpfl0->size = 1;
    dpfl0->level = 0;
    dpfl0->nodes = (block*)malloc(sizeof(block));
        dpfl0 -> nodes[0] = s[0];
    dpfl0->currt = (uint8_t*)malloc(sizeof(uint8_t));
        dpfl0 -> currt[0] = 0;

    dpfl1->size = 1;
    dpfl1->level = 0;
    dpfl1->nodes = (block*)malloc(sizeof(block));
        dpfl1 -> nodes[0] = s[1];
    dpfl1->currt = (uint8_t*)malloc(sizeof(uint8_t));
        dpfl1 -> currt[0] = 1;


    for (int i = 0; i < height; i++) {

#ifdef DPF_PROFILE_
    printf("START DPF LEVEL %d%d\n", dpfl0->level, current_timestamp());
#endif
        prg_eval_all_and_xor(dpfl0, ct, pt);
        prg_eval_all_and_xor(dpfl1, ct, pt);

        const uint8_t sig = static_cast<uint8_t>((index[0]+index[1]).value >> (group_bitwidth - 1 - i)) & 1 ^ 1;

        sigma[i] = dpfl0->zs[sig] ^ dpfl1->zs[sig];
        tau0[i] = dpfl0->zs[0] ^ toBlock(index[0].value) ^ dpfl1->zs[0] ^ toBlock(index[0].value) ^ toBlock(1);
        tau1[i] = dpfl0->zs[1] ^ toBlock(index[1].value) ^ dpfl1->zs[1] ^ toBlock(index[1].value) ^ toBlock(1);

    }
};