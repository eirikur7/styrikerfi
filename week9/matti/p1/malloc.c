#include "malloc.h"

#include <stdio.h>
#include <assert.h>
#include <pthread.h>

#define ALLOCATED_BLOCK_MAGIC (Block*)(0xbaadf00d)
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/*
 * This is the heap you should use.
 * The heap does not grow.
 */
uint8_t __attribute__ ((aligned(HEADER_SIZE))) _heapData[HEAP_SIZE];

/*
 * This should point to the first free block in memory.
 */
Block *_firstFreeBlock;

/*
 * Initialize the memory block. You don't need to change this.
 */
void initAllocator()
{
    _firstFreeBlock = (Block*)&_heapData[0];
    _firstFreeBlock->next = NULL;
    _firstFreeBlock->size = HEAP_SIZE;
    pthread_mutex_init(&mutex, NULL);
}

/*
 * Get the next block that should start after the current one.
 */
static Block *_getNextBlockBySize(const Block *current)
{
    static const Block *end = (Block*)&_heapData[HEAP_SIZE];
    Block *next = (Block*)&current->data[current->size - HEADER_SIZE];

    assert(next <= end);
    return (next == end) ? NULL : next;
}

/*
 * Dumps the allocator. You should not need to modify this.
 */
void dumpAllocator()
{
    Block *current;
    pthread_mutex_lock(&mutex);
    printf("All blocks:\n");
    current = (Block*)&_heapData[0];
    while (current) {
        assert((current->size & INV_HEADER_SIZE_MASK) == current->size);
        assert(current->size > 0);

        printf("  Block starting at %" PRIuPTR ", size %" PRIu64 " (%s)\n",
            ((uintptr_t)(void*)current - (uintptr_t)(void*)&_heapData[0]),
            current->size,
            (current->next == ALLOCATED_BLOCK_MAGIC) ? "allocated" : "free");

        current = _getNextBlockBySize(current);
    }

    printf("Current free block list:\n");
    current = _firstFreeBlock;
    while (current) {
        assert(current->next != ALLOCATED_BLOCK_MAGIC);

        printf("  Free block starting at %" PRIuPTR ", size %" PRIu64 "\n",
            ((uintptr_t)(void*)current - (uintptr_t)(void*)&_heapData[0]),
            current->size);

        current = current->next;
    }
    pthread_mutex_unlock(&mutex);
}

/*
 * Round the integer up to the block header size (16 Bytes).
 */
uint64_t roundUp(uint64_t n)
{
    return (n + HEADER_SIZE - 1) & INV_HEADER_SIZE_MASK;
}

static void *_allocate(Block **blockLink, uint64_t size)
{
    assert(blockLink != NULL);
    assert((size & INV_HEADER_SIZE_MASK) == size);
    assert(size >= HEADER_SIZE);

    Block *freeBlock = *blockLink;
    assert(freeBlock != NULL);
    assert(freeBlock->size >= size);

    if (freeBlock->size == size) {
        // The selected free block exactly fits the requested size. We thus
        // do not need to split the block.
        *blockLink = freeBlock->next;
    } else {
        // The free block is larger. We thus split it into two blocks. One
        // allocated that we return to the caller and a new free one.
        const uint64_t remainingSize = freeBlock->size - size;
        assert((remainingSize & INV_HEADER_SIZE_MASK) == remainingSize);
        assert(remainingSize >= HEADER_SIZE);

        freeBlock->size = size;

        Block *newFreeBlock = _getNextBlockBySize(freeBlock);
        newFreeBlock->size  = remainingSize;
        newFreeBlock->next  = freeBlock->next;
        *blockLink = newFreeBlock;
    }

    // Mark the current block as allocated by setting a magic next value
    freeBlock->next = ALLOCATED_BLOCK_MAGIC;

    return &freeBlock->data[0];
}

void *my_malloc(uint64_t size)
{
    // Calculate the minimum size of the free block we need to find.
    // We only allocate blocks that are multiples of 16 bytes in size, so we
    // round up the requested size. This potentially wastes some memory but
    // makes management easier. We also need to store our block header.
    
    const uint64_t requestedSize = roundUp(size) + HEADER_SIZE;
    pthread_mutex_lock(&mutex);
    Block *current = _firstFreeBlock;
    Block **link   = &_firstFreeBlock;


    uint64_t bestSize = 0;
    Block *bestBlock = NULL;
    Block **bestLink = NULL;
    while (current) {
        if (current->size >= requestedSize ) {
            if(bestBlock==NULL || current->size<bestSize) {
		// Either we are a better fit, or we don't have 
		bestSize = current->size;
		bestBlock = current;
		bestLink = link;
		// testing first fit:
        
		break;
            }
        }
        link = &current->next;
        current = current->next;
    }

    // We did not find a free block that offers enough space. Return NULL to
    // indicate that the allocation failed.
    
    if( bestBlock == NULL ) {
	printf("Out of memory\n");
    pthread_mutex_unlock(&mutex);
	return NULL;
    }
    void *result = _allocate(bestLink, requestedSize);
    pthread_mutex_unlock(&mutex);
    return result;
}

/*
 * Check if we can merge this block with the next one.
 */
static void _tryMerge(Block *freeBlock)
{
    assert(freeBlock != NULL);
    assert(freeBlock->next != ALLOCATED_BLOCK_MAGIC);

    // If there is no free block after the given one, there
    // is nothing we can merge.
    if (freeBlock->next == NULL) {
        return;
    }

    // Look at the next block in the heap. If it is linked with the free block
    // it is also free and we can merge the blocks.
    const Block *next = _getNextBlockBySize(freeBlock);

    if (next == freeBlock->next) {
        assert(next->next != ALLOCATED_BLOCK_MAGIC);

        freeBlock->size += next->size;
        freeBlock->next = next->next;
    }

}

void my_free(void *address)
{
    if (address == NULL) {
        return;
    }
    pthread_mutex_lock(&mutex);

    assert(address >= (void*)&_heapData[0]);
    assert(address <= (void*)&_heapData[HEAP_SIZE]);

    // We first need to get the block header for the given address.
    // address should point to the beginning of the block's data segment,
    // directly after the header -> decrement address by one header size.
    Block *block = (Block*)(address) - 1;
    assert(block->next == ALLOCATED_BLOCK_MAGIC);

    Block *freeBlock = _firstFreeBlock;

    // The free block must be inserted at the correct position in the free list
    // to allow merging with the neighbor blocks in the case they are free, too
    // We can find the correct position easily, by comparing the block addresses
    if ((freeBlock == NULL) || (freeBlock > block)) {
        // Insert the block at the front of the freelist
        _firstFreeBlock = block;
        block->next = freeBlock;

        _tryMerge(block);
    } else {
        // Iterate the free list until we reach its end or find the correct pos
        while ((freeBlock->next != NULL) && (freeBlock->next < block)) {
            freeBlock = freeBlock->next;
        }

        // Insert block into free list and try to merge with neighbor blocks
        block->next = freeBlock->next;
        freeBlock->next = block;

        _tryMerge(block);
        _tryMerge(freeBlock);
    }
    pthread_mutex_unlock(&mutex);
}
