#include "client.h"
#include "dpf.h"
#include "keys.h"
#include "server.h"
#include "server_trusted.h"

int main() {
    std::string ip[2] = {"127.0.0.1", "127.0.0.1"};
    int port[2] = {3000, 3001};

    Server p1 = Server(ip, port, 1);

    //Receive DPF key from P2
    GroupElement index = p1.recv_ge(16, 2);
    GroupElement payload = p1.recv_ge(bitlength, 2);
    dpf_key k1 = p1.recv_dpf_key(bitlength, 2);
    
    input_check_pack icp1;
    icp1.index = index;
    icp1.payload = payload;

    // std::cout<<"index "<<index.value<<" payload "<<payload<<"\n";
    // std::cout<<"Bout "<<k1.Bout<<" height "<<k1.height<<" s "<<k1.s<<"\n";
    // for(int i=0; i<k1.height; i++)
    //     std::cout<<"i "<<i<<" sigma "<<(k1.sigma)[i]<<" tau0 "<<static_cast<uint32_t>((k1.tau0)[i])<<" tau1 "<<static_cast<uint32_t>((k1.tau1)[i])<<"\n";
    // std::cout<<"\ngamma0 "<<(k1.gamma)[0].value<<" gamma1 "<<(k1.gamma)[1].value<<"\n";

    //Eval all
    GroupElement** out1 = dpf_eval_all(1, k1, &icp1);

    //Also have to free out1
    std::cout << "Bytes Sent: " << p1.bytes_sent << "\n";
    std::cout << "Bytes Recieved: " << p1.bytes_recieved << "\n";

    p1.close(0);
}