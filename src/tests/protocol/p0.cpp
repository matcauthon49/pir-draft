#include "client.h"
#include "dpf.h"
#include "keys.h"
#include "server.h"
#include "server_trusted.h"
#include <chrono>


int main() {
    auto start2 = std::chrono::high_resolution_clock::now();
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
            for(int j=0; j<database_size; j++) {
                databaseB[i][j] = random_ge(bitlength);
        }

        }
    }
    auto end2 = std::chrono::high_resolution_clock::now();
            auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end2-start2);
            std::cout<<duration2.count()*1e-6<<'\n';

    std::string ip[2] = {"127.0.0.1", "127.0.0.1"};
    int port[2] = {2000, 2001};
    std::string ipp[2] = {"127.0.0.1", "127.0.0.1"};
    int portp[2] = {4000, 4001};

    Server p0 = Server(ip, port, 0);
    p0.connect_to_client(ipp, portp);

    //Receive DPF key from P2
    GroupElement index  = p0.recv_ge(input_size, 2);
    GroupElement payload = p0.recv_ge(bitlength, 2);
    dpf_key k0 = p0.recv_dpf_key(bitlength, 2);
    int kgtime = p0.recv_int(2);

    input_check_pack icp0;
    icp0.index = index;
    icp0.payload = payload;

    //Eval All
    uint8_t* t = (uint8_t*)malloc(database_size*sizeof(uint8_t));
    auto start = std::chrono::high_resolution_clock::now();
    GroupElement** out0 = dpf_eval_all(0, k0, &icp0, t);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);

    
    uint8_t temp = 4;
    p0.send_uint8(temp, 3);
    p0.send_input_check_pack(icp0, input_size, bitlength, 3);
    
    uint8_t accept = p0.recv_uint8(3);
    
    if (accept) {
        GroupElement rotated_index = p0.recv_ge(icp0.size, 3);
        if(entry_size<=bitlength) {
            auto start2 = std::chrono::high_resolution_clock::now();
            GroupElement o, hato;
            std::tie(o, hato) = inner_prod(database_size, rotated_index, database, out0);
            auto end2 = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end2-start2);

            p0.send_ge(o, bitlength, 3);
            p0.send_ge(hato, bitlength, 3);
        }
        else {
            auto start2 = std::chrono::high_resolution_clock::now();
            GroupElement *o, *hato;
            std::tie(o, hato) = compute_inner_prod_Zp(database_size, rotated_index, databaseB, out0, block, 0);
            
            for(int i=0; i<block; i++) {
                p0.send_ge(o[i], bitlength, 3);
            }

            for(int i=0; i<block; i++) {
                p0.send_ge(hato[i], bitlength, 3);
            }
            auto end2 = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end2-start2);
            
            
        }

    } else {

        std::cerr << "Client rejected output.\n";

    }
    
    

    free_input_check_pack(icp0);
    free_dpf_key(k0);
    if(database != NULL) free(database);
    if(databaseB != NULL) {
        for(int i=0; i<block; i++)
            free(databaseB[i]);
        free(databaseB);
    }
    p0.close(0);
    p0.close(1);

    std::cout << "P0: Time taken for EvalAll: " << duration.count()*1e-6 <<"\n";
    std::cout << "P0: Time taken for Offline Phase: " << (duration.count() + kgtime)*1e-6 << "\n";
   
    
}