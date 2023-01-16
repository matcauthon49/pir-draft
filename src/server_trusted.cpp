#include "server_trusted.h"

int bitlength = 40;
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

void ServerTrusted::connect_to_client(std::string ip[2], int port[2]) {
    std::cerr << "establishing connection with client...\n";

    sleep(15);

    {
        sendsocket[2] = socket(AF_INET, SOCK_STREAM, 0);
        if (recvsocket < 0) {
            perror("sendsocket 2 failed");
            exit(1);
        }
        struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port[0]);
            addr.sin_addr.s_addr = inet_addr(ip[0].c_str());

        if (connect(sendsocket[2], (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            perror("send connection with Client failed");
            exit(1);
        }
        else {
            std::cout << "send connection with Client successful" <<"\n";
        }

        const int one = 1;
        setsockopt(sendsocket[2], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));

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
            addr.sin_port = htons(port[1]);
            addr.sin_addr.s_addr = inet_addr(ip[1].c_str());

        if (connect(recvsocket[2], (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            perror("recv connection with Client failed");
            exit(1);
        }
        else {
            std::cout << "recv connection with Client successful" <<"\n";
        }

        const int one = 1;
        setsockopt(recvsocket[2], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));

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

void ServerTrusted::send_input_check_pack_2(input_check_pack_2 &icp, int bw, int bl, int party){
    //ol is the bit length of gamma
    send_ge(icp.index[0], bw, party);
    send_ge(icp.index[1], bw, party);

    send_ge(icp.payload[0], bl, party);
    send_ge(icp.payload[1], bl, party);

    // send_block(icp.init_s[0], party);
    // send_block(icp.init_s[1], party);

    send_size(icp.size, party);

    for (size_t i = 0; i < icp.size; i++) {
        send_block(icp.zs0[0][i], party);
        send_block(icp.zs0[1][i], party);
        send_uint8(icp.zt0[0][i], party);
        send_uint8(icp.zt0[1][i], party);
    }

    for (size_t i = 0; i < icp.size; i++) {
        send_block(icp.zs1[0][i], party);
        send_block(icp.zs1[1][i], party);
        send_uint8(icp.zt1[0][i], party);
        send_uint8(icp.zt1[1][i], party);
    }
    
    for (size_t i = 0; i < icp.size; i++) {
        send_block(icp.sigma[i], party);
        send_uint8(icp.tau[0][i], party);
        send_uint8(icp.tau[1][i], party);
    }

    //Sending Final Correction Word
    //Taking bw to be bitlength of output.
    send_int((icp.T)[0], party);
    send_int((icp.T)[1], party);
    send_ge((icp.W)[0][0], bl, party);
    send_ge((icp.W)[0][1], bl, party);
    send_ge((icp.W)[1][0], bl, party);
    send_ge((icp.W)[1][1], bl, party);
    send_ge((icp.gamma)[0], bl, party);
    send_ge((icp.gamma)[1], bl, party);
};

void ServerTrusted::send_dpf_key(dpf_key k0, int bw, int bl, int party) {
    send_int(k0.height, party);
    send_int(k0.Bout, party);

    send_block(k0.s, party);
    send_uint8(k0.t, party);

    for (int i = 0; i < k0.height; i++) {
        send_block(k0.sigma[i], party);
    }

    for (int i = 0; i < k0.height; i++) {
        send_uint8(k0.tau0[i], party);
    }

    for (int i = 0; i < k0.height; i++) {
        send_uint8(k0.tau1[i], party);
    }

    send_ge(k0.gamma[0], bl, party);
    send_ge(k0.gamma[1], bl, party);

};
