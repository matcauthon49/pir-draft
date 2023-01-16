#include "client.h"
#include "dpf.h"
#include "keys.h"
#include "server.h"
#include "server_trusted.h"
#include <algorithm>
#include <chrono>


int main() {

    

    std::string ip[6] = {"127.0.0.1", "127.0.0.1", "127.0.0.1", "127.0.0.1", "127.0.0.1", "127.0.0.1"};
    int port[6] = {4000, 4001, 6000, 6001, 8000, 8001};

    Client c = Client(ip, port, 3);
    input_check_pack icp0 = c.recv_input_check_pack(bitlength, bitlength, 0);
    input_check_pack icp1 = c.recv_input_check_pack(bitlength, bitlength, 1);
    input_check_pack_2 ip2 = c.recv_input_check_pack_2(bitlength, bitlength, 2);

    // Print ICP0
    // std::cout << "------------------- ICP 0 BEGIN -------------------\n";
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
    // std::cout << "------------------- ICP 0 END -------------------\n";

    // std::cout << "------------------- ICP 1 BEGIN -------------------\n";
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
    // std::cout << "------------------- ICP 1 END -------------------\n";

    // std::cout << "------------------- ICP 2 BEGIN -------------------\n";

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

    // std::cout << "------------------- ICP 2 END -------------------\n";

    auto start = std::chrono::high_resolution_clock::now();
    bool output = check_xor(bitlength, icp0, icp1, ip2);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
    std::cout << "Time taken for ICP Check: " << duration.count()*1e-6 <<"\n";

    uint8_t out = output;

    std::cout << "Check: " << output << "\n";

    c.send_uint8(out, 0);
    c.send_uint8(out, 1);
    GroupElement index = GroupElement(10, ip2.size);    
    if (output) {

        GroupElement rotated_index = index - (ip2.index)[0] - (ip2.index)[1];
        c.send_ge(rotated_index, ip2.size, 0);
        c.send_ge(rotated_index, ip2.size, 1);

        // std::cout << "Rotated Index: " << rotated_index << "\n";

        GroupElement o0 = c.recv_ge(bitlength, 0);
        GroupElement ohat0 = c.recv_ge(bitlength, 0);
        GroupElement o1 = c.recv_ge(bitlength, 1);
        GroupElement ohat1 = c.recv_ge(bitlength, 1);

        if ((icp0.payload + icp1.payload) * (o0 + o1) != (ohat0 + ohat1)) {
            std::cerr << "Incorrect dpf evaluation\n";
            return -1;
        }

        GroupElement dbout = o0 + o1;

        std::cout << "\nOutput: " << dbout.value << ".\n\n"; 

        std::cout << "Bytes Sent: " << c.bytes_sent << "\n";
        std::cout << "Bytes Recieved: " << c.bytes_recieved << "\n";

    } else {

        std::cerr << "Inputs do not match.\n";

    }

    free_input_check_pack(icp0);
    free_input_check_pack(icp1);
    free_input_check_pack_2(ip2);

    c.close(0);    
    c.close(1);    
    c.close(2);       
}