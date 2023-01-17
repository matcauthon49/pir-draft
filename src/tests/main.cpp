#include <dpf.h>
#include <bits/stdc++.h>
#include <cryptoTools/Crypto/PRNG.h>
#include <chrono>
#include <algorithm>


using namespace osuCrypto;
// PRNG prng;
//Bitlength is length of output
//Bitwidth is length of index
// int bitwidth = 3;
PRNG prngShared;

static const char* charmap = "0123456789";

std::string uint128ToString(const __uint128_t& value)
{
    std::string result;
    result.reserve( 40 ); // max. 40 digits possible ( uint64_t has 20) 
    __uint128_t helper = value;

    do {
        result += charmap[ helper % 10 ];
        helper /= 10;
    } while ( helper );
    std::reverse( result.begin(), result.end() );
    return result;
}

int main() {
//     GroupElement g = GroupElement(10);
//     GroupElement g2 = GroupElement(100);
//     GroupElement g3 = GroupElement(1000);

//     // GroupElement temp = GroupElement(-5, 3);
//     // std::cout<<temp.value<<"\n";
//     // std::cout << g.bitsize << " " << g2.bitsize << " " << g3.bitsize << "\n";
    prng.SetSeed(toBlock(0, 0), sizeof(block));

    int database_size = (1<<15);
    GroupElement *database = new GroupElement[database_size];
    for(int i=0; i<database_size; i++) {
        database[i] = GroupElement(i, bitlength);
    }

    std::cout<<"----------------Running Key Gen-----------------\n";
    // time_t start, end;
    prng.SetSeed(toBlock(0, 1), sizeof(block));
    int Bout = bitlength;
    int Bin = 16;
    dpf_input_pack *dpfip[2];
    dpfip[0] = (dpf_input_pack*)malloc(sizeof(dpf_input_pack));
    dpfip[1] = (dpf_input_pack*)malloc(sizeof(dpf_input_pack));
    dpfip[0]->index = GroupElement(60000, Bin);
    dpfip[1]->index = GroupElement(5500, Bin);
    dpfip[0]->alpha = (GroupElement*)malloc(sizeof(GroupElement));
    dpfip[0]->alpha[0] = GroupElement(100, Bout);
    dpfip[1]->alpha = (GroupElement*)malloc(sizeof(GroupElement));
    dpfip[1]->alpha[0] = GroupElement(100, Bout);
    input_check_pack_2 ip2;
    // std::cout<<"Here\n";
    dpf_key k0, k1;
    auto start = std::chrono::high_resolution_clock::now();
    std::tie(k0, k1) = dpf_keygen(Bin, Bout, dpfip, &ip2);
    auto end  = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
    // std::cout<<(ip2.index)[0].value<<" "<<(k0.gamma)[0].value<<" "<<(k1.gamma)[0].value<<" "<<"\n";
    std::cout << "Time taken: " << duration.count()*1e-6 <<"\n";
    // for(int i=0; i<8; i++) {
    //     std::cout<<"i: "<<i<<" "<<(dpfip[0]->hats[i])<<" "<<(dpfip[1]->hats[i])<<"\n";
    // }

//     // std::cout<<"--------------------Running Eval-------------\n";
//     // GroupElement idx = GroupElement(65500, Bin);
//     // GroupElement* g1, *g2;
//     // // std::cout<<"Party 0:\n";
//     // g1 = dpf_eval(0, idx, k0);
//     // // std::cout<<"Party 1:\n";
//     // g2 = dpf_eval(1, idx, k1);
//     // std::cout<<(g1[0]+g2[0]).value<<" "<<(g1[1]+g2[1]).value<<"\n";
//     // // std::cout<<s0<<" "<<s1<<"\n";

    std::cout<<"--------------------Running Eval All-------------\n";
    GroupElement **t_vec_0, **t_vec_1;
    // std::cout<<"Party0:\n";
    input_check_pack icp0, icp1;
    icp0.index = (ip2.index)[0];
    icp1.index = (ip2.index)[1];
    icp0.payload = (ip2.payload)[0];
    icp1.payload = (ip2.payload)[1];
    // std::cout<<icp0.index.value<<"\n";
    start = std::chrono::high_resolution_clock::now();
    t_vec_0 = dpf_eval_all(0, k0, &icp0);
    end  = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
    // std::cout<<"Party1:\n";
    t_vec_1 = dpf_eval_all(1, k1, &icp1);
    // int final_ans = 0;
    // uint64_t count=0;
    // std::cout<<(icp0.sigma)[1]<<" "<<(k0.sigma)[1]<<"\n";
    // std::cout<<"Printing indexes\n";
    for(int i=0; i<(1<<Bin); i++) {
        // std::cout<<"i: "<<i<<" "<<(t_vec_0[i][0]+t_vec_1[i][0]).value<<" "<<(t_vec_0[i][1]+t_vec_1[i][1]).value<<"\n";
        if((t_vec_0[i][0]+t_vec_1[i][0]).value != 0 || (t_vec_0[i][1]+t_vec_1[i][1]).value != 0) {
           std::cout<<i<<"\n";
           std::cout<<(t_vec_0[i][0] + t_vec_1[i][0]).value<<" "<<(t_vec_0[i][1] + t_vec_1[i][1]).value<<"\n";
        }

    }
    std::cout << "Time taken: " << duration.count()*1e-6 <<"\n";
//     // std::cout<<"icp0:\nIndex = "<<icp0.index.value<<"\n";
//     // std::cout<<"Payload = "<<icp0.payload.value<<"\n";
//     // std::cout<<"Size = "<<icp0.size<<"\n";
//     // for(int i=0; i<icp0.size; i++) {
//     //     std::cout<<"i = "<<i<<" zs[0] = "<<(icp0.zs)[0][i]<<" zs[1] = "<<(icp0.zs)[1][i]<<" sigma = "<<(icp0.sigma)[i][i]<<"\n";
//     // }
//     // std::cout<<"T = "<<(icp0.T)<<" W[0] = "<<(icp0.W)[0].value<<" W[1] = "<<(icp0.W)[1].value<<" gamma = "<<(icp0.gamma)[0].value<<"\n\n\n";

//     // std::cout<<"icp1:\nIndex = "<<icp1.index.value<<"\n";
//     // std::cout<<"Payload = "<<icp1.payload.value<<"\n";
//     // std::cout<<"Size = "<<icp1.size<<"\n";
//     // for(int i=0; i<icp1.size; i++) {
//     //     std::cout<<"i = "<<i<<" zs[0] = "<<(icp1.zs)[0][i]<<" zs[1] = "<<(icp1.zs)[1][i]<<"\n";
//     // }
//     // std::cout<<"T = "<<(icp1.T)<<" W[0] = "<<(icp1.W)[0].value<<" W[1] = "<<(icp1.W)[1].value<<" gamma = "<<(icp1.gamma)[0].value<<"\n\n\n";

//     // std::cout<<"ip2:\nIndex = "<<(ip2.index)[0].value<<" "<<(ip2.index)[1].value<<"\n";
//     // std::cout<<"Payload = "<<(ip2.payload)[0].value<<" "<<(ip2.payload)[1].value<<"\n";
//     // std::cout<<"Size = "<<ip2.size<<"\n";
//     // for(int i=0; i<ip2.size; i++) 
//     //     std::cout<<"i = "<<i<<" zs0[0] = "<<(ip2.zs0)[0][i]<<" zs0[1] = "<<(ip2.zs0)[1][i]<<" zs1[0] = "<<(ip2.zs1)[0][i]<<"\n";
    
//     // std::cout<<"T[0] = "<<(ip2.T)[0]<<" T[1] = "<<(ip2.T)[1]<<" W0[0] = "<<(ip2.W)[0][0].value<<"W0[1]"<<(ip2.W)[0][1].value<<"\n";

//     // bool out = check_xor(Bout, icp0, icp1, ip2);
//     // std::cout<<"Check pass "<<out<<"\n";

//     // std::cout<<"----------------Running Convert Function-------------\n";
//     // const int out_bitwidth = 33;
//     // const int no_of_group_elements = 1;
//     // const block inp_block = toBlock(0,5);

//     // // std::cout<<"Here\n";
//     // GroupElement* out = convert(out_bitwidth, no_of_group_elements, inp_block);
//     // // std::cout<<"Here\n";
//     // std::cout<<out[0]<<"\n";
//     // std::cout<<"---------Running prg_eval_all-----------------\n";
//     // const size_t samples = 1<<10;
//     // dpf_layer* dpfl = new dpf_layer;
//     // dpfl->size=samples;
//     // dpfl->level=10;

//     // PRNG prng;
//     // time_t start,end;
//     // prng.SetSeed(toBlock(0, 1), sizeof(block));

//     // std::cout<<"Populating keynodes\n";
//     // block* keynodes = (block*)malloc(samples*sizeof(block));
//     // for(int i=0; i<samples; i++)
//     //     keynodes[i] = prng.get<block>();
//     // std::cout<<"Done\n";
//     // dpfl->nodes = NULL;
//     // dpfl->currt = NULL;
//     // dpfl->prevt = NULL;

//     // // set AES Plaintext
//     // static const block OneBlock = toBlock(0, 1);
//     // static const block ZeroBlock = toBlock(0, 0);
//     // static const block TwoBlock = toBlock(0, 2);
//     // static const block ThreeBlock = toBlock(0, 3);
//     // const static block pt[4] = {ZeroBlock, OneBlock, TwoBlock, ThreeBlock};
//     // block ct[4];
//     // time(&start);
//     // std::cout<<"Running function\n";
//     // prg_eval_all_and_xor(dpfl, keynodes);
//     // time(&end);
//     // std::cout<<"Time taken with parallelization for loop with "<<samples<<" iterations:"<<std::fixed<<double(end-start)<<std::setprecision(5)<<"\n";
//     // //Printing size and level of new dpf layer which should be 4,2.
//     // std::cout<<dpfl->size<<"\n";
//     // std::cout<<dpfl->level<<"\n";
//     // std::cout<<static_cast<uint32_t>(dpfl->zt[0])<<"\n";
//     // free(keynodes);
//     // free_dpf_layer(dpfl);
    return 0;

}
