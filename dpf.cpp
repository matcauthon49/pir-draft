#include "dpf.h"

using namespace osuCrypto;

struct dpf_layer {
    size_t size;
    size_t level;
    block *nodes;
    block z[4];
};

void free_dpf_layer(dpf_layer *dpfl) {
    free(dpfl->nodes);
    free(dpfl->z);
    free(dpfl);
};

void convert(const int inp_bitwidth, 
             const int out_bitwidth, 
             const int no_of_group_elements, 
             const block inp_block, 
             GroupElement *out_group_element)
             {};

void prg_eval_all_and_xor(dpf_layer *dpfl, block *ct, const block *pt) {

    block* output_nodes = (block*)malloc(4*sizeof(block));

    if (dpfl->level==0) {
        dpfl->z[0] = ZeroBlock;
        dpfl->z[1] = ZeroBlock;
        dpfl->z[2] = ZeroBlock;
        dpfl->z[3] = ZeroBlock;
    }

    #pragma omp parallel for
    for (int i = 0; i < (dpfl->size); i++) {

        // set key
        block k = dpfl->nodes[i];
        AES aes_key(k);

        // encrypt
        aes_key.ecbEncFourBlocks(pt, ct);
        output_nodes[4*i] = ct[0];
        output_nodes[4*i+1] = toBlock(0,lsb(ct[1]));
        output_nodes[4*i+2] = ct[2];
        output_nodes[4*i+3] = toBlock(0,lsb(ct[3]));

        // set xor
        dpfl->z[0] = dpfl->z[0] ^ ct[0];
        dpfl->z[1] = dpfl->z[1] ^ toBlock(0,lsb(ct[1]));
        dpfl->z[2] = dpfl->z[2] ^ ct[2];
        dpfl->z[3] = dpfl->z[3] ^ toBlock(0,lsb(ct[3]));
    }

    free(dpfl->nodes);
    dpfl->nodes = output_nodes;

    dpfl->size *= 4;
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

#ifdef DPF_PROFILE
    printf("START DPF LEVEL 0 %d\n", current_timestamp());
#endif

    // set dpf_layers
    dpf_layer *dpfl0;
    dpf_layer *dpfl1;

    dpfl0->size = 1;
    dpfl0->level = 0;
    dpfl0->nodes = (block*)malloc(sizeof(block));
        dpfl0 -> nodes[0] = s[0];

    dpfl1->size = 1;
    dpfl1->level = 0;
    dpfl1->nodes = (block*)malloc(sizeof(block));
        dpfl1 -> nodes[0] = s[1];


    for (int i = 0; i < height; i++) {

#ifdef DPF_PROFILE
    printf("START DPF LEVEL %d%d\n", dpfl0->level, current_timestamp());
#endif
        prg_eval_all_and_xor(dpfl0, ct, pt);
        prg_eval_all_and_xor(dpfl1, ct, pt);

        const uint8_t sig = static_cast<uint8_t>(index->value >> (group_bitwidth - 1 - i)) & 1;

        sigma[i] = toBlock(0,0);

    }
};