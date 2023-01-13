#include "client.h"
#include "dpf.h"
#include "keys.h"
#include "server.h"
#include "server_trusted.h"

int bitlength = 2;

int main() {
    uint8_t i = 10;
    int j = i;
    uint8_t k = j;
    std:: cout << j << "\n";
    std:: cout << unsigned(k) << "\n";
}