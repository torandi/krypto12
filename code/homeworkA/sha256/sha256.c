#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "sha256.h"
#include "sha256_constants.h"

#define DEBUG 1

/**
 * Must be called on hash_t before it is used
 * Not calling this will cause segfaults
 */
void init_hash(struct hash_t * target) {
	target->message = (uint32_t*) malloc(INITIAL_MESSAGE_SIZE);
	target->c_message = (unsigned char*)target->message;
	target->message_container_length = INITIAL_MESSAGE_SIZE;
	target->message_length = 0;
	target->c_message_length = 0;
	target->c_hash = (unsigned char*)target->hash;

	//Set initial hash values:
	int i;
	for(i=0;i<8;++i) {
		target->hash[i] = sha256_initial_values[i];
	}
}

/**
 * Doubles the message size of the hash
 * Calling this on an uninitialized och finalized hash will cause segfault resp. do nothing
 */
void resize_hash(struct hash_t * target) {
	target->message_container_length*=2;
	target->message = (uint32_t*) realloc(target->message, target->message_container_length);
	target->c_message = (unsigned char*)target->message;
}

/**
 * Frees memory allocated by init and resize. Sets all internal size counters to 0
 */
void finalize_hash(struct hash_t * target) {
	free(target->message);
	target->message_length = 0;
	target->c_message_length = 0;
	target->message_container_length = 0;
}

/**
 * Fills hash from stdin
 */
void from_stdin(struct hash_t * target) {
	unsigned int l;
	unsigned int r;
	do {
		r = scanf("%2x", &l);
		if(r == 1) { //Filled one variable
			if(target->c_message_length == target->message_container_length)
				resize_hash(target);
			target->c_message[target->c_message_length++] = (unsigned char)l;
		}
	} while(r == 1 && !feof(stdin));
}

/**
 * Fills hash from string
 */
void from_string(const char * input, struct hash_t * target) {
	unsigned int l;
	unsigned int r;
	unsigned int  pos=0;
	do {
		r = sscanf(input+pos,"%2x", &l);
		pos+=2;
		if(r == 1) { //Filled one variable
			if(target->c_message_length == target->message_container_length)
				resize_hash(target);
			target->c_message[target->c_message_length++] = (unsigned char)l;
		}
	} while(r == 1 && pos < strlen(input));
}

/**
 * fills target with the hash in hex representation
 */
void to_string(const struct hash_t * hash, char target[65]) {
	return data_to_string(hash->c_hash, 32, target);
}

void data_to_string(const unsigned char * data, unsigned int len, char * target) {
	int i;
	for(i=0; i<len;++i) {
		sprintf(target+(i*2), "%02x", data[i]);
	}
}

/*************************
 * Here begins the actual functions for sha256
 ************************/

/**
 * Rotate word (a, b, c, d) to (b, c, d, a) with n positions
 */
uint32_t rotr(uint32_t word, int n) {
	return ( word >> n ) | (word << (32-n));
}

void sha256_compute(struct hash_t * hash) {
	sha256_padd_message(hash);
	int i;
	int N=hash->message_length/16;
	for(i=0;i<N;++i) {
		sha256_compute_round(hash, i);
	}
}	

/**
 * Performs padding
 */
void sha256_padd_message(struct hash_t * hash) {
	uint64_t l_in_binary = (uint64_t) hash->c_message_length*8; //Get a 64-bit binary representation of message_length
	unsigned char * l = (unsigned char *)&l_in_binary; //Char pointer to l, to be able to get the msb order

	int k = (56-(hash->c_message_length+1))%64;  //Bytes to padd

	//Check if we need to resize the container:
	if(hash->c_message_length+k+9 > hash->message_container_length) { 
		//+3(bytes) comes from +1 (1000 0000) and then 8 bytes in the 64-bit representation of l
		hash->message_container_length = hash->c_message_length+k+3;
		hash->message = (uint32_t*) realloc(hash->message, hash->message_container_length);
		hash->c_message = (unsigned char*)hash->message;
	}
	
	//Append 1000 0000  = 0x80
	hash->c_message[hash->c_message_length++] = 0x80;

	bzero(hash->c_message+hash->c_message_length, k); //Add k bytes of 0
	hash->c_message_length+=k;

	//Add reversed byte order of l_in_binary
	int i=0;
	for(i=0;i<8;++i) {
		hash->c_message[hash->c_message_length++] = l[7-i];
	}

	hash->message_length = hash->c_message_length/4; //Set message length (in number of uint32_t)

	//Reverse the *@(#&*@$ byte order. GAAAH
	for(i=0;	i<hash->message_length;++i) {
		hash->message[i] = ntohl(hash->message[i]);
	}

}

void sha256_compute_round(struct hash_t * hash, int n) {
	uint32_t T[2];
	uint32_t tmp[8];
	uint32_t W[64];
	int i;

	memcpy(W, hash->message+n*16, 64); //Copy the 16 words for this message block into W

#if DEBUG 
	printf("\n=============== ROUND %d =================\n", n+1);
	printf("Initial hash values:\n");
	for(i=0;i<8;++i) {
		printf("H[%d] = %08x\n", i, hash->hash[i]);
	}
	printf("\nBlock contents:\n");
#endif

#if DEBUG
	for(i=0;i<16;++i)
		printf("W[%d] = %08x\n", i, W[i]);
#endif

	for(i=16; i<64;++i) {
		W[i] = SHA256_LITTLE_SIGMA1(W[i-2]) + W[i-7] + SHA256_LITTLE_SIGMA0(W[i-15]) + W[i-16];
	}

	//Initialize the tmp variables:
	memcpy(tmp, hash->hash, 32);

#if DEBUG
	printf("\n\n");
#endif

	for(i=0;i<64;++i) {
		T[0] = (tmp[7] + SHA256_BIG_SIGMA1(tmp[4]) + SHA256_CH(tmp[4], tmp[5], tmp[6]) + sha256_round_constants[i] +  W[i]);
		T[1] = (SHA256_BIG_SIGMA0(tmp[0]) + SHA256_MAJ(tmp[0], tmp[1], tmp[2]));
		tmp[7] = tmp[6];
		tmp[6] = tmp[5];
		tmp[5] = tmp[4];
		tmp[4] = (tmp[3]+T[0]);
		tmp[3] = tmp[2];
		tmp[2] = tmp[1];
		tmp[1] = tmp[0];
		tmp[0] = (T[0] + T[1]);

#if DEBUG
	printf("t=%2d: %08x %08x %08x %08x %08x %08x %08x %08x\n", i,
			tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5], tmp[6], tmp[7]);
#endif
	}
#if DEBUG
	printf("\n\n");
#endif
	for(i=0;i<8; ++i) {
#if DEBUG
		printf("H[%d] = %08x + %08x = ", i, hash->hash[i], tmp[i]);
#endif
		hash->hash[i] += tmp[i];
#if DEBUG
		printf("%08x\n", hash->hash[i]);
#endif
	}
}
