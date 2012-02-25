CFLAGS = -g
all: freq

freq: freq.cpp
		g++ -g freq.cpp -o freq
