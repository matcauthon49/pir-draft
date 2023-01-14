#include "client.h"
#include "dpf.h"
#include "keys.h"
#include "server.h"
#include "server_trusted.h"

int main() {

    int database_size = (1<<16);
    GroupElement database[database_size];
    for(int i=0; i<database_size; i++)
        database[i] = GroupElement(i, bitlength);

    std::string ip[2] = {"127.0.0.1", "127.0.0.1"};
    int port[2] = {3000, 3001};
    std::string ipq[2] = {"127.0.0.1", "127.0.0.1"};
    int portq[2] = {5000, 5001};

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
    // for(int i=0; i<8; i++)
    //     std::cout<<"P1 "<<i<<" "<<out1[i][0]<<" "<<out1[i][1]<<"\n";
    p1.connect_to_client(ipq, portq);
    p1.send_input_check_pack(icp1, bitlength, bitlength, 3);

    uint8_t accept = p1.recv_uint8(3);

    // Print ICP
    // std::cout << "P1 Index: " << icp1.index << "\n";
    // std::cout << "P1 Payload: " << icp1.payload << "\n";

    // std::cout << "P1 zs[0]: " << "\n";
    // for (int i = 0; i < icp1.size; i++) {
    //     std::cout << icp1.zs[0][i] << "\n";
    // }
    // std::cout << "P1 zs[1]: " << "\n";
    // for (int i = 0; i < icp1.size; i++) {
    //     std::cout << icp1.zs[1][i] << "\n";
    // }

    // std::cout << "P1 zt[0]: ";
    // for (int i = 0; i < icp1.size; i++) {
    //     std::cout << unsigned(icp1.zt[0][i]);
    // }
    // std::cout << "\n";
    // std::cout << "P1 zt[1]: ";
    // for (int i = 0; i < icp1.size; i++) {
    //     std::cout << unsigned(icp1.zt[1][i]);
    // }
    // std::cout << "\n";
    // std::cout << "P1 Sigma: " << "\n";
    // for (int i = 0; i < icp1.size; i++) {
    //     std::cout << icp1.sigma[i] << "\n";
    // }
    // std::cout << "P1 Tau0: ";
    // for (int i = 0; i < icp1.size; i++) {
    //     std::cout << unsigned(icp1.tau[0][i]);
    // }
    // std::cout << "\n";
    // std::cout << "P1 Tau1: ";
    // for (int i = 0; i < icp1.size; i++) {
    //     std::cout << unsigned(icp1.tau[1][i]);
    // }
    // std::cout << "\n";
    // std::cout << "P1 T: " << icp1.T << "\n";
    // std::cout << "P1 W: " << icp1.W[0] << " " << icp1.W[1] << "\n";
    // std::cout << "P1 Gamma: " << icp1.gamma[0] << " " << icp1.gamma[1] << "\n";

    if (accept) {

        GroupElement rotated_index = p1.recv_ge(icp1.size, 3);
        // std::cout << "Rotated Index: " << rotated_index.value << "\n";

        GroupElement o = GroupElement(0, bitlength);
        GroupElement hato = GroupElement(0, bitlength);
        for (int i = 0; i < database_size; i++) {
            GroupElement ind = GroupElement(i, icp1.size) + rotated_index;
            o = o + out1[i][0] * database[ind.value];
            hato = hato + out1[i][1] * database[ind.value];
        }
        // std::cout<<"P1 final "<<o.value<<" "<<hato.value<<"\n";
        p1.send_ge(o, bitlength, 3);
        p1.send_ge(hato, bitlength, 3);
        //Also have to free out1
        std::cout << "Bytes Sent: " << p1.bytes_sent << "\n";
        std::cout << "Bytes Recieved: " << p1.bytes_recieved << "\n";

    } else {

        std::cerr << "Client rejected output.\n";

    }

    free_input_check_pack(icp1);
    free_dpf_key(k1);

    p1.close(0);
    p1.close(1);
        
}