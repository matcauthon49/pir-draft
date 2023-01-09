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

struct dpf_input_pack {
    GroupElement *alpha;
    GroupElement index;
    block *hats;
    block *sigma;
    uint8_t *hatt;
    uint8_t *tau[2];
};

void free_dpf_input_pack(dpf_input_pack *dpfip) {
    free(dpfip->alpha);
    free(dpfip->sigma);
    free(dpfip->tau);
    free(dpfip->hats);
    free(dpfip->hats);
    free(dpfip);
};

inline int bytesize(const int bitsize) {
    return (bitsize % 8) == 0 ? bitsize / 8 : (bitsize / 8)  + 1;
}

uint64_t *convert_elem(const int inp_bitwidth, 
             const int out_bitwidth, 
             const int no_of_group_elements, 
             const block inp_block)
{
    uint64_t out[no_of_group_elements];

    const int bys = bytesize(inp_bitwidth);
    const int totalBys = bys * no_of_group_elements;

    if (bys * no_of_group_elements <= 16) {
        uint8_t *bptr = (uint8_t *)&inp_block;
        for(int i = 0; i < no_of_group_elements; i++) {
            out[i] = *(uint64_t *)(bptr + i * bys);
        }
    }

    else {
        int numblocks = totalBys % 16 == 0 ? totalBys / 16 : (totalBys / 16) + 1;

        AES aes(inp_block);
        block pt[numblocks];
        block ct[numblocks];

        for(int i = 0; i < numblocks; i++) {
            pt[i] = toBlock(0, i);
        }

        aes.ecbEncBlocks(pt, numblocks, ct);
        uint8_t *bptr = (uint8_t *)ct;

        for(int i = 0; i < no_of_group_elements; i++) {
            out[i] = *(uint64_t *)(bptr + i * bys);
        }
    }
};

void convert(const int inp_bitwidth, 
             const int out_bitwidth,
             const int size,
             const int no_of_group_elements, 
             const block *inp,
             uint64_t **out)
{
    #pragma omp parallel for
     for (int i = 0; i < size; i++) {
        out[i] = convert_elem(inp_bitwidth, out_bitwidth, no_of_group_elements, inp[i]);
     }
};

void prg_eval_all_and_xor(dpf_layer *dpfl, block *keynodes, block *ct, const block *pt) {

    block* output_nodes = (block*)malloc(2*dpfl->size*sizeof(block));
    uint8_t* output_t = (uint8_t*)malloc(2*dpfl->size*sizeof(uint8_t));

        dpfl->zs[0] = ZeroBlock;
        dpfl->zs[1] = ZeroBlock;
        dpfl->zt[0] = 0;
        dpfl->zt[1] = 0;

    #pragma omp parallel for
    for (int i = 0; i < (dpfl->size); i++) {

        // set key
        block k = keynodes[i];
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

    free(dpfl->currt);
    dpfl->currt = output_t;

    dpfl->size *= 2;
    dpfl->level += 1;
};

std::pair<dpf_key, dpf_key> dpf_keygen(int height, 
                                       const int group_bitwidth,
                                       dpf_input_pack *dpfip0, 
                                       dpf_input_pack *dpfip1)
{
    // sample hat{S}_i^{0,0}, the PRG keys
    auto keys = prng.get<std::array<block, 2>>();

    // set beta
    GroupElement beta[2][2] = {{0, *dpfip0->alpha}, {1, *dpfip1->alpha}};

    // initiate outputs
    const static block s[2] = {keys[0], keys[1]};

    block t[2] = {
        toBlock(0, 0), 
        toBlock(0, 1)
    };

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
        prg_eval_all_and_xor(dpfl0, dpfip0->hats, ct, pt);
        prg_eval_all_and_xor(dpfl1, dpfip1->hats, ct, pt);

        const uint8_t sig = static_cast<uint8_t>((dpfip0->index + dpfip1->index).value >> (group_bitwidth - 1 - i)) & 1;
        const uint8_t sig0 = static_cast<uint8_t>(dpfip0->index.value >> (group_bitwidth - 1 - i)) & 1;
        const uint8_t sig1 = static_cast<uint8_t>(dpfip1->index.value >> (group_bitwidth - 1 - i)) & 1;

        dpfip0->sigma[i] = dpfl0->zs[1^sig] ^ dpfl1->zs[1^sig];
        dpfip1->sigma[i] = dpfl0->zs[1^sig] ^ dpfl1->zs[1^sig];

        dpfip0->tau[0][i] = dpfl0->zt[0] ^ sig0 ^ dpfl1->zt[0] ^ sig1 ^ 1;
        dpfip1->tau[0][i] = dpfl0->zt[0] ^ sig0 ^ dpfl1->zt[0] ^ sig1 ^ 1;

        dpfip0->tau[1][i] = dpfl0->zt[1] ^ sig1 ^ dpfl1->zt[1] ^ sig1;
        dpfip1->tau[1][i] = dpfl0->zt[1] ^ sig1 ^ dpfl1->zt[1] ^ sig1;

        free(dpfip0->hats);
        free(dpfip1->hats);
        free(dpfip0->hatt);
        free(dpfip1->hatt);

        dpfip0->hats = (block*)malloc(dpfl0->size*sizeof(block));
        dpfip1->hats = (block*)malloc(dpfl1->size*sizeof(block));
        dpfip0->hatt = (uint8_t*)malloc(dpfl0->size*sizeof(uint8_t));
        dpfip1->hatt = (uint8_t*)malloc(dpfl1->size*sizeof(uint8_t));

        #pragma omp parallel for
        for (int j = 0; j < dpfl0->size; j++) {
            dpfip0->hats[j] = (dpfl0->nodes[j] ^ dpfl0->prevt[j/2]) * dpfip0->sigma[i];
            dpfip1->hats[j] = (dpfl1->nodes[j] ^ dpfl1->prevt[j/2]) * dpfip1->sigma[i];
            dpfip0->hatt[j] = (dpfl0->currt[j] ^ dpfl0->prevt[j/2]) * dpfip0->tau[j&1][i];
            dpfip1->hatt[j] = (dpfl1->currt[j] ^ dpfl1->prevt[j/2]) * dpfip1->tau[j&1][i];
        }

        dpfl0->prevt = dpfip0->hatt;
        dpfl1->prevt = dpfip1->hatt;

#ifdef DPF_PROFILE_
    printf("END DPF LEVEL %d%d\n", dpfl0->level, current_timestamp());
#endif

    }

#ifdef DPF_PROFILE_
    printf("DPF START OFFLINE COMPUTATION %d\n", current_timestamp());
#endif

    GroupElement w[2];
};