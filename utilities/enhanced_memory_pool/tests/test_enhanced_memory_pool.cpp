#include "../include/enhanced_memory_pool.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <cstring>
#include <random>
#include <algorithm>

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
    std::cout << "=== Enhanced Memory Pool Demo ===\n\n";
    
    // Create an enhanced memory pool for objects of size TestObject
    EnhancedMemoryPool<sizeof(TestObject)> pool(20);  // Pool with 20 blocks
    
    std::cout << "Initial state:\n";
    std::cout << "Total blocks: " << pool.getNumBlocks() << "\n";
    std::cout << "Free blocks: " << pool.getFreeBlocks() << "\n";
    std::cout << "Used blocks: " << pool.getUsedBlocks() << "\n\n";
    
    // Test basic allocation and deallocation
    std::cout << "Testing basic allocation/deallocation...\n";
    void* ptr1 = pool.allocate();
    void* ptr2 = pool.allocate();
    std::cout << "Allocated 2 objects - Free: " << pool.getFreeBlocks() << ", Used: " << pool.getUsedBlocks() << "\n";
    
    pool.deallocate(ptr1);
    pool.deallocate(ptr2);
    std::cout << "Deallocated 2 objects - Free: " << pool.getFreeBlocks() << ", Used: " << pool.getUsedBlocks() << "\n\n";
    
    // Test bulk allocation
    std::cout << "Testing bulk allocation of 5 objects...\n";
    std::vector<void*> bulk_allocs = pool.bulkAllocate(5);
    std::cout << "Bulk allocated " << bulk_allocs.size() << " objects - Free: " 
              << pool.getFreeBlocks() << ", Used: " << pool.getUsedBlocks() << "\n";
    
    // Test bulk deallocation
    std::cout << "Testing bulk deallocation...\n";
    pool.bulkDeallocate(bulk_allocs);
    std::cout << "Bulk deallocated " << bulk_allocs.size() << " objects - Free: " 
              << pool.getFreeBlocks() << ", Used: " << pool.getUsedBlocks() << "\n\n";
    
    // Test the pool under load with multiple threads
    std::cout << "=== Multi-threaded Stress Test ===\n";
    const int numThreads = 4;
    const int opsPerThread = 50;
    
    std::vector<std::thread> threads;
    std::atomic<int> successfulAllocations{0};
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    for (int t = 0; t < numThreads; ++t) {
        threads.emplace_back([&pool, &successfulAllocations, opsPerThread, t]() {
            std::vector<void*> local_allocs;
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(1, 10);  // Random delay
            
            for (int i = 0; i < opsPerThread; ++i) {
                // Allocate a few objects
                for (int j = 0; j < 3; ++j) {
                    void* ptr = pool.allocate();
                    if (ptr) {
                        // Initialize the object
                        TestObject* obj = new(ptr) TestObject(t * opsPerThread + i, i * 0.5, 
                                                            ("ThreadObj" + std::to_string(t) + "_" + std::to_string(i)).c_str());
                        local_allocs.push_back(ptr);
                        successfulAllocations++;
                    }
                }
                
                // Deallocate half of them randomly
                std::shuffle(local_allocs.begin(), local_allocs.end(), gen);
                int to_deallocate = local_allocs.size() / 2;
                for (int k = 0; k < to_deallocate && !local_allocs.empty(); ++k) {
                    void* ptr = local_allocs.back();
                    local_allocs.pop_back();
                    
                    // Call destructor if needed
                    TestObject* obj = static_cast<TestObject*>(ptr);
                    obj->~TestObject();
                    
                    pool.deallocate(ptr);
                }
                
                // Small random delay
                std::this_thread::sleep_for(std::chrono::microseconds(dis(gen) * 100));
            }
            
            // Deallocate remaining objects
            for (void* ptr : local_allocs) {
                TestObject* obj = static_cast<TestObject*>(ptr);
                obj->~TestObject();
                pool.deallocate(ptr);
            }
        });
    }
    
    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    std::cout << "Stress test completed in " << duration.count() << " ms\n";
    std::cout << "Successful allocations: " << successfulAllocations.load() << "\n";
    std::cout << "Final pool state - Free: " << pool.getFreeBlocks() 
              << ", Used: " << pool.getUsedBlocks() << "\n";
    
    // Validate pool integrity
    bool integrity_ok = pool.validateIntegrity();
    std::cout << "Pool integrity check: " << (integrity_ok ? "PASSED" : "FAILED") << "\n\n";
    
    // Test reset functionality
    std::cout << "=== Testing Reset Functionality ===\n";
    void* temp_ptr = pool.allocate();
    std::cout << "After allocation - Free: " << pool.getFreeBlocks() 
              << ", Used: " << pool.getUsedBlocks() << "\n";
              
    try {
        pool.reset(); // This should fail since we have an allocated block
        std::cout << "Reset succeeded unexpectedly!\n";
    } catch (const std::exception& e) {
        std::cout << "Reset correctly failed: " << e.what() << "\n";
    }
    
    pool.deallocate(temp_ptr); // Now we can reset
    std::cout << "After deallocation - Free: " << pool.getFreeBlocks() 
              << ", Used: " << pool.getUsedBlocks() << "\n";
              
    pool.reset();
    std::cout << "After reset - Free: " << pool.getFreeBlocks() 
              << ", Used: " << pool.getUsedBlocks() << "\n";
    
    std::cout << "\nEnhanced memory pool demo completed successfully!\n";
    
    return 0;
}