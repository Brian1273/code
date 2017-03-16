# Copyright (c) 2014 Bart Massey
# ALL RIGHTS RESERVED

CC = gcc -O4 
# CC = clang -O3

# Replace the machine with yours.
CFLAGS = -Wall -march=native -DX86_64

ilog2: ilog2.o

clean:
	-rm -f ilog2.o ilog2

distclean: clean
	-rm -f ilog2_table_8.c ilog2_table_16.c
