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
}