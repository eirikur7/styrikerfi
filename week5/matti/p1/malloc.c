#include "malloc.h"

#include <stdio.h>
#include <assert.h>

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
 * Initializes the memory block. You don't need to change this.
 */
void initAllocator()
{
    _firstFreeBlock = (Block*)&_heapData[0];
    _firstFreeBlock->next = NULL;
    _firstFreeBlock->size = HEAP_SIZE;
}

/*
 * Gets the next block that should start after the current one.
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

    printf("All blocks:\n");
    current = (Block*)&_heapData[0];
    while (current) {
        assert((current->size & INV_HEADER_SIZE_MASK) == current->size);
        assert(current->size > 0);

        printf("  Block starting at %" PRIuPTR ", size %" PRIu64 "\n",
            ((uintptr_t)(void*)current - (uintptr_t)(void*)&_heapData[0]),
            current->size);

        current = _getNextBlockBySize(current);
    }

    printf("Current free block list:\n");
    current = _firstFreeBlock;
    while (current) {
        printf("  Free block starting at %" PRIuPTR ", size %" PRIu64 "\n",
            ((uintptr_t)(void*)current - (uintptr_t)(void*)&_heapData[0]),
            current->size);

        current = current->next;
    }
}

/*
 * Round the integer up to the block header size (16 Bytes).
 */
uint64_t roundUp(uint64_t n)
{
    uint64_t reminder = n % 16;
    if (reminder == 0){
        return n;
    }
    return n + 16 - reminder;
}

void *my_malloc(uint64_t size)
{
    uint64_t rounded_size = roundUp(size);

    if(_firstFreeBlock->size < (rounded_size + HEADER_SIZE)){
        return NULL;
    }


       if (_firstFreeBlock->size == rounded_size + HEADER_SIZE){
        void* dataPtr = &(_firstFreeBlock->data[0]);
        if (_firstFreeBlock->next == NULL){
            _firstFreeBlock = (Block *)&_heapData[HEAP_SIZE-1];
		    _firstFreeBlock->size = 0;
        }
        else{
            _firstFreeBlock = _firstFreeBlock->next;
        }
        return dataPtr;
    }
   
    if (_firstFreeBlock->next == NULL){
        void* dataPtr = &(_firstFreeBlock->data[0]);
        uint64_t curSize = _firstFreeBlock->size;
        _firstFreeBlock->size = rounded_size + HEADER_SIZE;
        Block *new_block = _getNextBlockBySize(_firstFreeBlock);
        new_block ->next = _firstFreeBlock->next;
        new_block ->size = curSize - _firstFreeBlock->size;
        _firstFreeBlock = new_block;

        return dataPtr;
    }



    Block *current = _firstFreeBlock;
    uint64_t sizeof_best = 0;
    int first_best = 0;
    Block *best_fit = NULL;
    while(current->next != NULL){
        if(current->next->size == rounded_size + HEADER_SIZE){
            void* dataPtr = &(current->next->data[0]);
            current->next = current->next->next;
            return dataPtr;
        }
        if(current->next->size > rounded_size + HEADER_SIZE){
            if((best_fit == NULL)){
                if((current == _firstFreeBlock) && (current->size < current->next->size)){
		            best_fit = current;
		            first_best = 1;
		        }else{
		            best_fit = current;
		        }
                sizeof_best = current->next->size;   
            }
            else if(sizeof_best > current->next->size){
		        best_fit = current;
                sizeof_best = current->next->size;
	        }
        }
        current = current->next;
    }



    


    if (best_fit == NULL){
        return NULL;
    }





    if(first_best == 1){
	void* dataPtr = &(best_fit->data[0]);
	uint64_t curSize = best_fit->size;
	best_fit->size = rounded_size + HEADER_SIZE;
	Block *new_block = _getNextBlockBySize(best_fit);
	new_block ->next = best_fit->next;
	new_block ->size = curSize - best_fit->size;
	_firstFreeBlock = new_block;
	return dataPtr;
    }
	
    void* dataPtr = &(best_fit->next->data[0]);
    uint64_t curSize = best_fit->next->size;
    best_fit->next->size = rounded_size + HEADER_SIZE;
    Block *new_block = _getNextBlockBySize(best_fit->next);
    new_block ->next = best_fit->next->next;
    new_block ->size = curSize - best_fit->next->size;
    best_fit->next = new_block;

    return dataPtr;
    



}

void my_free(void *address)
{

    if(address == NULL){
        return;
    }


    Block *current = (Block *)( ((uint8_t *) address) - sizeof(Block));
    
    Block *node = _firstFreeBlock;
    Block *prev = NULL;

    while(node != NULL){
        if(node < current){
            prev = node;
        }
        if((node == _getNextBlockBySize(current)) && (_getNextBlockBySize(current) != NULL)){
            current->size += node->size;
            current->next = node->next;
        }

        if(_getNextBlockBySize(node) == current){
            if((_getNextBlockBySize(current) == node->next) && (node->next != NULL) ){
                node->size += node->next->size + current->size;
                node->next = node->next->next;
            }else{
                node->size += current->size;
            }
            return;
        }

        
        node = node->next;
    }

    if(prev == NULL){
        current->next = _firstFreeBlock;
        _firstFreeBlock = current;
    }else{
        current ->next = prev->next;
        prev->next = current;
    }
}


