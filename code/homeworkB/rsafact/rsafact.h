#include <gmp.h>

//Must be called before first call to rsafact
void init();

/**
 * Factorizes N into p and q with the help of e and d
 */
void rsafact(const mpz_t N, const mpz_t e, const mpz_t d, mpz_t p, mpz_t q);

//Find r such that in = 2^s * r
void calc_2rest(const mpz_t in, mpz_t rest);
