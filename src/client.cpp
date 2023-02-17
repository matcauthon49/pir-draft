#include "client.h"

// Opens Client and establishes connection.
Client::Client(std::string ip[6], int port[6], int sid) {
    std::cerr << "trying to connect with server..." << "\n";
    server_id = sid;

    bytes_recieved = 0;
    bytes_sent = 0;

    const int one = 1;

    {
        int mysocket0;

        if ((mysocket0 = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        if (setsockopt(mysocket0, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &one, sizeof(one))) {
            perror("setsockopt error");
            exit(1);
        }

        struct sockaddr_in dest_addr;
            dest_addr.sin_family = AF_INET; // specifies internet protocol v4 address
            dest_addr.sin_port = htons(port[0]); // speficies port and changes to big-endian, TCP byte order
            dest_addr.sin_addr.s_addr = inet_addr(ip[0].c_str()); // specifies address

        if (::bind(mysocket0, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr)) < 0) {
                perror("binding error");
                exit(1);
        }

        if (listen(mysocket0, 1) < 0) {
            perror("listening error");
            exit(1);
        }

        int dest_addrlen = sizeof(dest_addr);

        recvsocket[0] = accept(mysocket0, (struct sockaddr *)&dest_addr, (socklen_t*)&dest_addrlen);
        setsockopt(recvsocket[0], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));

        std::cout << server_id << " recv connected to P0" <<"\n";

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

        sendsocket[0] = accept(mysocket, (struct sockaddr *)&dest_addr, (socklen_t*)&dest_addrlen);
        setsockopt(sendsocket[0], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));

        std::cout << server_id << " send connected to P0" <<"\n";
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
            dest_addr.sin_port = htons(port[2]); // speficies port and changes to big-endian, TCP byte order
            dest_addr.sin_addr.s_addr = inet_addr(ip[2].c_str()); // specifies address

        if (::bind(mysocket, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr)) < 0) {
                perror("binding error");
                exit(1);
        }

        if (listen(mysocket, 1) < 0) {
            perror("listening error");
            exit(1);
        }

        int dest_addrlen = sizeof(dest_addr);

        recvsocket[1] = accept(mysocket, (struct sockaddr *)&dest_addr, (socklen_t*)&dest_addrlen);
        setsockopt(recvsocket[1], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));

        std::cout << server_id << " recv connected to P1" <<"\n";

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
            dest_addr.sin_port = htons(port[3]); // speficies port and changes to big-endian, TCP byte order
            dest_addr.sin_addr.s_addr = inet_addr(ip[3].c_str()); // specifies address

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

        std::cout << server_id << " send connected to P1" <<"\n";
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
            dest_addr.sin_port = htons(port[4]); // speficies port and changes to big-endian, TCP byte order
            dest_addr.sin_addr.s_addr = inet_addr(ip[4].c_str()); // specifies address

        if (::bind(mysocket, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr)) < 0) {
                perror("binding error");
                exit(1);
        }

        if (listen(mysocket, 1) < 0) {
            perror("listening error");
            exit(1);
        }

        int dest_addrlen = sizeof(dest_addr);

        recvsocket[2] = accept(mysocket, (struct sockaddr *)&dest_addr, (socklen_t*)&dest_addrlen);
        setsockopt(recvsocket[2], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));

        std::cout << server_id << " recv connected to P2" <<"\n";

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
            dest_addr.sin_port = htons(port[5]); // speficies port and changes to big-endian, TCP byte order
            dest_addr.sin_addr.s_addr = inet_addr(ip[5].c_str()); // specifies address

        if (::bind(mysocket, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr)) < 0) {
                perror("binding error");
                exit(1);
        }

        if (listen(mysocket, 1) < 0) {
            perror("listening error");
            exit(1);
        }

        int dest_addrlen = sizeof(dest_addr);

        sendsocket[2] = accept(mysocket, (struct sockaddr *)&dest_addr, (socklen_t*)&dest_addrlen);
        setsockopt(sendsocket[2], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));

        std::cout << server_id << " send connected to P2" <<"\n";
    }
    
    std::cerr << "All connections complete.\n" << std::endl;
};

// Closes the socket.
void Client::close(int party) {
    ::close(sendsocket[party]);
    ::close(recvsocket[party]);

    std::cout << server_id << " closed connection with " << party << "\n";
}

void Client::send_ge(GroupElement &ge, int bw, int party) {
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

void Client::send_uint8(uint8_t &i, int party) {
    char *buf = (char *)(&i);
    send(sendsocket[party], buf, 1, 0);
    bytes_sent += 1;
}

GroupElement Client::recv_ge(int bl, int party) {
    if (bl > 32) {
        char buf[8];
        recv(recvsocket[party], buf, 8, MSG_WAITALL);
        GroupElement g(*(uint64_t *)buf, bl);
        bytes_recieved += 8;
        return g;
    }
    else if (bl > 16) {
        char buf[4];
        recv(recvsocket[party], buf, 4, MSG_WAITALL);
        GroupElement g(*(uint64_t *)buf, bl);
        bytes_recieved += 4;
        return g;
    }
    else if (bl > 8) {
        char buf[2];
        recv(recvsocket[party], buf, 2, MSG_WAITALL);
        GroupElement g(*(uint64_t *)buf, bl);
        bytes_recieved += 2;
        return g;
    }
    else {
        char buf[1];
        recv(recvsocket[party], buf, 1, MSG_WAITALL);
        GroupElement g(*(uint64_t *)buf, bl);
        bytes_recieved += 1;
        return g;
    }
}

block Client::recv_block(int party) {
    char buf[sizeof(block)];
    recv(recvsocket[party], buf, sizeof(block), MSG_WAITALL);
    block b = *(block *)buf;
    bytes_recieved += sizeof(block);
    return b;
}

size_t Client::recv_size(int party) {
    char buf[sizeof(size_t)];
    recv(recvsocket[party], buf, sizeof(size_t), MSG_WAITALL);
    size_t b = *(size_t*)buf;
    bytes_recieved += sizeof(size_t);
    return b;
}

uint8_t Client::recv_uint8(int party) {
    char buf[sizeof(uint8_t)];
    recv(recvsocket[party], buf, sizeof(uint8_t), MSG_WAITALL);
    uint8_t b = *(uint8_t *)buf;
    bytes_recieved += sizeof(uint8_t);
    return b;
}

int Client::recv_int(int party) {
    char buf[sizeof(int)];
    recv(recvsocket[party], buf, sizeof(int), MSG_WAITALL);
    int b = *(int*)buf;
    bytes_recieved += sizeof(int);
    return b;
}

input_check_pack Client::recv_input_check_pack(int bl, int bw, int party) {
    input_check_pack icp;
    
    icp.index = recv_ge(bl, party);
    icp.payload = recv_ge(bw, party);
    // icp.init_s = recv_block(party);

    // size must be sent before the rest
    icp.size = recv_size(party);

    icp.zs[0] = new block[icp.size];
    icp.zs[1] = new block[icp.size];
    icp.zt[0] = new uint8_t[icp.size];
    icp.zt[1] = new uint8_t[icp.size];

    for (size_t i = 0; i < icp.size; i++) {
        icp.zs[0][i] = recv_block(party);
    }
    for (size_t i = 0; i < icp.size; i++) {
        icp.zs[1][i] = recv_block(party);
    }
    for (size_t i = 0; i < icp.size; i++) {
        icp.zt[0][i] = recv_uint8(party);
    }
    for (size_t i = 0; i < icp.size; i++) {
        icp.zt[1][i] = recv_uint8(party);
    }

    icp.sigma = (block*)malloc(icp.size*(sizeof(block)));
    icp.tau[0] = (uint8_t*)malloc(icp.size*(sizeof(uint8_t)));
    icp.tau[1] = (uint8_t*)malloc(icp.size*(sizeof(uint8_t)));

    for (size_t i = 0; i < icp.size; i++) {
        icp.sigma[i] = recv_block(party);
        icp.tau[0][i] = recv_uint8(party);
        icp.tau[1][i] = recv_uint8(party);
    }    

    //Receiving Final Correction Word
    icp.T = recv_int(party);
    icp.W[0] = recv_ge(bw, party);
    icp.W[1] = recv_ge(bw, party);
    icp.gamma[0] = recv_ge(bw, party);
    icp.gamma[1] = recv_ge(bw, party);
    return icp;
}

input_check_pack_2 Client::recv_input_check_pack_2(int bl, int bw, int party) {
    input_check_pack_2 icp;
    
    icp.index = new GroupElement[2];
    icp.index[0] = recv_ge(bl, party);
    icp.index[1] = recv_ge(bl, party);

    icp.payload = new GroupElement[2];
    icp.payload[0] = recv_ge(bw, party);
    icp.payload[1] = recv_ge(bw, party);


    // size must be sent before the rest
    icp.size = recv_size(party);

    icp.zs0[0] = (block*)malloc(icp.size*(sizeof(block)));
    icp.zs0[1] = (block*)malloc(icp.size*(sizeof(block)));
    icp.zt0[0] = (uint8_t*)malloc(icp.size*(sizeof(uint8_t)));
    icp.zt0[1] = (uint8_t*)malloc(icp.size*(sizeof(uint8_t)));


    for (size_t i = 0; i < icp.size; i++) {
        icp.zs0[0][i] = recv_block(party);
        icp.zs0[1][i] = recv_block(party);
        icp.zt0[0][i] = recv_uint8(party);
        icp.zt0[1][i] = recv_uint8(party);
    }

    icp.zs1[0] = (block*)malloc(icp.size*(sizeof(block)));
    icp.zs1[1] = (block*)malloc(icp.size*(sizeof(block)));
    icp.zt1[0] = (uint8_t*)malloc(icp.size*(sizeof(uint8_t)));
    icp.zt1[1] = (uint8_t*)malloc(icp.size*(sizeof(uint8_t)));

    for (size_t i = 0; i < icp.size; i++) {
        icp.zs1[0][i] = recv_block(party);
        icp.zs1[1][i] = recv_block(party);
        icp.zt1[0][i] = recv_uint8(party);
        icp.zt1[1][i] = recv_uint8(party);
    }

    icp.sigma = (block*)malloc(icp.size*(sizeof(block)));
    icp.tau[0] = (uint8_t*)malloc(icp.size*(sizeof(uint8_t)));
    icp.tau[1] = (uint8_t*)malloc(icp.size*(sizeof(uint8_t)));

    for (size_t i = 0; i < icp.size; i++) {
        icp.sigma[i] = recv_block(party);
        icp.tau[0][i] = recv_uint8(party);
        icp.tau[1][i] = recv_uint8(party);
    }

    //Receiving Final Correction Word
    icp.T[0] = recv_int(party);
    icp.T[1] = recv_int(party);
    icp.W[0][0] = recv_ge(bw, party);
    icp.W[0][1] = recv_ge(bw, party);
    icp.W[1][0] = recv_ge(bw, party);
    icp.W[1][1] = recv_ge(bw, party);
    icp.gamma[0] = recv_ge(bw, party);
    icp.gamma[1] = recv_ge(bw, party);  

    return icp;
}

void Client::send_int(int &i, int party) {
    char *buf = (char *)(&i);
    send(sendsocket[party], buf, sizeof(int), 0);
    bytes_sent += sizeof(int);
}

void Client::send_long(long &i, int party) {
    char *buf = (char *)(&i);
    send(sendsocket[party], buf, sizeof(long), 0);
    bytes_sent += sizeof(long);
}
