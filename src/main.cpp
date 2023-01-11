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
    time_t start, end;
    int Bout = 10;
    int Bin = 3;
    dpf_input_pack *dpfip[2];
    dpfip[0] = (dpf_input_pack*)malloc(sizeof(dpf_input_pack));
    dpfip[1] = (dpf_input_pack*)malloc(sizeof(dpf_input_pack));
    dpfip[0]->index = GroupElement(2, Bin);
    dpfip[1]->index = GroupElement(3, Bin);
    dpfip[0]->alpha = (GroupElement*)malloc(sizeof(GroupElement));
    dpfip[0]->alpha[0] = GroupElement(15, Bout);
    dpfip[1]->alpha = (GroupElement*)malloc(sizeof(GroupElement));
    dpfip[1]->alpha[0] = GroupElement(6, Bout);
    // std::cout<<"Here\n";
    dpf_key k0, k1;
    time(&start);
    std::tie(k0, k1) = dpf_keygen(Bin, Bout, dpfip);
    time(&end);
    std::cout<<"Time taken: "<<std::fixed<<double(end-start)<<std::setprecision(5)<<"\n";
    // for(int i=0; i<8; i++) {
    //     std::cout<<"i: "<<i<<" "<<(dpfip[0]->hats[i])<<" "<<(dpfip[1]->hats[i])<<"\n";
    // }

    // std::cout<<"--------------------Running Eval-------------\n";
    // GroupElement idx = GroupElement(0, Bin);
    // GroupElement* g1, *g2;
    // g1 = dpf_eval(0, idx, k0);
    // g2 = dpf_eval(1, idx, k1);
    // std::cout<<(g1[0]+g2[0]).value<<" "<<(g1[1]+g2[1]).value<<"\n";
    // // std::cout<<s0<<" "<<s1<<"\n";

    std::cout<<"--------------------Running Eval All-------------\n";
    GroupElement **t_vec_0, **t_vec_1;
    // std::cout<<"Party0:\n";
    t_vec_0 = dpf_eval_all(0, k0);
    // std::cout<<"Party1:\n";
    t_vec_1 = dpf_eval_all(1, k1);

    for(int i=0; i<(1<<Bin); i++) {
        std::cout<<"i: "<<i<<" "<<(t_vec_0[i][0]+t_vec_1[i][0]).value<<" "<<(t_vec_0[i][1]+t_vec_1[i][1]).value<<"\n";
    }

    // std::cout<<"----------------Running Convert Function-------------\n";
    // const int out_bitwidth = 33;
    // const int no_of_group_elements = 1;
    // const block inp_block = toBlock(0,5);

    // // std::cout<<"Here\n";
    // GroupElement* out = convert(out_bitwidth, no_of_group_elements, inp_block);
    // // std::cout<<"Here\n";
    // std::cout<<out[0]<<"\n";
    // std::cout<<"---------Running prg_eval_all-----------------\n";
    // const size_t samples = 1<<10;
    // dpf_layer* dpfl = new dpf_layer;
    // dpfl->size=samples;
    // dpfl->level=10;
    
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