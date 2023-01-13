#pragma once

#include <cryptoTools/Common/Defines.h>
#include <group_element.h>

using namespace osuCrypto;

struct dpf_key {
    int height, Bout, groupSize;
    block s;
    uint8_t t;
    block *sigma;
    uint8_t *tau0;
    uint8_t *tau1;
    GroupElement *gamma;

    dpf_key() {}
    dpf_key(int height, int Bout, int groupSize,
           block s,
           uint8_t t,
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
                tau1(tau1),
                gamma(gamma)
            {};
};

inline void free_dpf_key(dpf_key &key){
    delete[] key.sigma;
    delete[] key.tau0;
    delete[] key.tau1;
    delete[] key.gamma;
}

struct input_check_pack {
    GroupElement index;
    GroupElement payload;
    block *zs[2];
    uint8_t *zt[2];
    block *sigma;
    uint8_t *tau[2];
    size_t size;
    //Final Correction Word
    int T;
    GroupElement W[2];
    GroupElement gamma[2];
};

inline void free_input_check_pack(input_check_pack &icp) {
    delete[] icp.zs[0];
    delete[] icp.zs[1];
    delete[] icp.zt[0];
    delete[] icp.zt[1];
    delete[] icp.sigma;
    delete[] icp.tau[0];
    delete[] icp.tau[1];
}

struct input_check_pack_2 {
    GroupElement *index;
    GroupElement *payload;
    block *zs0[2];
    block *zs1[2];
    uint8_t *zt0[2];
    uint8_t *zt1[2];
    block *sigma;
    uint8_t *tau[2];
    size_t size;
    //Final Correction Word
    int T[2];
    GroupElement W[2][2];
    GroupElement gamma[2];
};

inline void free_input_check_pack_2(input_check_pack_2 &icp) {
    delete[] icp.zs0[0];
    delete[] icp.zs0[1];
    delete[] icp.zs1[0];
    delete[] icp.zs1[1];
    delete[] icp.zt0[0];
    delete[] icp.zt0[1];
    delete[] icp.zt1[0];
    delete[] icp.zt1[1];
    delete[] icp.sigma;
    delete[] icp.tau[0];
    delete[] icp.tau[1];
    delete[] icp.payload;
    delete[] icp.index;
}

bool check_xor(int bw, input_check_pack *x0, input_check_pack *x1, input_check_pack_2 *x2);
