/**
 * Test data for aes, taken from http://csrc.nist.gov/publications/fips/fips197/fips-197.pdf
 * Converted to c by Andreas Tarandi
 */

const char test_plaintext[32] = "00112233445566778899aabbccddeeff";
const char test_key[32] = "000102030405060708090a0b0c0d0e0f";
const char test_output[32] = "69c4e0d86a7b0430d8cdb78070b4c55a";

struct round_t {
	char start[32];
	char s_box[32];
	char s_row[32];
	char m_col[32];
	char k_sch[32];
};


void build_test_round(struct round_t test_round[10]) {
	sprintf(test_round[0].start,"00102030405060708090a0b0c0d0e0f0");
	sprintf(test_round[0].s_box,"63cab7040953d051cd60e0e7ba70e18c");
	sprintf(test_round[0].s_row,"6353e08c0960e104cd70b751bacad0e7");
	sprintf(test_round[0].m_col,"5f72641557f5bc92f7be3b291db9f91a");
	sprintf(test_round[0].k_sch,"d6aa74fdd2af72fadaa678f1d6ab76fe");
	sprintf(test_round[1].start,"89d810e8855ace682d1843d8cb128fe4");
	sprintf(test_round[1].s_box,"a761ca9b97be8b45d8ad1a611fc97369");
	sprintf(test_round[1].s_row,"a7be1a6997ad739bd8c9ca451f618b61");
	sprintf(test_round[1].m_col,"ff87968431d86a51645151fa773ad009");
	sprintf(test_round[1].k_sch,"b692cf0b643dbdf1be9bc5006830b3fe");
	sprintf(test_round[2].start,"4915598f55e5d7a0daca94fa1f0a63f7");
	sprintf(test_round[2].s_box,"3b59cb73fcd90ee05774222dc067fb68");
	sprintf(test_round[2].s_row,"3bd92268fc74fb735767cbe0c0590e2d");
	sprintf(test_round[2].m_col,"4c9c1e66f771f0762c3f868e534df256");
	sprintf(test_round[2].k_sch,"b6ff744ed2c2c9bf6c590cbf0469bf41");
	sprintf(test_round[3].start,"fa636a2825b339c940668a3157244d17");
	sprintf(test_round[3].s_box,"2dfb02343f6d12dd09337ec75b36e3f0");
	sprintf(test_round[3].s_row,"2d6d7ef03f33e334093602dd5bfb12c7");
	sprintf(test_round[3].m_col,"6385b79ffc538df997be478e7547d691");
	sprintf(test_round[3].k_sch,"47f7f7bc95353e03f96c32bcfd058dfd");
	sprintf(test_round[4].start,"247240236966b3fa6ed2753288425b6c");
	sprintf(test_round[4].s_box,"36400926f9336d2d9fb59d23c42c3950");
	sprintf(test_round[4].s_row,"36339d50f9b539269f2c092dc4406d23");
	sprintf(test_round[4].m_col,"f4bcd45432e554d075f1d6c51dd03b3c");
	sprintf(test_round[4].k_sch,"3caaa3e8a99f9deb50f3af57adf622aa");
	sprintf(test_round[5].start,"c81677bc9b7ac93b25027992b0261996");
	sprintf(test_round[5].s_box,"e847f56514dadde23f77b64fe7f7d490");
	sprintf(test_round[5].s_row,"e8dab6901477d4653ff7f5e2e747dd4f");
	sprintf(test_round[5].m_col,"9816ee7400f87f556b2c049c8e5ad036");
	sprintf(test_round[5].k_sch,"5e390f7df7a69296a7553dc10aa31f6b");
	sprintf(test_round[6].start,"c62fe109f75eedc3cc79395d84f9cf5d");
	sprintf(test_round[6].s_box,"b415f8016858552e4bb6124c5f998a4c");
	sprintf(test_round[6].s_row,"b458124c68b68a014b99f82e5f15554c");
	sprintf(test_round[6].m_col,"c57e1c159a9bd286f05f4be098c63439");
	sprintf(test_round[6].k_sch,"14f9701ae35fe28c440adf4d4ea9c026");
	sprintf(test_round[7].start,"d1876c0f79c4300ab45594add66ff41f");
	sprintf(test_round[7].s_box,"3e175076b61c04678dfc2295f6a8bfc0");
	sprintf(test_round[7].s_row,"3e1c22c0b6fcbf768da85067f6170495");
	sprintf(test_round[7].m_col,"baa03de7a1f9b56ed5512cba5f414d23");
	sprintf(test_round[7].k_sch,"47438735a41c65b9e016baf4aebf7ad2");
	sprintf(test_round[8].start,"fde3bad205e5d0d73547964ef1fe37f1");
	sprintf(test_round[8].s_box,"5411f4b56bd9700e96a0902fa1bb9aa1");
	sprintf(test_round[8].s_row,"54d990a16ba09ab596bbf40ea111702f");
	sprintf(test_round[8].m_col,"e9f74eec023020f61bf2ccf2353c21c7");
	sprintf(test_round[8].k_sch,"549932d1f08557681093ed9cbe2c974e");
	sprintf(test_round[9].start,"bd6e7c3df2b5779e0b61216e8b10b689");
	sprintf(test_round[9].s_box,"7a9f102789d5f50b2beffd9f3dca4ea7");
	sprintf(test_round[9].s_row,"7ad5fda789ef4e272bca100b3d9ff59f");
	sprintf(test_round[9].k_sch,"13111d7fe3944a17f307a78b4d2b30c5");
}
