#pragma once
#include <array>
#include <vector>
#include <utility>
#include <cryptoTools/Common/Defines.h>
#include <cryptoTools/Crypto/AES.h>
#include <cryptoTools/Crypto/PRNG.h>
#include <cryptoTools/gsl/span>
#include <group_element.h>
#include <keys.h>
#include <omp.h>

using namespace osuCrypto;
extern int Bin;
extern int Bout;
struct dpf_layer {
    size_t size;
    size_t level;
    block *nodes;
    uint8_t *prevt;
    uint8_t *currt;
    block zs[2];
    uint8_t zt[2];
};

struct dpf_input_pack {
    GroupElement *alpha;
    GroupElement index;
    block *hats;
    block *sigma;
    uint8_t *hatt;
    uint8_t *tau[2];
};

void clearAESevals();
inline u8 lsb(const block &b)
{
    return _mm_cvtsi128_si64x(b) & 1;
}

GroupElement* convert(const int out_bitwidth, const int no_of_group_elements, const block &inp_block);

std::pair<dpf_key, dpf_key> dpf_keygen(int height, int Bout, dpf_input_pack **dpfip, input_check_pack_2 *ip2);

GroupElement* dpf_eval(int party, GroupElement idx, const dpf_key &key);

GroupElement** dpf_eval_all(int party, const dpf_key &key, input_check_pack *icp);

void free_dpf_layer(dpf_layer* dpfl);
void free_dpf_input_pack(dpf_input_pack *dpfip);
void free_dpf_input_pack(dpf_input_pack dpfip);

void prg_eval_all_and_xor(dpf_layer *dpfl, block* keynodes);
