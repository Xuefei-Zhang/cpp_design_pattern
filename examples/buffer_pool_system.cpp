#include <iostream>
#include <memory>
#include <vector>
#include <queue>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <unordered_map>
#include <chrono>

// Buffer class that represents a memory buffer
class Buffer {
private:
    char* data;
    size_t size;
    size_t capacity;
    bool isPooled;
    std::atomic<bool> inUse{false};
    std::chrono::steady_clock::time_point lastUsed;
    
public:
    Buffer(size_t bufferCapacity) 
        : capacity(bufferCapacity), size(0), isPooled(true), data(new char[bufferCapacity]) {
        lastUsed = std::chrono::steady_clock::now();
    }
    
    // Constructor for external data (non-pooled)
    Buffer(char* externalData, size_t bufferSize) 
        : data(externalData), size(bufferSize), capacity(bufferSize), 
          isPooled(false), inUse(true) {
        lastUsed = std::chrono::steady_clock::now();
    }
    
    ~Buffer() {
        if (data && isPooled) {
            delete[] data;
            data = nullptr;
        }
    }
    
    // Non-copyable but movable
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    
    Buffer(Buffer&& other) noexcept
        : data(other.data), size(other.size), capacity(other.capacity), 
          isPooled(other.isPooled), inUse(other.inUse.load()), lastUsed(other.lastUsed) {
        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
        other.inUse = false;
    }
    
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            if (data && isPooled) {
                delete[] data;
            }
            data = other.data;
            size = other.size;
            capacity = other.capacity;
            isPooled = other.isPooled;
            inUse = other.inUse.load();
            lastUsed = other.lastUsed;
            
            other.data = nullptr;
            other.size = 0;
            other.capacity = 0;
            other.inUse = false;
        }
        return *this;
    }
    
    char* getData() { return data; }
    const char* getData() const { return data; }
    
    size_t getSize() const { return size; }
    void setSize(size_t newSize) { 
        if (newSize <= capacity) {
            size = newSize;
        }
    }
    
    size_t getCapacity() const { return capacity; }
    
    bool isInUse() const { return inUse.load(); }
    void setInUse(bool used) { inUse = used; }
    
    bool isPooledBuffer() const { return isPooled; }
    
    void setLastUsed(std::chrono::steady_clock::time_point time) {
        lastUsed = time;
    }
    
    std::chrono::steady_clock::time_point getLastUsed() const {
        return lastUsed;
    }
    
    // Reset buffer to initial state
    void reset() {
        size = 0;
    }
};

// Buffer Pool class that manages a pool of buffers
class BufferPool {
private:
    std::queue<std::unique_ptr<Buffer>> availableBuffers;
    std::vector<std::unique_ptr<Buffer>> allBuffers;  // Keep track of all buffers
    std::mutex poolMutex;
    std::condition_variable bufferAvailable;
    std::atomic<size_t> bufferSize;
    std::atomic<size_t> maxBuffers;
    std::atomic<size_t> currentBufferCount{0};
    std::atomic<size_t> bufferHitCount{0};
    std::atomic<size_t> bufferMissCount{0};
    
    // Configuration for pool management
    std::chrono::seconds maxIdleTime{30};  // Maximum time a buffer can be idle
    
public:
    BufferPool(size_t bufSize, size_t maxBufCount = 100) 
        : bufferSize(bufSize), maxBuffers(maxBufCount) {
        // Pre-allocate some buffers for immediate use
        for (size_t i = 0; i < std::min(maxBufCount, static_cast<size_t>(10)); ++i) {
            auto buffer = std::make_unique<Buffer>(bufSize);
            availableBuffers.push(std::move(buffer));
            allBuffers.push_back(std::make_unique<Buffer>(bufSize)); // Keep a reference
            currentBufferCount++;
        }
    }
    
    // Get a buffer from the pool
    std::unique_ptr<Buffer> getBuffer(size_t requiredSize = 0) {
        {
            std::lock_guard<std::mutex> lock(poolMutex);
            
            if (!availableBuffers.empty()) {
                auto buffer = std::move(availableBuffers.front());
                availableBuffers.pop();
                
                buffer->setInUse(true);
                buffer->setLastUsed(std::chrono::steady_clock::now());
                buffer->reset();  // Reset to initial state
                
                if (requiredSize > 0 && requiredSize <= buffer->getCapacity()) {
                    buffer->setSize(requiredSize);
                }
                
                bufferHitCount++;
                return buffer;
            }
        }
        
        bufferMissCount++;
        
        // If no buffer available and we haven't hit the limit, create a new one
        if (currentBufferCount < maxBuffers) {
            auto buffer = std::make_unique<Buffer>(bufferSize);
            buffer->setInUse(true);
            buffer->setLastUsed(std::chrono::steady_clock::now());
            
            if (requiredSize > 0 && requiredSize <= buffer->getCapacity()) {
                buffer->setSize(requiredSize);
            }
            
            {
                std::lock_guard<std::mutex> lock(poolMutex);
                allBuffers.push_back(std::make_unique<Buffer>(bufferSize)); // Keep reference
                currentBufferCount++;
            }
            
            return buffer;
        }
        
        // If pool is full, wait for a buffer to become available
        std::unique_lock<std::mutex> lock(poolMutex);
        while (availableBuffers.empty()) {
            bufferAvailable.wait_for(lock, std::chrono::milliseconds(100));
            if (!availableBuffers.empty()) {
                auto buffer = std::move(availableBuffers.front());
                availableBuffers.pop();
                
                buffer->setInUse(true);
                buffer->setLastUsed(std::chrono::steady_clock::now());
                buffer->reset();
                
                if (requiredSize > 0 && requiredSize <= buffer->getCapacity()) {
                    buffer->setSize(requiredSize);
                }
                
                return buffer;
            }
        }
        
        return nullptr; // Should not reach here
    }
    
    // Return a buffer to the pool
    void returnBuffer(std::unique_ptr<Buffer> buffer) {
        if (!buffer || !buffer->isPooledBuffer()) {
            return; // Don't return non-pooled buffers to the pool
        }
        
        buffer->setInUse(false);
        buffer->setLastUsed(std::chrono::steady_clock::now());
        
        {
            std::lock_guard<std::mutex> lock(poolMutex);
            availableBuffers.push(std::move(buffer));
        }
        
        bufferAvailable.notify_one();
    }
    
    // Periodically clean up old unused buffers
    void cleanup() {
        std::lock_guard<std::mutex> lock(poolMutex);
        
        auto now = std::chrono::steady_clock::now();
        auto it = allBuffers.begin();
        
        while (it != allBuffers.end()) {
            if ((*it) && !(*it)->isInUse()) {
                auto timeSinceLastUsed = std::chrono::duration_cast<std::chrono::seconds>(
                    now - (*it)->getLastUsed());
                
                if (timeSinceLastUsed > maxIdleTime && availableBuffers.size() > 5) {
                    // Only remove if we have more than 5 available buffers to maintain
                    it = allBuffers.erase(it);
                    currentBufferCount--;
                    continue;
                }
            }
            ++it;
        }
    }
    
    // Get pool statistics
    void printStats() {
        std::lock_guard<std::mutex> lock(poolMutex);
        std::cout << "Buffer Pool Stats:" << std::endl;
        std::cout << "  Current Buffers: " << currentBufferCount.load() << std::endl;
        std::cout << "  Available Buffers: " << availableBuffers.size() << std::endl;
        std::cout << "  Hit Count: " << bufferHitCount.load() << std::endl;
        std::cout << "  Miss Count: " << bufferMissCount.load() << std::endl;
        std::cout << "  Hit Rate: " << (bufferHitCount.load() * 100.0 /
                                       (bufferHitCount.load() + bufferMissCount.load())) << "%" << std::endl;
    }

    size_t getAvailableBufferCount() {
        std::lock_guard<std::mutex> lock(poolMutex);
        return availableBuffers.size();
    }
    
    size_t getTotalBufferCount() const {
        return currentBufferCount.load();
    }
    
    double getHitRate() const {
        size_t hits = bufferHitCount.load();
        size_t misses = bufferMissCount.load();
        return (hits + misses) > 0 ? (hits * 100.0 / (hits + misses)) : 0.0;
    }
};

// Buffer Manager that can manage multiple pools for different buffer sizes
class BufferManager {
private:
    std::unordered_map<size_t, std::shared_ptr<BufferPool>> pools;
    mutable std::mutex managerMutex;
    
    static BufferManager* instance;
    
public:
    static BufferManager* getInstance() {
        static std::once_flag onceFlag;
        std::call_once(onceFlag, []() {
            instance = new BufferManager();
        });
        return instance;
    }
    
    std::unique_ptr<Buffer> getBuffer(size_t size) {
        std::lock_guard<std::mutex> lock(managerMutex);
        
        // Find the smallest pool that can accommodate the requested size
        size_t poolSize = size;
        if (size < 1024) {
            poolSize = 1024;  // Minimum pool size
        } else {
            // Round up to next power of 2 for efficiency
            poolSize = 1;
            while (poolSize < size) {
                poolSize <<= 1;
            }
        }
        
        auto it = pools.find(poolSize);
        if (it == pools.end()) {
            // Create a new pool for this size
            auto newPool = std::make_shared<BufferPool>(poolSize, 100);
            pools[poolSize] = newPool;
            it = pools.find(poolSize);
        }
        
        return it->second->getBuffer(size);
    }
    
    void returnBuffer(std::unique_ptr<Buffer> buffer) {
        if (!buffer || !buffer->isPooledBuffer()) {
            return;
        }
        
        std::lock_guard<std::mutex> lock(managerMutex);
        
        // Find the appropriate pool based on the buffer's capacity
        size_t poolSize = buffer->getCapacity();
        auto it = pools.find(poolSize);
        
        if (it != pools.end()) {
            it->second->returnBuffer(std::move(buffer));
        }
    }
    
    void printAllStats() const {
        std::lock_guard<std::mutex> lock(managerMutex);
        std::cout << "\n=== Buffer Manager Statistics ===" << std::endl;
        for (const auto& pair : pools) {
            std::cout << "Pool size " << pair.first << " bytes:" << std::endl;
            pair.second->printStats();
            std::cout << std::endl;
        }
    }
    
    // Periodic cleanup of all pools
    void cleanup() {
        std::lock_guard<std::mutex> lock(managerMutex);
        for (auto& pair : pools) {
            pair.second->cleanup();
        }
    }
};

// Initialize static member
BufferManager* BufferManager::instance = nullptr;

// Example usage and testing
int main() {
    std::cout << "Buffer Pool System Example\n" << std::endl;
    
    auto bufferManager = BufferManager::getInstance();
    
    std::cout << "Testing buffer allocation and deallocation..." << std::endl;
    
    // Test 1: Simple allocation and deallocation
    std::vector<std::unique_ptr<Buffer>> buffers;
    
    for (int i = 0; i < 10; ++i) {
        auto buffer = bufferManager->getBuffer(512);
        if (buffer) {
            // Simulate using the buffer
            buffer->setSize(512);
            // Fill with some data
            for (size_t j = 0; j < 512 && j < buffer->getCapacity(); ++j) {
                buffer->getData()[j] = static_cast<char>(j % 256);
            }
            
            buffers.push_back(std::move(buffer));
        }
    }
    
    // Return all buffers to the pool
    for (auto& buf : buffers) {
        if (buf) {
            bufferManager->returnBuffer(std::move(buf));
        }
    }
    buffers.clear();
    
    std::cout << "\nFirst allocation test completed." << std::endl;
    
    // Test 2: Concurrent usage
    std::cout << "\nTesting concurrent usage..." << std::endl;
    
    std::vector<std::thread> threads;
    
    // Create multiple threads that allocate and return buffers
    for (int t = 0; t < 5; ++t) {
        threads.emplace_back([bufferManager, t]() {
            for (int i = 0; i < 20; ++i) {
                auto buffer = bufferManager->getBuffer(1024);
                
                if (buffer) {
                    // Simulate some work
                    buffer->setSize(1024);
                    for (size_t j = 0; j < 1024 && j < buffer->getCapacity(); ++j) {
                        buffer->getData()[j] = static_cast<char>((i + t) % 256);
                    }
                    
                    // Sleep briefly to simulate processing time
                    std::this_thread::sleep_for(std::chrono::microseconds(100));
                    
                    bufferManager->returnBuffer(std::move(buffer));
                }
                
                // Brief pause between allocations
                std::this_thread::sleep_for(std::chrono::microseconds(50));
            }
            std::cout << "Thread " << t << " completed" << std::endl;
        });
    }
    
    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }
    
    std::cout << "\nConcurrent test completed." << std::endl;
    
    // Print statistics
    bufferManager->printAllStats();
    
    // Test 3: Different buffer sizes
    std::cout << "\nTesting different buffer sizes..." << std::endl;
    
    std::vector<std::pair<size_t, std::unique_ptr<Buffer>>> sizedBuffers;
    
    std::vector<size_t> sizes = {256, 512, 1024, 2048, 4096};
    for (size_t size : sizes) {
        auto buffer = bufferManager->getBuffer(size);
        if (buffer) {
            std::cout << "Allocated buffer of size " << size 
                     << " with actual capacity " << buffer->getCapacity() << std::endl;
            sizedBuffers.push_back({size, std::move(buffer)});
        }
    }
    
    // Return all sized buffers
    for (auto& pair : sizedBuffers) {
        bufferManager->returnBuffer(std::move(pair.second));
    }
    
    std::cout << "\nDifferent sizes test completed." << std::endl;
    
    // Final statistics
    bufferManager->printAllStats();
    
    // Cleanup
    bufferManager->cleanup();
    
    std::cout << "\nBuffer Pool System demonstration completed!" << std::endl;
    
    return 0;
}