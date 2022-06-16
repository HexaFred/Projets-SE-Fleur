CC = gcc
CFLAGS = -std=c89 -pedantic -Wall -Werror -g -D_GNU_SOURCE
RM = rm -fv

.PHONY: all clean

all: pipe-main.out

pipe-common.o: pipe-common.c
	$(CC) $(CFLAGS) -c -o $@ $<

pipe-flower.o: pipe-flower.c
	$(CC) $(CFLAGS) -c -o $@ $<

pipe-main.out: pipe-main.c pipe-common.o pipe-flower.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) *.o *.out