#include "client.h"
#include "dpf.h"
#include "keys.h"
#include "server.h"
#include "server_trusted.h"
#include <chrono>

int main() {

    prng.SetSeed(toBlock(0, time(NULL)), sizeof(block));

    std::string ip[4] = {"127.0.0.1", "127.0.0.1", "127.0.0.1", "127.0.0.1"};
    int port[4] = {2000, 2001, 3000, 3001};
    std::string ipr[2] = {"127.0.0.1", "127.0.0.1"};
    int portr[2] = {8000, 8001};
    ServerTrusted p2 = ServerTrusted(ip, port, 2);
    p2.connect_to_client(ipr, portr);
    std::cout<<"----------------Running Key Gen-----------------\n";

    int Bout = bitlength;
    int Bin = 20;

    dpf_input_pack *dpfip[2];
    //P2 samples random index and payload which is hardcoded for now.

    dpfip[0] = (dpf_input_pack*)malloc(sizeof(dpf_input_pack));
    dpfip[1] = (dpf_input_pack*)malloc(sizeof(dpf_input_pack));
    // dpfip[0]->index = GroupElement(3, Bin);
    // dpfip[1]->index = GroupElement(4, Bin);
    dpfip[0]->index = random_ge(Bin);
    dpfip[1]->index = random_ge(Bin);
    dpfip[0]->alpha = (GroupElement*)malloc(sizeof(GroupElement));
    // dpfip[0]->alpha[0] = GroupElement(2, Bout);
    dpfip[0]->alpha[0] = random_ge(Bout);
    dpfip[1]->alpha = (GroupElement*)malloc(sizeof(GroupElement));
    // dpfip[1]->alpha[0] = GroupElement(10, Bout);
    dpfip[1]->alpha[0] = random_ge(Bout);

    input_check_pack_2 ip2;
    dpf_key k0, k1;

    auto start = std::chrono::high_resolution_clock::now();
    std::tie(k0, k1) = dpf_keygen(Bin, Bout, dpfip, &ip2);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
    int kgtime = static_cast<int>(duration.count());

    auto start_send = std::chrono::high_resolution_clock::now();
    //Sending key, index and payload to P2
    p2.send_ge(dpfip[0]->index, Bin, 0);
    p2.send_ge(dpfip[0]->alpha[0], Bout, 0);
    p2.send_dpf_key(k0, Bin, Bout, 0);
    p2.send_int(kgtime, 0);
    //Sending key, index and payload to P1
    p2.send_ge(dpfip[1]->index, Bin, 1);
    p2.send_ge(dpfip[1]->alpha[0], Bout, 1);
    p2.send_dpf_key(k1, Bin, Bout, 1);
    auto end_send = std::chrono::high_resolution_clock::now();
    auto duration_send = std::chrono::duration_cast<std::chrono::microseconds>(end_send-start_send);
    auto preprocess_comm = p2.bytes_sent;

    p2.send_input_check_pack_2(ip2, Bin, bitlength, 2);


    p2.close(0);
    p2.close(1);
    p2.close(2);
    std::cout << "P2: Time taken for keygen + sending to P0, P1: " << duration.count()*1e-6 + duration_send.count() * 1e-6 <<"\n";
    std::cout<< "P2: Bytes sent in offline phase: " << preprocess_comm << "\n";



}