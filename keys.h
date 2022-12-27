#pragma once

#include "deps/cryptoTools/cryptoTools/Common/Defines.h"
#include "deps/EzPC/group_element.h"

using namespace osuCrypto;

struct DPFKey {
    int height, Bout, groupSize;
    block *s;
    block *t;
    block *sigma;
    block *tau1;
    block *tau2;
    GroupElement *gamma;

    DPFKey() {}
    DPFKey(int height, int Bout, int groupSize,
           block *s,
           block *t,
           block *sigma,
           block *tau[2],
           GroupElement *gamma) : 
                height(height), 
                Bout(Bout), 
                groupSize(groupSize), 
                s(s), 
                t(t), 
                sigma(sigma), 
                tau1(tau1), 
                tau2(tau2) 
            {};
};

inline void freeDPFkey(DPFKey &key){
    delete[] key.s;
    delete[] key.t;
    delete[] key.sigma;
    delete[] key.tau1;
    delete[] key.tau2;
    delete[] key.gamma;
}