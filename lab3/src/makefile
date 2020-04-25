CC=gcc
CFLAGS=-I.
 
all: sequential_min_max parallel_min_max EXEC

EXEC: exec.o
	$(CC) -o EXEC exec.o

exec.o: ex5.c
	$(CC) -o exec.o -c ex5.c

sequential_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o sequential_min_max find_min_max.o utils.o sequential_min_max.c $(CFLAGS)

parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o -std=c99 parallel_min_max.c $(CFLAGS)

utils.o : utils.h
	$(CC) -o utils.o -c -std=c99 utils.c $(CFLAGS)

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c -std=c99 find_min_max.c $(CFLAGS)

clean :
	rm utils.o find_min_max.o sequential_min_max parallel_min_max EXEC exec.o
