#include "client.h"
#include "dpf.h"
#include "keys.h"
#include "prng.h"
#include "server.h"
#include "server_trusted.h"

int main() {

    std::string ip[4] = {"127.0.0.1", "127.0.0.1", "127.0.0.1", "127.0.0.1"};
    int port[4] = {2000, 2001, 3000, 3001};

    ServerTrusted p2 = ServerTrusted(ip, port, 2);

    // std::string ipp[2] = {"127.0.0.1", "127.0.0.1"};
    // int portp[2] = {4020, 4021};

    // p2.wait_for_client(ipp, portp);    
    // p2.close(0);
    // p2.close(1);
    // p2.close(2);

    block b0 = ZeroBlock;
    block b1 = OneBlock;

    p2.send_block(b0, 0);
    p2.send_block(b1, 1);

    std::cout << "Bytes Sent: " << p2.bytes_sent << "\n";
    std::cout << "Bytes Recieved: " << p2.bytes_recieved << "\n";

    p2.close(0);
    p2.close(1);
}