// Simeng Zhu, simenz
// Zicong Liang, zicong

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "mem_impl.h"

free_node* freeList = NULL;

// checks for possible problems with the free list structure
void check_heap() {
  free_node* current = freeList;

  if (current != NULL) {
    assert(current->size > 0);
    assert(current->size >= 1);

    while (current->next != NULL) {
      // Blocks are ordered with strictly increasing memory addresses
      assert(current < current->next);

      // Block sizes are positive numbers and no smaller than whatever
      // minimum size you are using in your implementation
      assert(current->next->size > 0);
      assert(current->next->size >= 1);

      // Blocks do not overlap (the start + length of a block is not
      // an address in the middle of a later block on the list)
      assert((int)((uintptr_t)current + current->size -
             (uintptr_t)(current->next)) < 0);

      // Blocks are not touching (the start + length of a block should
      // not be the same address as the next block on the list since in
      // that case the two blocks should have been combined into a single,
      // larger block.)
      assert((int)((uintptr_t)current + current->size -
             (uintptr_t)(current->next)) != 0);
      current = current -> next;
    }
  }
}

// helper function
// converts the given size into a new size which is a multiple of 16
uintptr_t convert(uintptr_t size) {
  uintptr_t newSize = size;
  uintptr_t divisor;
  if (newSize % 16 != 0) {
    divisor = newSize / 16;
    divisor++;
    newSize = divisor * 16;
  }
  return newSize;
}

// helper function
// splits the larger block into smaller blocks
free_node* split(uintptr_t size, free_node* cur) {
  free_node* newPtr = (free_node*)((uintptr_t)cur + cur->size - size);
  newPtr->size = size;
  newPtr->next = NULL;
  cur->size = cur->size - size - HEADER;
  return newPtr;
}

// helper function
// combines the smaller blocks into a larger block
void combine(free_node* first, free_node* second) {
  first->size = first->size + HEADER + second->size;
}
