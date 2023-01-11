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

#include "deps/cryptoTools/cryptoTools/Common/Defines.h"
#include "deps/EzPC/group_element.h"

#define PARTY0 0
#define PARTY1 1
#define PARTY_TRUSTED 2
#define CLIENT 3

class Client {
public:
    int sendsocket[3], recvsocket[3];

    u_int64_t bytes_sent;
    u_int64_t bytes_recieved;

    Client(std::string ip[3], int port[3]);

    void close();
    void send_ge(const GroupElement &ge, int bitwidth, int party);

    GroupElement Client::recv_ge(int bl, int party);
    osuCrypto::block recv_block();
    GroupElement recv_input();

    // blockPair recv_blockpair();
    // MUXinput recv_muxinput(); 
};