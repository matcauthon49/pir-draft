#include "client.h"
#include "dpf.h"
#include "keys.h"
#include "server.h"
#include "server_trusted.h"

int main() {

    std::string ip[6] = {"127.0.0.1", "127.0.0.1", "127.0.0.1", "127.0.0.1", "127.0.0.1", "127.0.0.1"};
    int port[6] = {4000, 4001, 5000, 5001, 6000, 6001};

    Client c = Client(ip, port, 3);
    GroupElement temprec = c.recv_ge(10, 2);
    std::cout<<"Value received by client "<<temprec.value<<"\n";
    c.close(0);    
    c.close(1);    
    c.close(2);       
}