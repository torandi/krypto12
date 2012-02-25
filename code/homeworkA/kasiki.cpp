/**
 *  Calculates kasiki distances in input file
 *  Usage: ./kasiki input_file
 * Author: Andreas Tarandi - taran at kth dot se
 */

#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <utility>
using namespace std;

bool ignore_newline = true;

int d_max = 200;

int main(int argc, char * argv[]) {
	map<int,int> distances;
	map<string, int> last_pos;
	map<string, int>::iterator lp_it;
	map<int, int>::iterator d_it;

	if(argc < 2) {
		printf("Missing argument (file)\n");
		return 1;
	}
	FILE * in = fopen(argv[1],"r");


	int count;
	int c, r, i;

	char mem[3];
	
	char byte;
	while((r =fread(&byte,1,1,in)) > 0) {
		if(byte==10 && ignore_newline)
			continue;

		++count;
		mem[0] = mem[1];
		mem[1] = mem[2];
		mem[2] = byte;
		if(count > 3) {
			string cur = string(mem, 3);
			//See if this string has occurred earlier:
			lp_it = last_pos.find(cur);
			if(lp_it != last_pos.end()) {
				//Hit!
				int d = count-lp_it->second;
            if(d <= d_max) {
              d_it = distances.find(d);
              if(d_it == distances.end()) {
                distances[d] = 1;
              } else {
                d_it->second++;
              }
            }
			} else {
				//New combination
				last_pos[cur] = count;
			}
		} 
	}
	fclose(in);

	//Print result:
	printf("Found following distances: (distance, hit count)\n");
	for(d_it = distances.begin(); d_it!=distances.end(); ++d_it) {
		printf("(%d: %d), ",d_it->first, d_it->second);	
	}
	printf("\n");
}

