#include "keys.h"

bool check_xor(int bw, input_check_pack *x0, input_check_pack *x1, input_check_pack_2 *x2) {
    bool out = 1;
    if (x0->index != x2->index[0] || x1->index != x2->index[1]) {
        return 0;
    }
    else if (x0->payload != x2->payload[0] || x1->payload != x2->payload[1]) {
        return 0;
    }
    else if (x0->size == x1->size && x1->size == x2->size) {
        
        for(size_t i=0; i<x1->size; i++) {
            uint8_t rj = ((x0->index + x1->index).value >> (x0->size - 1 - i)) & 1;
            out &= eq(x0->zs[0][i], x2->zs0[0][i]);
            out &= eq(x1->zs[0][i], x2->zs1[0][i]);
            out &= eq(x0->zs[1][i], x2->zs0[1][i]);
            out &= eq(x1->zs[1][i], x2->zs1[1][i]);
            
            out &= eq(x0->sigma[i], x2->sigma[i]);
            out &= eq(x1->sigma[i], x2->sigma[i]);
            out &= eq(x2->sigma[i], x0->zs[1^rj][i] ^ x1->zs[1^rj][i]);
            //Check for correctness of tau
            out &= (x0->zt[0][i] == x2->zt0[0][i]);

            out &= (x1->zt[0][i] == x2->zt1[0][i]);
            out &= (x0->zt[1][i] == x2->zt0[1][i]);
            out &= (x1->zt[1][i] == x2->zt1[1][i]);
            

            out &= (x0->tau[0][i] == x2->tau[0][i]);
            out &= (x0->tau[1][i] == x2->tau[1][i]);
            out &= (x1->tau[0][i] == x2->tau[0][i]);
            out &= (x1->tau[1][i] == x2->tau[1][i]);
            out &= (x2->tau[0][i] == (x0->zt[0][i] ^ x1->zt[0][i] ^ rj ^ 1));
            out &= (x2->tau[1][i] == (x0->zt[1][i] ^ x1->zt[1][i] ^ rj));

        }
    }
    else {return 0;}

    //Final Correction word
    if(x0->T == x2->T[0] || x1->T == x2->T[1]) {
        GroupElement beta[2][2] = {{GroupElement(0, bw), x0->payload}, {GroupElement(1, bw), x1->payload}};
        // std::cout<<out<<"\n";
        out &= (x0->W[0] == x2->W[0][0]);
        out &= (x1->W[0] == x2->W[1][0]);
        out &= (x0->W[1] == x2->W[0][1]);
        out &= (x1->W[1] == x2->W[1][1]);
        out &= (x0->gamma[0] == x2->gamma[0]);
        out &= (x1->gamma[0] == x2->gamma[0]);
        out &= (x0->gamma[1] == x2->gamma[1]);
        out &= (x1->gamma[1] == x2->gamma[1]);
        uint8_t t1 = (x0->T < x1->T)?1:0;
        // std::cout<<out<<"\n";
        std::cout<<"gamma 0 = "<<x0->gamma[0]<<" "<<"gamma 1 = "<<x0->gamma[1]<<"\n";
        if(t1) {
            out &= (x0->gamma[0] == (x0->W[0] - beta[0][0] -beta[1][0] - x1->W[0]));
            out &= (x0->gamma[1] == (x0->W[1] - beta[0][1] - beta[1][1] - x1->W[1]));
        }
        else {
            // std::cout<<"here\n";
            out &= (x0->gamma[0] == (beta[0][0] + beta[1][0] - x0->W[0] + x1->W[0]));
            out &= (x0->gamma[1] == (beta[0][1] + beta[1][1] - x0->W[1] + x1->W[1]));
        }
        std::cout<<out<<"\n";
    }
    else {return 0;}

    return out;
}