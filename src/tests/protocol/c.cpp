#include "client.h"
#include "dpf.h"
#include "keys.h"
#include "server.h"
#include "server_trusted.h"
#include <algorithm>
#include <chrono>
#include<NTL/GF2E.h>
#include<NTL/GF2X.h>
#include<NTL/GF2XFactoring.h>
#include<fstream>

int main() {

    int input_size = 3;
    int entry_size = 50;
    //Initializing GF2E if entry size is 1KB.
    if(entry_size>40) {
        NTL::GF2X irredpol_13;
        irredpol_13 = NTL::BuildSparseIrred_GF2X(8);
        // std::ifstream myfile("irredpol_13_c.txt");
        // myfile>>irredpol_13;
        // myfile.close();
        // std::cout<<"Client is here\n";
        NTL::GF2E::init(irredpol_13);
    }

    std::string ip[6] = {"127.0.0.1", "127.0.0.1", "127.0.0.1", "127.0.0.1", "127.0.0.1", "127.0.0.1"};
    int port[6] = {4000, 4001, 6000, 6001, 8000, 8001};

    Client c = Client(ip, port, 3);

    auto temp0 = c.recv_uint8(0);
    auto temp1 = c.recv_uint8(1);
    auto start = std::chrono::high_resolution_clock::now();
    input_check_pack icp0 = c.recv_input_check_pack(input_size, bitlength, 0);
    input_check_pack icp1 = c.recv_input_check_pack(input_size, bitlength, 1);
    input_check_pack_2 ip2 = c.recv_input_check_pack_2(input_size, bitlength, 2);


    bool output = check_xor(bitlength, icp0, icp1, ip2);

    uint8_t out = output;

    c.send_uint8(out, 0);
    c.send_uint8(out, 1);
    auto end = std::chrono::high_resolution_clock::now();
    GroupElement index = GroupElement(3, ip2.size);    
    if (output) {
            GroupElement rotated_index = index - (ip2.index)[0] - (ip2.index)[1];
            c.send_ge(rotated_index, ip2.size, 0);
            c.send_ge(rotated_index, ip2.size, 1);

        if(entry_size<= 40) {
           GroupElement o0 = c.recv_ge(bitlength, 0);
            GroupElement ohat0 = c.recv_ge(bitlength, 0);
            GroupElement o1 = c.recv_ge(bitlength, 1);
            GroupElement ohat1 = c.recv_ge(bitlength, 1);

            if (((__uint128_t)(icp0.payload + icp1.payload).value * (o0 + o1).value & ((uint64_t(1) << bitlength) - 1)) != (ohat0 + ohat1).value) {
                std::cerr << "Incorrect dpf evaluation\n";
                return -1;
            }

            GroupElement dbout = o0 + o1;

            std::cout << "\nOutput: " << dbout.value << ".\n\n"; 
        }
        else {
            NTL::GF2E o0 = c.recv_GF2E(7, 0);
            NTL::GF2E o1 = c.recv_GF2E(7, 1);
            NTL::GF2E dbout = o0 + o1;
            std::ofstream myfile("output.txt");
            myfile<<o0<<std::endl;
            myfile<<o1<<std::endl;
            myfile<<dbout;
            myfile.close();
        }

    } else {

        std::cerr << "Inputs do not match.\n";

    }
    free_input_check_pack(icp0);
    free_input_check_pack(icp1);
    free_input_check_pack_2(ip2);

    c.close(0);    
    c.close(1);    
    c.close(2);       
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
        std::cout << "Runtime for client: " << duration.count()*1e-6 <<"\n";
        std::cout<<"C: Communication during online phase: "<<c.bytes_recieved + c.bytes_sent -2<< "\n";

}