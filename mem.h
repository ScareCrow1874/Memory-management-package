// Simeng Zhu, simenz
// Zicong Liang, zicong

#ifndef MEM_H
#define MEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

// gets a block of memory with given size
void* getmem(uintptr_t size);

// frees the block with given pointer p
void freemem(void* p);

// stores statistics about the current state of the memory manager with
// the three given integers
void get_mem_stats(uintptr_t* total_size, uintptr_t* total_free,
                   uintptr_t* n_free_blocks);

// prints a formatted listing on file f showing the blocks on the free list
void print_heap(FILE * f);

#endif
