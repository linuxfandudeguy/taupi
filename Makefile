# makefile for building taupi from pi.c

# Default target
all: taupi

taupi: pi.c
	 gcc -o taupi pi.c -O3 -Ofast -lmpfr -lm -lgmp

clean:
	 rm -rf ./taupi
