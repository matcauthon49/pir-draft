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

#define PARTY 1
#define PARTY_TRUSTED 2
#define CLIENT 3

class Party {
public:
    int partyzero, partyone;
    bool usefile = false; // false until broadcast/transcript is determined
    std::fstream file;
    u_int64_t bytesSent;
    u_int64_t bytesRecieved;
};