CFLAGS = -g

all: aes test

aes.o: aes.c aes.h

main.o: main.c 

aes: aes.o main.o

test.o: test.c

unit_test.o: unit_test.h unit_test.c

sh_colors.o: sh_colors.h sh_colors.c

test: unit_test.o aes.o test.o sh_colors.o

clean:
	rm *.o aes test 
