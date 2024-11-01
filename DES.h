//
// Created by VoidWalker on 10/29/2024.
//

#ifndef DES_H
#define DES_H

typedef union {
    uint8_t full_byte;
    struct {
        uint8_t right_nibble : 4;
        uint8_t left_nibble : 4;
    };
} byte;

uint8_t s_box[][16] = {
    {14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
    { 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
    { 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
    {15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13}
};

uint8_t round_key;

#endif //DES_H
