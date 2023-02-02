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
    auto start = std::chrono::high_resolution_clock::now();
    GroupElement** out1 = dpf_eval_all(1, k1, &icp1);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);

    uint8_t temp = 4;
    p1.send_uint8(temp, 3);
    auto start_online = std::chrono::high_resolution_clock::now();
    p1.send_input_check_pack(icp1, input_size, bitlength, 3);

    uint8_t accept = p1.recv_uint8(3);


    if (accept) {

        GroupElement rotated_index = p1.recv_ge(icp1.size, 3);
        auto start2 = std::chrono::high_resolution_clock::now();
        GroupElement o, hato;
        std::tie(o, hato) = inner_prod(database_size, rotated_index, database, out1);
        auto end2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end2-start2);
        std::cout << "Time taken for DB Parse-Through: " << duration.count()*1e-6 <<"\n";
        p1.send_ge(o, bitlength, 3);
        p1.send_ge(hato, bitlength, 3);

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