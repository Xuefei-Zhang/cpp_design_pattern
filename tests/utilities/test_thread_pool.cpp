#include "utilities/thread_pool.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <cmath>
#include <algorithm>

// Function to simulate work that takes some time
void simulateWork(int taskId, int durationMs) {
    std::this_thread::sleep_for(std::chrono::milliseconds(durationMs));
    std::cout << "Task " << taskId << " completed after " << durationMs << "ms\n";
}

// Function with return value for testing futures
int calculateSquare(int x) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Simulate computation
    return x * x;
}

// Function to calculate prime numbers up to n (CPU-intensive task)
std::vector<int> findPrimes(int n) {
    std::vector<int> primes;
    for (int i = 2; i <= n; ++i) {
        bool isPrime = true;
        for (int j = 2; j * j <= i; ++j) {
            if (i % j == 0) {
                isPrime = false;
                break;
            }
        }
        if (isPrime) {
            primes.push_back(i);
        }
    }
    return primes;
}

int main() {
    std::cout << "=== Thread Pool Demo ===\n\n";
    
    // Create a thread pool with 4 threads
    ThreadPool pool(4);
    
    std::cout << "Created thread pool with " << pool.getThreadCount() << " threads\n";
    std::cout << "Current queue size: " << pool.getQueueSize() << "\n\n";
    
    // Test 1: Submit simple tasks with different durations
    std::cout << "Test 1: Submitting 8 tasks with random durations...\n";
    auto start_time = std::chrono::high_resolution_clock::now();
    
    std::vector<std::future<void>> futures1;
    for (int i = 1; i <= 8; ++i) {
        int duration = 100 + (i * 50); // Increasing durations
        futures1.push_back(pool.submit(simulateWork, i, duration));
    }
    
    // Wait for all tasks to complete
    for (auto& future : futures1) {
        future.wait();
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "All tasks completed in " << duration.count() << "ms (parallel execution)\n\n";
    
    // Test 2: Submit tasks with return values using futures
    std::cout << "Test 2: Submitting tasks with return values...\n";
    start_time = std::chrono::high_resolution_clock::now();
    
    std::vector<std::future<int>> futures2;
    for (int i = 1; i <= 10; ++i) {
        futures2.push_back(pool.submit(calculateSquare, i));
    }
    
    // Collect results
    std::cout << "Results: ";
    for (auto& future : futures2) {
        std::cout << future.get() << " ";
    }
    std::cout << "\n";
    
    end_time = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Tasks with return values completed in " << duration.count() << "ms\n\n";
    
    // Test 3: CPU-intensive tasks to demonstrate parallel processing
    std::cout << "Test 3: Submitting CPU-intensive prime-finding tasks...\n";
    start_time = std::chrono::high_resolution_clock::now();
    
    std::vector<std::future<std::vector<int>>> futures3;
    std::vector<int> limits = {100, 200, 150, 300, 250};
    
    for (int limit : limits) {
        futures3.push_back(pool.submit(findPrimes, limit));
    }
    
    // Collect and display results
    for (size_t i = 0; i < futures3.size(); ++i) {
        auto primes = futures3[i].get();
        std::cout << "Primes up to " << limits[i] << ": " << primes.size() << " found\n";
    }
    
    end_time = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Prime-finding tasks completed in " << duration.count() << "ms\n\n";
    
    // Test 4: Queue size tracking
    std::cout << "Test 4: Queue size monitoring during task submission...\n";
    start_time = std::chrono::high_resolution_clock::now();
    
    std::vector<std::future<void>> futures4;
    for (int i = 0; i < 15; ++i) {
        futures4.push_back(pool.submit([](int id) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            // Silent task
        }, i));
        
        if (i % 3 == 0) {
            std::cout << "Submitted " << (i+1) << " tasks, current queue size: " << pool.getQueueSize() << "\n";
        }
    }
    
    // Wait for all tasks to complete
    for (auto& future : futures4) {
        future.wait();
    }
    
    end_time = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Queue size after all tasks: " << pool.getQueueSize() << "\n";
    std::cout << "Batch tasks completed in " << duration.count() << "ms\n\n";
    
    // Test 5: Demonstrating graceful shutdown
    std::cout << "Test 5: Checking thread pool status...\n";
    std::cout << "Thread pool running: " << (pool.isRunning() ? "Yes" : "No") << "\n";
    std::cout << "Thread count: " << pool.getThreadCount() << "\n";
    std::cout << "Queue size: " << pool.getQueueSize() << "\n\n";
    
    // Test 6: Performance comparison - sequential vs parallel
    std::cout << "Performance Comparison: Sequential vs Parallel Execution\n";
    
    // Sequential execution
    start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 5; ++i) {
        auto primes = findPrimes(200 + i * 50);
        // Use result to prevent optimization away
        volatile size_t count = primes.size();
    }
    auto seq_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start_time
    );
    
    // Parallel execution using thread pool
    start_time = std::chrono::high_resolution_clock::now();
    std::vector<std::future<std::vector<int>>> perf_futures;
    for (int i = 0; i < 5; ++i) {
        perf_futures.push_back(pool.submit(findPrimes, 200 + i * 50));
    }
    
    for (auto& future : perf_futures) {
        volatile size_t count = future.get().size(); // Use result to prevent optimization away
    }
    
    auto par_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start_time
    );
    
    std::cout << "Sequential execution time: " << seq_duration.count() << "ms\n";
    std::cout << "Parallel execution time: " << par_duration.count() << "ms\n";
    std::cout << "Speedup: " << static_cast<double>(seq_duration.count()) / par_duration.count() << "x\n\n";
    
    std::cout << "Thread pool demo completed successfully!\n";
    
    return 0;
}