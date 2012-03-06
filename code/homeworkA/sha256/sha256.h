#ifndef SHA256_H
#define SHA256_H

#include <stdint.h>

#define INITIAL_MESSAGE_SIZE 64

struct hash_t {
	uint32_t * message;			//The message array
	unsigned char * c_message; //Pointer to the message array, as a char array

	unsigned int message_container_length; //Length of the container in bytes

	unsigned int message_length; //Length of the message in ints
	unsigned int c_message_length;		//Length in chars

	uint32_t hash[8];
	unsigned char * c_hash; //Char array access to the hash
};

/**
 * Must be called on hash_t before it is used
 * Not calling this will cause segfaults
 */
void init_hash(struct hash_t * target);
/**
 * Doubles the message size of the hash
 * Calling this on an uninitialized och finalized hash will cause segfault resp. do nothing
 */
void resize_hash(struct hash_t * target);
/**
 * Frees memory allocated by init and resize. Sets all internal size counters to 0
 */
void finalize_hash(struct hash_t * target);

void from_stdin(struct hash_t * target);
void from_string(const char * input, struct hash_t * target);
void to_string(const struct hash_t * data, char * target);
void data_to_string(const unsigned char * data, unsigned int len, char * target);

uint32_t rotr(uint32_t word, int n);
void sha256_padd_message(struct hash_t * hash);
void sha256_compute(struct hash_t * hash);
void sha256_compute_round(struct hash_t * hash, int n); //n=round

#define TWO_POW_32 4294967296

/**
 * Macros for sha256
 * x,y,z are 32-bit words.
 * Return values are 32-bit words
 */
#define SHA256_SHR(x, n) (x >> n)
#define SHA256_ROTR(x, n) ( x >> n ) | (x << (32-n))

#define SHA256_CH(x,y,z) (x & y) ^ (~x & y)
#define SHA256_MAJ(x,y,z) (x & y) ^ (x & z) ^ (y & z)

#define SHA256_BIG_SIGMA0(x) \
	SHA256_ROTR(x, 2) ^ SHA256_ROTR(x, 13) ^ SHA256_ROTR(x, 22)

#define SHA256_BIG_SIGMA1(x) \
	SHA256_ROTR(x, 6) ^ SHA256_ROTR(x, 11) ^ SHA256_ROTR(x, 25)

#define SHA256_LITTLE_SIGMA0(x) \
	SHA256_ROTR(x, 7) ^ SHA256_ROTR(x, 18) ^ SHA256_ROTR(x, 3)

#define SHA256_LITTLE_SIGMA1(x) \
	SHA256_ROTR(x, 17) ^ SHA256_ROTR(x, 19) ^ SHA256_ROTR(x, 10)

#endif
