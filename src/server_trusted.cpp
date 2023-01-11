#include "server_trusted.h"

// Opens Server and establishes connection.
ServerTrusted::ServerTrusted(std::string ip[2], int port[2]) {
    std::cerr << "establishing connection...\n";
    const int one = 1;

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
            addr.sin_port = htons(port[0]+3);
            addr.sin_addr.s_addr = inet_addr(ip[0].c_str());

        if (connect(sendsocket[0], (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            perror("send connection with P0 failed");
            exit(1);
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
            addr.sin_port = htons(port[1]);
            addr.sin_addr.s_addr = inet_addr(ip[1].c_str());

        if (connect(recvsocket[1], (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            perror("recv connection with P1 failed");
            exit(1);
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
            addr.sin_port = htons(port[1]+3);
            addr.sin_addr.s_addr = inet_addr(ip[1].c_str());

        if (connect(sendsocket[1], (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            perror("send connection with P1 failed");
            exit(1);
        }

        const int one = 1;
        setsockopt(sendsocket[1], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
    }
    
};

void ServerTrusted::wait_for_client(std::string ip, int port) {
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

        sendsocket[2] = accept(mysocket, (struct sockaddr *)&dest_addr, (socklen_t*)&dest_addrlen);
        setsockopt(sendsocket[2], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
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

        recvsocket[2] = accept(mysocket, (struct sockaddr *)&dest_addr, (socklen_t*)&dest_addrlen);
        setsockopt(recvsocket[2], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
    }
}

void ServerTrusted::close() {
    ::close(sendsocket[0]);
    ::close(sendsocket[1]);
    ::close(sendsocket[2]);
    ::close(sendsocket[0]);
    ::close(recvsocket[1]);
    ::close(recvsocket[2]);
}
