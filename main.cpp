#include "config.h"
#include <iostream>

#define EXAMPLE_PLAIN_TEXT 0b10111111
#define EXAMPLE_CIPHER_KEY 0b101011

void one_round(uint8_t p_text, uint8_t r_key);
void three_rounds(uint8_t p_text, uint8_t r_key);

int main() {
    printf("Simplified DES Example\n");

    printf("\nOne Round:\n");
    one_round(EXAMPLE_PLAIN_TEXT, EXAMPLE_CIPHER_KEY);

    printf("\nThree Rounds:\n");
    three_rounds(EXAMPLE_PLAIN_TEXT, EXAMPLE_CIPHER_KEY);
    return 0;
}

void one_round(const uint8_t p_text, const uint8_t r_key) {
    set_Round_Key(r_key);
    printf("Round key set to 0x%02X\n", get_Round_Key());
    const uint8_t cipher_text = encrypt_DES(p_text);
    const uint8_t decrypted_text = decrypt_DES(cipher_text);
    printf("Plain Text: 0x%02X\n"
           "Cipher Text: 0x%02X\n"
           "Decrypted Text: 0x%02X\n",
           p_text, cipher_text, decrypted_text);
}

void three_rounds(const uint8_t p_text, const uint8_t r_key) {
    set_Round_Key(r_key);
    printf("Round key set to 0x%02X\n"
           "Initial Plain Text: 0x%02X\n",
           get_Round_Key(), p_text);

    uint8_t text = p_text;

    printf("\nEncryption\n");
    for (int round = 1; round <= 3; round++) {
        const uint8_t new_text = encrypt_DES(text);
        printf("Round %d: 0x%02X ---> 0x%02X, new round key: 0x%02X\n",
               round, text, new_text, get_Round_Key());
        text = new_text;
    }

    printf("Cipher Text: 0x%02X\n\nDecryption\n", text);
    for (int round = 1; round <= 3; round++) {
        const uint8_t new_text = decrypt_DES(text);
        printf("Round %d: 0x%02X ---> 0x%02X, new round key: 0x%02X\n",
               round, text, new_text, get_Round_Key());
        text = new_text;
    }
    printf("Plain Text: 0x%02X\n", text);
}
