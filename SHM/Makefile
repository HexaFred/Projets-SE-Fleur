CC = gcc
CFLAGS = -std=c89 -pedantic -Wall -Werror -g -D_GNU_SOURCE
RM = rm -fv

.PHONY: all clean

all: shm-client.out shm-server.out

shm-common.o: shm-common.c
	$(CC) $(CFLAGS) -c -o $@ $<

shm-flower.o: shm-flower.c
	$(CC) $(CFLAGS) -c -o $@ $<

shm-client.out: shm-client.c shm-common.o shm-flower.o
	$(CC) $(CFLAGS) -o $@ $^

shm-server.out:	shm-server.c shm-common.o shm-flower.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) *.o *.out