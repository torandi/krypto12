#include <stdlib.h>
#include <stdio.h>
#include "rsafact.h"

int main() {
	mpz_t N, e, d, p, q;
	mpz_init(N); mpz_init(e); mpz_init(d); mpz_init(p); mpz_init(q); 
	int r;
	init();
	while(r = gmp_scanf("%Zd %Zd %Zd", N, e, d) == 3) {
		rsafact(N, e, d, p, q);
		gmp_printf("%Zd %Zd\n", p, q);
	}
}
