#ifndef AES_H
#define AES_H

struct aes_t {
	//Key
	char key[16];

	char expanded_key[16*11];

	char iv[16]; //The current value. Can be: plaintext (input), ciphertext(output) or intermediate value
};

void string_to_data(const char input[32], char target[16]);
void data_to_string(const char data[16], char target[32]);

void aes_expand_key(struct aes_t * aes);
void aes_run_round(int round, struct aes_t * aes);

#endif
