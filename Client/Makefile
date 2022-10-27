CC = gcc
CFLAGS = -Wall

all: ../client

.SUFFIXES:
	MAKEFLAGS += -r

../client: client2.o
	@$(CC) $(CFLAGS) -o $@ $^

%.o: %.c 
	@$(CC) $(CFLAGS) -c -o $@ $^
