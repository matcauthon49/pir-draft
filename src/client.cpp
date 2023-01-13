#include "client.h"

// Opens Client and establishes connection.
Client::Client(std::string ip[6], int port[6], int sid) {
    std::cerr << "trying to connect with server..." << "\n";
    server_id = sid;

    bytes_recieved = 0;
    bytes_sent = 0;

    {
        recvsocket[0] = socket(AF_INET, SOCK_STREAM, 0);
        if (recvsocket < 0) {
            perror("recvsocket 0 failed");
            exit(1);
        }
        struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port[0]);
            addr.sin_addr.s_addr = inet_addr(ip[0].c_str());

        if (connect(recvsocket[0], (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            perror("recv connection with P0 failed");
            exit(1);
        }

        const int one = 1;
        setsockopt(recvsocket[0], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));

        std::cout << "Client recv connected to P0" <<"\n";

    }

    sleep(1);

    {
        sendsocket[0] = socket(AF_INET, SOCK_STREAM, 0);
        if (sendsocket < 0) {
            perror("sendsocket 0 failed");
            exit(1);
        }
        struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port[1]);
            addr.sin_addr.s_addr = inet_addr(ip[1].c_str());

        if (connect(sendsocket[0], (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            perror("send connection with P0 failed");
            exit(1);
        }

        const int one = 1;
        setsockopt(sendsocket[0], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));

        std::cout << "Client send connected to P0" <<"\n";
    }

    sleep(1);

    {
        recvsocket[1] = socket(AF_INET, SOCK_STREAM, 0);
        if (recvsocket < 0) {
            perror("recvsocket 1 failed");
            exit(1);
        }
        struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port[2]);
            addr.sin_addr.s_addr = inet_addr(ip[2].c_str());

        if (connect(recvsocket[1], (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            perror("recv connection with P1 failed");
            exit(1);
        }

        const int one = 1;
        setsockopt(recvsocket[1], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));

        std::cout << "Client recv connected to P1" <<"\n";

    }

    sleep(1);

    {
        sendsocket[1] = socket(AF_INET, SOCK_STREAM, 0);
        if (sendsocket < 0) {
            perror("sendsocket 1 failed");
            exit(1);
        }
        struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port[3]);
            addr.sin_addr.s_addr = inet_addr(ip[3].c_str());

        if (connect(sendsocket[1], (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            perror("send connection with P1 failed");
            exit(1);
        }

        const int one = 1;
        setsockopt(sendsocket[1], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));

        std::cout << "Client send connected to P1" <<"\n";

    }

    sleep(1);

    {
        recvsocket[2] = socket(AF_INET, SOCK_STREAM, 0);
        if (recvsocket < 0) {
            perror("recvsocket 2 failed");
            exit(1);
        }
        struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port[4]);
            addr.sin_addr.s_addr = inet_addr(ip[4].c_str());

        if (connect(recvsocket[2], (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            perror("recv connection with P2 failed");
            exit(1);
        }

        const int one = 1;
        setsockopt(recvsocket[2], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));

        std::cout << "Client recv connected to P2" <<"\n";
    }

    sleep(1);

    {
        sendsocket[2] = socket(AF_INET, SOCK_STREAM, 0);
        if (sendsocket < 0) {
            perror("sendsocket 2 failed");
            exit(1);
        }
        struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port[5]);
            addr.sin_addr.s_addr = inet_addr(ip[5].c_str());

        if (connect(sendsocket[2], (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            perror("send connection with P0 failed");
            exit(1);
        }

        const int one = 1;
        setsockopt(sendsocket[2], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));

        std::cout << "Client send connected to P2" <<"\n";
    }
    
    std::cerr << "All connections complete." << std::endl;
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
    recv(recvsocket[party-2], buf, sizeof(uint8_t), MSG_WAITALL);
    uint8_t b = *(uint8_t *)buf;
    bytes_recieved += sizeof(uint8_t);
    return b;
}

int Client::recv_int(int party) {
    char buf[sizeof(int)];
    recv(recvsocket[party-2], buf, sizeof(int), MSG_WAITALL);
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

    icp.zs[0] = (block*)malloc(icp.size*(sizeof(block)));
    icp.zs[1] = (block*)malloc(icp.size*(sizeof(block)));
    icp.zt[0] = (uint8_t*)malloc(icp.size*(sizeof(uint8_t)));
    icp.zt[1] = (uint8_t*)malloc(icp.size*(sizeof(uint8_t)));

    for (size_t i = 0; i < icp.size; i++) {
        icp.zs[0][i] = recv_block(party);
        icp.zs[1][i] = recv_block(party);
        icp.zt[0][i] = recv_uint8(party);
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

    // icp.init_s = new block[2];
    // icp.init_s[0] = recv_block(bl);
    // icp.init_s[1] = recv_block(bl);

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
