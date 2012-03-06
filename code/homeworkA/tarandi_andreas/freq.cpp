/*
 * A small c++ program that performs frequency analysis of an input text
 * Usage:
 * ./freq input_file
 * Author: Andreas Tarandi - taran at kth dot se
 */

/****
 * Settings
 ***/

// Set to true to ignore newlines from input
bool ignore_newline = true;

//Threat space as space (that is, reset {bi,tri}gram count and don't count frequency)
bool space_is_space = false;
// False: Print stats in numbers, True: print stats in "histogram form"
bool print_columns = true;

//Thresholds for {single_char, bigram, trigram}

//Ignore all combinations with less that this number of hits
int count_th[] = {0, 20, 20}; 
//Ignore all combinations with less frequency than
float freq_th[] = {0, 0.001, 0.001};

//Scale the histogram by this numbers
int percent_scale[] = {1, 5, 10};

/***
 * End settings
 ***/

#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <set>
using namespace std;


struct freq_pair {
	string str;
	int freq;

	bool operator<(const freq_pair &o) const {
		return freq < o.freq;
	}
};

void convert_map(multiset<freq_pair> &ret, int &count, const map<string, int> &m);

map<string, int> bigrams;
map<string, int> trigrams;
map<char, int> freq;

int main(int argc, char * argv[]) {
	if(argc < 2) {
		printf("Missing argument (file)\n");
		return 1;
	}
	FILE * in = fopen(argv[1],"r");


	int count, bigram_count, trigram_count;
	int c, r, i;

	char mem[3];
	//mem[4] = 0;
	
	char byte;
	while((r =fread(&byte,1,1,in)) > 0) {
			if(byte==10 && ignore_newline)
				continue;
			if(byte == ' ' && space_is_space) {
				count = 0;
				continue;
			}
			++freq[byte];
			++count;
			mem[0] = mem[1];
			mem[1] = mem[2];
			mem[2] = byte;
			if(count > 2) {
				++bigrams[string(mem+1,2)];
			}
			if(count > 3) {
				++trigrams[string(mem, 3)];
			} 
	}
	fclose(in);

	//move into multisets

	multiset<freq_pair> frequences[3];
	int counts[] = {0, 0, 0};

	for(map<char, int>::iterator it = freq.begin();it!=freq.end();++it) {
		if(it->second>0) {
			freq_pair p;
			p.str = std::string(1, it->first);
			p.freq = it->second;
			frequences[0].insert(p);
			counts[0]+=p.freq;
		}
	}
	convert_map(frequences[1], counts[1], bigrams);
	convert_map(frequences[2], counts[2], trigrams);


	//Output
	multiset<freq_pair>::reverse_iterator it;
	for(int i=0; i<3; ++i) {
		printf("======= level %d ===========\n", i);
		for(it=frequences[i].rbegin(); it!=frequences[i].rend(); ++it) {
			float freq = ((float)it->freq/(float)counts[i]);
			if(it->freq >= count_th[i] && freq >= freq_th[i]) {
				printf("%s:",it->str.c_str());
				if(print_columns) {
					for(int n = 0;n<freq*100.0*percent_scale[i];++n) {
						printf("#");
					}
					printf("\n");
				} else {
					printf(" freq: %.2f, num hits: %d\n", freq, it->freq);
				}
			}
		}
	}

}

void convert_map(multiset<freq_pair> &ret, int &count, const map<string, int> &m) {
	map<string, int>::const_iterator it;
	for(it = m.begin();it!=m.end();++it) {
		if(it->second>0) {
			freq_pair p;
			p.str = it->first;
			p.freq = it->second;
			ret.insert(p);
			count+=it->second;
		}
	}
}
