#include <cstdlib>
#include <iostream>
#include <gmp.h>
#include <gmpxx.h>
#include <list>
#include <string>

#include "primes.h"

using namespace std;

#define PRIME_TEST_REPS 10
#define MAX_ITERATIONS 200000  //40000 gives 2 less points and 5s more time
#define FIRST_PRIME_STOP 2000


void init();
bool factorize(mpz_t num);
int factorize_with_primes(mpz_t num,int start, int max);
bool pollard(mpz_t ret, mpz_t N);
void square_plus_one(mpz_t &z);

list<string> output_buffer;

gmp_randstate_t state;

int main() {
   mpz_t num;
   mpz_init(num);
   init();

   gmp_randinit_default(state);

   while(!cin.eof()) {
      cin>>num;
      if(!cin.eof()) {
         output_buffer.clear();
         if(factorize(num)) {
            for(list<string>::iterator it=output_buffer.begin();it!=output_buffer.end(); ++it) {
               cout<<*it<<endl;
            }
         } else {
            cout<<"fail"<<endl;
         }
         cout<<endl;
      }
   }
}

void init() {

}

void output_num(mpz_t num) {
   char buffer[512];
   gmp_snprintf(buffer,512,"%Zd",num);
   output_buffer.push_back(string(buffer));
}

void output_num(unsigned int num) {
   char buffer[512];
   sprintf(buffer,"%u",num);
   output_buffer.push_back(string(buffer));
}

bool factorize(mpz_t num) {
   if(mpz_cmp_ui(num,1)==0) {
      return true;
   }
   int prob_prime = mpz_probab_prime_p(num, PRIME_TEST_REPS);
   //sleep(1);
   if(prob_prime == 0) {
      cerr<<"factorize("<<num<<")"<<endl;
      int fwp = factorize_with_primes(num, 0, FIRST_PRIME_STOP);
      if( fwp == 1 )
         return true;
      else if(fwp == 0)
         return false;

      mpz_t d, tmp;
      mpz_init(d);mpz_init(tmp);// mpz_init(x0);
      if(pollard(d, num)) {
         mpz_fdiv_q(tmp, num, d);
         if(factorize(d) && factorize(tmp))
            return true;
         else
            return false;
      }

      return factorize_with_primes(num, FIRST_PRIME_STOP,-1)==1;
   } else {
      output_num(num);
      return true;
   }
}

int factorize_with_primes(mpz_t num,int start, int max) {
   mpz_t r, q;
   mpz_init(r); mpz_init(q);
   if(max==-1)
      max = num_primes;
   for(int i=start;i<max;++i)  {
      mpz_fdiv_qr_ui(q, r , num, primes[i]); 
      if(mpz_cmp_ui(r, 0) == 0) {
         output_num(primes[i]);
         if(factorize(q))
            return 1;
         else
            return 0;
      }
   }
   return -1;
}

bool pollard(mpz_t g, mpz_t N) {
   int iterations=0;
   
   if(mpz_fdiv_ui(N, 2)==0) {// num % 2
      mpz_set_ui(g, 2);
      return true;
   }
   mpz_t c, m, k;
   mpz_t r, q, ys;
   mpz_t i, x, y;
   mpz_t tmp1, tmp2;

   mpz_init(c); mpz_init(m); mpz_init(k);
   mpz_init(r); mpz_init(q); mpz_init(ys);
   mpz_init(i); mpz_init(x); mpz_init(y);
   mpz_init(tmp1); mpz_init(tmp2);

   mpz_urandomm(y, state, N);
   mpz_urandomm(c, state, N);
   mpz_urandomm(m, state, N);
   mpz_set_ui(g, 1);
   mpz_set_ui(r, 1);
   mpz_set_ui(q, 1);

   while(mpz_cmp_ui(g,1)==0 ) {
      mpz_set(x,y);
      for(mpz_set_ui(i, 0); mpz_cmp(i, r) < 0; mpz_add_ui(i, i, 1)) {
         mpz_powm_ui(y, y, 2, N);
         mpz_add(y, y, c);
         mpz_mod(y, y,N);
      }
      mpz_set_ui(k,0);
      while(mpz_cmp(k, r) < 0 && mpz_cmp_ui(g, 1) == 0) {
         mpz_set(ys, y);
         mpz_sub(tmp1, r, k);
         if(mpz_cmp(tmp1, m) > 0) // tmp1 = min(tmp1, m)
            mpz_set(tmp1, m);
         
         for(mpz_set_ui(i, 0); mpz_cmp(i, tmp1) < 0; mpz_add_ui(i, i, 1)) {
            if(++iterations > MAX_ITERATIONS) {
               return false;
            }
            mpz_powm_ui(y, y, 2, N);
            mpz_add(y, y, c);
            mpz_mod(y,y,N);

            mpz_sub(tmp2, x, y);
            mpz_abs(tmp2, tmp2);
            mpz_mul(tmp2, q, tmp2);
            mpz_mod(q, tmp2, N);
         }
         mpz_gcd(g, q, N);
         mpz_add(k, k, m);
      }
      mpz_mul_ui(r, r, 2);
   }
   if (mpz_cmp(g,N)==0) {
      while(mpz_cmp_ui(g, 1)>0) {
         if(++iterations > MAX_ITERATIONS) {
            return false;
         }
         mpz_powm_ui(ys, ys, 2, N);
         mpz_add(ys, ys, c);
         mpz_mod(ys, ys,N);

         mpz_sub(tmp2, x, ys);
         mpz_abs(tmp2, tmp2);
         mpz_gcd(g, tmp2, N);
      }
   }
   return !(mpz_cmp(g,N)==0);
}

void square_plus_one(mpz_t &z) {
   mpz_pow_ui(z, z , 2);
   mpz_add_ui(z ,z ,1);
}
