#include "client.h"
#include "dpf.h"
#include "keys.h"
#include "prng.h"
#include "server.h"
#include "server_trusted.h"

int main() {
    std::string ip[2] = {"127.0.0.1", "127.0.0.1"};
    int port[2] = {3000, 3001};

    Server p1 = Server(ip, port, 1);

    // std::string ipp[2] = {"127.0.0.1", "127.0.0.1"};
    // int portp[2] = {4010, 4011};

    // p1.wait_for_client(ipp, portp);
    // p1.close(0);
    // p1.close(1);

    block b1 = p1.recv_block(0);
    std::cout << b1 << "\n";

    std::cout << "Bytes Sent: " << p1.bytes_sent << "\n";
    std::cout << "Bytes Recieved: " << p1.bytes_recieved << "\n";

    p1.close(0);
}