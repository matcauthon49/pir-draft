#pragma once

#include "client.h"

#define PORT 8080

// Client class.
class Client {
public:
    int c_sock;

    Client(std::string ip, int port);

    void close();
    void send_msg(char* msg);
};

// Opens Client and establishes connection.
Client::Client(std::string ip, int port) {
    std::cerr << "establishing connection...\n";

    // constructs socket
    c_sock = socket(AF_INET, SOCK_STREAM, 0);

    // constructs the address of the server with which to connect
    struct sockaddr_in addr;
        addr.sin_family = AF_INET; // specifies internet protocol v4 address
        addr.sin_port = htons(port); // speficies port and changes to big-endian, TCP byte order
        addr.sin_addr.s_addr = inet_addr(ip.c_str()); // specifies address

    // establishes connection with address
    if (connect(c_sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            perror("connect");
            exit(1);
        }
    
    const int one = 1;

    // Sets socket information.
    // IPPROTO_TCP: setsockopt will function on the TCP level.
    // TCP_NODELAY: Disables Nagle's algorithm for optimization.
    setsockopt(c_sock, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
};

// Closes the socket.
void Client::close() {
        ::close(c_sock);
}

// Sends a chosen string.
void Client::send_msg(char* msg) {
    char buffer[1024] = {0};
    send(c_sock, msg, strlen(msg), 0);
}
