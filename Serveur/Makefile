CC = gcc
CFLAGS = -Wall -g

all: ../server

.SUFFIXES:
	MAKEFLAGS += -r

../server: server2.o
	@$(CC) $(CFLAGS) -o $@ $^

%.o: %.c 
	@$(CC) $(CFLAGS) -c -o $@ $^
