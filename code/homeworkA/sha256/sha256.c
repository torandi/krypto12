#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "sha256.h"


/**
 * Must be called on hash_t before it is used
 * Not calling this will cause segfaults
 */
void init_hash(struct hash_t * target) {
	target->message = (uint32_t*) malloc(INITIAL_MESSAGE_SIZE);
	target->c_message = target->message;
	target->message_container_length = INITIAL_MESSAGE_SIZE;
	target->message_length = 0;
	target->c_message_length = 0;
	target->c_hash = target->hash;

}

/**
 * Doubles the message size of the hash
 * Calling this on an uninitialized och finalized hash will cause segfault resp. do nothing
 */
void resize_hash(struct hash_t * target) {
	target->message_container_length*=2;
	target->message = (uint32_t*) realloc(target->message, target->message_container_length);
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
			target->message[target->c_message_length++] = (unsigned char)l;
		}
	} while(r == 1);
}

/**
 * Fills hash from string
 */
void from_string(const char * input, struct hash_t * target) {
	unsigned int l;
	unsigned int r;
	unsigned char pos=0;
	do {
		r = sscanf(input+pos,"%2x", &l);
		pos+=2;
		if(r == 1) { //Filled one variable
			if(target->c_message_length == target->message_container_length)
				resize_hash(target);
			target->message[target->c_message_length++] = (unsigned char)l;
		}
	} while(r == 1);
}

/**
 * fills target with the hash in hex representation
 */
void to_string(const struct hash_t * hash, char target[65]) {
	int i;
	for(i=0; i<32;++i) {
		sprintf(target+(i*2), "%02x", hash->c_hash[i]);
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

/**
 * Performs padding
 */
void sha256_padd_message(struct hash_t * hash) {
	
}
