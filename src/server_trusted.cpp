#include "server_trusted.h"

int bitlength = 2;

// Opens Server and establishes connection.
ServerTrusted::ServerTrusted(std::string ip[4], int port[4], int sid) {
    std::cerr << "establishing connection...\n";
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
        else {
            std::cout << "recv connection with P0 successful" <<"\n";
        }

        const int one = 1;
        setsockopt(recvsocket[0], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));

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
        } else {
            std::cout << "send connection with P0 successful" <<"\n";
        }

        const int one = 1;
        setsockopt(sendsocket[0], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
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
        } else {
            std::cout << "recv connection with P1 successful" <<"\n";
        }

        const int one = 1;
        setsockopt(recvsocket[1], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));

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
        } else {
            std::cout << "send connection with P1 successful" <<"\n";
        }

        const int one = 1;
        setsockopt(sendsocket[1], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
    }
    
};

void ServerTrusted::wait_for_client(std::string ip[2], int port[2]) {
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

        sendsocket[2] = accept(mysocket, (struct sockaddr *)&dest_addr, (socklen_t*)&dest_addrlen);
        setsockopt(sendsocket[2], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));

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

        recvsocket[2] = accept(mysocket, (struct sockaddr *)&dest_addr, (socklen_t*)&dest_addrlen);
        setsockopt(recvsocket[2], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));

        std::cout << server_id << " recv connected to Client" <<"\n";

    }
}

void ServerTrusted::close(int party) {
    ::close(sendsocket[party]);
    ::close(recvsocket[party]);

    std::cout << server_id << " closed connection with " << party << "\n";
}

void ServerTrusted::send_ge(GroupElement &ge, int bw, int party) {
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

void ServerTrusted::send_block(block &b, int party) {
    char *buf = (char *)(&b);
    send(sendsocket[party], buf, sizeof(block), 0);
    bytes_sent += sizeof(block);
}

void ServerTrusted::send_uint8(uint8_t &i, int party) {
    char *buf = (char *)(&i);
    send(sendsocket[party], buf, 1, 0);
    bytes_sent += 1;
}

void ServerTrusted::send_size(size_t &i, int party) {
    char *buf = (char *)(&i);
    send(sendsocket[party], buf, sizeof(size_t), 0);
    bytes_sent += sizeof(size_t);
}

void ServerTrusted::send_int(int &i, int party) {
    char *buf = (char *)(&i);
    send(sendsocket[party], buf, sizeof(int), 0);
    bytes_sent += sizeof(int);
}

GroupElement ServerTrusted::recv_ge(int bl, int party) {
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

void ServerTrusted::send_input_check_pack_2(input_check_pack_2 &icp, int bl, int party){
    send_ge(icp.index[0], bl, party);
    send_ge(icp.index[1], bl, party);

    send_ge(icp.payload[0], bl, party);
    send_ge(icp.payload[1], bl, party);

    send_block(icp.init_s[0], party);
    send_block(icp.init_s[1], party);

    send_size(icp.size, party);

    for (size_t i = 0; i < icp.size; i++) {
        send_block(icp.z0[0][i], party);
        send_block(icp.z0[1][i], party);
    }

    for (size_t i = 0; i < icp.size; i++) {
        send_block(icp.z1[0][i], party);
        send_block(icp.z1[1][i], party);
    }
    
    for (size_t i = 0; i < icp.size; i++) {
        send_block(icp.sigma[i], party);
    }
};

void ServerTrusted::send_dpf_key(dpf_key &dpfk, int bw, int party, size_t size) {
    send_int(dpfk.height, party);
    send_int(dpfk.groupSize, party);
    send_int(dpfk.Bout, party);

    send_block(dpfk.s, party);
    send_uint8(dpfk.t, party);

    send_size(size, party);

    for (size_t i = 0; i < size; i++) {
        send_block(dpfk.sigma[i], party);
    }

    for (size_t i = 0; i < size; i++) {
        send_uint8(dpfk.tau0[i], party);
    }

    for (size_t i = 0; i < size; i++) {
        send_uint8(dpfk.tau1[i], party);
    }

    send_ge(dpfk.gamma[0], bw, party);
    send_ge(dpfk.gamma[1], bw, party);
};

