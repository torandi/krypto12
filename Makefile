CFLAGS = -g
all: freq kasiki

freq: freq.cpp
		g++ ${CFLAGS} freq.cpp -o freq
kasiki: kasiki.cpp
		g++ ${CFLAGS} kasiki.cpp -o kasiki
