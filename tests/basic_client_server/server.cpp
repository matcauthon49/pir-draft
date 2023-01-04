#include <string>
#include <iostream>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>

#define PORT 8080

// Server class.
class Server {
public:
    int s_sock;

    Server(std::string ip, int port);

    void close();
};


// Opens Server and establishes connection.
Server::Server(std::string ip, int port) {
    std::cerr << "establishing connection...\n";

    const int one = 1;

    int p_sock;

    if ((p_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // ensures that the address supplied to bind() allows reuse of local addresses
    // since client is also 127.0.0.1, this should be enabled
    if (setsockopt(p_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &one, sizeof(one))) {
        perror("setsockopt error");
        exit(1);
    }

    // constructs the address of the server with which to connect
    // sockaddr_in because we are using IP-based communication
    struct sockaddr_in dest_addr;
        dest_addr.sin_family = AF_INET; // specifies internet protocol v4 address
        dest_addr.sin_port = htons(port); // speficies port and changes to big-endian, TCP byte order
        dest_addr.sin_addr.s_addr = inet_addr(ip.c_str()); // specifies address

    // binds p_sock with the sockaddress
    if (::bind(p_sock, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr)) < 0) {
            perror("binding error");
            exit(1);
        }

    // listens for connection
    if (listen(p_sock, 1) < 0) {
            perror("listening error");
            exit(1);
        }

    int dest_addrlen = sizeof(dest_addr);

    s_sock = accept(p_sock, (struct sockaddr *)&dest_addr, (socklen_t*)&dest_addrlen);
    
    // Sets socket information.
    // IPPROTO_TCP: setsockopt will function on the TCP level.
    // TCP_NODELAY: Disables Nagle's algorithm for optimization.
    setsockopt(s_sock, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
};

void Server::close() {
    ::close(s_sock);
}

int main() {

    Server *s =  new Server("127.0.0.1", PORT);
    s->close();

    char buffer[1024] = {0};
    recv(s->s_sock, buffer, 1024, MSG_WAITALL);

    std::cout << buffer;

    return 0;
}