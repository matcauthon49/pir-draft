#include "client.h"
#include "dpf.h"
#include "keys.h"
#include "prng.h"
#include "server.h"
#include "server_trusted.h"

int main() {
    std::string ip1 = "127.0.0.1";
    int port1 = 2000;

    Server p0 = Server(ip1, port1);

    std::string ip2 = "127.0.0.1";
    int port2 = 2001;

    Server p1 = Server(ip2, port2);

    std::string ip3[2] = {"127.0.0.1", "127.0.0.1"};
    int port3[2] = {2000, 2001};

    ServerTrusted p2 = ServerTrusted(ip3, port3);
}