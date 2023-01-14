#include "client.h"
#include "dpf.h"
#include "keys.h"
#include "server.h"
#include "server_trusted.h"

int main() {
    std::string ip[2] = {"127.0.0.1", "127.0.0.1"};
    int port[2] = {2000, 2001};
    std::string ipp[2] = {"127.0.0.1", "127.0.0.1"};
    int portp[2] = {4000, 4001};

    Server p0 = Server(ip, port, 0);

    // std::string ipp[2] = {"127.0.0.1", "127.0.0.1"};
    // int portp[2] = {4000, 4001};

//     p0.wait_for_client(ipp, portp);
//     p0.close(0);
//     p0.close(1);

//P2 does not need to send S_0 separately, it is part of the key.

    // block x = p0.recv_block(PARTY_TRUSTED);
    // AES w = AES(x);

    // std::cout << w.ecbEncBlock(ZeroBlock) << "\n";

    //Receive DPF key from P2
    // dpf_key k0 = p0.recv_dpf_key(bitlength, 2);

    // std::cout << "Height = " << k0.height << "\n";
    // std::cout << "Group Size = " << k0.groupSize << "\n";
    // std::cout << "Bout = " << k0.Bout << "\n";
    // std::cout << "S = " << k0.s << "\n";
    // std::cout << "T = " << unsigned(k0.t) << "\n";

    // std::cout << ":: Sigma ::" << "\n";
    // for (int i = 0; i < k0.height; i++) {
    //     std::cout << k0.sigma[i] << "\n";
    // }

    // std::cout << ":: Tau0 ::" << "\n";
    // for (int i = 0; i < k0.height; i++) {
    //     std::cout << unsigned(k0.tau0[i]);
    // }

    // std::cout << "\n";

    // std::cout << ":: Tau1 ::" << "\n";
    // for (int i = 0; i < k0.height; i++) {
    //     std::cout << unsigned(k0.tau1[i]);
    // }

    // std::cout << "\n";

    // // std::cout << ":: Gamma ::" << "\n";
    // // for (int i = 0; i < 2; i++) {
    // //     std::cout << k0.gamma[i];
    // // }

    // std::cout << k0.gamma[0] << "\n" << k0.gamma[1] << "\n";



    //Receive DPF key from P2
    GroupElement index  = p0.recv_ge(16, 2);
    GroupElement payload = p0.recv_ge(bitlength, 2);
    dpf_key k0 = p0.recv_dpf_key(bitlength, 2);

    //For checking key obtained is correct
    // std::cout<<"index "<<index.value<<" payload "<<payload<<"\n";
    // std::cout<<"Bout "<<k0.Bout<<" height "<<k0.height<<" s "<<k0.s<<"\n";
    // for(int i=0; i<k0.height; i++)
    //     std::cout<<"i "<<i<<" sigma "<<(k0.sigma)[i]<<" tau0 "<<static_cast<uint32_t>((k0.tau0)[i])<<" tau1 "<<static_cast<uint32_t>((k0.tau1)[i])<<"\n";
    // std::cout<<"\ngamma0 "<<(k0.gamma)[0].value<<" gamma1 "<<(k0.gamma)[1].value<<"\n";

    input_check_pack icp0;
    icp0.index = index;
    icp0.payload = payload;

    //Eval All
    GroupElement** out0 = dpf_eval_all(0, k0, &icp0);
    // std::cout<<"out "<<out0[5][0]<<" "<<out0[5][1]<<"\n";

    p0.connect_to_client(ipp, portp);
    p0.send_input_check_pack(icp0, bitlength, bitlength, 3);

    
    // Print ICP
    // std::cout << "P0 Index: " << icp0.index << "\n";
    // std::cout << "P0 Payload: " << icp0.payload << "\n";

    // std::cout << "P0 zs[0]: " << "\n";
    // for (int i = 0; i < icp0.size; i++) {
    //     std::cout << icp0.zs[0][i] << "\n";
    // }
    // std::cout << "P0 zs[1]: " << "\n";
    // for (int i = 0; i < icp0.size; i++) {
    //     std::cout << icp0.zs[1][i] << "\n";
    // }

    // std::cout << "P0 zt[0]: ";
    // for (int i = 0; i < icp0.size; i++) {
    //     std::cout << unsigned(icp0.zt[0][i]);
    // }
    // std::cout << "\n";
    // std::cout << "P0 zt[1]: ";
    // for (int i = 0; i < icp0.size; i++) {
    //     std::cout << unsigned(icp0.zt[1][i]);
    // }
    // std::cout << "\n";
    // std::cout << "P0 Sigma: " << "\n";
    // for (int i = 0; i < icp0.size; i++) {
    //     std::cout << icp0.sigma[i] << "\n";
    // }
    // std::cout << "P0 Tau0: ";
    // for (int i = 0; i < icp0.size; i++) {
    //     std::cout << unsigned(icp0.tau[0][i]);
    // }
    // std::cout << "\n";
    // std::cout << "P0 Tau1: ";
    // for (int i = 0; i < icp0.size; i++) {
    //     std::cout << unsigned(icp0.tau[1][i]);
    // }
    // std::cout << "\n";
    // std::cout << "P0 T: " << icp0.T << "\n";
    // std::cout << "P0 W: " << icp0.W[0] << " " << icp0.W[1] << "\n";
    // std::cout << "P0 Gamma: " << icp0.gamma[0] << " " << icp0.gamma[1] << "\n";
 
    //Have to free out0
    std::cout << "Bytes Sent: " << p0.bytes_sent << "\n";
    std::cout << "Bytes Recieved: " << p0.bytes_recieved << "\n";

    p0.close(0);
    p0.close(1);
}