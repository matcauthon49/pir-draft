#include "server.h"

// Opens Server and establishes connection.
Server::Server(std::string ip[2], int port[2], int sid) {
    std::cerr << "establishing connection...\n";
    const int one = 1;

    server_id = sid;
    bytes_recieved = 0;
    bytes_sent = 0;

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
            dest_addr.sin_port = htons(port[0]); // speficies port and changes to big-endian, TCP byte order
            dest_addr.sin_addr.s_addr = inet_addr(ip[0].c_str()); // specifies address

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
        std::cout << server_id << " send connected to P2" <<"\n";

    }

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
            dest_addr.sin_port = htons(port[1]); // speficies port and changes to big-endian, TCP byte order
            dest_addr.sin_addr.s_addr = inet_addr(ip[1].c_str()); // specifies address

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
        std::cout << server_id << " recv connected to P2" <<"\n";

    }

};

void Server::wait_for_client(std::string ip[2], int port[2]) {
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
            dest_addr.sin_port = htons(port[0]); // speficies port and changes to big-endian, TCP byte order
            dest_addr.sin_addr.s_addr = inet_addr(ip[0].c_str()); // specifies address

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

        std::cout << server_id << " send connected to Client" <<"\n";

    }

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
            dest_addr.sin_port = htons(port[1]); // speficies port and changes to big-endian, TCP byte order
            dest_addr.sin_addr.s_addr = inet_addr(ip[1].c_str()); // specifies address

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

        std::cout << server_id << " recv connected to Client" <<"\n";
    }
}

void Server::close(int party) {
    ::close(sendsocket[party]);
    ::close(recvsocket[party]);

    std::cout << server_id << " closed connection with " << party+2 << "\n";
}

void Server::send_ge(GroupElement &ge, int bw, int party) {
    if (bw > 32) {
        char *buf = (char *)(&ge.value);
        send(sendsocket[party], buf, 8, 0);
        bytes_sent += 8;
    }
    else if (bw > 16) {
        char *buf = (char *)(&ge.value);
        send(sendsocket[party], buf, 4, 0);
        bytes_sent += 4;
    }
    else if (bw > 8) {
        char *buf = (char *)(&ge.value);
        send(sendsocket[party], buf, 2, 0);
        bytes_sent += 2;
    }
    else {
        char *buf = (char *)(&ge.value);
        send(sendsocket[party], buf, 1, 0);
        bytes_sent += 1;
    }
}

void Server::send_block(block &b, int party) {
    char *buf = (char *)(&b);
    send(sendsocket[party], buf, sizeof(block), 0);
    bytes_sent += sizeof(block);
}

void Server::send_uint8(uint8_t &i, int party) {
    char *buf = (char *)(&i);
    send(sendsocket[party], buf, 1, 0);
    bytes_sent += 1;
}

void Server::send_size(size_t &i, int party) {
    char *buf = (char *)(&i);
    send(sendsocket[party], buf, sizeof(size_t), 0);
    bytes_sent += sizeof(size_t);
}

GroupElement Server::recv_ge(int bl, int party) {
    if (bl > 32) {
        char buf[8];
        recv(recvsocket[party-2], buf, 8, MSG_WAITALL);
        GroupElement g(*(uint64_t *)buf, bl);
        bytes_recieved += 8;
        return g;
    }
    else if (bl > 16) {
        char buf[4];
        recv(recvsocket[party-2], buf, 4, MSG_WAITALL);
        GroupElement g(*(uint64_t *)buf, bl);
        bytes_recieved += 4;
        return g;
    }
    else if (bl > 8) {
        char buf[2];
        recv(recvsocket[party-2], buf, 2, MSG_WAITALL);
        GroupElement g(*(uint64_t *)buf, bl);
        bytes_recieved += 2;
        return g;
    }
    else {
        char buf[1];
        recv(recvsocket[party-2], buf, 1, MSG_WAITALL);
        GroupElement g(*(uint64_t *)buf, bl);
        bytes_recieved += 1;
        return g;
    }
}

void Server::send_int(int &i, int party) {
    char *buf = (char *)(&i);
    send(sendsocket[party], buf, sizeof(int), 0);
    bytes_sent += sizeof(int);
}

void Server::send_input_check_pack(input_check_pack icp, int bw, int bl, int party){
    send_ge(icp.index, bw, party);
    send_ge(icp.payload, bl, party);
    // send_block(icp.init_s, party);

    send_size(icp.size, party);

    for (size_t i = 0; i < icp.size; i++) {
        send_block(icp.zs[0][i], party);
        send_block(icp.zs[1][i], party);
        send_uint8(icp.zt[0][i], party);
        send_uint8(icp.zt[1][i], party);
    }

    for (size_t i = 0; i < icp.size; i++) {
        send_block(icp.sigma[i], party);
        send_uint8(icp.tau[0][i], party);
        send_uint8(icp.tau[1][i], party);
    }


    //Sending Final Correction word
    send_int(icp.T, party);
    send_ge((icp.W)[0], bl, party);
    send_ge((icp.W)[1], bl, party);
    send_ge((icp.gamma)[0], bl, party);
    send_ge((icp.gamma)[1], bl, party);
}

block Server::recv_block(int party) {
    char buf[sizeof(block)];
    recv(recvsocket[party-2], buf, sizeof(block), MSG_WAITALL);
    block b = *(block *)buf;
    bytes_recieved += sizeof(block);
    return b;
}

size_t Server::recv_size(int party) {
    char buf[sizeof(size_t)];
    recv(recvsocket[party-2], buf, sizeof(size_t), MSG_WAITALL);
    size_t b = *(size_t*)buf;
    bytes_recieved += sizeof(size_t);
    return b;
}

char Server::recv_char(int party) {
    char buf[1];
    recv(recvsocket[party-2], buf, 1, MSG_WAITALL);
    char b = *buf;
    bytes_recieved += 1;
    return b;
}

uint8_t Server::recv_uint8(int party) {
    char buf[sizeof(uint8_t)];
    recv(recvsocket[party-2], buf, sizeof(uint8_t), MSG_WAITALL);
    uint8_t b = *(uint8_t *)buf;
    bytes_recieved += sizeof(uint8_t);
    return b;
}

int Server::recv_int(int party) {
    char buf[sizeof(int)];
    recv(recvsocket[party-2], buf, sizeof(int), MSG_WAITALL);
    int b = *(int*)buf;
    bytes_recieved += sizeof(int);
    return b;
}

dpf_key Server::recv_dpf_key(int bl, int party) {
    
    int height = recv_int(party);
    int Bout = recv_int(party);
    block s = recv_block(party);
    uint8_t t = recv_uint8(party);

    block *sigma = new block[height];
    for (int i = 0; i < height; i++) {
        sigma[i] = recv_block(party);
    }

    uint8_t *tau0 = new uint8_t[height];
    for (int i = 0; i < height; i++) {
        tau0[i] = recv_uint8(party);
    }

    uint8_t *tau1 = new uint8_t[height];
    for (int i = 0; i < height; i++) {
        tau1[i] = recv_uint8(party);
    }

    GroupElement* gamma = new GroupElement[2];
    gamma[0] = recv_ge(bl, 2);
    gamma[1] = recv_ge(bl, 2);

    return dpf_key(height, Bout, s, t, sigma, tau0, tau1, gamma);
}