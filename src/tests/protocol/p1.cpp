#include "client.h"
#include "dpf.h"
#include "keys.h"
#include "server.h"
#include "server_trusted.h"
#include <chrono>
#include<NTL/GF2E.h>
#include<NTL/GF2X.h>
#include<NTL/GF2XFactoring.h>
#include<fstream>

int main() {
    int input_size = 3;
    int database_size = (1<<input_size);
    int entry_size = 50;

    //Creating database for both cases when entry size < 40 bits and 1KB.
    static NTL::GF2E *databaseB;
    static GroupElement *database;
    if(entry_size>bitlength) {
        NTL::GF2X irredpol = NTL::BuildSparseIrred_GF2X(8);
        // std::ifstream myfile("irredpol_13_1.txt");
        // myfile>>irredpol;
        // myfile.close();
        NTL::GF2E::init(irredpol);

        databaseB = new NTL::GF2E[database_size];
        NTL::SetSeed(NTL::conv<NTL::ZZ>((long)0));
        for(int i=0; i<database_size; i++)
        databaseB[i] = NTL::random_GF2E();
    }
    else {
        database = new GroupElement[database_size];
        for(int i=0; i<database_size; i++)
        database[i] = GroupElement(i, bitlength);
    }
    
    // std::ofstream myfile("entry1.txt");
    std::cout<<"P1: "<<databaseB[3]<<std::endl;
    // myfile<<databaseB[5]<<std::endl;
    // myfile.close();  

    std::string ip[2] = {"127.0.0.1", "127.0.0.1"};
    int port[2] = {3000, 3001};
    std::string ipq[2] = {"127.0.0.1", "127.0.0.1"};
    int portq[2] = {6000, 6001};

    Server p1 = Server(ip, port, 1);
    p1.connect_to_client(ipq, portq);
    //Receive DPF key from P2
    GroupElement index = p1.recv_ge(input_size, 2);
    GroupElement payload = p1.recv_ge(bitlength, 2);
    dpf_key k1 = p1.recv_dpf_key(bitlength, 2);
    input_check_pack icp1;
    icp1.index = index;
    icp1.payload = payload;

    //Eval all
    uint8_t* t = (uint8_t*)malloc(database_size*sizeof(uint8_t));
    auto start = std::chrono::high_resolution_clock::now();
    GroupElement** out1 = dpf_eval_all(1, k1, &icp1, t);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);

    // std::ofstream myfile("entry1.txt", std::ios::app);
    //     myfile<<icp1.index.value<<std::endl;
    // for(int i=0; i<8; i++)
    //     myfile<<unsigned(t[i])<<std::endl;
    // myfile.close();
    uint8_t temp = 4;
    p1.send_uint8(temp, 3);
    auto start_online = std::chrono::high_resolution_clock::now();
    p1.send_input_check_pack(icp1, input_size, bitlength, 3);

    uint8_t accept = p1.recv_uint8(3);


    if (accept) {
        GroupElement rotated_index = p1.recv_ge(icp1.size, 3);
        GroupElement hato;
        if(entry_size<=40) {
            auto start2 = std::chrono::high_resolution_clock::now();
            GroupElement o;
            std::tie(o, hato) = inner_prod(database_size, rotated_index, database, out1);
            auto end2 = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end2-start2);
            std::cout << "Time taken for DB Parse-Through: " << duration.count()*1e-6 <<"\n";
            p1.send_ge(o, bitlength, 3);
            p1.send_ge(hato, bitlength, 3);
        }
        else {
            auto start2 = std::chrono::high_resolution_clock::now();
            NTL::GF2E o = compute_o(database_size, rotated_index, databaseB, t, 1);
            auto end2 = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end2-start2);
            std::cout << "Time taken for DB Parse-Through: " << duration.count()*1e-6 <<"\n";
            std::ofstream myfile("entry1.txt");
            myfile<<o;
            myfile.close();
            std::cout<<"Deg: "<<NTL::deg(NTL::conv<NTL::GF2X>(o))<<"\n";
            p1.send_GF2E(o, 7, 3);
        }

    } else {

        std::cerr << "Client rejected output.\n";

    }

    auto end_online = std::chrono::high_resolution_clock::now();
    auto duration_online = std::chrono::duration_cast<std::chrono::microseconds>(end_online-start_online);

    free_input_check_pack(icp1);
    free_dpf_key(k1);

    p1.close(0);
    p1.close(1);
    std::cout << "P1: Time taken for EvalAll: " << duration.count()*1e-6 <<"\n";
     std::cout<<"P1: Time Taken for Online Phase: "<<duration_online.count()*1e-6 <<"\n";

        
}