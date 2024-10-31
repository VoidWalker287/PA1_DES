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

// permutate a nibble for DES
uint8_t permutate(const uint8_t value) {
    return (value & 0b0100) << 1 | (value & 0b1000) >> 1 | (value & 0b0001) << 1 | (value & 0b0010) >> 1;
}

// generate round key for each round
uint8_t generate_key(const uint8_t old_key) {
    uint8_t left_half = (old_key & 0b111000) >> 3;
    uint8_t right_half = (old_key & 0b000111);

    left_half = left_half << 1 & 0b111 | (left_half & 0b100) >> 2;
    right_half = right_half << 1 & 0b111 | (right_half & 0b100) >> 2;

    byte temp;
    temp.full_byte = left_half << 3 | right_half;

    byte new_key;

    new_key.bit0 = 0;
    new_key.bit1 = 0;

    return new_key.full_byte;
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

    // generate new round key
    round_key = generate_key(round_key);

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
 * @param cipher_text the input cipher text
 * @return plain text
 */
uint8_t decrypt_DES(const uint8_t cipher_text) {
    // swap left and right nibbles
    const uint8_t swapped = swap_nibbles(cipher_text);

    // DES function stays the same
    const uint8_t new_value = encrypt_DES(swapped);

    // swap nibbles again
    return swap_nibbles(new_value);
}