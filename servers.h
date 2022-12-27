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

#define PARTY0 1
#define PARTY1 2
#define PARTY_TRUSTED 3
#define CLIENT 4

class Party {
public:
    int sendsocket, recvsocket;

    u_int64_t bytesSent;
    u_int64_t bytesRecieved;

    Party(std::string ip, int port);
    Party(int sendsocket, int recvsocket):
          sendsocket(sendsocket),
          recvsocket(recvsocket) {}

    void close();
    void send_ge(const GroupElement &ge, int bitwidth);
    void send_block(const osuCrypto::block &b);
    void send_input(const GroupElement &g);
    
    // void send_block_pair(blockPair &bp);
    // void send_muxinput(MUXinput &mi);

    GroupElement recv_ge();
    osuCrypto::block recv_block();
    GroupElement recv_input();

    // blockPair recv_blockpair();
    // MUXinput recv_muxinput(); 
};