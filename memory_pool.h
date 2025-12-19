#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <cstddef>
#include <vector>
#include <mutex>

/**
 * A simple fixed-size memory pool implementation for educational purposes
 * Allocates chunks of fixed size from a pre-allocated block of memory
 */
template<size_t BlockSize>
class MemoryPool {
public:
    // Constructor: initializes the memory pool with a specified number of blocks
    explicit MemoryPool(size_t numBlocks = 100);

    // Destructor: frees all allocated memory
    ~MemoryPool();

    // Allocate a block of memory from the pool
    void* allocate();

    // Return a block of memory to the pool
    void deallocate(void* ptr);

    // Get statistics about the memory pool
    size_t getNumBlocks() const { return numBlocks_; }
    size_t getUsedBlocks() const { return numBlocks_ - getFreeBlockCount(); }
    size_t getFreeBlocks() const { return getFreeBlockCount(); }

private:
    // Disable copy constructor and assignment operator
    MemoryPool(const MemoryPool&) = delete;
    MemoryPool& operator=(const MemoryPool&) = delete;

    // Count number of blocks in free list
    size_t getFreeBlockCount() const {
        size_t count = 0;
        Block* current = freeList_;
        while (current) {
            ++count;
            current = current->next_;
        }
        return count;
    }

    // Structure representing a memory block
    struct Block {
        alignas(std::max_align_t) char data_[BlockSize];
        Block* next_{nullptr};
    };

    // Members
    std::vector<Block> memoryPool_;
    Block* freeList_{nullptr};
    size_t numBlocks_;
    mutable std::mutex mutex_;
};

// Implementation
template<size_t BlockSize>
MemoryPool<BlockSize>::MemoryPool(size_t numBlocks) : numBlocks_(numBlocks) {
    // Pre-allocate the entire memory pool
    memoryPool_.resize(numBlocks);
    
    // Initialize the free list by linking all blocks together
    for (size_t i = 0; i < numBlocks - 1; ++i) {
        memoryPool_[i].next_ = &memoryPool_[i + 1];
    }
    
    // Last block points to nullptr
    memoryPool_[numBlocks - 1].next_ = nullptr;
    
    // Set the free list head to the first block
    freeList_ = &memoryPool_[0];
}

template<size_t BlockSize>
MemoryPool<BlockSize>::~MemoryPool() {
    // Destructor handles cleanup automatically via vector destructor
}

template<size_t BlockSize>
void* MemoryPool<BlockSize>::allocate() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (!freeList_) {
        // Pool is empty, return nullptr
        return nullptr;
    }
    
    // Take the first block from the free list
    Block* block = freeList_;
    freeList_ = freeList_->next_;
    
    // Return pointer to the data portion
    return static_cast<void*>(block->data_);
}

template<size_t BlockSize>
void MemoryPool<BlockSize>::deallocate(void* ptr) {
    if (!ptr) return; // Nothing to deallocate
    
    // Calculate which block contains this pointer
    // This assumes ptr points to a valid block within our pool
    std::lock_guard<std::mutex> lock(mutex_);
    
    Block* block = reinterpret_cast<Block*>(
        reinterpret_cast<char*>(ptr) - offsetof(Block, data_)
    );
    
    // Put the block back on the free list
    block->next_ = freeList_;
    freeList_ = block;
}

#endif // MEMORY_POOL_H