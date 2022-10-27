CC = gcc
CFLAGS = -Wall

all: ../server

.SUFFIXES:
	MAKEFLAGS += -r

../server: server2.o
	@$(CC) $(CFLAGS) -o $@ $^

%.o: %.c 
	@$(CC) $(CFLAGS) -c -o $@ $^
