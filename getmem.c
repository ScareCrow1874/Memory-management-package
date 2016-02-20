// Simeng Zhu, simenz
// Zicong Liang, zicong

#include "mem.h"
#include "mem_impl.h"
uintptr_t totalSize = 0;

// returns a block of memory with the given size
void* getmem(uintptr_t size) {
  check_heap();  // checks the free list

  // converts the requested size to a multiple of 16
  uintptr_t newSize = convert(size);

  free_node* current = freeList;
  // the block needs to be returned later
  free_node* returnNode = NULL;
  int big = 0;

  // free list is not empty
  if (current != NULL) {
    if (current->size > newSize) {
      if (current->size - newSize > THRESHOLD) {  // splits
        returnNode = split(newSize, current);
      } else {
          returnNode = current;
          freeList = current->next;
          returnNode->next = NULL;
      }
    }

    // traverses the free list
    while (current->next != NULL) {
      if (current->next->size > newSize) {
        if (current->next->size - newSize > THRESHOLD) {  // splits
          returnNode = split(newSize, current->next);
        } else {
            returnNode = current->next;
            current->next = current->next->next;
            returnNode->next = NULL;
        }
        break;
      }
      current = current->next;
    }
  }

  // haven't got the return block yet
  if (returnNode == NULL) {
    if (newSize > MALLOC) {
      // request size > default size just give the request size
      big = newSize;
      returnNode = (free_node*)malloc(big + HEADER);
      returnNode->size = big;
      returnNode->next = NULL;
    } else {
        big = MALLOC;
        if (big - newSize > THRESHOLD) {  // splits
        free_node* block = (free_node*)malloc(big + HEADER);
        block->size = big;
        block->next = NULL;
        returnNode = split(newSize, block);
        if (current == NULL) {
          freeList = block;
        } else {
            current->next = block;
        }
      } else {
          returnNode = (free_node*)malloc(big + HEADER);
          returnNode->size = big;
          returnNode->next = NULL;
      }
    }
    // sums the total amount of storage acquired from the
    // underlying system
    totalSize = totalSize + big + HEADER;
  }
  check_heap();  // checks the free list
  return (void*)((uintptr_t)returnNode + HEADER);
}
