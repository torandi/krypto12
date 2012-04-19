CFLAGS = -g
LDFLAGS = -lgmp

all: rsafact test

rsafact: main.o rsafact.o

main.o: main.c

rsafact.o: rsafact.c rsafact.h

test: ../../unit_test.o ../../sh_colors.o rsafact.o test.o

test.o: test.c

kattis: 
	kattis -p rsafact main.c rsafact.h rsafact.c 

clean:
	rm *.o rsafact test 
