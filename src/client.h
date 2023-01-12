#pragma once

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
#include "keys.h"

#define PARTY0 0
#define PARTY1 1
#define PARTY_TRUSTED 2
#define CLIENT 3

using namespace osuCrypto;

class Client {
public:
    int sendsocket[3], recvsocket[3];
    int server_id;

    u_int64_t bytes_sent;
    u_int64_t bytes_recieved;

    Client(std::string ip[3], int port[3]);

    void close();
    void send_ge(GroupElement &ge, int bitwidth, int party);

    GroupElement recv_ge(int bl, int party);
    block recv_block(int party);
    size_t recv_size(int party);

    input_check_pack recv_input_check_pack(int bl, int party);
    input_check_pack_2 recv_input_check_pack_2(int bl, int party);

};