Simeng Zhu, simenz
Zicong Liang, zicong
12/03/15

// Work division
Simeng: freemem.c, print_heap.c, mem_impl.h, mem_impl.c, Makefile
Zicong: getmem.c, get_mem_stats.c, mem.h, bench.c
Each of us did our part separately, though we cooperated on occasionally on some of these code files.

// Heap structure & Algorithm
Our heap is designed in the form of a linked list, the list is loaded with free_node pointers. free_node is a struct that we constructed with a size field with type uintptr_t and a next field with the type of a free_node pointer. Each free_node on the freeList has an overhead of 16 bytes.

When getmem is called:
We first traverse the freeList to find a block bigger than the requested size, if found, we check whether that block can be splitted. We then split a smaller block of the requested size rounded to be an integer multiple of 16 from the big enough block if it can be splitted without leaving tiny fragments on the list. If splitting is not the best course of action, though, we'll just return the whole big enough block that we found on the freeList. If no blocks on the freeList is big enough to satisfy the request, we'll use malloc to get a new block of memory with the size of 4096 bytes(if the request is smaller than 4096, this new block will be splitted if fit) or the requested size rounded to be the multiple of 16(if the request is greater than 4096). After all these, the pointer to the proper block of memory(not including header for that block) will be returned.

When freemem is called:
freemem will receive a void pointer pointing to a block of memory that getmem gave out, the first thing we did was to subtract the size of header from the given pointer to point to the header of that block of memory and find out the size of this block of memory. Then we traverse the freeList to find a proper spot for it on the freeList, which is ranked in ascending order of memory address. In the process, if the memory address of the end of one block is the same as the memory address of the start of another block, they will be combined into one. Nothing is returned after the call of freemem.

// improvements & experiments
In order to improve our basic algorithm of getmem and freemem so that we don't end up with a freeList filled with tiny fragments, we experimented with various threshold value of memory that the memory size of any blocks on the freeList must be greater than. Initially we tested with a threshold percentage of 20%, but the actual memory left on the list will be proportional to the original block we are doing the split on. Sometimes a splitable block will be recognized as unsplitable just because of the 20% threshold, returning too much of unnecessary memory. Any number smaller than 32 is too small. We've tried 16, only to discover later that leaving a block with size of 16 on the freeList is virtually useless since the header itself will take up 16 bytes. After a bunch of tests we chose 32 as our magic number because it works well in not leaving behind super tiny fragments on the freeList. Another reason we chose 32 was that it's exactly the size of a free_node struct plus the size of a header.

// result summary
When bench program is run, ten reports about the total malloc-ed memory, number of free_nodes on the list, and the average size of all free_nodes on the list. Our program is fast enough to run 5000000 trials under 8 seconds in some of the best cases, and the worst case for 5000000 trials is only 40 seconds. We have run our program using default parameter for over 300 times and the time used is all under 0.01s and no crashes happened. We've also tested while inputting arguments and no errors occurred. Minimum amount of memory was acquired from the underlying system due to the successful operation of combine function.

// resources consulted
c standard library and lecture
