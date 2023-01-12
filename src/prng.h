#pragma once
#include "group_element.h"
#include "client.h"
#include "server.h"
#include "server_trusted.h"
#include <thread>
#include <chrono>

void input_prng_init(Server *server, int party, block *input_prng);