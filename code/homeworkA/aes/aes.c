#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "aes.h"
#include "aes_constants.h"

static uint32_t aes_sub_bytes_word(uint32_t word);

void aes_encrypt(struct aes_t * aes) {
	int i;
	//Pre round
	aes_expand_key(aes);
	aes_add_round_key(aes, 0);

	for(i=1;i<10;++i) {
		//Normal round
		aes_sub_bytes(aes);
		aes_shift_rows(aes);
		aes_mix_columns(aes);
		aes_add_round_key(aes, i);
	}
	//Last round
	aes_sub_bytes(aes);
	aes_shift_rows(aes);
	aes_add_round_key(aes, 10);
}

void string_to_data(const char input[33], uint32_t * target) {
	int i;
	unsigned char * b = (unsigned char*)target;
	for(i=0;i<16; ++i) {
		unsigned char c;
		sscanf(input+i*2,"%2x", (unsigned int*)(b+i));
	}
}

void data_to_string(const uint32_t * data, char target[33]) {
	int i;
	unsigned char * b = (unsigned char*)data;
	for(i=0;i<16; ++i) {
		sprintf(target+(i*2), "%02x", b[i]);
	}
}

void aes_expand_key(struct aes_t * aes) {
	int i;
	//First key is the original key
	memcpy(aes->expanded_key,aes->key,16); 
	//Expand for the rest of the keys
	uint32_t temp; //Temporary variable
	for(i=4;i< 4*11;++i) {
		temp = aes->expanded_key[i-1]; //Previous key column
		if(i % 4 == 0) { //Start of new round key
			temp = aes_sub_bytes_word(aes_rot(temp)) ^ ( ((uint32_t)rcon[i/4])); 
		} 
		aes->expanded_key[i] = aes->expanded_key[i-4] ^ temp; 
	}
}

/**
 * Rotate word (a, b, c, d) to (b, c, d, a)
 */
uint32_t aes_rot(uint32_t word) {
	return ( word >> 8 ) | ((word & 0xff)<< 24);
}

/**
 * Performs sub_bytes on a single word (byte for byte)
 */
uint32_t aes_sub_bytes_word(uint32_t word) {
	unsigned char * b=(unsigned char*)&word;
	return (
			( sbox[b[0]]  ) |
			( sbox[b[1]] << 8 ) |
			( sbox[b[2]] << 16 ) |
			( sbox[b[3]] << 24)
		);
}

void aes_sbox(uint32_t iv[4]) {
	int i;
	for(i=0;i<4;++i) {
		iv[i] = aes_sub_bytes_word(iv[i]);
	}
}

void aes_sub_bytes(struct aes_t * aes) {
	aes_sbox(aes->iv);
}

void aes_shift_rows(struct aes_t * aes) {
	uint32_t iv[4];
	memcpy(iv,aes->iv,16); //Make a copy
	int i;
	for(i=0;i<4; ++i) {
		aes->iv[i] = (
			(iv[(i)%4] & 0xff)  |
			(  iv[(i+1)%4] & 0xff00) |
			(  iv[(i+2)%4] & 0xff0000) |
			(  iv[(i+3)%4] & 0xff000000)
		);
	}
}


void aes_mix_columns(struct aes_t * aes) {
	unsigned char iv_out[16];
	unsigned char i;

	for(i=0;i<4; ++i) {
		unsigned char * iv = (unsigned char*) (aes->iv+i);

		//Perform transform i Rijndael's Galois field
		//mulitplication is done by lookup and addition by xor
			
		iv_out[i*4] = galois_2[iv[0]] ^ galois_3[iv[1]] ^ iv[2] ^ iv[3];
		iv_out[i*4+1] = iv[0] ^ galois_2[iv[1]] ^ galois_3[iv[2]] ^ iv[3];
		iv_out[i*4+2] = iv[0] ^ iv[1] ^ galois_2[iv[2]] ^ galois_3[iv[3]];
		iv_out[i*4+3] = galois_3[iv[0]] ^ iv[1] ^ iv[2] ^ galois_2[iv[3]];

		
	}

	memcpy(aes->iv,iv_out,16);  //Copy output to iv
}

void aes_add_round_key(struct aes_t  * aes, int round) {
	aes->iv[0] ^= aes->expanded_key[4*round];
	aes->iv[1] ^= aes->expanded_key[4*round+1];
	aes->iv[2] ^= aes->expanded_key[4*round+2];
	aes->iv[3] ^= aes->expanded_key[4*round+3];
}
