#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sha256.h"
#include "../../unit_test.h"

int main() {
	begin_test_suite();

	struct hash_t hash;
	int i;


	begin_context("rotr()");
	{
		uint32_t w = 0x01020304;
		begin_test("0x01020304");
		assert_ints_equal(rotr(w, 8), 0x04010203);
	}
	end_context();

	begin_context("from_string()");
	{
		const char c1[] = "e5";
		const char c2[] = "0102030405060708090a0b0c0d0e0f10";

		init_hash(&hash);
		begin_test("e5");
		from_string(c1, &hash);
		assert_ints_equal(hash.message[0], 0xe5);	
		assert_ints_equal(hash.message_length, strlen(c1)/2);
		finalize_hash(&hash);

		init_hash(&hash);
	
		from_string(c2, &hash);
		begin_test("0102030405060708090a0b0c0d0e0f10");
		assert_ints_equal(hash.message_length, strlen(c2)/2);
		for(i=0;i<0x10;++i) {
			assert_ints_equal(hash.message[i], i+1);
		}
		finalize_hash(&hash);

	}
	end_context();

	begin_context("to_string()");
		init_hash(&hash);
		begin_test("0-31");
		char correct[65];
		char test[65];
		for(i=0;i<32;++i) {
			hash.c_hash[i] = i;
			sprintf(correct+i*2, "%02x", i);
		}
		to_string(&hash, test);

		assert_strings_equal(test, correct);

		finalize_hash(&hash);

	end_context();

	end_test_suite();

}
