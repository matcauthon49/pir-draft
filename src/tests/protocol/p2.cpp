#include "client.h"
#include "dpf.h"
#include "keys.h"
#include "server.h"
#include "server_trusted.h"
#include <time.h>

int main() {

    GroupElement *database;

    prng.SetSeed(toBlock(0, 1), sizeof(block));

    std::string ip[4] = {"127.0.0.1", "127.0.0.1", "127.0.0.1", "127.0.0.1"};
    int port[4] = {2000, 2001, 3000, 3001};
    std::string ipr[2] = {"127.0.0.1", "127.0.0.1"};
    int portr[2] = {6000, 6001};

    ServerTrusted p2 = ServerTrusted(ip, port, 2);

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

    clock_t begin = clock();
    std::tie(k0, k1) = dpf_keygen(Bin, Bout, dpfip, &ip2);
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    std::cout << "TIME TAKEN FOR KEYGEN: " << elapsed_secs << "\n";

    //Sending key, index and payload to P2
    p2.send_ge(dpfip[0]->index, Bin, 0);
    p2.send_ge(dpfip[0]->alpha[0], Bout, 0);
    p2.send_dpf_key(k0, Bin, Bout, 0);

    //Sending key, index and payload to P1
    p2.send_ge(dpfip[1]->index, Bin, 1);
    p2.send_ge(dpfip[1]->alpha[0], Bout, 1);
    p2.send_dpf_key(k1, Bin, Bout, 1);

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

    p2.connect_to_client(ipr, portr);

    p2.send_input_check_pack_2(ip2, bitlength, bitlength, 2);

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

    std::cout << "Bytes Sent: " << p2.bytes_sent << "\n";
    std::cout << "Bytes Recieved: " << p2.bytes_recieved << "\n";

    // free_dpf_input_pack(dpfip[0]);
    // free_dpf_input_pack(dpfip[1]);
    // free_input_check_pack_2(ip2);
    // free_dpf_key(k0);
    // free_dpf_key(k1);

    p2.close(0);
    p2.close(1);
    p2.close(2);
}