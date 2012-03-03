#ifndef AES_H
#define AES_H

#define NUM_ROUNDS 10

struct aes_t {
	//Key
	char key[32];

	//Round keys
	char round_keys[NUM_ROUNDS][16];

	char iv[32]; //The current value. Can be: plaintext (input), ciphertext(output) or intermediate value
};

void string_to_data(const char input[32], char target[16]);
void data_to_string(const char data[16], char target[32]);

void aes_key_expansion(struct aes_t * aes);
void aes_run_round(int round, struct aes_t * aes);

#endif
