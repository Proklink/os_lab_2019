CC=gcc
CFLAGS=-I.
 
all: process_memory parallel_min_max  

parallel_sum : utils.o ex1/utils.h
	$(CC) -o parallel_sum utils.o -lpthread -std=c99 parallel_sum.c $(CFLAGS)

process_memory : 
	$(CC) -o process_memory -c -std=c99 process_memory.c

parallel_min_max : utils.o find_min_max.o ex1/utils.h ex1/find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o -std=c99 ex1/parallel_min_max.c $(CFLAGS)

utils.o : ex1/utils.h
	$(CC) -o utils.o -c -std=c99 ex1/utils.c $(CFLAGS)

find_min_max.o : ex1/utils.h ex1/find_min_max.h
	$(CC) -o find_min_max.o -c -std=c99 ex1/find_min_max.c $(CFLAGS)

clean :
	rm utils.o find_min_max.o parallel_min_max process_memory.o process_memory parallel_sum parallel_sum.o