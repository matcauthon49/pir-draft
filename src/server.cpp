#include "server.h"

// Opens Server and establishes connection.
Server::Server(std::string ip, int port) {
    std::cerr << "establishing connection...\n";
    const int one = 1;

    {
        int mysocket;

        if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        // ensures that the address supplied to bind() allows reuse of local addresses
        // since client is also 127.0.0.1, this should be enabled
        if (setsockopt(mysocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &one, sizeof(one))) {
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
        if (::bind(mysocket, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr)) < 0) {
                perror("binding error");
                exit(1);
        }

        // listens for connection
        if (listen(mysocket, 1) < 0) {
            perror("listening error");
            exit(1);
        }

        int dest_addrlen = sizeof(dest_addr);

        sendsocket[0] = accept(mysocket, (struct sockaddr *)&dest_addr, (socklen_t*)&dest_addrlen);
        
        // Sets socket information.
        // IPPROTO_TCP: setsockopt will function on the TCP level.
        // TCP_NODELAY: Disables Nagle's algorithm for optimization.
        setsockopt(sendsocket[0], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
    }

    sleep(1);

    {
        int mysocket;

        if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        if (setsockopt(mysocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &one, sizeof(one))) {
            perror("setsockopt error");
            exit(1);
        }

        struct sockaddr_in dest_addr;
            dest_addr.sin_family = AF_INET; // specifies internet protocol v4 address
            dest_addr.sin_port = htons(port); // speficies port and changes to big-endian, TCP byte order
            dest_addr.sin_addr.s_addr = inet_addr(ip.c_str()); // specifies address

        if (::bind(mysocket, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr)) < 0) {
                perror("binding error");
                exit(1);
        }

        if (listen(mysocket, 1) < 0) {
            perror("listening error");
            exit(1);
        }

        int dest_addrlen = sizeof(dest_addr);

        recvsocket[0] = accept(mysocket, (struct sockaddr *)&dest_addr, (socklen_t*)&dest_addrlen);
        setsockopt(recvsocket[0], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
    }
};

void Server::wait_for_client(std::string ip, int port) {
    std::cerr << "establishing connection with client...\n";
    const int one = 1;

    {
        int mysocket;

        if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        if (setsockopt(mysocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &one, sizeof(one))) {
            perror("setsockopt error");
            exit(1);
        }

        struct sockaddr_in dest_addr;
            dest_addr.sin_family = AF_INET; // specifies internet protocol v4 address
            dest_addr.sin_port = htons(port); // speficies port and changes to big-endian, TCP byte order
            dest_addr.sin_addr.s_addr = inet_addr(ip.c_str()); // specifies address

        if (::bind(mysocket, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr)) < 0) {
                perror("binding error");
                exit(1);
        }

        if (listen(mysocket, 1) < 0) {
            perror("listening error");
            exit(1);
        }

        int dest_addrlen = sizeof(dest_addr);

        sendsocket[1] = accept(mysocket, (struct sockaddr *)&dest_addr, (socklen_t*)&dest_addrlen);
        setsockopt(sendsocket[1], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
    }

    sleep(1);

    {
        int mysocket;

        if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        if (setsockopt(mysocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &one, sizeof(one))) {
            perror("setsockopt error");
            exit(1);
        }

        struct sockaddr_in dest_addr;
            dest_addr.sin_family = AF_INET; // specifies internet protocol v4 address
            dest_addr.sin_port = htons(port); // speficies port and changes to big-endian, TCP byte order
            dest_addr.sin_addr.s_addr = inet_addr(ip.c_str()); // specifies address

        if (::bind(mysocket, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr)) < 0) {
                perror("binding error");
                exit(1);
        }

        // listens for connection
        if (listen(mysocket, 1) < 0) {
            perror("listening error");
            exit(1);
        }

        int dest_addrlen = sizeof(dest_addr);

        recvsocket[1] = accept(mysocket, (struct sockaddr *)&dest_addr, (socklen_t*)&dest_addrlen);
        setsockopt(recvsocket[1], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
    }
}

void Server::close() {
    ::close(sendsocket[0]);
    ::close(sendsocket[1]);
    ::close(sendsocket[0]);
    ::close(recvsocket[1]);
}
