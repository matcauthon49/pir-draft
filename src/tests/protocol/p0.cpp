#include "client.h"
#include "dpf.h"
#include "keys.h"
#include "server.h"
#include "server_trusted.h"

int main() {
    std::string ip[2] = {"127.0.0.1", "127.0.0.1"};
    int port[2] = {2000, 2001};

    Server p0 = Server(ip, port, 0);
//     std::string ipp[2] = {"127.0.0.1", "127.0.0.1"};
//     int portp[2] = {4000, 4001};

//     p0.wait_for_client(ipp, portp);
//     p0.close(0);
//     p0.close(1);

    block x = p0.recv_block(PARTY_TRUSTED);
    AES w = AES(x);

    std::cout << w.ecbEncBlock(ZeroBlock) << "\n";

    dpf_key k0 = p0.recv_dpf_key(bitlength, 2);

    std::cout << "Height = " << k0.height << "\n";
    std::cout << "Group Size = " << k0.groupSize << "\n";
    std::cout << "Bout = " << k0.Bout << "\n";
    std::cout << "S = " << k0.s << "\n";
    std::cout << "T = " << unsigned(k0.t) << "\n";

    std::cout << ":: Sigma ::" << "\n";
    for (int i = 0; i < k0.height; i++) {
        std::cout << k0.sigma[i] << "\n";
    }

    std::cout << ":: Tau0 ::" << "\n";
    for (int i = 0; i < k0.height; i++) {
        std::cout << unsigned(k0.tau0[i]);
    }

    std::cout << "\n";

    std::cout << ":: Tau1 ::" << "\n";
    for (int i = 0; i < k0.height; i++) {
        std::cout << unsigned(k0.tau1[i]);
    }

    std::cout << "\n";

    // std::cout << ":: Gamma ::" << "\n";
    // for (int i = 0; i < 2; i++) {
    //     std::cout << k0.gamma[i];
    // }

    std::cout << k0.gamma[0] << "\n" << k0.gamma[1] << "\n";
 
    std::cout << "Bytes Sent: " << p0.bytes_sent << "\n";
    std::cout << "Bytes Recieved: " << p0.bytes_recieved << "\n";

    p0.close(0);

}