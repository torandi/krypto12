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
		assert_ints_equal(SHA256_ROTR(w, 8), 0x04010203);
	}
	end_context();

	begin_context("from_string()");
	{
		const char c1[] = "e5";
		const char c2[] = "0102030405060708090a0b0c0d0e0f10";

		init_hash(&hash);
		begin_test("e5");
		from_string(c1, &hash);
		assert_ints_equal(hash.c_message[0], 0xe5);	
		assert_ints_equal(hash.c_message_length, strlen(c1)/2);
		finalize_hash(&hash);

		init_hash(&hash);
	
		from_string(c2, &hash);
		begin_test("0102030405060708090a0b0c0d0e0f10");
		assert_ints_equal(hash.c_message_length, strlen(c2)/2);
		for(i=0;i<0x10;++i) {
			assert_ints_equal(hash.c_message[i], i+1);
		}
		finalize_hash(&hash);

		init_hash(&hash);
		char c3[4096];
		for(i=0;i<2048;++i) {
			sprintf(c3+i*2, "%02x", i%0xff);
		}

		from_string(c3, &hash);
		begin_test("Long string");
		assert_ints_equal(hash.c_message_length, strlen(c3)/2);
		for(i=0;i<2048;++i) {
			assert_ints_equal(hash.c_message[i], i%0xff);
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

	begin_context("Message padding");
	{
		begin_test("abc");

		init_hash(&hash);
		from_string("616263", &hash);

		sha256_padd_message(&hash);

		begin_test("abc: Length test");
		assert_ints_equal(hash.c_message_length, 64);
		assert_ints_equal(hash.message_length, 16);

		begin_test("abc: Data test: first word");
		assert_ints_equal(hash.message[0], 0x80636261);
		for(i=1;i<15;++i)  {
			begin_test("abc: Data test: middle word");
			assert_ints_equal(hash.message[i], 0);
		}
		
		begin_test("abc: Data test: last  word");
		assert_ints_equal(hash.message[15], 0x3000000);

		finalize_hash(&hash);

		init_hash(&hash);

		begin_test("ab61ba11a38b007ff98baa3ab20e2a584e15269fd428db3c857e2a2d568b5725");
		from_string("ab61ba11a38b007ff98baa3ab20e2a584e15269fd428db3c857e2a2d568b5725", &hash);

		sha256_padd_message(&hash);

		assert_ints_equal(hash.c_message_length%64, 0);
		assert_ints_equal(hash.message_length%16, 0);

		finalize_hash(&hash);


		init_hash(&hash);
		begin_test("Long string");

		char c3[4096];
		for(i=0;i<2048;++i) {
			sprintf(c3+i*2, "%02x", i%0xff);
		}

		from_string(c3, &hash);
		printf("Begin padd\n");
		sha256_padd_message(&hash);

		assert_ints_equal(hash.c_message_length%64, 0);
		assert_ints_equal(hash.message_length%16, 0);

		finalize_hash(&hash);
	}	
	end_context();

	end_test_suite();

}
