#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sha256.h"
#include "../../unit_test.h"

int main() {
	begin_test_suite();

	struct hash_t hash;

	begin_context("rotr()");
	{
		int i;
		uint32_t w = 0x01020304;
		assert_ints_equal(rotr(w, 8), 0x04010203);
	}
	
	end_context();

	begin_context("from_string()");
	{
		const char c1[] = "e5";
		const char c2[] = "0102030405060708090a0b0c0d0e0f10";

		begin_test("e5");
		from_string(c1, &hash);
		assert_ints_equal(hash.data[0], 0xe5);	
	
		begin_test("0102030405060708090a0b0c0d0e0f10");
		from_string(c2, &hash);
		int i;
		for(i=0;i<0x10;++i) {
			assert_ints_equal(hash.data[i], i+1);
		}

	}
	end_context();
	
	end_test_suite();

}
