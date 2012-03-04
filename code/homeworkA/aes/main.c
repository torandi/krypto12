#include <stdio.h>
#include <stdlib.h>

#include "aes.h"

int main() {
	char hex[32];
	struct aes_t aes;
	fread(aes.key, 4,4, stdin);
	int i;

	
	while(!feof(stdin)) {	
		fread(aes.iv, 4,4, stdin);
		if(!feof(stdin)) {
			aes_encrypt(&aes);
			fwrite(aes.iv, 4, 4, stdout);
		}
	} 
	return 0;
}
