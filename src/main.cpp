#include <dpf.h>
#include<bits/stdc++.h>
#include <cryptoTools/Crypto/PRNG.h>
#include<time.h>

using namespace osuCrypto;
PRNG prng;
int bitlength = 2;
PRNG prngShared;

int main() {
    prng.SetSeed(toBlock(0, 0), sizeof(block));

    std::cout<<"----------------Running Key Gen-----------------\n";
    dpf_input_pack* dpfip0 = new dpf_input_pack;
    dpfip0->index = GroupElement(5, 3);
    dpf_input_pack* dpfip1 = new dpf_input_pack;
    dpfip1->index = GroupElement(1, 3);

    dpf_keygen(3, 3, dpfip0, dpfip1);
    // std::cout<<"---------Running prg_eval_all-----------------\n";
    // const size_t samples = 1<<20;
    // dpf_layer* dpfl = new dpf_layer;
    // dpfl->size=samples;
    // dpfl->level=20;
    
    // PRNG prng;
    // time_t start,end;
    // prng.SetSeed(toBlock(0, 1), sizeof(block));

    // std::cout<<"Populating keynodes\n";
    // block* keynodes = (block*)malloc(samples*sizeof(block));
    // for(int i=0; i<samples; i++)
    //     keynodes[i] = prng.get<block>();
    // std::cout<<"Done\n";
    // dpfl->nodes = NULL;
    // dpfl->currt = NULL;
    // dpfl->prevt = NULL;

    // // set AES Plaintext
    // static const block OneBlock = toBlock(0, 1);
    // static const block ZeroBlock = toBlock(0, 0);
    // static const block TwoBlock = toBlock(0, 2);
    // static const block ThreeBlock = toBlock(0, 3);
    // const static block pt[4] = {ZeroBlock, OneBlock, TwoBlock, ThreeBlock};
    // block ct[4];
    // time(&start);
    // std::cout<<"Running function\n";
    // prg_eval_all_and_xor(dpfl, keynodes);
    // time(&end);
    // std::cout<<"Time taken with parallelization for loop with "<<samples<<" iterations:"<<std::fixed<<double(end-start)<<std::setprecision(5)<<"\n";
    // //Printing size and level of new dpf layer which should be 4,2.
    // std::cout<<dpfl->size<<"\n";
    // std::cout<<dpfl->level<<"\n";
    // std::cout<<static_cast<uint32_t>(dpfl->zt[0])<<"\n";
    // free(keynodes);
    // free_dpf_layer(dpfl);
    return 0;
}