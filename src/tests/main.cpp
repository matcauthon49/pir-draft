#include "client.h"
#include "dpf.h"
#include "keys.h"
#include "server.h"
#include "server_trusted.h"

int main() {
    GroupElement g = GroupElement(10);
    GroupElement g2 = GroupElement(100);
    GroupElement g3 = GroupElement(1000);

    std::cout << g.bitsize << " " << g2.bitsize << " " << g3.bitsize << "\n";
}