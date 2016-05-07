CC=gcc
CFLAGS=-lpulse-simple -lm -Wall -std=c11

ALL: simple_pa_client

pcm16s.o: pcm16s.c pcm16s.h
	${CC} ${CFLAGS} -c -o pcm16s.o pcm16s.c

simple_pa_client: pcm16s.o simple_pa_client.c
	${CC} ${CFLAGS} simple_pa_client.c pcm16s.o -o simple_pa_client
