#include "client.h"
#include "dpf.h"
#include "keys.h"
#include "prng.h"
#include "server.h"
#include "server_trusted.h"

int main() {

    std::string ip[6] = {"127.0.0.1", "127.0.0.1", "127.0.0.1", "127.0.0.1", "127.0.0.1", "127.0.0.1"};
    int port[6] = {4000, 4001, 4010, 4011, 4020, 4021};

    Client c = Client(ip, port, 3);
    c.close(0);    
    c.close(1);    
    c.close(2);    
}