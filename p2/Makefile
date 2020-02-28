CFLAGS=-g
OBJS=compress.o chunk_archive.o options.o queue.o comp.o
LIBS=-lz -pthread
CC=gcc

all: comp
	gcc -g -pthread exemplo.c queue.c -o exemploE1

comp: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

clean:
	rm -f *.o comp exemploE1 saida
