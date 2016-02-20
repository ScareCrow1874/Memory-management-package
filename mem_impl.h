// Simeng Zhu, simenz
// Zicong Liang, zicong

#ifndef MEM_IMPL_H
#define MEM_IMPL_H
#define HEADER 16     // value of the header size
#define MALLOC 4096   // initial value of the malloc size
#define THRESHOLD 32  // value of the threshold size for splitting

#include <inttypes.h>

// linkedlist structures
typedef struct free_node {
  uintptr_t size;
  struct free_node* next;
} free_node;

extern free_node* freeList;
extern uintptr_t totalSize;

// checks for possible problems with the free list structure
void check_heap();

// converts the given size into a new size which is a multiple of 16
uintptr_t convert(uintptr_t size);

// splits the given block with size into a smaller block and the remaining
// block size should be stayed in the freeList
free_node* split(uintptr_t, free_node*);

// combines the two given small blocks into a larger block
void combine(free_node*, free_node*);

#endif
