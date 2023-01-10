#include <dpf.h>
#include<bits/stdc++.h>
#include <cryptoTools/Crypto/PRNG.h>
#include<time.h>

using namespace osuCrypto;

int main() {

    const size_t samples = 1<<26;
    dpf_layer* dpfl = new dpf_layer;
    dpfl->size=samples;
    dpfl->level=26;
    
    PRNG prng;
    time_t start,end;
    prng.SetSeed(toBlock(0, 1), sizeof(block));

    std::cout<<"Populating keynodes\n";
    block* keynodes = (block*)malloc(samples*sizeof(block));
    for(int i=0; i<samples; i++)
        keynodes[i] = prng.get<block>();
    std::cout<<"Done\n";
    dpfl->nodes = NULL;
    dpfl->currt = NULL;
    dpfl->prevt = NULL;

    // set AES Plaintext
    static const block OneBlock = toBlock(0, 1);
    static const block ZeroBlock = toBlock(0, 0);
    static const block TwoBlock = toBlock(0, 2);
    static const block ThreeBlock = toBlock(0, 3);
    const static block pt[4] = {ZeroBlock, OneBlock, TwoBlock, ThreeBlock};
    block ct[4];
    time(&start);
    std::cout<<"Running function\n";
    prg_eval_all_and_xor(dpfl, keynodes, ct, pt);
    time(&end);
    std::cout<<"Time taken with parallelization for loop with "<<samples<<" iterations:"<<std::fixed<<double(end-start)<<std::setprecision(5)<<"\n";
    //Printing size and level of new dpf layer which should be 4,2.
    std::cout<<dpfl->size<<"\n";
    std::cout<<dpfl->level<<"\n";
    std::cout<<dpfl->zs[0]<<"\n";
    free(keynodes);
    free_dpf_layer(dpfl);
    return 0;
}