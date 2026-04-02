#include "../include/memory_pool.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <cstring>

// Simple structure to test allocation
struct TestObject {
    int id;
    double value;
    char name[32];
    
    TestObject(int i = 0, double v = 0.0, const char* n = "") : id(i), value(v) {
        strncpy(name, n, sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';
    }
    
    void print() const {
        std::cout << "TestObject{id: " << id << ", value: " << value << ", name: " << name << "}\n";
    }
};

int main() {
    std::cout << "=== Memory Pool Demo ===\n\n";
    
    // Create a memory pool for objects of size TestObject
    MemoryPool<sizeof(TestObject)> pool(10);  // Pool with 10 blocks
    
    std::cout << "Initial state:\n";
    std::cout << "Total blocks: " << pool.getNumBlocks() << "\n";
    std::cout << "Free blocks: " << pool.getFreeBlocks() << "\n";
    std::cout << "Used blocks: " << pool.getUsedBlocks() << "\n\n";
    
    // Allocate some objects
    std::vector<void*> allocations;
    
    std::cout << "Allocating 5 objects...\n";
    for (int i = 0; i < 5; ++i) {
        void* ptr = pool.allocate();
        if (ptr) {
            allocations.push_back(ptr);
            // Construct object at the allocated memory
            TestObject* obj = new(ptr) TestObject(i, i * 3.14, ("Object" + std::to_string(i)).c_str());
            obj->print();
        } else {
            std::cout << "Allocation failed!\n";
        }
    }
    
    std::cout << "\nAfter allocations:\n";
    std::cout << "Free blocks: " << pool.getFreeBlocks() << "\n";
    std::cout << "Used blocks: " << pool.getUsedBlocks() << "\n\n";
    
    // Try to allocate more objects than available
    std::cout << "Trying to allocate 7 more objects (should fail after 5)...\n";
    for (int i = 0; i < 7; ++i) {
        void* ptr = pool.allocate();
        if (ptr) {
            allocations.push_back(ptr);
            // Construct object at the allocated memory
            TestObject* obj = new(ptr) TestObject(i + 10, i * 2.71, ("ExtraObject" + std::to_string(i)).c_str());
            obj->print();
        } else {
            std::cout << "Allocation " << i << " failed - pool exhausted!\n";
        }
    }
    
    std::cout << "\nAfter all allocations:\n";
    std::cout << "Free blocks: " << pool.getFreeBlocks() << "\n";
    std::cout << "Used blocks: " << pool.getUsedBlocks() << "\n\n";
    
    // Deallocate some objects
    std::cout << "Deallocating 3 objects...\n";
    for (int i = 0; i < 3; ++i) {
        if (!allocations.empty()) {
            void* ptr = allocations.back();
            allocations.pop_back();
            
            // Explicitly call destructor if needed
            TestObject* obj = static_cast<TestObject*>(ptr);
            obj->~TestObject();
            
            pool.deallocate(ptr);
        }
    }
    
    std::cout << "\nAfter deallocations:\n";
    std::cout << "Free blocks: " << pool.getFreeBlocks() << "\n";
    std::cout << "Used blocks: " << pool.getUsedBlocks() << "\n\n";
    
    // Allocate again to show re-use
    std::cout << "Allocating 2 more objects (these should reuse deallocated space)...\n";
    for (int i = 0; i < 2; ++i) {
        void* ptr = pool.allocate();
        if (ptr) {
            allocations.push_back(ptr);
            TestObject* obj = new(ptr) TestObject(i + 20, i * 1.41, ("ReusedObject" + std::to_string(i)).c_str());
            obj->print();
        } else {
            std::cout << "Allocation failed!\n";
        }
    }
    
    std::cout << "\nFinal state:\n";
    std::cout << "Free blocks: " << pool.getFreeBlocks() << "\n";
    std::cout << "Used blocks: " << pool.getUsedBlocks() << "\n\n";
    
    // Clean up remaining allocations
    while (!allocations.empty()) {
        void* ptr = allocations.back();
        allocations.pop_back();
        
        TestObject* obj = static_cast<TestObject*>(ptr);
        obj->~TestObject();
        
        pool.deallocate(ptr);
    }
    
    std::cout << "After cleaning up:\n";
    std::cout << "Free blocks: " << pool.getFreeBlocks() << "\n";
    std::cout << "Used blocks: " << pool.getUsedBlocks() << "\n";
    
    std::cout << "\n=== Thread Safety Test ===\n";
    
    // Test thread safety
    MemoryPool<64> threadSafePool(20);  // Pool with 20 blocks of 64 bytes each
    
    std::vector<std::thread> threads;
    std::vector<void*> threadAllocations;
    std::mutex outputMutex;
    
    // Create multiple threads that allocate and deallocate
    for (int t = 0; t < 3; ++t) {
        threads.emplace_back([&threadSafePool, &threadAllocations, &outputMutex, t]() {
            for (int i = 0; i < 5; ++i) {
                void* ptr = threadSafePool.allocate();
                {
                    std::lock_guard<std::mutex> lock(outputMutex);
                    std::cout << "Thread " << t << " allocated block " << ptr << std::endl;
                }
                
                if (ptr) {
                    // Simulate some work
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    
                    {
                        std::lock_guard<std::mutex> lock(outputMutex);
                        std::cout << "Thread " << t << " deallocating block " << ptr << std::endl;
                    }
                    
                    threadSafePool.deallocate(ptr);
                }
                
                // Small delay to allow other threads to run
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
        });
    }
    
    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }
    
    std::cout << "\nThread safety test completed.\n";
    std::cout << "Final thread-safe pool state:\n";
    std::cout << "Free blocks: " << threadSafePool.getFreeBlocks() << "\n";
    std::cout << "Used blocks: " << threadSafePool.getUsedBlocks() << "\n";
    
    return 0;
}