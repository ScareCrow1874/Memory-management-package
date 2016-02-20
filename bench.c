// Simeng Zhu, simenz
// Zicong Liang, zicong

/* this program is a testing program which is designed for
 * a larger number of calls to to functions getmem and freemem
 * allocate and free blocks of random sizes and in random order
 *
 * bench [ntrials [pctget [pctlarge [small_limit [large_limit [random_seed ]]]]]]
 *
 * ntrials: total number of getmem plus freemem calls to randomly perform during this test
 * pctget: percent of the total getmem/freemem calls that should be getmem
 * pctlarge: percent of the getmem calls that should request "large" blocks with a size greater than
 *           small_limit
 * small_limit: largest size in bytes of a "small" block
 * large_limit: largest size in bytes of a "large" block
 * random_seed: initial seed value for the random number generator
 */

#include <inttypes.h>
#include <time.h>
#include "mem.h"

#define NTRIALS 10000
#define PCTGET 50
#define PCTLARGE 10
#define SMALL_LIMIT 200
#define LARGE_LIMIT 20000
#define RANDOM_SEED time(NULL)
#define OPTION 6

void setDefault(int* argument);
void report();

int main(int argc, char** argv) {
  int argument[OPTION];
  setDefault(argument);

  // in case the user gives other values
  // resets the default values base on the user input
  // keeps the default while invalid input arguments or without inputs
  switch (argc) {
    case 7 :
      if (atoi(argv[6]) == 0) {
        fprintf(stderr, "Error: Invalid random_seed!");
      } else {
          argument[5] = atoi(argv[6]);
      }
    case 6 :
      if (atoi(argv[5]) == 0) {
        fprintf(stderr, "Error: Invalid large_limit!\n");
        printf("Using the default value\n");
      } else {
          argument[4] = atoi(argv[5]);
      }
    case 5 :
      if (atoi(argv[4]) == 0) {
        fprintf(stderr, "Error: Invalid small_limit!\n");
        printf("Using the default value\n");
      } else {
          argument[3] = atoi(argv[4]);
      }
    case 4 :
      if (atoi(argv[3]) == 0) {
        fprintf(stderr, "Error: Invalid pctlarge!\n");
        printf("Using the default value\n");
      } else {
          argument[2] = atoi(argv[3]);
      }
    case 3 :
      if (atoi(argv[2]) == 0) {
        fprintf(stderr, "Error: Invalid pctget\n");
        printf("Using the default value\n");
      } else {
          argument[1] = atoi(argv[2]);
      }
    case 2 :
      if (atoi(argv[1]) == 0) {
        fprintf(stderr, "Error: Invalid ntrials!\n");
        printf("Using the default value\n");
      } else {
          argument[0] = atoi(argv[1]);
      }
  }

  clock_t start;
  clock_t end;
  double cpu_time;
  srand(argument[5]);
  start = clock();  // start time

  int flip_coin;  // calls for get_mem or free_mem
  int getCount = 0;    // # of get_mem be called
  int getLimit = (int)(argument[0] * (argument[1] / 100.0));   // default 5000
  int freeCount = 0;   // # of free_mem be called
  int freeLimit = argument[0] - getLimit;  // default 5000
  int small = 0;      // # of get_mem for small block
  int smallLimit = (int)(getLimit * (1 - argument[2] / 100.0));  // default 4500
  int large = 0;      // # of get_mem for large block
  int largeLimit = getLimit - smallLimit;      // default 500
  int random = 0;
  int array_length = 0;
  int randNum = 0;

  void* p = NULL;  // pointer returns by getmem
  // a pointer array for storing the pointers
  void** pointer = (void**)malloc(argument[0]);
  for (int i = 0; i < NTRIALS; i++) {
    pointer[i] = NULL;
  }

  for (int i = 1; i <= argument[0]; i++) {
    if (getCount < getLimit && freeCount < freeLimit) {
      random = rand();
      flip_coin = random % 2;  // 1st flip for calling getmem or freemem
    } else if (getCount == getLimit) {
        flip_coin = 1;  // calls for get_mem are done
    } else {  // free == freeLimit
        flip_coin = 0;  // calls for free_mem are done
    }
    if (flip_coin == 0) {  // call get_mem
      uintptr_t size;
      if (small < smallLimit && large < largeLimit) {
        // 2nd flip for calling small block or large block
        flip_coin = rand() % 2;
      } else if (small == smallLimit) {
          flip_coin = 1;
      } else {  // large == largeLimit
          flip_coin = 0;
      }
      if (flip_coin == 0) {  // call small block
        small++;
        size = rand() % (argument[3] - 1) + 1;  // small block from 1 to 199
      } else {  // call large block
          large++;
          // large block from 200 to 19999
          size = (rand() % (argument[4] - argument[3])) + argument[3];
      }
      p = getmem(size);
      pointer[array_length] = p;
      array_length++;
      getCount++;
    } else {  // flip_coin == 1
        // call free_mem
        if (array_length != 0) {
          randNum = rand() % array_length;
          freemem(pointer[randNum]);
          pointer[randNum] = pointer[array_length - 1];
          pointer[array_length - 1] = NULL;
          array_length--;
        }
        freeCount++;
    }
    if (i % (argument[0] / 10) == 0 && i != argument[0]) {
      report();
      printf("\n");
    }
  }

  end = clock();  // end time
  cpu_time = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Total CPU time used by the benchmark test so far ");
  printf("in seconds: %f\n", cpu_time);
  report();

  FILE* f = fopen("print_heap.txt", "w");
  if (f) {
    print_heap(f);
  } else {
      fprintf(stderr, "Error: can not write to the f");
  }

  return 0;
}

// helper function
void setDefault(int* argument) {
  argument[0] = NTRIALS;           // ntrials
  argument[1] = PCTGET;            // pctget
  argument[2] = PCTLARGE;          // pctlarge
  argument[3] = SMALL_LIMIT;       // small_limit
  argument[4] = LARGE_LIMIT;       // large_limit
  argument[5] = (int)RANDOM_SEED;  // random_seed
}

// reports the statistic from the memorey manager for the current status
void report() {
  uintptr_t total_size;
  uintptr_t total_free;
  uintptr_t n_free_blocks;
  get_mem_stats(&total_size, &total_free, &n_free_blocks);
  printf("Total amount of storage acquired from the underlying system by the");
  printf(" memory manager during the test: %lu\n", total_size);
  printf("Total number of blocks on the free storage list at this point ");
  printf("in the test: %lu\n", n_free_blocks);
  if (n_free_blocks > 0) {
    printf("Average number of bytes in the free storage blocks at this point ");
    printf("in the test: %lu\n", total_free / n_free_blocks);
  } else {
    printf("Average number of bytes in the free storage blocks at this point ");
    printf("in the test: 0\n");
  }
}
