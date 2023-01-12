#include "prng.h"

using namespace osuCrypto;

void input_prng_init(Server *server, int party, AES *input_prng)
{
    if (server->server_id == PARTY_TRUSTED) {
        AES aesSeed(toBlock(1, time(NULL)));

        auto seed0 = aesSeed.ecbEncBlock(ZeroBlock);
        server->send_block(seed0, PARTY0);
        auto seed1 = aesSeed.ecbEncBlock(OneBlock);
        server->send_block(seed1, PARTY1);

        input_prng[0] = AES(seed0);
        input_prng[1] = AES(seed1);
    }

    else {
        auto seed = server->recv_block(party);
        input_prng[0] = AES(seed);
    }
}