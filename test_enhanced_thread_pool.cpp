#include "enhanced_thread_pool.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <cmath>
#include <algorithm>
#include <iomanip>

// Function to simulate work that takes some time
void simulateWork(int taskId, int durationMs, const std::string& description = "") {
    std::this_thread::sleep_for(std::chrono::milliseconds(durationMs));
    std::cout << "Task " << taskId << " (" << description << ") completed after " << durationMs << "ms\n";
}

// Function with return value for testing futures
int calculateSquare(int x) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Simulate computation
    return x * x;
}

// Function to calculate factorial (CPU-intensive)
long long calculateFactorial(int n) {
    long long result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Simulate work
    }
    return result;
}

int main() {
    std::cout << "=== Enhanced Thread Pool Demo ===\n\n";
    
    // Create an enhanced thread pool with 3 threads
    EnhancedThreadPool pool(3);
    
    std::cout << "Created enhanced thread pool with " << pool.getThreadCount() << " threads\n";
    std::cout << "Current queue size: " << pool.getQueueSize() << "\n";
    std::cout << "Active tasks: " << pool.getActiveTaskCount() << "\n\n";
    
    // Test 1: Priority-based task execution
    std::cout << "Test 1: Priority-based task execution...\n";
    auto start_time = std::chrono::high_resolution_clock::now();
    
    std::vector<std::future<void>> futures1;
    
    // Submit tasks with different priorities
    futures1.push_back(pool.submitWithPriority(simulateWork, 1, 10, 100, "Low Priority"));
    futures1.push_back(pool.submitWithPriority(simulateWork, 10, 20, 200, "High Priority"));
    futures1.push_back(pool.submitWithPriority(simulateWork, 5, 30, 50, "Medium Priority"));
    futures1.push_back(pool.submitWithPriority(simulateWork, 1, 40, 150, "Low Priority"));
    futures1.push_back(pool.submitWithPriority(simulateWork, 10, 50, 300, "High Priority"));
    
    // Wait for all tasks to complete
    for (auto& future : futures1) {
        future.wait();
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Priority tasks completed in " << duration.count() << "ms\n";
    std::cout << "Total tasks processed: " << pool.getTasksProcessed() << "\n\n";
    
    // Test 2: Deadline-based task execution
    std::cout << "Test 2: Deadline-based task execution...\n";
    start_time = std::chrono::high_resolution_clock::now();
    
    std::vector<std::future<void>> futures2;
    
    // Create a deadline 100ms from now
    auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(100);
    
    // Submit a task that will complete before the deadline
    futures2.push_back(pool.submitWithDeadline(simulateWork, deadline, 60, 50, "On-time Task"));
    
    // Submit a task that will take longer than the deadline
    futures2.push_back(pool.submitWithDeadline(simulateWork, deadline, 70, 200, "Overdue Task"));
    
    // Wait for the tasks to complete or time out
    for (auto& future : futures2) {
        try {
            future.wait();
            std::cout << "Task completed or skipped\n";
        } catch (...) {
            std::cout << "Task failed\n";
        }
    }
    
    end_time = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Deadline tasks completed in " << duration.count() << "ms\n\n";
    
    // Test 3: Task with return values and performance metrics
    std::cout << "Test 3: Tasks with return values and performance tracking...\n";
    start_time = std::chrono::high_resolution_clock::now();
    
    std::vector<std::future<int>> futures3;
    for (int i = 1; i <= 8; ++i) {
        futures3.push_back(pool.submit(calculateSquare, i));
    }
    
    // Collect results
    std::cout << "Results: ";
    for (auto& future : futures3) {
        std::cout << future.get() << " ";
    }
    std::cout << "\n";
    
    end_time = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Tasks with return values completed in " << duration.count() << "ms\n";
    std::cout << "Total execution time tracked: " << pool.getTotalExecutionTime().count() << "ms\n";
    std::cout << "Active tasks during execution: " << pool.getActiveTaskCount() << "\n\n";
    
    // Test 4: CPU-intensive tasks with priority
    std::cout << "Test 4: CPU-intensive tasks with different priorities...\n";
    start_time = std::chrono::high_resolution_clock::now();
    
    std::vector<std::future<long long>> futures4;
    
    // Submit CPU-intensive tasks with different priorities
    futures4.push_back(pool.submitWithPriority(calculateFactorial, 1, 10)); // Low priority
    futures4.push_back(pool.submitWithPriority(calculateFactorial, 10, 12)); // High priority
    futures4.push_back(pool.submitWithPriority(calculateFactorial, 5, 11)); // Medium priority
    futures4.push_back(pool.submitWithPriority(calculateFactorial, 10, 13)); // High priority
    
    std::cout << "Results: ";
    for (auto& future : futures4) {
        std::cout << future.get() << " ";
    }
    std::cout << "\n";
    
    end_time = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "CPU-intensive tasks completed in " << duration.count() << "ms\n";
    std::cout << "Total tasks processed: " << pool.getTasksProcessed() << "\n\n";
    
    // Test 5: Dynamic thread scaling
    std::cout << "Test 5: Dynamic thread scaling...\n";
    std::cout << "Before scaling - Thread count: " << pool.getThreadCount() 
              << ", Idle threads: " << pool.getIdleThreadCount() << "\n";
    
    // Add 2 more threads
    pool.increaseThreads(2);
    std::cout << "After adding 2 threads - Thread count: " << pool.getThreadCount() << "\n\n";
    
    // Test 6: Submit more tasks to utilize the new threads
    std::cout << "Submitting 10 more tasks to test the expanded thread pool...\n";
    std::vector<std::future<void>> futures5;
    for (int i = 100; i < 110; ++i) {
        futures5.push_back(pool.submit([](int id) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            // Silent task to prevent console spam during this test
        }, i));
    }
    
    // Wait for all tasks to complete
    for (auto& future : futures5) {
        future.wait();
    }
    
    std::cout << "After expansion - Active tasks: " << pool.getActiveTaskCount() 
              << ", Queue size: " << pool.getQueueSize()
              << ", Total processed: " << pool.getTasksProcessed() << "\n\n";
    
    // Test 7: Statistics and performance comparison
    std::cout << "Test 7: Performance metrics and statistics...\n";
    std::cout << "Thread pool status - Running: " << (pool.isRunning() ? "Yes" : "No") << "\n";
    std::cout << "Thread count: " << pool.getThreadCount() << "\n";
    std::cout << "Idle threads: " << pool.getIdleThreadCount() << "\n";
    std::cout << "Total tasks processed: " << pool.getTasksProcessed() << "\n";
    std::cout << "Total execution time: " << pool.getTotalExecutionTime().count() << "ms\n\n";
    
    // Test 8: Graceful shutdown with timeout
    std::cout << "Test 8: Testing graceful shutdown...\n";
    
    // Submit a few more tasks to demonstrate shutdown behavior
    std::vector<std::future<void>> futures6;
    for (int i = 200; i < 205; ++i) {
        futures6.push_back(pool.submit([](int id) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }, i));
    }
    
    // Attempt to stop with timeout
    bool stopped = pool.stopWithTimeout(std::chrono::milliseconds(1000));
    std::cout << "Thread pool stopped gracefully: " << (stopped ? "Yes" : "No") << "\n";
    std::cout << "Thread pool running status: " << (pool.isRunning() ? "Yes" : "No") << "\n\n";
    
    std::cout << "Enhanced thread pool demo completed successfully!\n";
    
    return 0;
}