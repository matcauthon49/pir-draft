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

class Server {
public:
    int sendsocket[2], recvsocket[2];
    int server_id;

    u_int64_t bytes_sent;
    u_int64_t bytes_recieved;

    Server(std::string ip, int port);
    
    void wait_for_client(std::string ip, int port);

    void close();
    void send_ge(GroupElement &ge, int bitwidth, int party);
    void send_block(block &b, int party);
    void send_uint8(uint8_t &i, int party);
    void send_size(size_t &i, int party);
    void send_input_check_pack(input_check_pack icp, int bl, int party);

    GroupElement recv_ge(int bl, int party);
    block recv_block(int party);
    size_t recv_size(int party);
    uint8_t recv_uint8(int party);
    int recv_int(int party);
    dpf_key *recv_dpf_key(int bl, int party);
};