#include <dpf.h>
#include<iostream>
#include <cryptoTools/Crypto/PRNG.h>

using namespace osuCrypto;

int main() {

    dpf_layer* dpfl = new dpf_layer;

    dpfl->size=2;
    dpfl->level=1;
    
    PRNG prng;
    prng.SetSeed(toBlock(0, 1), sizeof(block));
    auto keys = prng.get<std::array<block, 2>>();
    block* keynodes = (block*)malloc(2*sizeof(block));
    keynodes[0] = keys[0];
    keynodes[1] = keys[1];
    dpfl->nodes = keynodes;
    dpfl->currt = new uint8_t;
    dpfl->prevt = new uint8_t;

    // set AES Plaintext
    static const block OneBlock = toBlock(0, 1);
    static const block ZeroBlock = toBlock(0, 0);
    static const block TwoBlock = toBlock(0, 2);
    static const block ThreeBlock = toBlock(0, 3);
    const static block pt[4] = {ZeroBlock, OneBlock, TwoBlock, ThreeBlock};
    block ct[4];

    prg_eval_all_and_xor(dpfl, keynodes, ct, pt);

    //Printing size and level of new dpf layer which should be 4,2.
    std::cout<<dpfl->size<<"\n";
    std::cout<<dpfl->level<<"\n";


    return 0;
}