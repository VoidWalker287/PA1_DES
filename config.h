#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

uint8_t encrypt_DES(uint8_t plain_text);

static uint8_t round_key = 0b00111111;

#endif //CONFIG_H
