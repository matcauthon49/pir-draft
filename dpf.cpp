#include "dpf.h"

using namespace osuCrypto;

void convert(const int inp_bitwidth, const int out_bitwidth, const int no_of_group_elements, 
             const block inp_block, GroupElement *out_group_element){};

void prg(const int inp_bitwidth, const block inp_block, block *out_block) {};

std::pair<dpf_key, dpf_key> dpf_keygen(int height, const int grou_bitwidth, int no_of_group_elements,
                                    GroupElement *payload, GroupElement *index)
{
    // sample hat{S}_i^{0,0}
    auto s = prng.get<std::array<block, 2>>();

    // set t and beta
    block t[2] = {toBlock(0, 0), toBlock(0, 1)};
    GroupElement m_payload[2][2] = {{0, payload[1]}, {1, payload[2]}};

    // initiate outputs
    block sigma[height];
    block tau0[height];
    block tau1[height];

    for (int i = 0; i < height; i++) {


    }
};