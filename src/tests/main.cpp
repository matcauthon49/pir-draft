#include <dpf.h>
#include <bits/stdc++.h>
#include <cryptoTools/Crypto/PRNG.h>
#include <chrono>
#include <algorithm>
#include<fstream>

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
    prng.SetSeed(toBlock(0, 0), sizeof(block));
    int Bin = 20;
    int Bout = 40;
    int database_size = (1<<Bin);
    int entry_size = 8192;
    bitlength = Bout;
    int blocks;
    if(entry_size%bitlength != 0) blocks = entry_size/bitlength + 1;
    else blocks = entry_size/bitlength;
    std::cout<<blocks<<"\n";
    //Populating database.
    GroupElement **database = (GroupElement**)malloc(blocks*sizeof(GroupElement*));
    for(int i=0; i<blocks; i++) {
        database[i] = (GroupElement*)malloc(database_size*sizeof(GroupElement));
        for(int j=0; j<database_size; j++) {
            database[i][j] = random_ge(bitlength);
            // std::cout<<database[i][j].value<<" ";
        }
        // std::cout<<"\n\n\n\n";
    }

    // std::cout<<database[2][0].value<<" "<<database[2][1].value<<"\n";

    std::cout<<"----------------Running Key Gen-----------------\n";
    prng.SetSeed(toBlock(0, 0), sizeof(block));

    
    dpf_input_pack *dpfip[2];
    dpfip[0] = (dpf_input_pack*)malloc(sizeof(dpf_input_pack));
    dpfip[1] = (dpf_input_pack*)malloc(sizeof(dpf_input_pack));
    dpfip[0]->index = GroupElement(0, Bin);
    dpfip[1]->index = GroupElement(2, Bin);
    dpfip[0]->alpha = (GroupElement*)malloc(sizeof(GroupElement));
    dpfip[0]->alpha[0] = GroupElement(2, Bout);
    dpfip[1]->alpha = (GroupElement*)malloc(sizeof(GroupElement));
    dpfip[1]->alpha[0] = GroupElement(10, Bout);
    input_check_pack_2 ip2;
    dpf_key k0, k1;
    auto start = std::chrono::high_resolution_clock::now();
    std::tie(k0, k1) = dpf_keygen(Bin, Bout, dpfip, &ip2);
    auto end  = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);

    std::cout<<"--------------------Running Eval All-------------\n";
    GroupElement **t_vec_0, **t_vec_1;
    input_check_pack icp0, icp1;
    icp0.index = (ip2.index)[0];
    icp1.index = (ip2.index)[1];
    icp0.payload = (ip2.payload)[0];
    icp1.payload = (ip2.payload)[1];
    uint8_t *t0, *t1;
    t0 = (uint8_t*)malloc((1<<Bin)*sizeof(uint8_t));
    t1 = (uint8_t*)malloc((1<<Bin)*sizeof(uint8_t));
    start = std::chrono::high_resolution_clock::now();
    t_vec_0 = dpf_eval_all(0, k0, &icp0, t0);
    end  = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
    t_vec_1 = dpf_eval_all(1, k1, &icp1, t1);

    for(int i=0; i<(1<<Bin); i++) {
        if((t_vec_0[i][0]+t_vec_1[i][0]).value != 0 || (t_vec_0[i][1]+t_vec_1[i][1]).value != 0) {
           std::cout<<i<<"\n";
           std::cout<<(t_vec_0[i][0] + t_vec_1[i][0]).value<<" "<<(t_vec_0[i][1] + t_vec_1[i][1]).value<<"\n";
        }
        if(t0[i]^t1[i]) std::cout<<"this "<<i<<"\n";

    }

    //Computing output now differently.
    GroupElement *o0, *o1, *hato0, *hato1;
    // GroupElement to0, to1, hatto0, hatto1;
    start = std::chrono::high_resolution_clock::now();
    std::tie(o0, hato0) = compute_inner_prod_Zp(database_size, GroupElement(0, Bin), database, t_vec_0, blocks, 0);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
    std::cout<<"Time taken for parsing "<<duration.count()*1e-6<<"\n";
    std::tie(o1, hato1) = compute_inner_prod_Zp(database_size, GroupElement(0, Bin), database, t_vec_1, blocks, 1);

    // for(int i=0; i<blocks; i++) {
    //     std::cout<<i<<" "<<(o0[i]+o1[i]).value<<" "<<(hato0[i] + hato1[i]).value<<"\n";
    // }
    // std::cout<<"\n\n\n";
    bool check = 1;
    for(int j=0; j<blocks; j++) {
        check &= (((__uint128_t)(icp0.payload + icp1.payload).value * (o0[j] + o1[j]).value & ((uint64_t(1) << bitlength) - 1)) == (hato0[j] + hato1[j]).value);
        // std::cout<<i<<" "<<check<<"\n";
    }

    std::cout<<check<<"\n";
    //Output lies in xors. We build that first.
    // GroupElement hato0, hato1;
    // NTL::GF2E o0, o1, dbout;
    // o0 = compute_o(database_size, GroupElement(0, Bin), databaseB, t0, 0);
    // o1 = compute_o(database_size, GroupElement(0, Bin), databaseB, t1, 1);
    // dbout = o0+o1;
    // std::cout<<dbou/t<<"\n";

    // hato0 = compute_hato(database_size, GroupElement(0, Bin), db, t_vec_0, 0);
    // hato1 = compute_hato(database_size, GroupElement(0, Bin), db, t_vec_1, 1);
    // std::cout<<"hato0: "<<hato0.value<<"\nhato1: "<<hato1.value<<"\n";
    // GroupElement hasho = transformelem(dbout, mu, v);
    // std::cout<<"hasho: "<<hasho.value<<"\n";
    // std::cout<<(uint64_t)hasho.value<<"\n";
    // std::cout<<(((((__uint128_t)icp0.payload.value + icp1.payload.value)*(hasho.value)) & ((__uint128_t(1)<<Bout) - 1)) == (hato0+hato1).value);
    // delete[] databaseB;
    // delete[] db;
    // std::cout << "Time taken in eval all: " << duration.count()*1e-6 <<"\n";

    // GroupElement o0, hato0, o1, hato1;
    // start = std::chrono::high_resolution_clock::now();
    // std::tie(o0, hato0) = inner_prod(database_size, GroupElement(0, Bin), database, t_vec_0);
    // end  = std::chrono::high_resolution_clock::now();
    // duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);

    // std::tie(o1, hato1) = inner_prod(database_size, GroupElement(0, Bin), database, t_vec_1);
    // std::cout << "Time taken in inner product: " << duration.count()*1e-6 <<"\n";
    // // total_time += duration.count();
    // std::cout<<"Output: "<<(o0 + o1).value<<"\n";

// std::cout<<"Inner product average: "<<total_time*1e-7<<"\n";
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
