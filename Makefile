#Simeng Zhu, simenz
#Zicong Liang, zicong
#CSE 374, HW6 Part2

CC = gcc
CFLAGS = -Wall -g -std=c11

bench: bench.o getmem.o freemem.o get_mem_stats.o print_heap.o mem_impl.o
	$(CC) $(CFLAGS) -o bench bench.o getmem.o freemem.o get_mem_stats.o print_heap.o mem_impl.o

bench.o: bench.c mem.h
	$(CC) $(CFLAGS) -c bench.c

freemem.o: freemem.c mem.h mem_impl.h mem_impl.c
	$(CC) $(CFLAGS) -c freemem.c

getmem.o: getmem.c mem.h mem_impl.h mem_impl.c
	$(CC) $(CFLAGS) -c getmem.c

get_mem_stats.o: get_mem_stats.c mem.h mem_impl.h mem_impl.c
	$(CC) $(CFLAGS) -c get_mem_stats.c

print_heap.o: print_heap.c mem.h mem_impl.h mem_impl.c
	$(CC) $(CFLAGS) -c print_heap.c

mem_impl.o: mem_impl.c mem_impl.h
	$(CC) $(CFLAGS) -c mem_impl.c

test: bench
	./bench

dist: git
	tar -cvf hw6.tar bench.c getmem.c freemem.c get_mem_stats.c print_heap.c mem_impl.c mem.h mem_impl.h git.log Makefile README.txt print_heap.txt

git:
	git log > git.log

clean:
	rm -rf *.o bench *~
