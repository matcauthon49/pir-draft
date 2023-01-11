#pragma once

#include <cryptoTools/Common/Defines.h>
#include <group_element.h>

using namespace osuCrypto;

struct dpf_key {
    int height, Bout, groupSize;
    block *s;
    uint8_t *t;
    block *sigma;
    uint8_t *tau0;
    uint8_t *tau1;
    GroupElement *gamma;

    dpf_key() {}
    dpf_key(int height, int Bout, int groupSize,
           block *s,
           uint8_t *t,
           block *sigma,
           uint8_t *tau0,
           uint8_t *tau1,
           GroupElement *gamma) : 
                height(height), 
                Bout(Bout), 
                groupSize(groupSize), 
                s(s), 
                t(t), 
                sigma(sigma), 
                tau0(tau0), 
                tau1(tau1) 
            {};
};

inline void free_dpf_key(dpf_key &key){
    delete[] key.s;
    delete[] key.t;
    delete[] key.sigma;
    delete[] key.tau0;
    delete[] key.tau1;
    delete[] key.gamma;
}