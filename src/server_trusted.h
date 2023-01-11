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

class ServerTrusted {
public:
    int sendsocket[3], recvsocket[3];

    u_int64_t bytes_sent;
    u_int64_t bytes_recieved;

    ServerTrusted(std::string ip[2], int port[2]);

    void wait_for_client(std::string ip, int port);

    void close();
    void send_ge(const GroupElement &ge, int bitwidth, int party);
    void send_block(const block &b, int party);
    void send_input_check_pack(const input_check_pack *icp, int bl, int party);

    dpf_key recv_dpf_key(int bl, int party);
    GroupElement recv_ge(int bl, int party);
};