#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "aes.h"
#include "unit_test.h"

#include "test_data.h"

int main() {
	char hex1[33] = "00000000000000000000000000000000";
	char hex2[33];
	int i=0;

	struct aes_t aes;

	struct round_t test_round[10];
	build_test_round(test_round);
	//Execute test:
	begin_test_suite();
	
		begin_context("Hex conversion");
			begin_test("00000000000000000000000000000000");
			string_to_data(hex1, aes.key);
			data_to_string(aes.key, hex2);
			assert_strings_equal_n(hex2,hex1,32);

			begin_test("01010101010101010101010101010101");
			sprintf(hex1, "01010101010101010101010101010101");
			string_to_data(hex1, aes.key);
			data_to_string(aes.key, hex2);
			assert_strings_equal_n(hex2,hex1,32);
			for(i=0;i<4; ++i) {
				assert_ints_equal(aes.key[i], 0x01010101);
			}

			begin_test("ffffffffffffffffffffffffffffffffff");
			sprintf(hex1, "ffffffffffffffffffffffffffffffff");
			
			string_to_data(hex1, aes.key);
			data_to_string(aes.key, hex2);
			assert_strings_equal_n(hex2, hex1,32);
			for(i=0;i<4; ++i) {
				assert_ints_equal(aes.key[i], 0xffffffff);
			}

			begin_test("000102030405060708090a0b0c0d0e0f");
			sprintf(hex1, "000102030405060708090a0b0c0d0e0f");
			string_to_data(hex1, aes.key);
			data_to_string(aes.key, hex2);
			assert_strings_equal_n(hex2, hex1,32);

			begin_test("Byte order");
			unsigned char  * b = (unsigned char*)aes.key;
			memset(b, 0, 16);

			for(i=0;i<16;++i) {
				b[i] = i+1;
			}
			data_to_string(aes.key, hex1);
			assert_strings_equal_n(hex1, "0102030405060708090a0b0c0d0e0f10",32);

		end_context();

		
		begin_context("aes_rot");
			begin_test("Word = 0");
			assert_ints_equal(aes_rot(0), 0);

			begin_test("Word = (0x1A in msb)");
			assert_ints_equal( aes_rot(0x1A<<24), 0x1a);
		end_context();

		begin_context("aes_sub_bytes");
			{
				const char sbox_test_values[][32] = {
					"00102030405060708090a0b0c0d0e0f0", "63cab7040953d051cd60e0e7ba70e18c",
					"89d810e8855ace682d1843d8cb128fe4", "a761ca9b97be8b45d8ad1a611fc97369",
					"4915598f55e5d7a0daca94fa1f0a63f7", "3b59cb73fcd90ee05774222dc067fb68",
					"fa636a2825b339c940668a3157244d17", "2dfb02343f6d12dd09337ec75b36e3f0", 
					"247240236966b3fa6ed2753288425b6c", "36400926f9336d2d9fb59d23c42c3950",
					"c81677bc9b7ac93b25027992b0261996", "e847f56514dadde23f77b64fe7f7d490"
				};
				char test_name[32];
				for(i=0;i<6; ++i) {
					sprintf(test_name, "SubBytes Run %d", i+1);
					begin_test(test_name);
					string_to_data(sbox_test_values[i*2], aes.iv);
					aes_sbox(aes.iv);
					data_to_string(aes.iv, hex1);
					assert_strings_equal_n(hex1,sbox_test_values[i*2+1] , 32);
				}
			}
		end_context();

		begin_context("Round keys");
			{
				sprintf(hex1, "000102030405060708090a0b0c0d0e0f");
				string_to_data(hex1, aes.key);
				aes_expand_key(&aes);
				const char round_keys[11][32] = {
					"000102030405060708090a0b0c0d0e0f",
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
				for(i=0;i<11;++i) {
					sprintf(test_name, "Round %d", i+1);
					begin_test(test_name);
					data_to_string(aes.expanded_key+(i*4), hex1);
					assert_strings_equal_n(hex1, round_keys[i], 32);
				}
			}
		end_context();


		begin_context("AES Step through");
		{
			string_to_data(test_key, aes.key);
			string_to_data(test_plaintext, aes.iv);

			aes_expand_key(&aes);

			aes_add_round_key(&aes, 0);

			begin_test("PreRound: AddRoundKey");
			data_to_string(aes.iv, hex1);
			assert_strings_equal_n(hex1, test_round[0].start, 32);

			char test_name[32];
			for(i = 0; i<10; ++i) {
				//Key Expansion
				
				sprintf(test_name, "Round %d: Verify round key", i+1);
				begin_test(test_name);
				data_to_string(aes.expanded_key+(i+1)*4, hex1);
				assert_strings_equal_n(hex1, test_round[i].k_sch, 32);


				//SubBytes
				string_to_data(test_round[i].start, aes.iv); //Reset iv to start
				sprintf(test_name, "Round %d: SubBytes", i+1);
				begin_test(test_name);
				aes_sub_bytes(&aes);
				data_to_string(aes.iv, hex1);
				assert_strings_equal_n(hex1, test_round[i].s_box, 32);
				string_to_data(test_round[i].s_box, aes.iv); //Reset iv to prevent error propagation

				//ShiftRows
				sprintf(test_name, "Round %d: ShiftRows", i+1);
				begin_test(test_name);
				aes_shift_rows(&aes);
				data_to_string(aes.iv, hex1);
				assert_strings_equal_n(hex1, test_round[i].s_row, 32);
				string_to_data(test_round[i].s_row, aes.iv); //Reset iv to prevent error propagation

				if(i != 9) {
					//MixColumns
					sprintf(test_name, "Round %d: MixColumns", i+1);
					begin_test(test_name);
					aes_mix_columns(&aes);
					data_to_string(aes.iv, hex1);
					assert_strings_equal_n(hex1, test_round[i].m_col, 32);
					string_to_data(test_round[i].m_col, aes.iv); //Reset iv to prevent error propagation
				}

				//AddRoundKey
				sprintf(test_name, "Round %d: AddRoundKey", i+1);
				begin_test(test_name);
				aes_add_round_key(&aes,i+1);
				data_to_string(aes.iv, hex1);
				if(i != 9)
					assert_strings_equal_n(hex1, test_round[i+1].start, 32);
				else
					assert_strings_equal_n(hex1, test_output, 32);

			}
		}	
		end_context();

		begin_context("Complete run");
			begin_test("Kattis example data");
			sprintf(hex1, "F4C020A0A1F604FD343FAC6A7E6AE0F9");
			string_to_data(hex1, aes.key);
			sprintf(hex1, "F295B9318B994434D93D98A4E449AFD8");
			string_to_data(hex1, aes.iv);
			aes_encrypt(&aes);
			data_to_string(aes.iv, hex1);
			assert_strings_equal_n(hex1, "52e418cbb1be4949308b381691b109fe", 32);
		end_context();
		
/*
		printf("MixColums debug\n");
		//sprintf(hex1, "d4bf5d30e0b452aeb84111f11e2798e5");
		//sprintf(hex1, "d4d4d4d5010101010101010101010101");
		sprintf(hex1, "d4d4d4d5c6c6c6c6c6c6c6c6c6c6c6c6");
		string_to_data(hex1, aes.iv);
		unsigned char * iv = (unsigned char *)aes.iv;
		int j;
		for(i=0;i<4;++i) {
			for(j=0;j<4;++j) {
				printf("%02x ", iv[j*4+(3-i)]);
			}
			printf("\n");
		}
		printf("----\n");
		aes_mix_columns(&aes);
		for(i=0;i<4;++i) {
			for(j=0;j<4;++j) {
				printf("%02x ", iv[j*4+3-i]);
			}
			printf("\n");
		}
		*/
	end_test_suite();

}
