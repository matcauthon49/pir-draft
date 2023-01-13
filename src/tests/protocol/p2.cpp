#include "client.h"
#include "dpf.h"
#include "keys.h"
#include "server.h"
#include "server_trusted.h"
#include <time.h>

int main() {

    prng.SetSeed(toBlock(0, 1), sizeof(block));

    std::string ip[4] = {"127.0.0.1", "127.0.0.1", "127.0.0.1", "127.0.0.1"};
    int port[4] = {2000, 2001, 3000, 3001};

    ServerTrusted p2 = ServerTrusted(ip, port, 2);

    std::cout<<"----------------Running Key Gen-----------------\n";

    int Bout = bitlength;
    int Bin = 16;
    dpf_input_pack *dpfip[2];
    //P2 samples random index and payload which is hardcoded for now.

    dpfip[0] = (dpf_input_pack*)malloc(sizeof(dpf_input_pack));
    dpfip[1] = (dpf_input_pack*)malloc(sizeof(dpf_input_pack));
    dpfip[0]->index = GroupElement(2, Bin);
    dpfip[1]->index = GroupElement(3, Bin);
    dpfip[0]->alpha = (GroupElement*)malloc(sizeof(GroupElement));
    dpfip[0]->alpha[0] = GroupElement(15, Bout);
    dpfip[1]->alpha = (GroupElement*)malloc(sizeof(GroupElement));
    dpfip[1]->alpha[0] = GroupElement(6, Bout);

    input_check_pack_2 ip2;
    dpf_key k0, k1;

    std::tie(k0, k1) = dpf_keygen(Bin, Bout, dpfip, &ip2);
    //Sending key, index and payload to P0
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

    std::cout << "Bytes Sent: " << p2.bytes_sent << "\n";
    std::cout << "Bytes Recieved: " << p2.bytes_recieved << "\n";

    p2.close(0);
    p2.close(1);
}