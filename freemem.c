// Simeng Zhu, simenz
// Zicong Liang, zicong

#include "mem_impl.h"
#include "mem.h"

// frees the blocks with given pointer p
// (put the pointer into the free list)
void freemem(void* p) {
  check_heap();
  free_node* realP;
  if (p != NULL) {
    realP = (free_node*)((uintptr_t)p - HEADER);
    uintptr_t size = realP->size + HEADER;
    free_node* current = freeList;
    int combinedAtEnd = 0;
    int operated = 0;

    // free list is not empty
    if (current != NULL) {
      // add as separate node in front of current
      if ((uintptr_t)realP + size < (uintptr_t)current) {
        realP->next = current;
        freeList = realP;
      }

      // combine current at end of realP, realP at front
      if ((uintptr_t)realP + size == (uintptr_t)current) {
        freeList = realP;
        realP->next = current->next;
        current->next = NULL;
        combine(realP, current);
      }

      // combine realP at end of current, current at front
      if ((uintptr_t)current + current->size + HEADER == (uintptr_t)realP) {
        combine(current, realP);
        combinedAtEnd = 1;
        operated = 1;
      }

      while (current->next != NULL) {
        // combine realP at end of current, current in between
        if ((uintptr_t)current + current->size + HEADER ==
            (uintptr_t)realP && operated == 0) {
          combine(current, realP);
          combinedAtEnd =1;
        }
        operated = 0;
        // combine current->next at end of current
        if ((uintptr_t)current + current->size + HEADER ==
            (uintptr_t)(current->next)) {
          free_node* temp = current->next;
          current->next = current->next->next;
          combine(current, temp);
          break;
        }
        // add realP between current and current->next
        if ((uintptr_t)realP + size < (uintptr_t)current->next
            && combinedAtEnd == 0) {
          realP->next = current->next;
          current->next = realP;
          break;
        }
        // combine current->next at the end of realP
        if ((uintptr_t)realP + size == (uintptr_t)current->next
            && combinedAtEnd == 0) {
          free_node* temp = current->next;
          current->next = realP;
          realP->next = temp->next;
          temp->next = NULL;
          combine(realP, temp);
          break;
        }
        current = current->next;
      }
    }

    // add realP to the start or the end the list or combine to the last node
    if (realP->next == NULL && combinedAtEnd == 0) {
      if (current == NULL) {
        freeList = realP;
      } else if ((uintptr_t)current + current->size + HEADER ==
                 (uintptr_t)realP && operated == 0) {
          combine(current, realP);
      } else {
          current->next = realP;
      }
    }
    check_heap();
  }
}
