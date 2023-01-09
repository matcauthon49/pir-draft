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
#include<omp.h>

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

void clearAESevals();
inline u8 lsb(const block &b)
{
    return _mm_cvtsi128_si64x(b) & 1;
}

// std::pair<dpf_key, dpf_key> dpf_keygen(int height, int Bout, int groupSize,
                        // GroupElement *ran, GroupElement idx);

// void dpf_eval(int party, GroupElement *ran, GroupElement idx, const dpf_key &key);

void prg_eval_all_and_xor(dpf_layer *dpfl, block* keynodes, block *ct, const block *pt);