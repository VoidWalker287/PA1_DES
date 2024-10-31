#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

void set_Round_Key(uint8_t r_key);
uint8_t get_Round_Key();
uint8_t encrypt_DES(uint8_t plain_text);
uint8_t decrypt_DES(uint8_t cipher_text);

#endif //CONFIG_H
