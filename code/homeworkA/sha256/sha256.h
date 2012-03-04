#ifndef SHA256_H
#define SHA256_H

#include <stdint.h>

struct hash_t {
	unsigned char * data;
	unsigned int length;
};

void from_string(const char * input, struct hash_t * target);
void to_string(const struct hash_t * data, char * target);

uint32_t rotr(uint32_t word, int n);

#endif
