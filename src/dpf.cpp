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
    free(dpfip->hats);
    free(dpfip);
};

inline int bytesize(const int bitsize) {
    return (bitsize % 8) == 0 ? bitsize / 8 : (bitsize / 8)  + 1;
}
//Divyu: Commenting to test keygen without this.
// uint64_t *convert_elem(const int inp_bitwidth, 
//              const int out_bitwidth, 
//              const int no_of_group_elements, 
//              const block inp_block)
// {
//     uint64_t out[no_of_group_elements];

//     const int bys = bytesize(inp_bitwidth);
//     const int totalBys = bys * no_of_group_elements;

//     if (bys * no_of_group_elements <= 16) {
//         uint8_t *bptr = (uint8_t *)&inp_block;
//         for(int i = 0; i < no_of_group_elements; i++) {
//             out[i] = *(uint64_t *)(bptr + i * bys);
//         }
//     }

//     else {
//         int numblocks = totalBys % 16 == 0 ? totalBys / 16 : (totalBys / 16) + 1;

//         AES aes(inp_block);
//         block pt[numblocks];
//         block ct[numblocks];

//         for(int i = 0; i < numblocks; i++) {
//             pt[i] = toBlock(0, i);
//         }

//         aes.ecbEncBlocks(pt, numblocks, ct);
//         uint8_t *bptr = (uint8_t *)ct;

//         for(int i = 0; i < no_of_group_elements; i++) {
//             out[i] = *(uint64_t *)(bptr + i * bys);
//         }
//     }
// };

// void convert(const int inp_bitwidth, 
//              const int out_bitwidth,
//              const int size,
//              const int no_of_group_elements, 
//              const block *inp,
//              uint64_t **out)
// {
//     #pragma omp parallel for
//      for (int i = 0; i < size; i++) {
//         out[i] = convert_elem(inp_bitwidth, out_bitwidth, no_of_group_elements, inp[i]);
//      }
// };

void prg_eval_all_and_xor(dpf_layer *dpfl, block *keynodes) {

    block* output_nodes = (block*)malloc(2*dpfl->size*sizeof(block));
    uint8_t* output_t = (uint8_t*)malloc(2*dpfl->size*sizeof(uint8_t));

        dpfl->zs[0] = ZeroBlock;
        dpfl->zs[1] = ZeroBlock;
        dpfl->zt[0] = 0;
        dpfl->zt[1] = 0;


    //Parallelizing aes calls
    int num_threads = 4;
    block thread_zs[2][num_threads] = {{ZeroBlock}};
    uint8_t thread_zt[2][num_threads] = {{0}};

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

        #pragma omp for  schedule(static, 1)
        for(int i=0; i<dpfl->size; i++) {
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
            thread_zs[0][thread_id] = thread_zs[0][thread_id] ^ ctt[0];
            thread_zs[1][thread_id] = thread_zs[1][thread_id] ^ ctt[2];
            thread_zt[0][thread_id] = thread_zt[0][thread_id] ^ lsb(ctt[1]);
            thread_zt[1][thread_id] = thread_zt[1][thread_id] ^ lsb(ctt[3]);

        }
            #pragma omp for
            for(int i=0; i<num_threads; i++) {
                dpfl->zs[0] = dpfl->zs[0] ^ thread_zs[0][i];
                dpfl->zs[1] = dpfl->zs[1] ^ thread_zs[1][i];
                dpfl->zt[0] = dpfl->zt[0] ^ thread_zt[0][i];
                dpfl->zt[1] = dpfl->zt[1] ^ thread_zt[1][i];
            }
    }



    
    free(dpfl->nodes);
    dpfl->nodes = output_nodes;
    free(dpfl->prevt);
    // dpfl->prevt = dpfl->currt;
    free(dpfl->currt);
    dpfl->currt = output_t;

    dpfl->size *= 2;
    dpfl->level += 1;
};



void dpf_keygen(int height, const int group_bitwidth, dpf_input_pack *dpfip0, dpf_input_pack *dpfip1)
{
    // sample hat{S}_i^{0,0}, the PRG keys
    auto keys = prng.get<std::array<block, 2>>();
    // set beta
    // GroupElement beta[2][2] = {{0, *dpfip0->alpha}, {1, *dpfip1->alpha}};

    // initiate outputs
    const static block s[2] = {keys[0], keys[1]};

    // block t[2] = {
    //     toBlock(0, 0), 
    //     toBlock(0, 1)
    // };

    // set AES Plaintext
    // static const block TwoBlock = toBlock(0, 2);
    // static const block ThreeBlock = toBlock(0, 3);

    // const static block pt[4] = {ZeroBlock, OneBlock, TwoBlock, ThreeBlock};

    // // set AES ciphertext
    // block ct[4];

#ifdef DPF_PROFILE_
    printf("START DPF LEVEL 0 %d\n", current_timestamp());
#endif
    

    // set dpf_layers
    dpf_layer* dpfl0 = new dpf_layer;
    dpf_layer* dpfl1 = new dpf_layer;

    //Divyu: Initialising dpf layer for P0.
    dpfl0->size = 1;
    dpfl0->level = 0;
    dpfl0->nodes = NULL;
    dpfl0->currt = NULL;
    dpfl0->prevt = NULL;
    
    //Initialisig root hat(s), hat(t) for P0
    dpfip0->hats = (block*)malloc(sizeof(block));
    dpfip0->hats[0] = s[0];
    dpfip0->hatt = (uint8_t*)malloc(sizeof(uint8_t));
        dpfip0->hatt[0] = 0;
    
    //Divyu: Initialising empty sigma, tau[0] and tau[1] for key1.
    dpfip0->sigma = (block*)malloc(height*sizeof(block));
    dpfip0->tau[0] = (uint8_t*)malloc(height*sizeof(uint8_t));
    dpfip0->tau[1] = (uint8_t*)malloc(height*sizeof(uint8_t));
    
    //Divyu: Same for P1
    dpfl1->size = 1;
    dpfl1->level = 0;
    dpfl1->nodes = NULL;
    dpfl1->currt = NULL;
    dpfl1->prevt = NULL;
    
    //Initialising root hat(s), hat(t) for P1.
    dpfip1->hats = (block*)malloc(sizeof(block));
        dpfip1->hats[0] = s[1];
    dpfip1->hatt = (uint8_t*)malloc(sizeof(uint8_t));
        dpfip1->hatt[0] = 1;
    
    //Divyu: Initialising empty sigma, tau[0] and tau[1] for key1.
    dpfip1->sigma = (block*)malloc(height*sizeof(block));
    dpfip1->tau[0] = (uint8_t*)malloc(height*sizeof(uint8_t));
    dpfip1->tau[1] = (uint8_t*)malloc(height*sizeof(uint8_t));
        // std::cout<<(((dpfip0->index + dpfip1->index).value >> (height - 1)) & 1)<<"\n";


    for (int i = 0; i < height; i++) {
        // std::cout<<"Iteration: "<<i<<"\n";
#ifdef DPF_PROFILE_
    printf("START DPF LEVEL %d%d\n", dpfl0->level, current_timestamp());
#endif
        prg_eval_all_and_xor(dpfl0, dpfip0->hats);
        prg_eval_all_and_xor(dpfl1, dpfip1->hats);

        uint8_t sig = (((dpfip0->index + dpfip1->index).value >> (height - 1 - i)) & 1);
        // std::cout<<"sig "<<static_cast<uint32_t>(sig)<<"\n";
        //sig0 & sig1 are only useful if index is boolean shared. With arithmetic shares, there
        //is no direct relationship between these three values.
        // const uint8_t sig0 = static_cast<uint8_t>(dpfip0->index.value >> (group_bitwidth - 1 - i)) & 1;
        // const uint8_t sig1 = static_cast<uint8_t>(dpfip1->index.value >> (group_bitwidth - 1 - i)) & 1;

        //Divyu: Computing sigma[i], tau[0][i], tau[1][i].
        dpfip0->sigma[i] = dpfl0->zs[1^sig] ^ dpfl1->zs[1^sig];
        dpfip1->sigma[i] = dpfl0->zs[1^sig] ^ dpfl1->zs[1^sig];

        dpfip0->tau[0][i] = dpfl0->zt[0] ^ dpfl1->zt[0] ^ sig ^ 1;
        dpfip1->tau[0][i] = dpfl0->zt[0] ^ dpfl1->zt[0] ^ sig ^ 1;

        dpfip0->tau[1][i] = dpfl0->zt[1] ^ dpfl1->zt[1] ^ sig;
        dpfip1->tau[1][i] = dpfl0->zt[1] ^ dpfl1->zt[1] ^ sig;

        dpfl0->prevt = dpfip0->hatt;
        dpfl1->prevt = dpfip1->hatt;

        //These are causing double free so there is some error in assignment.
        //Comment these and then you can see that t value differs only on index point which means the computations are correct.
        // free(dpfip0->hats);
        // free(dpfip1->hats);
        // free(dpfip0->hatt);
        // free(dpfip1->hatt);

        dpfip0->hats = (block*)malloc(dpfl0->size*sizeof(block));
        dpfip1->hats = (block*)malloc(dpfl1->size*sizeof(block));
        dpfip0->hatt = (uint8_t*)malloc(dpfl0->size*sizeof(uint8_t));
        dpfip1->hatt = (uint8_t*)malloc(dpfl1->size*sizeof(uint8_t));
        std::cout<<"Level: "<<i<<"\n";
        std::cout<<"Input Bit: "<<static_cast<uint32_t>(sig)<<"\n";
        // #pragma omp parallel for
        for (int j = 0; j < dpfl0->size; j++) {
            if(dpfl0->prevt[j/2]==1) {
                dpfip0->hats[j] = dpfl0->nodes[j] ^ dpfip0->sigma[i];
                dpfip0->hatt[j] = dpfl0->currt[j] ^ dpfip0->tau[j&1][i];
            }
            else {
                dpfip0->hats[j] = dpfl0->nodes[j];
                dpfip0->hatt[j] = dpfl0->currt[j];
            }
            
            if(dpfl1->prevt[j/2]==1) {
                dpfip1->hats[j] = dpfl1->nodes[j] ^ dpfip1->sigma[i];
                dpfip1->hatt[j] = dpfl1->currt[j] ^ dpfip0->tau[j&1][i];
            }
            else {
                dpfip1->hats[j] = dpfl1->nodes[j];
                dpfip1->hatt[j] = dpfl1->currt[j];
            }

            std::cout<<"j: "<<j<<"\nP0: "<<dpfip0->hats[j]<<" "<<static_cast<uint32_t>(dpfip0->hatt[j])<<"\n";
            std::cout<<"P1: "<<dpfip1->hats[j]<<" "<<static_cast<uint32_t>(dpfip1->hatt[j])<<"\n";
            
        }

        std::cout<<"\n";

#ifdef DPF_PROFILE_
    printf("END DPF LEVEL %d%d\n", dpfl0->level, current_timestamp());
#endif

    }

#ifdef DPF_PROFILE_
    printf("DPF START OFFLINE COMPUTATION %d\n", current_timestamp());
#endif

    // uint64_t **hats_converted0 = (uint64_t**)malloc(sizeof(uint64_t[ELEMENTNO]));
    // uint64_t **hats_converted1 = (uint64_t**)malloc(sizeof(uint64_t[ELEMENTNO]));

    // convert(INPBW, OUTBW, dpfl0->size, ELEMENTNO, dpfip0->hats, hats_converted0);
    // convert(INPBW, OUTBW, dpfl1->size, ELEMENTNO, dpfip1->hats, hats_converted1);

    // GroupElement w[2] = {GroupElement(0), GroupElement(0)};

    // for (int i = 0; i < dpfl0->size; i++) {
    //     w[0] = w[0] + GroupElement(*hats_converted0[i]);
    //     w[1] = w[1] + GroupElement(*hats_converted1[i]);
    // }

    // uint8_t T[2] = {0,0};

    free_dpf_layer(dpfl0);
    free_dpf_layer(dpfl1);
};