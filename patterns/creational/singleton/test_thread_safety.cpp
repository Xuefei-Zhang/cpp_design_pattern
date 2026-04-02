#include "creational/singleton_patterns.h"
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <cassert>
#include <chrono>
#include <set>
#include <mutex>

/**
 * Thread Safety Test Cases for Singleton Pattern
 *
 * Tests concurrent access and initialization of singletons
 */

std::mutex output_mutex;

void testConcurrentInitialization() {
    std::cout << "Test 1: Concurrent Singleton Initialization\n";
    std::cout << "---------------------------------------------\n";

    const int NUM_THREADS = 20;
    std::vector<std::thread> threads;
    std::vector<ThreadSafeSingleton*> instances(NUM_THREADS);

    // Launch multiple threads that all try to get the singleton simultaneously
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back([&instances, i]() {
            instances[i] = &ThreadSafeSingleton::getInstance();
        });
    }

    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }

    // Verify all threads got the same instance
    for (int i = 1; i < NUM_THREADS; ++i) {
        assert(instances[i] == instances[0]);
    }

    std::cout << "✓ All " << NUM_THREADS << " threads received the same instance\n";
    std::cout << "✓ Concurrent Initialization test passed!\n\n";
}

void testConcurrentCounterIncrement() {
    std::cout << "Test 2: Concurrent Counter Increment\n";
    std::cout << "--------------------------------------\n";

    const int NUM_THREADS = 10;
    const int INCREMENTS_PER_THREAD = 1000;
    std::vector<std::thread> threads;

    // Reset counter by creating a fresh state
    auto& singleton = ThreadSafeSingleton::getInstance();
    int initialCount = 0;

    // Launch threads that increment the counter
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back([INCREMENTS_PER_THREAD]() {
            auto& instance = ThreadSafeSingleton::getInstance();
            for (int j = 0; j < INCREMENTS_PER_THREAD; ++j) {
                instance.incrementCounter();
            }
        });
    }

    // Wait for all threads
    for (auto& thread : threads) {
        thread.join();
    }

    int finalCount = singleton.incrementCounter() - 1;  // Subtract the test increment
    int expectedMin = NUM_THREADS * INCREMENTS_PER_THREAD;

    std::cout << "Final counter value: " << finalCount << "\n";
    std::cout << "Expected minimum: " << expectedMin << "\n";
    assert(finalCount >= expectedMin);
    std::cout << "✓ Counter incremented correctly under concurrent access\n";
    std::cout << "✓ Concurrent Counter Increment test passed!\n\n";
}

void testConcurrentDataAccess() {
    std::cout << "Test 3: Concurrent Data Read/Write\n";
    std::cout << "------------------------------------\n";

    const int NUM_THREADS = 15;
    std::vector<std::thread> threads;
    std::atomic<int> completed{0};

    // Mix of reader and writer threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        if (i % 3 == 0) {
            // Writer thread
            threads.emplace_back([i, &completed]() {
                auto& instance = ThreadSafeSingleton::getInstance();
                instance.setData("Thread_" + std::to_string(i));
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                completed++;
            });
        } else {
            // Reader thread
            threads.emplace_back([&completed]() {
                auto& instance = ThreadSafeSingleton::getInstance();
                std::string data = instance.getData();
                // Just read the data
                (void)data;  // Avoid unused variable warning
                completed++;
            });
        }
    }

    // Wait for all threads
    for (auto& thread : threads) {
        thread.join();
    }

    assert(completed == NUM_THREADS);
    std::cout << "✓ All " << NUM_THREADS << " threads completed successfully\n";
    std::cout << "✓ Concurrent Data Access test passed!\n\n";
}

void testMultipleSingletonsConcurrently() {
    std::cout << "Test 4: Multiple Singleton Types Accessed Concurrently\n";
    std::cout << "--------------------------------------------------------\n";

    const int NUM_THREADS = 12;
    std::vector<std::thread> threads;
    std::set<void*> basicInstances;
    std::set<void*> threadSafeInstances;
    std::mutex instancesMutex;

    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back([&basicInstances, &threadSafeInstances, &instancesMutex]() {
            // Access different singleton types
            auto* basic = BasicSingleton::getInstance();
            auto& threadSafe = ThreadSafeSingleton::getInstance();

            {
                std::lock_guard<std::mutex> lock(instancesMutex);
                basicInstances.insert(static_cast<void*>(basic));
                threadSafeInstances.insert(static_cast<void*>(&threadSafe));
            }

            // Do some work with the instances
            basic->setData("Data");
            threadSafe.setData("Data");
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    // Each singleton type should have exactly one unique instance
    assert(basicInstances.size() == 1);
    assert(threadSafeInstances.size() == 1);

    std::cout << "✓ BasicSingleton had exactly 1 unique instance across all threads\n";
    std::cout << "✓ ThreadSafeSingleton had exactly 1 unique instance across all threads\n";
    std::cout << "✓ Multiple Singletons Concurrent Access test passed!\n\n";
}

void testRaceConditionProtection() {
    std::cout << "Test 5: Race Condition Protection\n";
    std::cout << "-----------------------------------\n";

    const int NUM_THREADS = 30;
    std::vector<std::thread> threads;
    std::atomic<bool> startFlag{false};
    std::vector<void*> capturedAddresses(NUM_THREADS);

    // Create threads that wait for a start signal
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back([i, &startFlag, &capturedAddresses]() {
            // Wait for start signal to maximize concurrency
            while (!startFlag.load()) {
                std::this_thread::yield();
            }

            // Try to get instance at the same time
            auto* instance = DoubleCheckedSingleton::getInstance();
            capturedAddresses[i] = static_cast<void*>(instance);
        });
    }

    // Small delay to ensure all threads are waiting
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // Release all threads simultaneously
    startFlag.store(true);

    // Wait for completion
    for (auto& thread : threads) {
        thread.join();
    }

    // All addresses should be identical
    for (int i = 1; i < NUM_THREADS; ++i) {
        assert(capturedAddresses[i] == capturedAddresses[0]);
    }

    std::cout << "✓ All " << NUM_THREADS << " concurrent accesses returned the same instance\n";
    std::cout << "✓ Race Condition Protection test passed!\n\n";
}

void testHighContentionScenario() {
    std::cout << "Test 6: High Contention Scenario\n";
    std::cout << "----------------------------------\n";

    const int NUM_THREADS = 50;
    const int ITERATIONS = 100;
    std::vector<std::thread> threads;
    std::atomic<int> totalOperations{0};

    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back([&totalOperations]() {
            for (int j = 0; j < ITERATIONS; ++j) {
                auto& instance = ThreadSafeSingleton::getInstance();
                instance.incrementCounter();
                instance.setData("Thread operation");
                totalOperations++;
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    int expectedOperations = NUM_THREADS * ITERATIONS;
    assert(totalOperations == expectedOperations);

    std::cout << "✓ Completed " << totalOperations << " operations under high contention\n";
    std::cout << "✓ High Contention test passed!\n\n";
}

void testConfigManagerThreadSafety() {
    std::cout << "Test 7: Config Manager Thread Safety\n";
    std::cout << "--------------------------------------\n";

    const int NUM_THREADS = 20;
    std::vector<std::thread> threads;

    // Writer threads
    for (int i = 0; i < NUM_THREADS / 2; ++i) {
        threads.emplace_back([i]() {
            auto& config = ConfigManager::getInstance();
            config.setConfig("key_" + std::to_string(i), "value_" + std::to_string(i));
        });
    }

    // Reader threads
    for (int i = 0; i < NUM_THREADS / 2; ++i) {
        threads.emplace_back([i]() {
            auto& config = ConfigManager::getInstance();
            std::string value = config.getConfig("key_" + std::to_string(i));
            // Value might be empty if reader runs before writer
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    // Verify some values were set
    auto& config = ConfigManager::getInstance();
    bool foundValue = false;
    for (int i = 0; i < NUM_THREADS / 2; ++i) {
        std::string value = config.getConfig("key_" + std::to_string(i));
        if (!value.empty()) {
            foundValue = true;
            break;
        }
    }

    assert(foundValue);
    std::cout << "✓ Config manager handled concurrent read/write operations\n";
    std::cout << "✓ Config Manager Thread Safety test passed!\n\n";
}

void performanceComparison() {
    std::cout << "Test 8: Performance Comparison (Thread-Safe vs Basic)\n";
    std::cout << "-------------------------------------------------------\n";

    const int ITERATIONS = 100000;

    // Test ThreadSafeSingleton performance
    auto start1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        auto& instance = ThreadSafeSingleton::getInstance();
        (void)instance;  // Prevent optimization
    }
    auto end1 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1);

    // Test BasicSingleton performance
    auto start2 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; ++i) {
        auto* instance = BasicSingleton::getInstance();
        (void)instance;  // Prevent optimization
    }
    auto end2 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2);

    std::cout << "ThreadSafeSingleton: " << duration1.count() << " μs for " << ITERATIONS << " accesses\n";
    std::cout << "BasicSingleton:      " << duration2.count() << " μs for " << ITERATIONS << " accesses\n";
    std::cout << "✓ Performance Comparison completed\n\n";
}

int main() {
    std::cout << "╔════════════════════════════════════════════════════╗\n";
    std::cout << "║   Singleton Pattern - Thread Safety Test Suite    ║\n";
    std::cout << "╚════════════════════════════════════════════════════╝\n\n";

    try {
        testConcurrentInitialization();
        testConcurrentCounterIncrement();
        testConcurrentDataAccess();
        testMultipleSingletonsConcurrently();
        testRaceConditionProtection();
        testHighContentionScenario();
        testConfigManagerThreadSafety();
        performanceComparison();

        std::cout << "╔════════════════════════════════════════════════════╗\n";
        std::cout << "║    ALL THREAD SAFETY TESTS PASSED! ✓              ║\n";
        std::cout << "╚════════════════════════════════════════════════════╝\n";

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "✗ Test failed with exception: " << e.what() << "\n";
        return 1;
    }
}
