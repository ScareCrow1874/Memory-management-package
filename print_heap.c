// Simeng Zhu, simenz
// Zicong Liang, zicong

#include "mem_impl.h"
#include "mem.h"

// prints a formatted listing on file f showing the blocks on the free list
void print_heap(FILE* f) {
  free_node* current = freeList;
  int nodeCount = 0;
  while (current) {
    nodeCount++;
    fprintf(f, "address: %p, length: %lu, node: %d\n", current,
            current->size, nodeCount);
    current = current->next;
  }
}
