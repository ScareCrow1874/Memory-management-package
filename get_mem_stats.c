// Simeng Zhu, simenz
// Zicong Liang, zicong

#include "mem_impl.h"
#include "mem.h"

// stores statistics about the current state of the memory
// manager with the three given integers
void get_mem_stats(uintptr_t* total_size, uintptr_t* total_free,
                   uintptr_t* n_free_blocks) {
  free_node* current = freeList;
  *total_size = totalSize;
  *total_free = 0;
  *n_free_blocks = 0;
  while (current != NULL) {
    (*n_free_blocks)++;
    *total_free = *total_free + current->size + HEADER;
    current = current -> next;
  }
}
