#include "config.h"
#include "DES.h"

#include <ios>

// expand a nibble to 6 bits for DES ( 1 2 3 4 -> 1 1 2 3 4 4)
uint8_t expand(const uint8_t nibble) {
    return ((nibble & 0b1000) << 2) | (nibble << 1) | (nibble & 0x0001);
}

// substitute right nibble using S-box
uint8_t substitute(const uint8_t value) {
    return s_box[(value & 0b100000) >> 4 | value & 0b0001][(value & 0b011110) >> 1];
}

// permutate a nibble for DES
uint8_t permutate(const uint8_t value) {
    return (value & 0b0100) << 1 | (value & 0b1000) >> 1 | (value & 0b0001) << 1 | (value & 0b0010) >> 1;
}

/**
 * applies simplified DES encryption algorithm to 8-bit plain text
 * @param plain_text the input plain text
 * @return cipher text
 */
uint8_t encrypt_DES(const uint8_t plain_text) {
    byte old_byte;
    old_byte.full_byte = plain_text;

    byte new_byte;

    // the right half becomes the new left half
    new_byte.left_nibble = old_byte.right_nibble;

    //expand the right half
    uint8_t expanded = expand(old_byte.right_nibble);

    // XOR with round key
    expanded ^= round_key;

    // perform S-box substitution
    const uint8_t substitution = substitute(expanded);

    // perform straight permutation
    const uint8_t permutation = permutate(substitution);

    // XOR with left half to get new right half
    new_byte.right_nibble = old_byte.left_nibble ^ permutation;

    return new_byte.full_byte;
}

uint8_t generate_key(const uint8_t old_key) {
    uint8_t left_half = (old_key & 0b111000) >> 3;
    uint8_t right_half = (old_key & 0b000111);

    left_half = left_half << 1 & 0b111 | (left_half & 0b100) >> 2;
    right_half = right_half << 1 & 0b111 | (right_half & 0b100) >> 2;

    const uint8_t new_key = left_half << 3 | right_half;

    return (new_key & 0b010000) << 1 | (new_key & 0b100000) >> 1 | (new_key & 0b000001) << 3 | (new_key & 0b001000) >> 1 | new_key & 0b000010 | (new_key & 0b000100) >> 2;
}