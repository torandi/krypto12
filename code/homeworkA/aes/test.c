#include <stdlib.h>
#include <stdio.h>

#include "aes.h"
#include "unit_test.h"

int main() {
	char hex1[32] = "00000000000000000000000000000000";
	char hex2[32];
	int i=0;

	struct aes_t aes;

	//Execute test:
	begin_test_suite();
		begin_context("Hex conversion");
			begin_test("00000000000000000000000000000000");
			string_to_data(hex1, aes.key);
			data_to_string(aes.key, hex2);
			assert_strings_equal_n(hex1, hex2,32);

			begin_test("01010101010101010101010101010101");
			sprintf(hex1, "01010101010101010101010101010101");
			string_to_data(hex1, aes.key);
			data_to_string(aes.key, hex2);
			assert_strings_equal_n(hex1, hex2,32);
			for(i=0;i<16; ++i) {
				assert_ints_equal(aes.key[i], 1);
			}

			begin_test("ffffffffffffffffffffffffffffffffff");
			sprintf(hex1, "ffffffffffffffffffffffffffffffffff");
			string_to_data(hex1, aes.key);
			data_to_string(aes.key, hex2);
			assert_strings_equal_n(hex1, hex2,32);
			for(i=0;i<16; ++i) {
				assert_true(aes.key[i]==(char)0xff);
			}

	//char hex1[32] = "000102030405060708090a0b0c0d0e0f";

		end_context();

		begin_context("Key Expansion");

		end_context();
	end_test_suite();
}
