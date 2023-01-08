#include <dpf.h>
#include<iostream>
#include <cryptoTools/Crypto/PRNG.h>

using namespace osuCrypto;

int main() {

    dpf_layer* dpfl = new dpf_layer;

    dpfl->size=1;
    dpfl->level=0;
    
    PRNG prng;
    //PRNG needs to have some initial seed setup I guess. It is throwing error prng is not keyed
    // auto key = prng.get<std::array<block, 2>>();
    
    dpfl->nodes = (block*)malloc(sizeof(block));
    dpfl->nodes[0] = toBlock(0, 0);


    // set AES Plaintext
    static const block OneBlock = toBlock(0, 1);
    static const block ZeroBlock = toBlock(0, 0);
    static const block TwoBlock = toBlock(0, 2);
    static const block ThreeBlock = toBlock(0, 3);
    const static block pt[4] = {ZeroBlock, OneBlock, TwoBlock, ThreeBlock};
    block ct[4];

    prg_eval_all_and_xor(dpfl, ct, pt);

    //Printing size and level of new dpf layer which should be 2,1
    std::cout<<dpfl->size<<"\n";
    std::cout<<dpfl->level<<"\n";


    return 0;
}