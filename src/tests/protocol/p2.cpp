#include "client.h"
#include "dpf.h"
#include "keys.h"
#include "server.h"
#include "server_trusted.h"
#include <chrono>

int main() {

    GroupElement *database;

    prng.SetSeed(toBlock(0, time(NULL)), sizeof(block));

    std::string ip[4] = {"127.0.0.1", "127.0.0.1", "127.0.0.1", "127.0.0.1"};
    int port[4] = {2000, 2001, 3000, 3001};
    std::string ipr[2] = {"127.0.0.1", "127.0.0.1"};
    int portr[2] = {8000, 8001};
    ServerTrusted p2 = ServerTrusted(ip, port, 2);
    p2.connect_to_client(ipr, portr);
    std::cout<<"----------------Running Key Gen-----------------\n";

    int Bout = bitlength;
    int Bin = 16;

    dpf_input_pack *dpfip[2];
    //P2 samples random index and payload which is hardcoded for now.

    dpfip[0] = (dpf_input_pack*)malloc(sizeof(dpf_input_pack));
    dpfip[1] = (dpf_input_pack*)malloc(sizeof(dpf_input_pack));
    dpfip[0]->index = GroupElement(65000, Bin);
    dpfip[1]->index = GroupElement(500, Bin);
    dpfip[0]->alpha = (GroupElement*)malloc(sizeof(GroupElement));
    dpfip[0]->alpha[0] = GroupElement(100, Bout);
    dpfip[1]->alpha = (GroupElement*)malloc(sizeof(GroupElement));
    dpfip[1]->alpha[0] = GroupElement(100, Bout);

    input_check_pack_2 ip2;
    dpf_key k0, k1;

    auto start = std::chrono::high_resolution_clock::now();
    std::tie(k0, k1) = dpf_keygen(Bin, Bout, dpfip, &ip2);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);

    

    auto start_send = std::chrono::high_resolution_clock::now();
    //Sending key, index and payload to P2
    p2.send_ge(dpfip[0]->index, Bin, 0);
    p2.send_ge(dpfip[0]->alpha[0], Bout, 0);
    p2.send_dpf_key(k0, Bin, Bout, 0);

    //Sending key, index and payload to P1
    p2.send_ge(dpfip[1]->index, Bin, 1);
    p2.send_ge(dpfip[1]->alpha[0], Bout, 1);
    p2.send_dpf_key(k1, Bin, Bout, 1);
    auto end_send = std::chrono::high_resolution_clock::now();
    auto duration_send = std::chrono::duration_cast<std::chrono::microseconds>(end_send-start_send);
    auto preprocess_comm = p2.bytes_sent;
    // std::cout << "P2: Time taken: " << duration_send.count()*1e-6 <<"\n";

    // // std::cout<<"index "<<index.value<<" payload "<<payload<<"\n";
    // std::cout<<"Bout "<<k0.Bout<<" height "<<k0.height<<" s "<<k0.s<<"\n";
    // for(int i=0; i<k0.height; i++)
    //     std::cout<<"i "<<i<<" sigma "<<(k0.sigma)[i]<<" tau0 "<<static_cast<uint32_t>((k0.tau0)[i])<<" tau1 "<<static_cast<uint32_t>((k0.tau1)[i])<<"\n";
    // std::cout<<"\ngamma0 "<<(k0.gamma)[0].value<<" gamma1 "<<(k0.gamma)[1].value<<"\n";

    // std::cout<<"Bout "<<k1.Bout<<" height "<<k1.height<<" s "<<k1.s<<"\n";
    // for(int i=0; i<k1.height; i++)
    //     std::cout<<"i "<<i<<" sigma "<<(k1.sigma)[i]<<" tau0 "<<static_cast<uint32_t>((k1.tau0)[i])<<" tau1 "<<static_cast<uint32_t>((k1.tau1)[i])<<"\n";
    // std::cout<<"\ngamma0 "<<(k1.gamma)[0].value<<" gamma1 "<<(k1.gamma)[1].value<<"\n";
    // //Eval all
    // std::cout<<"Here 2\n";
    // auto temp0 = p2.recv_uint8(0);
    // auto temp1 = p2.recv_uint8(1);
    // std::cout<<"temp received from P0: "<< static_cast<uint32_t>(temp0) << "\n";
    // std::cout<<"Here 2\n";
    p2.send_input_check_pack_2(ip2, Bin, bitlength, 2);

    // std::cout << "P2 Index: " << ip2.index << "\n";
    // std::cout << "P2 Payload: " << ip2.payload << "\n";

    // std::cout << "P2 zs0[0]: " << "\n";
    // for (int i = 0; i < ip2.size; i++) {
    //     std::cout << ip2.zs0[0][i] << "\n";
    // }
    // std::cout << "P2 zs0[1]: " << "\n";
    // for (int i = 0; i < ip2.size; i++) {
    //     std::cout << ip2.zs0[1][i] << "\n";
    // }

    // std::cout << "P2 zs1[0]: " << "\n";
    // for (int i = 0; i < ip2.size; i++) {
    //     std::cout << ip2.zs1[0][i] << "\n";
    // }
    // std::cout << "P2 zs1[1]: " << "\n";
    // for (int i = 0; i < ip2.size; i++) {
    //     std::cout << ip2.zs1[1][i] << "\n";
    // }

    // std::cout << "P2 zt0[0]: ";
    // for (int i = 0; i < ip2.size; i++) {
    //     std::cout << unsigned(ip2.zt0[0][i]);
    // }
    // std::cout << "\n";
    // std::cout << "P2 zt0[1]: ";
    // for (int i = 0; i < ip2.size; i++) {
    //     std::cout << unsigned(ip2.zt0[1][i]);
    // }
    // std::cout << "\n";
    // std::cout << "P2 zt1[0]: ";
    // for (int i = 0; i < ip2.size; i++) {
    //     std::cout << unsigned(ip2.zt1[0][i]);
    // }
    // std::cout << "\n";
    // std::cout << "P2 zt1[1]: ";
    // for (int i = 0; i < ip2.size; i++) {
    //     std::cout << unsigned(ip2.zt1[1][i]);
    // }

    // std::cout << "\n";
    // std::cout << "P2 Sigma: " << "\n";
    // for (int i = 0; i < ip2.size; i++) {
    //     std::cout << ip2.sigma[i] << "\n";
    // }
    // std::cout << "P2 Tau0: ";
    // for (int i = 0; i < ip2.size; i++) {
    //     std::cout << unsigned(ip2.tau[0][i]);
    // }
    // std::cout << "\n";
    // std::cout << "P2 Tau1: ";
    // for (int i = 0; i < ip2.size; i++) {
    //     std::cout << unsigned(ip2.tau[1][i]);
    // }
    // std::cout << "\n";
    // std::cout << "P2 T: " << ip2.T << "\n";
    // std::cout << "P2 W: " << ip2.W[0] << " " << ip2.W[1] << "\n";
    // std::cout << "P2 Gamma: " << ip2.gamma[0] << " " << ip2.gamma[1] << "\n";

    // std::cout << "Bytes Sent: " << p2.bytes_sent << "\n";
    // std::cout << "Bytes Recieved: " << p2.bytes_recieved << "\n";

    // free_dpf_input_pack(dpfip[0]);
    // free_dpf_input_pack(dpfip[1]);
    // free_input_check_pack_2(ip2);
    // free_dpf_key(k0);
    // free_dpf_key(k1);

    p2.close(0);
    p2.close(1);
    p2.close(2);
    std::cout << "P2: Time taken for keygen + sending to P0, P1: " << duration.count()*1e-6 + duration_send.count() * 1e-6 <<"\n";
    std::cout<< "P2: Bytes sent in offline phase: " << preprocess_comm << "\n";



}