#pragma once

#include "client.h"

#define PORT 8080

// Opens Client and establishes connection.
Client::Client(std::string ip[3], int port[3]) {
 std::cerr << "trying to connect with server...";
    {
        recvsocket[0] = socket(AF_INET, SOCK_STREAM, 0);
        if (recvsocket < 0) {
            perror("socket");
            exit(1);
        }
        struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port[0]);
            addr.sin_addr.s_addr = inet_addr(ip[0].c_str());

        if (connect(recvsocket[0], (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            perror("connect");
            exit(1);
        }

        const int one = 1;
        setsockopt(recvsocket[0], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
    }

    sleep(1);

    {
        sendsocket[0] = socket(AF_INET, SOCK_STREAM, 0);
        if (sendsocket < 0) {
            perror("socket");
            exit(1);
        }
        struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port[0]+3);
            addr.sin_addr.s_addr = inet_addr(ip[0].c_str());

        if (connect(sendsocket[0], (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            perror("connect");
            exit(1);
        }

        const int one = 1;
        setsockopt(sendsocket[0], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
    }

    {
        recvsocket[1] = socket(AF_INET, SOCK_STREAM, 0);
        if (recvsocket < 0) {
            perror("socket");
            exit(1);
        }
        struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port[1]);
            addr.sin_addr.s_addr = inet_addr(ip[1].c_str());

        if (connect(recvsocket[1], (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            perror("connect");
            exit(1);
        }

        const int one = 1;
        setsockopt(recvsocket[1], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
    }

    sleep(1);

    {
        sendsocket[1] = socket(AF_INET, SOCK_STREAM, 0);
        if (sendsocket < 0) {
            perror("socket");
            exit(1);
        }
        struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port[1]+3);
            addr.sin_addr.s_addr = inet_addr(ip[1].c_str());

        if (connect(sendsocket[1], (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            perror("connect");
            exit(1);
        }

        const int one = 1;
        setsockopt(sendsocket[1], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
    }

    {
        recvsocket[2] = socket(AF_INET, SOCK_STREAM, 0);
        if (recvsocket < 0) {
            perror("socket");
            exit(1);
        }
        struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port[2]);
            addr.sin_addr.s_addr = inet_addr(ip[2].c_str());

        if (connect(recvsocket[2], (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            perror("connect");
            exit(1);
        }

        const int one = 1;
        setsockopt(recvsocket[2], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
    }

    sleep(1);

    {
        sendsocket[2] = socket(AF_INET, SOCK_STREAM, 0);
        if (sendsocket < 0) {
            perror("socket");
            exit(1);
        }
        struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port[2]+3);
            addr.sin_addr.s_addr = inet_addr(ip[2].c_str());

        if (connect(sendsocket[2], (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            perror("connect");
            exit(1);
        }

        const int one = 1;
        setsockopt(sendsocket[2], IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
    }
    
    std::cerr << "connected" << std::endl;
};

// Closes the socket.
void Client::close() {
        ::close(sendsocket[0]);
        ::close(recvsocket[0]);
        ::close(sendsocket[1]);
        ::close(recvsocket[1]);
        ::close(sendsocket[2]);
        ::close(recvsocket[2]);
}
