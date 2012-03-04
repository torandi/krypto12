#include <stdio.h>
#include <stdlib.h>

#include "aes.h"

int main() {
	char hex[32];
	struct aes_t aes;
	fgets((char*)aes.key, 17, stdin);
	int i;

	/*for(i=0;i<4;++i) {
		aes.key[i] = ntohl(aes.key[i]);
	}*/
	data_to_string(aes.key, hex);
	printf("Key: %.32s\n", hex);
	
	while(!feof(stdin)) {	
		fgets((char*)aes.iv, 17, stdin);
		if(!feof(stdin)) {
			for(i=0;i<4;++i) {
			//	aes.iv[i] = ntohl(aes.iv[i]);
			}
			data_to_string(aes.iv, hex);
			printf("Encryption block: %.32s\n", hex);
			aes_encrypt(&aes);
			data_to_string(aes.iv, hex);
			printf("%.32s\n", hex);
			fprintf(stderr, "%.32s\n", aes.iv);
		}
	} 
	return 0;
}
