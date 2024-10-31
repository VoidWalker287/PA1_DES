#include "config.h"
#include "DES.h"

/**
 * Set the round key
 * @param r_key the value of the round key
 */
void set_Round_Key(const uint8_t r_key) {
    round_key = r_key;
}

/**
 * Get the current round key
 * @return the round key
 */
uint8_t get_Round_Key() {
    return round_key;
}

// expand a nibble to 6 bits for DES ( 1 2 3 4 -> 1 1 2 3 4 4)
uint8_t expand(const uint8_t nibble) {
    return ((nibble & 0b1000) << 2) | (nibble << 1) | (nibble & 0x0001);
}

// substitute right nibble using S-box
uint8_t substitute(const uint8_t value) {
    return s_box[(value & 0b100000) >> 4 | value & 0b0001][(value & 0b011110) >> 1];
}

// permute a nibble for DES
uint8_t permute(const uint8_t value) {
    return (value & 0b0100) << 1 | (value & 0b1000) >> 1 | (value & 0b0001) << 1 | (value & 0b0010) >> 1;
}

// generate round key for each encryption or decryption round
uint8_t rotate_key(const uint8_t old_key, bool is_encryption) {
    uint8_t left_half = (old_key & 0b111000) >> 3;
    uint8_t right_half = (old_key & 0b000111);

    if (is_encryption) {
        left_half = left_half << 1 & 0b111 | (left_half & 0b100) >> 2;
        right_half = right_half << 1 & 0b111 | (right_half & 0b100) >> 2;
    } else {
        left_half = left_half >> 1 & 0b111 | (left_half & 0b001) << 2;
        right_half = right_half >> 1 & 0b111 | (right_half & 0b001) << 2;
    }

    return left_half << 3 | right_half;
}

// permute the round key to use in DES
uint8_t permute_key(const uint8_t key) {
    // 4 5 0 3 1 2
    return  (key & 0b010000) << 1 | (key & 0b100000) >> 1 | (key & 0b000001) << 3 |
            (key & 0b001000) >> 1 | key & 0b000010 | (key & 0b000100) >> 2;
}

uint8_t DES(const uint8_t text, bool is_encryption) {
    byte old_byte;
    old_byte.full_byte = text;

    byte new_byte;

    // the right half becomes the new left half
    new_byte.left_nibble = old_byte.right_nibble;

    //expand the right half
    uint8_t expanded = expand(old_byte.right_nibble);

    // generate new round key
    round_key = rotate_key(round_key, is_encryption);

    // XOR with permutation of round key
    expanded ^= permute_key(round_key);

    // perform S-box substitution
    const uint8_t substitution = substitute(expanded);

    // perform straight permutation
    const uint8_t permutation = permute(substitution);

    // XOR with left half to get new right half
    new_byte.right_nibble = old_byte.left_nibble ^ permutation;

    return new_byte.full_byte;
}

// swap nibbles of a byte
uint8_t swap_nibbles(const uint8_t value) {
    byte v;
    v.full_byte = value;

    const uint8_t temp = v.left_nibble;
    v.left_nibble = v.right_nibble;
    v.right_nibble = temp;

    return v.full_byte;
}

/**
 * applies simplified DES encryption algorithm to 8-bit plain text
 * @param plain_text the input plain text
 * @return cipher text
 */
uint8_t encrypt_DES(const uint8_t plain_text) {
    return DES(plain_text, true);
}

/**
 * applies simplified DES encryption algorithm to 8-bit plain text
 * @param cipher_text the input cipher text
 * @return plain text
 */
uint8_t decrypt_DES(const uint8_t cipher_text) {
    // swap left and right nibbles
    const uint8_t swapped = swap_nibbles(cipher_text);

    // DES function stays the same (keys used in reverse)
    const uint8_t new_value = DES(swapped, false);

    // swap nibbles again
    return swap_nibbles(new_value);
}