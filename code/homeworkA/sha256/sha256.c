#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "sha256.h"
#include "sha256_constants.h"


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
		target->hash[i] = ntohl(sha256_initial_values[i]);
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

	bzero(hash->c_message+hash->c_message_length, k);
	hash->c_message_length+=k;
	int i=0;
	for(i=0;i<8;++i) {
		hash->c_message[hash->c_message_length++] = l[7-i];
	}

	hash->message_length = hash->c_message_length/4; //Set message length (in number of uint32_t)
}
