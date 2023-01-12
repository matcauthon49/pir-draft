#include "client.h"
#include "dpf.h"
#include "keys.h"
#include "prng.h"
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

    block b0 = p0.recv_block(0);
    std::cout << b0 << "\n";
 
    std::cout << "Bytes Sent: " << p0.bytes_sent << "\n";
    std::cout << "Bytes Recieved: " << p0.bytes_recieved << "\n";

    p0.close(0);

}