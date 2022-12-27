#include "dpf.h"

using namespace osuCrypto;

std::pair<DPFKey, DPFKey> keyGenDPF(int height, int Bout, int groupSize,
                        GroupElement *ran, GroupElement idx) {

    block hats[2][2][height];
    block hatt[2][2][height];

    auto s = prng.get<std::array<block, 2>>();
}