#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "aes.h"


void string_to_data(const char input[32], char * target) {
	int i;
	for(i=0;i<16; ++i) {
		unsigned int v;
		sscanf(input+(i*2),"%2x",&v);
		target[i] = (char)v;
	}
}

void data_to_string(const char * data, char target[32]) {
	int i;
	for(i=0;i<16; ++i) {
		sprintf(target+(i*2), "%02x", data[i]);
	}
}

void aes_expand_key(struct aes_t * aes) {
	int i;
	//First key is the original key
	memcpy(aes->expanded_key,aes->key,16); 
	//Expand for the rest of the keys
	char * prev; //Previous 4 byte word
	char temp[4]; //Temporary variable
	for(i=4;i< 4*11;++i) {
		prev = aes->expanded_key+(i-1)*4; //Previous 4 byte word
		if(i % 4 == 0) { //Start of new round key
			
		}
	}
}
