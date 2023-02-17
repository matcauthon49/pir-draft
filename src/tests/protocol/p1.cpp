#include "client.h"
#include "dpf.h"
#include "keys.h"
#include "server.h"
#include "server_trusted.h"
#include <chrono>
#include<fstream>

int main() {
    int input_size = 18;
    int database_size = (1<<input_size);
    int entry_size = 8192;

    int block;
    if(entry_size%bitlength!=0) block = entry_size/bitlength + 1;
    else block = entry_size/bitlength;
    prng.SetSeed(toBlock(0, 0), sizeof(block));

    //Creating database for both cases when entry size < 40 bits and 1KB.
    GroupElement *database = NULL;
    GroupElement **databaseB = NULL;
    if(entry_size<= bitlength) {
        database = new GroupElement[database_size];
        for(int i=0; i<database_size; i++)
            database[i] = random_ge(bitlength);
    }
    else {
        databaseB = (GroupElement**)malloc(block*sizeof(GroupElement*));
        for(int i=0; i<block; i++) {
            databaseB[i] = (GroupElement*)malloc(database_size*sizeof(GroupElement));
            for(int j=0; j<database_size; j++)
                databaseB[i][j] = random_ge(bitlength);
        }

    }


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

    uint8_t temp = 4;
    p1.send_uint8(temp, 3);
    auto start_online = std::chrono::high_resolution_clock::now();
    p1.send_input_check_pack(icp1, input_size, bitlength, 3);

    uint8_t accept = p1.recv_uint8(3);

    if (accept) {
        GroupElement rotated_index = p1.recv_ge(icp1.size, 3);
        if(entry_size<=bitlength) {
            auto start2 = std::chrono::high_resolution_clock::now();
            GroupElement o, hato;
            std::tie(o, hato) = inner_prod(database_size, rotated_index, database, out1);
            auto end2 = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end2-start2);
            std::cout << "Time taken for DB Parse-Through: " << duration.count()*1e-6 <<"\n";
            p1.send_ge(o, bitlength, 3);
            p1.send_ge(hato, bitlength, 3);
        }
        else {
            auto start2 = std::chrono::high_resolution_clock::now();
            auto start_0 = std::chrono::high_resolution_clock::now();
            GroupElement *o, *hato;
            std::tie(o, hato) = compute_inner_prod_Zp(database_size, rotated_index, databaseB, out1, block, 0);
            auto end_0 = std::chrono::high_resolution_clock::now();
            auto dur_0 = std::chrono::duration_cast<std::chrono::microseconds>(end_0-start_0);

            //Sending o0, hato0 to C
            auto start_1 = std::chrono::high_resolution_clock::now();
            for(int i=0; i<block; i++) {
                p1.send_ge(o[i], bitlength, 3);
            }
            for(int i=0; i<block; i++) {
                p1.send_ge(hato[i], bitlength, 3);
            }
            auto end_1 = std::chrono::high_resolution_clock::now();
            auto dur_1 = std::chrono::duration_cast<std::chrono::microseconds>(end_1-start_1);

            auto end2 = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end2-start2);

            auto end_online = std::chrono::high_resolution_clock::now();
            auto duration_online = std::chrono::duration_cast<std::chrono::microseconds>(end_online-start_online);

            std::cout << "P1: Time taken for EvalAll: " << duration.count()*1e-6 <<"\n";
            std::cout<<"Time taken for computing o1 "<<dur_0.count()*1e-6<<"\n";
            std::cout<<"Time taken for sending o1 "<<dur_1.count()*1e-6<<"\n";
            std::cout << "Time taken for DB Parse-Through: " << duration.count()*1e-6 <<"\n"; 
            std::cout<<"P1: Time Taken for Online Phase: "<<duration_online.count()*1e-6 <<"\n";
        }

    } else {

        std::cerr << "Client rejected output.\n";

    }


    free_input_check_pack(icp1);
    free_dpf_key(k1);
    if(database != NULL)  free(database);
    if(databaseB != NULL) {
        for(int i=0; i<block; i++)
            free(databaseB[i]);
        free(databaseB);
    }
    p1.close(0);
    p1.close(1);
    

        
}