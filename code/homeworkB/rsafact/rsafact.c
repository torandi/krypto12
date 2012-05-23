#include <gmp.h>
#include <stdio.h>
#include "rsafact.h"

gmp_randstate_t state;

//Find factor x in N
//Returns 1 if a factor is found, otherwise 0
int try_find_factor(const mpz_t N, const mpz_t N_minus_1, const mpz_t r, mpz_t x);

void init() {
	gmp_randinit_default(state); //Initialize gmp random state
}

void rsafact(const mpz_t N, const mpz_t e, const mpz_t d, mpz_t p, mpz_t q) {
	mpz_t tmp, r, N_minus_1;
	mpz_init(tmp);  mpz_init(r); mpz_init(N_minus_1);

	//Calculate (2^s)*r = e*d-1
	mpz_mul(tmp, e, d); //tmp = ed
	mpz_sub_ui(tmp, tmp, 1); //tmp = ed -1 
	mpz_sub_ui(N_minus_1, N, 1); //Save the value of N-1 in a separate variable

	calc_2rest(tmp, r);

	//Try to find a factor in N using r until we succedes. (result stored in p)
	while(try_find_factor(N, N_minus_1, r, p) == 0) {};

	//And find the other factor
	mpz_divexact(q, N, p);

	//Kattis requires the lower factor to be first, swap if this is not the case
	if(mpz_cmp(p, q) > 0) 
		mpz_swap(p, q);
}

//Find r such that in = 2^s * r
void calc_2rest(const mpz_t in, mpz_t rest) {
	mpz_t r, q;
	mpz_init(r); mpz_init(q);
	mpz_set(q, in);
	do {
		mpz_set(rest, q);
		mpz_fdiv_qr_ui(q, r, rest, 2);
	} while(mpz_cmp_ui(r, 0) == 0);
}

int try_find_factor(const mpz_t N, const mpz_t N_minus_1, const mpz_t r, mpz_t x) {
	mpz_t w,v, v0;
	mpz_init(w);
	mpz_urandomm(w, state, N_minus_1);
	
	mpz_gcd(x, w, N);
	//If gcd is >1 and <N x it is a factor
	if(mpz_cmp_ui(x, 1) > 0 && mpz_cmp(x, N) < 0) {
		//X is a factor
		return 1;
	}	
	mpz_init(v); mpz_init(v0);

	mpz_powm(v, w, r, N);

	if(mpz_cmp_ui(v, 1) == 0)
		return 0; //fail

	mpz_set_ui(v0, 0);
	while(mpz_cmp_ui(v, 1) != 0) {
		/*if(mpz_cmp(v0, v) == 0) {
			return 0; //fail
		}*/

		mpz_set(v0, v);
		mpz_powm_ui(v, v, 2, N);
	}
	
	if(mpz_congruent_p(v0, N_minus_1, N) == 1) {
		return 0;	
	} else {
		mpz_add_ui(v0, v0, 1);
		mpz_gcd(x, v0, N);
		return 1;
	}
}
