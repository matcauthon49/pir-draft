#include "keys.h"

bool check_xor(xor_check_pack *x0, xor_check_pack *x1, xor_check_pack_2 *x2, GroupElement index0, GroupElement index1) {
    if (x0->size == x1->size && x1->size == x2->size) {
        bool out = 1;
        for (int i = 0; i < x0->size; i++){
            uint8_t rj0 = (index0.value >> (x0->size - 1 - i)) & 1;
            uint8_t rj1 = (index1.value >> (x1->size - 1 - i)) & 1;

            out &= eq(x0->z[0][i], x2->z0[0][i]);
            out &= eq(x1->z[0][i], x2->z1[0][i]);
            out &= eq(x0->z[1][i], x2->z0[1][i]);
            out &= eq(x1->z[1][i], x2->z1[1][i]);

            out &= eq(x0->sigma[i], x1->sigma[i]);
            out &= eq(x0->sigma[i], x0->z[rj0][i] ^ x1->z[rj1][i]);
        }
        return out;
    }
    else {return 0;}
}