CFLAGS = -g -pg
LDFLAGS = -pg

all: aes test

aes.o: aes.c aes.h

main.o: main.c 

aes: aes.o main.o

test.o: test.c

test: ../../unit_test.o ../../sh_colors.o aes.o test.o 

kattis: 
	kattis -p oldkattis:aes main.c aes.h aes.c aes_constants.h

clean:
	rm *.o aes test 
