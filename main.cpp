#include "config.h"



int main() {
    uint8_t plain_text = 0xA5;
    uint8_t cipher_text = encrypt_DES(plain_text);

    return 0;
}


