#ifndef ENHANCED_MEMORY_POOL_H
#define ENHANCED_MEMORY_POOL_H

#include <cstddef>
#include <vector>
#include <mutex>
#include <atomic>
#include <memory>
#include <unordered_set>

/**
 * An enhanced memory pool implementation with additional features for educational purposes
 * Features:
 * - Fixed-size block allocation from pre-allocated memory
 * - Thread-safe operations using atomic operations where possible
 * - Support for different block sizes
 * - Memory alignment guarantees
 * - Statistics tracking
 */
template<size_t BlockSize>
class EnhancedMemoryPool {
public:
    // Constructor: initializes the memory pool with a specified number of blocks
    explicit EnhancedMemoryPool(size_t numBlocks = 100);

    // Destructor: frees all allocated memory
    ~EnhancedMemoryPool();

    // Allocate a block of memory from the pool
    void* allocate();

    // Return a block of memory to the pool
    void deallocate(void* ptr);

    // Bulk allocate multiple blocks at once (for performance)
    std::vector<void*> bulkAllocate(size_t count);

    // Bulk deallocate multiple blocks at once (for performance)
    void bulkDeallocate(const std::vector<void*>& ptrs);

    // Get statistics about the memory pool
    size_t getNumBlocks() const { return numBlocks_; }
    size_t getUsedBlocks() const { return numBlocks_ - getFreeBlockCount(); }
    size_t getFreeBlocks() const { return getFreeBlockCount(); }
    
    // Reset the pool to its initial state (only safe when no blocks are in use)
    void reset();

    // Debug: validate the integrity of the free list
    bool validateIntegrity() const;

private:
    // Disable copy constructor and assignment operator
    EnhancedMemoryPool(const EnhancedMemoryPool&) = delete;
    EnhancedMemoryPool& operator=(const EnhancedMemoryPool&) = delete;

    // Structure representing a memory block
    struct alignas(std::max_align_t) Block {
        char data_[BlockSize];
        Block* next_{nullptr};
        
        // Method to get the user data pointer
        void* getDataPtr() { return static_cast<void*>(data_); }
        
        // Method to get block from user data pointer
        static Block* fromDataPtr(void* ptr) {
            if (!ptr) return nullptr;
            return reinterpret_cast<Block*>(
                reinterpret_cast<char*>(ptr) - offsetof(Block, data_)
            );
        }
    };

    // Members
    std::unique_ptr<Block[]> memoryPool_;
    std::atomic<Block*> freeList_{nullptr};  // Using atomic for thread-safe head updates
    size_t numBlocks_;
    
    // Statistics
    mutable std::atomic<size_t> allocCount_{0};
    mutable std::atomic<size_t> deallocCount_{0};
    
    // Mutex for bulk operations and reset
    mutable std::mutex bulkMutex_;
    
    // Count number of blocks in free list (thread-safe iteration)
    size_t getFreeBlockCount() const;
};

// Implementation
template<size_t BlockSize>
EnhancedMemoryPool<BlockSize>::EnhancedMemoryPool(size_t numBlocks) : numBlocks_(numBlocks) {
    // Pre-allocate the entire memory pool
    memoryPool_ = std::make_unique<Block[]>(numBlocks);
    
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
EnhancedMemoryPool<BlockSize>::~EnhancedMemoryPool() {
    // Destructor handles cleanup automatically
}

template<size_t BlockSize>
void* EnhancedMemoryPool<BlockSize>::allocate() {
    Block* oldHead;
    Block* newHead;
    
    // Atomic compare-and-swap to pop from free list
    do {
        oldHead = freeList_.load();
        if (oldHead == nullptr) {
            // Pool is empty
            return nullptr;
        }
        newHead = oldHead->next_;
    } while (!freeList_.compare_exchange_weak(oldHead, newHead));
    
    // Successfully acquired a block
    allocCount_.fetch_add(1);
    
    // Clear the next pointer to ensure clean state
    oldHead->next_ = nullptr;
    
    return oldHead->getDataPtr();
}

template<size_t BlockSize>
void EnhancedMemoryPool<BlockSize>::deallocate(void* ptr) {
    if (!ptr) return; // Nothing to deallocate
    
    Block* block = Block::fromDataPtr(ptr);
    
    // Atomic compare-and-swap to push to free list
    Block* oldHead;
    do {
        oldHead = freeList_.load();
        block->next_ = oldHead;
    } while (!freeList_.compare_exchange_weak(oldHead, block));
    
    // Successfully returned block to pool
    deallocCount_.fetch_add(1);
}

template<size_t BlockSize>
std::vector<void*> EnhancedMemoryPool<BlockSize>::bulkAllocate(size_t count) {
    std::vector<void*> result;
    result.reserve(count);
    
    std::lock_guard<std::mutex> lock(bulkMutex_);
    
    for (size_t i = 0; i < count; ++i) {
        void* ptr = allocate();
        if (ptr) {
            result.push_back(ptr);
        } else {
            // If we can't allocate all requested blocks,
            // deallocate the ones we did allocate
            for (void* allocatedPtr : result) {
                // Manually return these to the free list
                Block* block = Block::fromDataPtr(allocatedPtr);
                Block* oldHead;
                do {
                    oldHead = freeList_.load();
                    block->next_ = oldHead;
                } while (!freeList_.compare_exchange_weak(oldHead, block));
            }
            result.clear();
            break;
        }
    }
    
    return result;
}

template<size_t BlockSize>
void EnhancedMemoryPool<BlockSize>::bulkDeallocate(const std::vector<void*>& ptrs) {
    if (ptrs.empty()) return;
    
    std::lock_guard<std::mutex> lock(bulkMutex_);
    
    // Link all the blocks to be deallocated together
    for (size_t i = 0; i < ptrs.size(); ++i) {
        Block* currentBlock = Block::fromDataPtr(ptrs[i]);
        Block* nextBlock = (i + 1 < ptrs.size()) ? Block::fromDataPtr(ptrs[i + 1]) : freeList_.load();
        currentBlock->next_ = nextBlock;
    }
    
    // Atomically link the chain to the front of the free list
    Block* firstBlock = Block::fromDataPtr(ptrs[0]);
    Block* oldHead;
    do {
        oldHead = freeList_.load();
        Block* lastBlock = Block::fromDataPtr(ptrs[ptrs.size() - 1]);
        lastBlock->next_ = oldHead;
    } while (!freeList_.compare_exchange_weak(oldHead, firstBlock));
    
    deallocCount_.fetch_add(ptrs.size());
}

template<size_t BlockSize>
size_t EnhancedMemoryPool<BlockSize>::getFreeBlockCount() const {
    size_t count = 0;
    Block* current = freeList_.load();
    while (current) {
        ++count;
        current = current->next_;
    }
    return count;
}

template<size_t BlockSize>
void EnhancedMemoryPool<BlockSize>::reset() {
    std::lock_guard<std::mutex> lock(bulkMutex_);
    
    // Check if all blocks are currently free
    if (getUsedBlocks() != 0) {
        throw std::runtime_error("Cannot reset memory pool: some blocks are still in use");
    }
    
    // Reinitialize the free list
    for (size_t i = 0; i < numBlocks_ - 1; ++i) {
        memoryPool_[i].next_ = &memoryPool_[i + 1];
    }
    memoryPool_[numBlocks_ - 1].next_ = nullptr;
    freeList_ = &memoryPool_[0];
    
    allocCount_ = 0;
    deallocCount_ = 0;
}

template<size_t BlockSize>
bool EnhancedMemoryPool<BlockSize>::validateIntegrity() const {
    // Simple validation to detect cycles and invalid pointers
    std::unordered_set<const Block*> seen;
    const Block* current = freeList_.load();
    size_t count = 0;
    
    while (current) {
        // Check if we've seen this block before (cycle detection)
        if (seen.find(current) != seen.end()) {
            return false;
        }
        
        // Check if block is within our memory pool range
        const Block* poolStart = memoryPool_.get();
        const Block* poolEnd = poolStart + numBlocks_;
        if (current < poolStart || current >= poolEnd) {
            return false;
        }
        
        seen.insert(current);
        current = current->next_;
        ++count;
        
        // Prevent infinite loop on corrupted list
        if (count > numBlocks_) {
            return false;
        }
    }
    
    return true;
}

#endif // ENHANCED_MEMORY_POOL_H