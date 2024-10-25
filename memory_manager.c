#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/mman.h>
#include "memory_manager.h"

static void *memory_pool;  // Pointer to the memory pool
static size_t pool_size;   // Total size of the memory pool
static pthread_mutex_t mem_lock = PTHREAD_MUTEX_INITIALIZER;  // Mutex for thread safety

// Initialization function
void mem_init(size_t size)
{
    // Use mmap instead of malloc
    memory_pool = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (memory_pool == MAP_FAILED) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);  // Exit if allocation failed
    }

    pool_size = size;  // Set the total size of the pool
    memset(memory_pool, 0, pool_size);  // Initialize memory to zero
}

void* mem_alloc(size_t size)
{
    pthread_mutex_lock(&mem_lock);  // Lock for thread safety

    // Ensure that size is not zero
    if (size == 0 || size + sizeof(size_t) > pool_size) {
        pthread_mutex_unlock(&mem_lock);
        return NULL;  // Not enough space in the pool or zero size
    }

    void* current_block = memory_pool;  // Start at the beginning of the pool

    // Traverse through the pool to find a free block
    while ((char*)current_block < (char*)memory_pool + pool_size) {
        size_t block_size = *(size_t*)current_block;  // Get the block size

        // Check if the block is free (block size is 0)
        if (block_size == 0) { 
            // Check if the remaining space is sufficient for the new block
            if (pool_size - ((char*)current_block - (char*)memory_pool) >= size + sizeof(size_t)) {
                // Mark block as allocated
                *(size_t*)current_block = size;  // Set the size of the allocated block
                pthread_mutex_unlock(&mem_lock);  // Unlock after allocation
                return (char*)current_block + sizeof(size_t);  // Return memory after size field
            }
        }

        // Move to the next block in the pool
        current_block = (char*)current_block + block_size + sizeof(size_t);
    }

    pthread_mutex_unlock(&mem_lock);  // Unlock if no free block found
    return NULL;  // No free block found
}


// Deallocation function
void mem_free(void* block)
{
    if (block == NULL) {
        return;  // Do nothing if the block is null
    }

    pthread_mutex_lock(&mem_lock);  // Lock for thread safety

    // Set the size of the block to 0, indicating it's free
    size_t* block_size_ptr = (size_t*)((char*)block - sizeof(size_t));
    *block_size_ptr = 0;  // Mark the block as free by setting size to 0

    pthread_mutex_unlock(&mem_lock);  // Unlock after freeing
}

// Resize function
void* mem_resize(void* block, size_t new_size)
{
    if (block == NULL) {
        return mem_alloc(new_size);  // Allocate new if block is NULL
    }

    pthread_mutex_lock(&mem_lock);  // Lock for thread safety

    // Get the original size of the block
    size_t original_size = *(size_t*)((char*)block - sizeof(size_t));

    // Allocate new memory for the block
    void* new_block = mem_alloc(new_size);

    // Copy the data from the old block to the new block
    if (new_block != NULL) {
        memcpy(new_block, block, original_size < new_size ? original_size : new_size);
    }

    // Deallocate the old block
    mem_free(block);

    pthread_mutex_unlock(&mem_lock);  // Unlock after resizing

    return new_block;  // Return the new block
}

// Deinitialization function
void mem_deinit()
{
    if (memory_pool != NULL) {
        munmap(memory_pool, pool_size);  // Use munmap to free the allocated memory
        memory_pool = NULL;
    }
}
