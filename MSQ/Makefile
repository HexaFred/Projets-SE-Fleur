CC = gcc
CFLAGS = -std=c89 -pedantic -Wall -Werror -g -D_GNU_SOURCE
RM = rm -fv

.PHONY: all clean

all: msq-client.out msq-server.out

msq-common.o: msq-common.c
	$(CC) $(CFLAGS) -c -o $@ $<

msq-flower.o: msq-flower.c
	$(CC) $(CFLAGS) -c -o $@ $<

msq-client.out: msq-client.c msq-common.o msq-flower.o
	$(CC) $(CFLAGS) -o $@ $^

msq-server.out:	msq-server.c msq-common.o msq-flower.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) *.o *.out