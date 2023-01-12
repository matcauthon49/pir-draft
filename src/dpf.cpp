#include "dpf.h"
//Divyu: To print statements for debugging
#include<iostream>
#define ELEMENTNO 2
#define INPBW 128
#define OUTBW 64

using namespace osuCrypto;


void free_dpf_layer(dpf_layer *dpfl) {
    free(dpfl->nodes);
    free(dpfl->prevt);
    free(dpfl->currt);
    //Divyu: Made these changes.
    delete dpfl;
};


void free_dpf_input_pack(dpf_input_pack *dpfip) {
    free(dpfip->alpha);
    free(dpfip->sigma);
    free(dpfip->tau);
    free(dpfip->hats);
    free(dpfip->hatt);
    free(dpfip);
};

inline int bytesize(const int bitsize) {
    return (bitsize % 8) == 0 ? bitsize / 8 : (bitsize / 8)  + 1;
}

GroupElement* convert(const int out_bitwidth, const int no_of_group_elements, const block &inp_block) {
    GroupElement* out = (GroupElement*)malloc(no_of_group_elements*sizeof(GroupElement));
    
    const int bys = bytesize(out_bitwidth);
    const int totalbys = bys*no_of_group_elements;
    if(totalbys <= 16) {
        uint8_t *bptr = (uint8_t *)&inp_block;

        for(int i = 0; i < no_of_group_elements; i++) 
            out[i] = GroupElement(*(uint64_t *)(bptr + i * bys), out_bitwidth);
    }
    else {
        int numblocks = totalbys % 16 == 0 ? totalbys / 16 : (totalbys / 16) + 1;

        AES aes(inp_block);
        block pt[numblocks];
        block ct[numblocks];

        for(int i = 0; i < numblocks; i++) {
            pt[i] = toBlock(0, i);
        }

        aes.ecbEncBlocks(pt, numblocks, ct);
        uint8_t *bptr = (uint8_t *)ct;

        for(int i = 0; i < no_of_group_elements; i++) {
            out[i] = GroupElement(*(uint64_t *)(bptr + i * bys), out_bitwidth);
        }
    }

    return out;
};

void convert_parallel(const int out_bitwidth, const int no_of_group_elements, const int no_of_input_blocks, block* inp_block, GroupElement** out) {
    #pragma omp parallel for schedule(static, 1)
    for(int i=0; i<no_of_input_blocks; i++) {
        out[i] = convert(out_bitwidth, no_of_group_elements, inp_block[i]);
    }
};

void prg_eval_all_and_xor(dpf_layer *dpfl, block *keynodes) {

    block* output_nodes = (block*)malloc(2*dpfl->size*sizeof(block));
    uint8_t* output_t = (uint8_t*)malloc(2*dpfl->size*sizeof(uint8_t));

        dpfl->zs[0] = ZeroBlock;
        dpfl->zs[1] = ZeroBlock;
        dpfl->zt[0] = 0;
        dpfl->zt[1] = 0;

    //Parallelizing aes calls
    int num_threads = 4;
    block thread_zs[num_threads][2] = {{ZeroBlock}};
    uint8_t thread_zt[num_threads][2] = {{0}};

    #pragma omp parallel num_threads(4)
    {
        int thread_id = omp_get_thread_num();

        //Divyu: Need local pt and ct for each thread to eliminate race condition.
        // set AES Plaintext
        static const block ZeroBlock = toBlock(0, 0);
        static const block OneBlock = toBlock(0, 1);
        static const block TwoBlock = toBlock(0, 2);
        static const block ThreeBlock = toBlock(0, 3);
        const static block ptt[4] = {ZeroBlock, OneBlock, TwoBlock, ThreeBlock};
        block ctt[4];

        #pragma omp for schedule(static, 1)
        for(size_t i=0; i<dpfl->size; i++) {
            // set key
            block k = keynodes[i];
            AES aes_key(k);

            // encrypt
            aes_key.ecbEncFourBlocks(ptt, ctt);
            output_nodes[2*i] = ctt[0];
            output_nodes[2*i+1] = ctt[2];
            output_t[2*i] = lsb(ctt[1]);
            output_t[2*i+1] = lsb(ctt[3]);

            //set xor
            thread_zs[thread_id][0] = thread_zs[thread_id][0] ^ ctt[0];
            thread_zs[thread_id][1] = thread_zs[thread_id][1] ^ ctt[2];
            thread_zt[thread_id][0] = thread_zt[thread_id][0] ^ lsb(ctt[1]);
            thread_zt[thread_id][1] = thread_zt[thread_id][1] ^ lsb(ctt[3]);

        }
            
    }

    #pragma omp for 
        for(int i=0; i<num_threads; i++) {
                dpfl->zs[0] = dpfl->zs[0] ^ thread_zs[i][0];
                dpfl->zs[1] = dpfl->zs[1] ^ thread_zs[i][1];
                dpfl->zt[0] = dpfl->zt[0] ^ thread_zt[i][0];
                dpfl->zt[1] = dpfl->zt[1] ^ thread_zt[i][1];
            }

            



    
    free(dpfl->nodes);
    dpfl->nodes = output_nodes;
    // free(dpfl->prevt);
    // dpfl->prevt = dpfl->currt;
    free(dpfl->currt);
    dpfl->currt = output_t;

    dpfl->size *= 2;
    dpfl->level += 1;
};



std::pair<dpf_key, dpf_key> dpf_keygen(int height, const int group_bitwidth, dpf_input_pack **dpfip)
{
    // sample hat{S}_i^{0,0}, the PRG keys
    auto keys = prng.get<std::array<block, 2>>();
    // set beta
    GroupElement beta[2][2] = {{GroupElement(0, group_bitwidth), *dpfip[0]->alpha}, {GroupElement(1, group_bitwidth), *dpfip[1]->alpha}};


#ifdef DPF_PROFILE_
    printf("START DPF LEVEL 0 %d\n", current_timestamp());
#endif
    
    //Initialize keys
    dpf_key *key0 = new dpf_key;
    dpf_key *key1 = new dpf_key;
    key0->height = height;
    key1->height = height;
    key0->Bout = group_bitwidth;
    key1->Bout = group_bitwidth;

    // set dpf_layers
    dpf_layer* dpfl[2];
    dpfl[0] = new dpf_layer;
    dpfl[1] = new dpf_layer;

    //Divyu: Initialising dpf layer for P0.
    dpfl[0]->size = 1;
    dpfl[0]->level = 0;
    dpfl[0]->nodes = NULL;
    dpfl[0]->currt = NULL;
    dpfl[0]->prevt = NULL;
    
    //Initialisig root hat(s), hat(t) for P0
    dpfip[0]->hats = (block*)malloc(sizeof(block));
    dpfip[0]->hats[0] = keys[0];
    
    dpfip[0]->hatt = (uint8_t*)malloc(sizeof(uint8_t));
        dpfip[0]->hatt[0] = 0;
    
    key0->s = keys[0];
    key0->t = 0;
    
    //Divyu: Initialising empty sigma, tau[0] and tau[1] for key0.
    dpfip[0]->sigma = (block*)malloc(height*sizeof(block));
    dpfip[0]->tau[0] = (uint8_t*)malloc(height*sizeof(uint8_t));
    dpfip[0]->tau[1] = (uint8_t*)malloc(height*sizeof(uint8_t));
    key0->sigma = (block*)malloc(height*sizeof(block));
    key0->tau0 = (uint8_t*)malloc(height*sizeof(uint8_t));
    key0->tau1 = (uint8_t*)malloc(height*sizeof(uint8_t));
    
    //Divyu: Same for P1
    dpfl[1]->size = 1;
    dpfl[1]->level = 0;
    dpfl[1]->nodes = NULL;
    dpfl[1]->currt = NULL;
    dpfl[1]->prevt = NULL;
    
    //Initialising root hat(s), hat(t) for P1.
    dpfip[1]->hats = (block*)malloc(sizeof(block));
        dpfip[1]->hats[0] = keys[1];
    dpfip[1]->hatt = (uint8_t*)malloc(sizeof(uint8_t));
        dpfip[1]->hatt[0] = 1;
    key1->s = keys[1];
    key1->t = 1;

    
    //Divyu: Initialising empty sigma, tau[0] and tau[1] for key1.
    dpfip[1]->sigma = (block*)malloc(height*sizeof(block));
    dpfip[1]->tau[0] = (uint8_t*)malloc(height*sizeof(uint8_t));
    dpfip[1]->tau[1] = (uint8_t*)malloc(height*sizeof(uint8_t));
    key1->sigma = (block*)malloc(height*sizeof(block));
    key1->tau0 = (uint8_t*)malloc(height*sizeof(uint8_t));
    key1->tau1 = (uint8_t*)malloc(height*sizeof(uint8_t));


    for (int i = 0; i < height; i++) {
#ifdef DPF_PROFILE_
    printf("START DPF LEVEL %d%d\n", dpfl[0]->level, current_timestamp());
#endif
    #pragma omp for
    for(size_t j=0; j<2; j++) {
        prg_eval_all_and_xor(dpfl[j], dpfip[j]->hats);
    }

        uint8_t sig = (((dpfip[0]->index + dpfip[1]->index).value >> (height - 1 - i)) & 1);
        //sig0 & sig1 are only useful if index is boolean shared. With arithmetic shares, there
        //is no direct relationship between these three values.
        // const uint8_t sig0 = static_cast<uint8_t>(dpfip[0]->index.value >> (group_bitwidth - 1 - i)) & 1;
        // const uint8_t sig1 = static_cast<uint8_t>(dpfip[1]->index.value >> (group_bitwidth - 1 - i)) & 1;

        //Divyu: Computing sigma[i], tau[0][i], tau[1][i].
        dpfip[0]->sigma[i] = dpfl[0]->zs[1^sig] ^ dpfl[1]->zs[1^sig];
        dpfip[1]->sigma[i] = dpfl[0]->zs[1^sig] ^ dpfl[1]->zs[1^sig];
        key0->sigma[i] = dpfip[0]->sigma[i];
        key1->sigma[i] = dpfip[1]->sigma[i];

        dpfip[0]->tau[0][i] = dpfl[0]->zt[0] ^ dpfl[1]->zt[0] ^ sig ^ 1;
        dpfip[1]->tau[0][i] = dpfl[0]->zt[0] ^ dpfl[1]->zt[0] ^ sig ^ 1;
        key0->tau0[i] = dpfip[0]->tau[0][i];
        key1->tau0[i] = dpfip[1]->tau[0][i];

        dpfip[0]->tau[1][i] = dpfl[0]->zt[1] ^ dpfl[1]->zt[1] ^ sig;
        dpfip[1]->tau[1][i] = dpfl[0]->zt[1] ^ dpfl[1]->zt[1] ^ sig;
        key0->tau1[i] = dpfip[0]->tau[1][i];
        key1->tau1[i] = dpfip[1]->tau[1][i];

        dpfl[0]->prevt = dpfip[0]->hatt;
        dpfl[1]->prevt = dpfip[1]->hatt;
        
        free(dpfip[0]->hats);
        free(dpfip[1]->hats);

        dpfip[0]->hats = (block*)malloc(dpfl[0]->size*sizeof(block));
        dpfip[1]->hats = (block*)malloc(dpfl[1]->size*sizeof(block));
        dpfip[0]->hatt = (uint8_t*)malloc(dpfl[0]->size*sizeof(uint8_t));
        dpfip[1]->hatt = (uint8_t*)malloc(dpfl[1]->size*sizeof(uint8_t));

        #pragma omp for schedule(static, 1)
        for (size_t j = 0; j < dpfl[0]->size; j++) {
            if(dpfl[0]->prevt[j/2]==1) {
                dpfip[0]->hats[j] = dpfl[0]->nodes[j] ^ dpfip[0]->sigma[i];
                dpfip[0]->hatt[j] = dpfl[0]->currt[j] ^ dpfip[0]->tau[j&1][i];
            }
            else {
                dpfip[0]->hats[j] = dpfl[0]->nodes[j];
                dpfip[0]->hatt[j] = dpfl[0]->currt[j];
            }
            
            if(dpfl[1]->prevt[j/2]==1) {
                dpfip[1]->hats[j] = dpfl[1]->nodes[j] ^ dpfip[1]->sigma[i];
                dpfip[1]->hatt[j] = dpfl[1]->currt[j] ^ dpfip[1]->tau[j&1][i];
            }
            else {
                dpfip[1]->hats[j] = dpfl[1]->nodes[j];
                dpfip[1]->hatt[j] = dpfl[1]->currt[j];
            }
            
        }


#ifdef DPF_PROFILE_
    printf("END DPF LEVEL %d%d\n", dpfl[0]->level, current_timestamp());
#endif

    }

#ifdef DPF_PROFILE_
    printf("DPF START OFFLINE COMPUTATION %d\n", current_timestamp());
#endif
    GroupElement *W0 = (GroupElement*)malloc(2*sizeof(GroupElement));
    GroupElement *W1 = (GroupElement*)malloc(2*sizeof(GroupElement));
    W0[0] = GroupElement(0, key0->Bout);
    W0[1] = GroupElement(0, key0->Bout);
    W1[0] = GroupElement(0, key1->Bout);
    W1[1] = GroupElement(0, key1->Bout);

    uint64_t T0 = 0;
    uint64_t T1 = 0;
    //Divyu: Don't delete this is parallelized code for final correction words which needs some changes.
    // GroupElement** gamma_indp0 = (GroupElement**)malloc(dpfl[0]->size*sizeof(GroupElement*));
    // GroupElement** gamma_indp1 = (GroupElement**)malloc(dpfl[1]->size*sizeof(GroupElement*));
    // std::cout<<"Here\n";

    // convert_parallel(key0->Bout, 2, dpfl[0]->size, dpfip[0]->hats, gamma_indp0);
    // convert_parallel(key1->Bout, 2, dpfl[1]->size, dpfip[1]->hats, gamma_indp1);
    // free(dpfip[0]->hats);
    // free(dpfip[1]->hats);
    //Try parallelizing this

    // int num_threads = 4;
    // GroupElement thread_W0[num_threads][2] = {{GroupElement(0, key0->Bout)}};
    // GroupElement thread_W1[num_threads][2] = {{GroupElement(0, key1->Bout)}};
    // uint64_t thread_T0[num_threads] = {0};
    // uint64_t thread_T1[num_threads] = {0};
    // #pragma omp parallel num_threads(4)
    // {
    //     int thread_id = omp_get_num_threads();
    //     // std::cout<<thread_id<<"\n";
    //     #pragma omp for
    //     for(size_t j=0; j<dpfl[0]->size; j++) {
    //                 // int thread_id = omp_get_num_threads();
    //     // std::cout<<thread_id<<"\n";
    //         auto gamma_ind0 = convert(key0->Bout, 2, dpfip[0]->hats[j]);
    //         thread_W0[thread_id][0] = thread_W0[thread_id][0] + gamma_ind0[0];
    //         thread_W0[thread_id][1] = thread_W1[thread_id][1] + gamma_ind0[1];

    //         auto gamma_ind1 = convert(key1->Bout, 2, dpfip[1]->hats[j]);
    //         thread_W1[thread_id][0] = thread_W1[thread_id][0] + gamma_ind1[0];
    //         thread_W1[thread_id][1] = thread_W1[thread_id][1] + gamma_ind1[1];

    //         thread_T0[thread_id] = thread_T0[thread_id] + static_cast<uint64_t>(dpfip[0]->hatt[j]);
    //         thread_T1[thread_id] = thread_T1[thread_id] + static_cast<uint64_t>(dpfip[1]->hatt[j]);
    //         // free(gamma_indp0[j]);
    //         // free(gamma_indp1[j]);
    //     }
    // }

    // for(size_t j=0; j<num_threads; j++) {
    //     std::cout<<"W0[0] "<<j<<" "<<thread_W0[j][0]<<"\n";
    //     W0[0] = W0[0] + thread_W0[j][0];
    //     W0[1] = W0[1] + thread_W0[j][1];
    //     W1[0] = W1[0] + thread_W1[j][0];
    //     W1[1] = W1[1] + thread_W1[j][1];
    //     T0 = T0 + thread_T0[j];
    //     T1 = T1 + thread_T1[j];
    // }

    //Above commented code I had written for parallelization of summing below but it doesn't seem to work.
    //Invokes only thread 4 but sums in thread 1 variable which is weird.
    //convert is parallelized and the sum loop is serialized for now.
    // #pragma omp parallel for
    for(size_t j=0; j<dpfl[0]->size; j++) {
        auto gamma_ind0 = convert(key0->Bout, 2, dpfip[0]->hats[j]);
        W0[0] = W0[0] + gamma_ind0[0];
        W0[1] = W0[1] + gamma_ind0[1];

        auto gamma_ind1 = convert(key1->Bout, 2, dpfip[1]->hats[j]);
        W1[0] = W1[0] + gamma_ind1[0];
        W1[1] = W1[1] + gamma_ind1[1];
        T0 = T0 + dpfip[0]->hatt[j];
        T1 = T1 + dpfip[1]->hatt[j];
        free(gamma_ind0);
        free(gamma_ind1); 
    }
    // free(gamma_indp0);
    // free(gamma_indp1);
    free(dpfip[0]->hatt);
    free(dpfip[1]->hatt);

    uint8_t t1 = (T0<T1)?1:0;
    key0->gamma = (GroupElement*)malloc(2*sizeof(GroupElement));
    key1->gamma = (GroupElement*)malloc(2*sizeof(GroupElement));
    if(t1) {
        key0->gamma[0] = W0[0] - beta[0][0] -beta[1][0] - W1[0];
        key1->gamma[0] = W0[0] - beta[0][0] -beta[1][0] - W1[0];
        key0->gamma[1] = W0[1] - beta[0][1] -beta[1][1] - W1[1];
        key1->gamma[1] = W0[1] - beta[0][1] -beta[1][1] - W1[1];
    }
    else {
        key0->gamma[0] = beta[0][0] + beta[1][0] -W0[0] + W1[0];
        key1->gamma[0] = beta[0][0] + beta[1][0] -W0[0] + W1[0];
        key0->gamma[1] = beta[0][1] + beta[1][1] -W0[1] + W1[1];
        key1->gamma[1] = beta[0][1] + beta[1][1] -W0[1] + W1[1];
    }
    
    free_dpf_layer(dpfl[0]);
    free_dpf_layer(dpfl[1]);

    return std::pair<dpf_key, dpf_key>(*key0, *key1);
};

GroupElement* dpf_eval(int party, GroupElement idx, const dpf_key &key) {
    uint8_t hatt = party;
    block hats = key.s;

    //Set plaintext
        static const block ZeroBlock = toBlock(0, 0);
        static const block OneBlock = toBlock(0, 1);
        static const block TwoBlock = toBlock(0, 2);
        static const block ThreeBlock = toBlock(0, 3);
        const static block ptt[4] = {ZeroBlock, OneBlock, TwoBlock, ThreeBlock};
        block ctt[4];
    for(int i=0; i<key.height; i++) {
        AES aes_key(hats);

        aes_key.ecbEncFourBlocks(ptt, ctt);
        uint8_t sig = ((idx.value >> (key.height - 1 - i)) & 1);
        block s = ctt[2*sig];
        uint8_t t = lsb(ctt[2*sig+1]);
        uint8_t tau = (sig == 0)?(key.tau0)[i]:(key.tau1)[i];
        if(hatt==1) {
            hats = s ^ (key.sigma)[i];
            hatt = t ^ tau;
        }
        else {
            hats = s;
            hatt = t;
        }
    }
    GroupElement* out = (GroupElement*)malloc(2*sizeof(GroupElement));
    auto gamma_ind = convert(key.Bout, 2, hats);
    if(party==0) {
        if(hatt==0) {
            out[0] = gamma_ind[0];
            out[1] = gamma_ind[1];
        }
        else {
            out[0] = gamma_ind[0] + (key.gamma)[0];
            out[1] = gamma_ind[1] + (key.gamma)[1];
        }
    }
    else {
        if(hatt==0) {
            out[0] = GroupElement(0, key.Bout) - gamma_ind[0];
            out[1] = GroupElement(0, key.Bout) - gamma_ind[1];
        }
        else {
            out[0] = GroupElement(0, key.Bout) - gamma_ind[0] - (key.gamma)[0];
            out[1] = GroupElement(0, key.Bout) - gamma_ind[1] - (key.gamma)[1];

        }
    }

    free(gamma_ind);
    return out;
};

GroupElement** dpf_eval_all(int party, const dpf_key &key) {

    //Intialize dpf layer
    dpf_layer* dpfl = (dpf_layer*)malloc(sizeof(dpf_layer));
    dpfl->size = 1;
    dpfl->level = 0;
    dpfl->nodes = NULL;
    dpfl->currt = NULL;
    dpfl->prevt = NULL;
    block* hats = (block*)malloc(sizeof(block));
    hats[0] = key.s;
    uint8_t* hatt = (uint8_t*)malloc(sizeof(uint8_t));
    hatt[0] = party;

    for(int i=0; i<key.height; i++) {
        prg_eval_all_and_xor(dpfl, hats);
        dpfl->prevt = hatt;
        free(hats);
        hats = (block*) malloc(dpfl->size*sizeof(block));
        hatt = (uint8_t*)malloc(dpfl->size*sizeof(uint8_t));
        #pragma omp parallel for
        for(size_t j=0; j<dpfl->size; j++) {
            uint8_t tau = (j%2==0)?(key.tau0)[i]:(key.tau1)[i];

            if(dpfl->prevt[j/2]==1){
                hats[j] = dpfl->nodes[j] ^ (key.sigma)[i];
                hatt[j] = dpfl->currt[j] ^ tau;
            }
            else {
            hats[j] = dpfl->nodes[j];
            hatt[j] = dpfl->currt[j];
            }
        }
    }

    GroupElement** out = (GroupElement**) malloc(dpfl->size*sizeof(GroupElement*));
    GroupElement** gamma_ind = (GroupElement**)malloc(dpfl->size*sizeof(GroupElement*));
    convert_parallel(key.Bout, 2, dpfl->size, hats, gamma_ind);
    #pragma omp parallel for
    for(size_t j=0; j<dpfl->size; j++) {
        out[j] = (GroupElement*)malloc(2*sizeof(GroupElement));
        // auto gamma_ind = convert(key.Bout, 2, hats[j]);
        if(party==0) {
            if(hatt[j]==0) {
                out[j][0] = gamma_ind[j][0];
                out[j][1] = gamma_ind[j][1];
            }
            else {
                out[j][0] = gamma_ind[j][0] + (key.gamma)[0];
                out[j][1] = gamma_ind[j][1] + (key.gamma)[1];
            }
        }
        else {
            if(hatt[j]==0) {
                out[j][0] = GroupElement(0, key.Bout) - gamma_ind[j][0];
                out[j][1] = GroupElement(0, key.Bout) - gamma_ind[j][1];
            }
            else {
                out[j][0] = GroupElement(0, key.Bout) - gamma_ind[j][0] - (key.gamma)[0];
                out[j][1] = GroupElement(0, key.Bout) - gamma_ind[j][1] - (key.gamma)[1];

            }
        }
        free(gamma_ind[j]);
    }
    free(hats);
    free(hatt);
    free_dpf_layer(dpfl);
    free(gamma_ind);
    return out;
};