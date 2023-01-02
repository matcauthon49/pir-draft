#include "dpf.h"

using namespace osuCrypto;

void convert(const int inp_bitwidth, 
             const int out_bitwidth, 
             const int no_of_group_elements, 
             const block inp_block, 
             GroupElement *out_group_element)
             {};

void dpf_prg();

std::pair<dpf_key, dpf_key> dpf_keygen(int height, 
                                       const int group_bitwidth, 
                                       int no_of_group_elements,
                                       GroupElement *alpha, 
                                       GroupElement *index)
{
    static const block notOneBlock = toBlock(~0, ~1);
    static const block notThreeBlock = toBlock(~0, ~3);
    static const block TwoBlock = toBlock(0, 2);
    static const block ThreeBlock = toBlock(0, 3);

    // set AES plaintext
    const static block pt[4] = {ZeroBlock, OneBlock, TwoBlock, ThreeBlock};

    // sample hat{S}_i^{0,0}, the PRG keys
    auto keys = prng.get<std::array<block, 2>>();

    // set beta
    GroupElement beta[2][2] = {{0, alpha[0]}, {1, alpha[1]}};

    // initiate outputs
    block s[2];
    s[0] = keys[0];
    s[1] = keys[1];

    block t[2] = {
        toBlock(0, 0), 
        toBlock(0, 1)
    };

    block sigma[height];
    block tau0[height];
    block tau1[height];

    // set AES ciphertext
    block ct[4];

    for (int i = 0; i < height; i++) {

        // set z box
        block z[2][4];

    }
};