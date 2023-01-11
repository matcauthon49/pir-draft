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

struct input_check_pack {
    GroupElement index;
    GroupElement payload;
    block init_s;
    block *z[2];
    block *sigma;
    size_t size;
};

inline void free_input_check_pack(input_check_pack &icp) {
    delete[] icp.z[0];
    delete[] icp.z[1];
    delete[] icp.sigma;
}

struct input_check_pack_2 {
    GroupElement *index;
    GroupElement *payload;
    block *init_s;
    block *z0[2];
    block *z1[2];
    block *sigma;
    size_t size;
};

inline void free_input_check_pack_2(input_check_pack_2 &icp) {
    delete[] icp.z0[0];
    delete[] icp.z0[1];
    delete[] icp.z1[0];
    delete[] icp.z1[1];
    delete[] icp.sigma;
    delete[] icp.payload;
    delete[] icp.index;
    delete[] icp.init_s;
}