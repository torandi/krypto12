CFLAGS = -g -pg
LDFLAGS = -pg

all: test sha256

sha256.o: sha256.c sha256.h sha256_constants.h

main.o: main.c 

sha256: sha256.o main.o

test.o: test.c

test: ../../unit_test.o ../../sh_colors.o sha256.o test.o 

convert_constants: convert_constants.c

kattis: 
	kattis.py -p sha256 main.c sha256.c sha256.h sha256_constants.h

clean:
	rm *.o test 
