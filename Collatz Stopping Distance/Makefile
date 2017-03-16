# Copyright © 2016 Bart Massey
# Makefile for collatz in x86-64 assembly.

LIB = /lib/x86_64-linux-gnu

.SUFFIXES: .S

collatz: collatz.o
	ld -L$(LIB)/ -I$(LIB)/ld-linux-x86-64.so.2 -o collatz collatz.o -lc

collatz.o: collatz.S
	as --gstabs+ -o collatz.o collatz.S

clean:
	-rm -f collatz.o collatz

