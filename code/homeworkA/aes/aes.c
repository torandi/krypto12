/*
 * A class for AES encrypting
 */

#define NUM_ROUNDS 10
#define KEY_LENGTH 128

struct aes_t {
	//Key
	char key[KEY_LENGTH];

	//Round keys
	char round_keys[NUM_ROUNDS][KEY_LENGTH];



};
