#ifndef MALLOC_H
#define MALLOC_H
#include <inttypes.h>

typedef struct _Block {
    /*
     * Pointer to the header of the next free block.
     * Only valid if this block is also free.
     * This is null for the last Block of the free list.
     */
    struct _Block *next;

    /*
     * Our header should always have a size of 16 Bytes.
     * This is just for 32 bit systems.
     */
    uint8_t padding[8 - sizeof(void*)];

    /*
     * The size of this block, including the header
     * Always a multiple of 16 bytes.
     */
    uint64_t size;

    /*
     * The data area of this block.
     */
    uint8_t data[];
} Block;

#define HEADER_SIZE sizeof(Block)

#define INV_HEADER_SIZE_MASK ~((uint64_t)HEADER_SIZE - 1)

/*
 * This is the heap you should use.
 * 32 MiB heap space per default. The heap does not grow.
 */
#define HEAP_SIZE (32 * 1024 * 1024)


void initAllocator();
void dumpAllocator();

uint64_t roundUp(uint64_t n);
void *my_malloc(uint64_t size);
void my_free(void *address);

#endif

