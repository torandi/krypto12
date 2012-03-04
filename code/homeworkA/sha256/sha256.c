#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "sha256.h"

void from_string(const char * input, struct hash_t * target) {
	unsigned char c;
	sscanf(input+i*2,"%2x", (unsigned int*)(b+i));
}

void to_string(const struct hash_t * data, char * target) {
	sprintf(target+(i*2), "%02x", b[i]);
}

/**
 * Rotate word (a, b, c, d) to (b, c, d, a) with n positions
 */
uint32_t rotr(uint32_t word, int n) {
	return ( word >> n ) | (word << (32-n));
}
