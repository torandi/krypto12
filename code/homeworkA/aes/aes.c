#include <stdlib.h>
#include <stdio.h>

#include "aes.h"


void string_to_data(const char input[32], char target[16]) {
	int i;
	for(i=0;i<16; ++i) {
		unsigned int v;
		sscanf(input+(i*2),"%2x",&v);
		target[i] = (char)v;
	}
}

void data_to_string(const char data[16], char target[32]) {
	int i;
	for(i=0;i<16; ++i) {
		sprintf(target+(i*2), "%02x", data[i]);
	}
}

void aes_key_expansion(struct aes_t * aes) {

}
