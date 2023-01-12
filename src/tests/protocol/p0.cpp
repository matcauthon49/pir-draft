#include "client.h"
#include "dpf.h"
#include "keys.h"
#include "prng.h"
#include "server.h"
#include "server_trusted.h"

int main() {
    std::string ip = "127.0.0.1";
    int port = 2000;

    Server p0 = Server(ip, port);
}