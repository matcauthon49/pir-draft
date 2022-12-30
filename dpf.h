#pragma once
#include <array>
#include <vector>
#include <utility>
#include <cryptoTools/Common/Defines.h>
#include <cryptoTools/Crypto/AES.h>
#include <cryptoTools/Crypto/PRNG.h>
#include <cryptoTools/gsl/span>
#include "deps/EzPC/group_element.h"
#include "keys.h"

using namespace osuCrypto;

void clearAESevals();
inline u8 lsb(const block &b)
{
    return _mm_cvtsi128_si64x(b) & 1;
}

std::pair<dpf_key, dpf_key> dpf_keygen(int height, int Bout, int groupSize,
                        GroupElement *ran, GroupElement idx);

void dpf_eval(int party, GroupElement *ran, GroupElement idx, const dpf_key &key);