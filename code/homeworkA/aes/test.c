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


		end_context();

		begin_context("Key Expansion");
			{
				sprintf(hex1, "000102030405060708090a0b0c0d0e0f");
				string_to_data(hex1, aes.key);
				aes_expand_key(&aes);
				const char round_keys[10][32] = {
					"d6aa74fdd2af72fadaa678f1d6ab76fe",
					"b692cf0b643dbdf1be9bc5006830b3fe",
					"b6ff744ed2c2c9bf6c590cbf0469bf41",
					"47f7f7bc95353e03f96c32bcfd058dfd",
					"3caaa3e8a99f9deb50f3af57adf622aa",
					"5e390f7df7a69296a7553dc10aa31f6b",
					"14f9701ae35fe28c440adf4d4ea9c026",
					"47438735a41c65b9e016baf4aebf7ad2",
					"549932d1f08557681093ed9cbe2c974e",
					"13111d7fe3944a17f307a78b4d2b30c5"
				};
				char test_name[32];
				for(i=0;i<10;++i) {
					sprintf(test_name, "Round %d", i+1);
					begin_test(test_name);
					data_to_string(aes.round_keys[i], hex1);
					assert_strings_equal_n(hex1, round_keys[i], 32);
				}
			}
		end_context();
	end_test_suite();
}
