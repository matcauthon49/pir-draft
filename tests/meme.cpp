#include <array>
#include <vector>
#include <utility>

int main() {
    uint64_t meme = 69;
    // 01000101

    uint8_t sig = static_cast<uint8_t>(meme >> 0) & 1;
    printf("%d\n", sig);
    sig = static_cast<uint8_t>(meme >> 1) & 1;
    printf("%d\n", sig);
    sig = static_cast<uint8_t>(meme >> 2) & 1;
    printf("%d\n", sig);
    sig = static_cast<uint8_t>(meme >> 3) & 1;
    printf("%d\n", sig);
    sig = static_cast<uint8_t>(meme >> 4) & 1;
    printf("%d\n", sig);
    sig = static_cast<uint8_t>(meme >> 5) & 1;
    printf("%d\n", sig);
    sig = static_cast<uint8_t>(meme >> 6) & 1;
    printf("%d\n", sig);
    sig = static_cast<uint8_t>(meme >> 7) & 1;
    printf("%d\n", sig);
}