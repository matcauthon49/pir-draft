#include "client.h"
#include "dpf.h"
#include "keys.h"
#include "server.h"
#include "server_trusted.h"
#include <time.h>

PRNG prng;

int main() {

    prng.SetSeed(toBlock(0, 0), sizeof(block));

    std::string ip[4] = {"127.0.0.1", "127.0.0.1", "127.0.0.1", "127.0.0.1"};
    int port[4] = {2000, 2001, 3000, 3001};

    ServerTrusted p2 = ServerTrusted(ip, port, 2);

    // std::string ipp[2] = {"127.0.0.1", "127.0.0.1"};
    // int portp[2] = {4020, 4021};

    // p2.wait_for_client(ipp, portp);    
    // p2.close(0);
    // p2.close(1);
    // p2.close(2);

    AES *input = new AES[2];

    AES aesSeed(toBlock(1, time(NULL)));

    block seed0 = aesSeed.ecbEncBlock(ZeroBlock);
    p2.send_block(seed0, PARTY0);

    block seed1 = aesSeed.ecbEncBlock(OneBlock);
    // p2.send_block(seed1, PARTY1);

    input[0] = AES(seed0);
    input[1] = AES(seed1);

    std::cout << input[0].ecbEncBlock(ZeroBlock) << "\n";
    std::cout << input[1].ecbEncBlock(ZeroBlock) << "\n";

    std::cout<<"----------------Running Key Gen-----------------\n";

    int Bout = bitlength;
    int Bin = 16;
    dpf_input_pack *dpfip[2];
    dpfip[0] = (dpf_input_pack*)malloc(sizeof(dpf_input_pack));
    dpfip[1] = (dpf_input_pack*)malloc(sizeof(dpf_input_pack));
    dpfip[0]->index = GroupElement(2, Bin);
    dpfip[1]->index = GroupElement(3, Bin);
    dpfip[0]->alpha = (GroupElement*)malloc(sizeof(GroupElement));
    dpfip[0]->alpha[0] = GroupElement(15, Bout);
    dpfip[1]->alpha = (GroupElement*)malloc(sizeof(GroupElement));
    dpfip[1]->alpha[0] = GroupElement(6, Bout);
    // std::cout<<"Here\n";
    dpf_key k0, k1;
    // std::tie(k0, k1) = dpf_keygen(Bin, Bout, dpfip);

    p2.send_dpf_key(k0, bitlength, 0);

    // GroupElement g = GroupElement(10);
    // p2.send_ge(g, bitlength, 0);


    std::cout << k0.gamma[0] << "\n";
    std::cout << k0.gamma[1] << "\n";

    std::cout << "Bytes Sent: " << p2.bytes_sent << "\n";
    std::cout << "Bytes Recieved: " << p2.bytes_recieved << "\n";

    p2.close(0);
    p2.close(1);
}