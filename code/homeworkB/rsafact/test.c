#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "rsafact.h"
#include "../../unit_test.h"

int main() {

	init();

	begin_test_suite();
	
	mpz_t t1, t2, t3, t4, t5;
	mpz_init(t1); mpz_init(t2);
	mpz_init(t3); mpz_init(t4);
	mpz_init(t5); 

	begin_context("calc_2rest");
		begin_test("9");
			mpz_set_ui(t1, 9);
			calc_2rest(t1, t2);
			assert_ints_equal(mpz_get_ui(t2), 9);

		begin_test("8");
			mpz_set_ui(t1, 8);
			calc_2rest(t1, t2);
			assert_ints_equal(mpz_get_ui(t2), 1);

		begin_test("23492462");
			mpz_set_ui(t1, 23492462);
			calc_2rest(t1, t2);
			assert_ints_equal(mpz_get_ui(t2), 11746231);
		


		end_context();

		begin_context("RSA factoring");
			begin_test("299 5 53: p");
				mpz_set_ui(t1, 299);
				mpz_set_ui(t2, 5);
				mpz_set_ui(t3, 53);
				rsafact(t1, t2, t3, t4, t5);
				assert_ints_equal(mpz_get_ui(t4), 13);
			begin_test("299 5 53: q");
				assert_ints_equal(mpz_get_ui(t5), 23);

			begin_test("22 3 7: p");
				mpz_set_ui(t1, 22);
				mpz_set_ui(t2, 3);
				mpz_set_ui(t3, 7);
				rsafact(t1, t2, t3, t4, t5);
				assert_ints_equal(mpz_get_ui(t4), 2);
			begin_test("22 3 7: q");
				assert_ints_equal(mpz_get_ui(t5), 11);
	end_test_suite();

}
