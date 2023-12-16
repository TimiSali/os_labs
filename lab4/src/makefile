CC=gcc
CFLAGS=-I.

./zad1/parallel_min_max : ./zad1/utils.o ./zad1/find_min_max.o ./zad1/utils.h ./zad1/find_min_max.h
	$(CC) -o ./zad1/parallel_min_max ./zad1/utils.o ./zad1/find_min_max.o ./zad1/parallel_min_max.c $(CFLAGS)

./zad1/utils.o : ./zad1/utils.h
	$(CC) -o ./zad1/utils.o -c ./zad1/utils.c $(CFLAGS)

./zad1/find_min_max.o : ./zad1/utils.h ./zad1/find_min_max.h
	$(CC) -o ./zad1/find_min_max.o -c ./zad1/find_min_max.c $(CFLAGS)

process_memory :
	$(CC) -o process_memory process_memory.c $(CFLAGS)

parallel_sum : parallel_sum.c ./zad1/utils.o
	$(CC) -o parallel_sum ./zad1/utils.o parallel_sum.c $(CFLAGS) -lpthread

all : parallel_sum
	$(CC) -o process_memory process_memory.c $(CFLAGS)
	$(CC) -o ./zad1/parallel_min_max ./zad1/utils.o ./zad1/find_min_max.o ./zad1/parallel_min_max.c $(CFLAGS)
clean :
	rm process_memory ./zad1/find_min_max.o ./zad1/utils.o
