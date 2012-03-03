#ifndef AES_H
#define AES_H

#include <stdint.h>

struct aes_t {
	uint32_t key[4];
	uint32_t expanded_key[4*11];
	uint32_t iv[4]; //The current value. Can be: plaintext (input), ciphertext(output) or intermediate value
};

void string_to_data(const char input[32], uint32_t * target);
void data_to_string(const uint32_t * data, char target[32]);

void aes_expand_key(struct aes_t * aes);
void aes_run_round(int round, struct aes_t * aes);

uint32_t aes_rot(uint32_t word);
void aes_sbox(uint32_t iv[4]);

#endif
