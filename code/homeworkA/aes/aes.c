#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "aes.h"

static uint32_t aes_sub_bytes_word(uint32_t word);

void string_to_data(const char input[32], uint32_t * target) {
	int i;
	for(i=0;i<4; ++i) {
		sscanf(input+i*8,"%8x", target+i);
	}
}

void data_to_string(const uint32_t * data, char target[32]) {
	int i;
	for(i=0;i<4; ++i) {
		sprintf(target+(i*8), "%08x", data[i]);
	}
}

void aes_expand_key(struct aes_t * aes) {
	int i;
	//First key is the original key
	memcpy(aes->expanded_key,aes->key,4); 
	//Expand for the rest of the keys
	int32_t temp; //Temporary variable
	for(i=4;i< 4*11;++i) {
		temp = aes->expanded_key[i-1]; //Previous key column
		if(i % 4 == 0) { //Start of new round key
			//temp = aes_sub_bytes(aes_rot(temp)) ^ ( rcon[i/4] << 24); 
		}
		aes->expanded_key[i] = aes->expanded_key[i-4] ^ temp; 
	}
}

/**
 * Rotate word (a, b, c, d) to (b, c, d, a)
 */
uint32_t aes_rot(uint32_t word) {
	return ( (word << 8) & 0xffffff00) ^ ((word >> 24) & 0xff);
}
