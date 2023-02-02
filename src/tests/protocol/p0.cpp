#include "client.h"
#include "dpf.h"
#include "keys.h"
#include "server.h"
#include "server_trusted.h"
#include <chrono>

int main() {
    int input_size = 25;
    int database_size = (1<<input_size);
    GroupElement *database = new GroupElement[database_size];
    for(int i=0; i<database_size; i++)
        database[i] = GroupElement(i, bitlength);
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
    auto start = std::chrono::high_resolution_clock::now();
    GroupElement** out0 = dpf_eval_all(0, k0, &icp0);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);

    
    uint8_t temp = 4;
    p0.send_uint8(temp, 3);
    p0.send_input_check_pack(icp0, input_size, bitlength, 3);
    
    uint8_t accept = p0.recv_uint8(3);
    
    if (accept) {

        GroupElement rotated_index = p0.recv_ge(icp0.size, 3);
        auto start2 = std::chrono::high_resolution_clock::now();
        GroupElement o, hato;
        std::tie(o, hato) = inner_prod(database_size, rotated_index, database, out0);
        auto end2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end2-start2);
        std::cout << "Time taken for DB Parse-Through: " << duration.count()*1e-6 <<"\n";

        p0.send_ge(o, bitlength, 3);
        p0.send_ge(hato, bitlength, 3);

    } else {

        std::cerr << "Client rejected output.\n";

    }
    
    

    free_input_check_pack(icp0);
    free_dpf_key(k0);

    p0.close(0);
    p0.close(1);

    std::cout << "P0: Time taken for EvalAll: " << duration.count()*1e-6 <<"\n";
    std::cout << "P0: Time taken for Offline Phase: " << (duration.count() + kgtime)*1e-6 << "\n";
   
    
}