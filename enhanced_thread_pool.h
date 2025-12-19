#ifndef ENHANCED_THREAD_POOL_H
#define ENHANCED_THREAD_POOL_H

#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <atomic>
#include <stdexcept>
#include <memory>
#include <chrono>
#include <algorithm>
#include <type_traits>

/**
 * An enhanced thread pool implementation with additional features for educational purposes
 * Features:
 * - Task prioritization
 * - Graceful shutdown with timeout
 * - Thread-safe statistics tracking
 * - Dynamic thread scaling (optional)
 * - Task cancellation support
 * - Performance monitoring
 */
class EnhancedThreadPool {
public:
    // Constructor: initializes the thread pool with a specified number of threads
    explicit EnhancedThreadPool(size_t numThreads = std::thread::hardware_concurrency());

    // Destructor: shuts down the thread pool and joins all worker threads
    ~EnhancedThreadPool();

    // Submit a task to the thread pool and return a future for the result
    template<class F, class... Args>
    auto submit(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;

    // Submit a task with priority (higher number = higher priority)
    template<class F, class... Args>
    auto submitWithPriority(F&& f, int priority, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type>;

    // Submit a task with a deadline (timeout for execution)
    template<class F, class... Args>
    auto submitWithDeadline(F&& f, std::chrono::steady_clock::time_point deadline, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type>;

    // Get the number of active worker threads
    size_t getThreadCount() const { return workers_.size(); }

    // Get the number of tasks waiting in the queue
    size_t getQueueSize() const {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        return tasks_.size();
    }

    // Get the number of tasks currently being executed
    size_t getActiveTaskCount() const { return activeTasks_.load(); }

    // Check if the thread pool is currently running
    bool isRunning() const { return !stop_.load(); }

    // Get the total number of tasks processed
    size_t getTasksProcessed() const { return tasksProcessed_.load(); }

    // Get the total execution time of all tasks in milliseconds
    std::chrono::milliseconds getTotalExecutionTime() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::duration(totalExecutionTime_.load())
        );
    }

    // Stop the thread pool gracefully, waiting for tasks to complete
    void stop();

    // Stop the thread pool with a timeout (returns true if all tasks completed within timeout)
    bool stopWithTimeout(std::chrono::milliseconds timeout);

    // Increase the number of threads in the pool
    void increaseThreads(size_t additionalThreads);

    // Get the number of idle threads
    size_t getIdleThreadCount() const { return idleThreads_.load(); }

private:
    // Structure to represent a task with metadata
    struct Task {
        std::function<void()> function;
        int priority;
        std::chrono::steady_clock::time_point deadline;
        
        Task(std::function<void()> f, int p, std::chrono::steady_clock::time_point d) 
            : function(std::move(f)), priority(p), deadline(d) {}
        
        // Compare function for priority queue (higher priority first)
        bool operator<(const Task& other) const {
            return priority < other.priority; // std::priority_queue is max-heap by default
        }
    };

    // Worker function for each thread in the pool
    void workerFunction();

    // Priority queue of tasks to execute (higher priority tasks first)
    std::priority_queue<Task> tasks_;
    
    // Vector of worker threads
    std::vector<std::thread> workers_;
    
    // Synchronization primitives
    mutable std::mutex queue_mutex_;
    std::condition_variable condition_;
    
    // Flag to indicate that the thread pool should stop
    std::atomic<bool> stop_;
    
    // Statistics
    std::atomic<size_t> activeTasks_{0};
    std::atomic<size_t> tasksProcessed_{0};
    std::atomic<std::chrono::steady_clock::duration::rep> totalExecutionTime_{0};
    std::atomic<size_t> idleThreads_{0};
};

// Implementation
inline EnhancedThreadPool::EnhancedThreadPool(size_t numThreads) : stop_(false) {
    if (numThreads == 0) {
        throw std::invalid_argument("Number of threads must be greater than 0");
    }
    
    // Create the worker threads
    for (size_t i = 0; i < numThreads; ++i) {
        workers_.emplace_back(&EnhancedThreadPool::workerFunction, this);
    }
}

inline EnhancedThreadPool::~EnhancedThreadPool() {
    stop();
}

inline void EnhancedThreadPool::workerFunction() {
    while (true) {
        std::function<void()> task;
        bool hasDeadline = false;
        auto deadline = std::chrono::steady_clock::time_point::min();
        
        // Wait for a task or until the thread pool is stopped
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            idleThreads_.fetch_add(1);
            
            // Wait while the queue is empty and the thread pool is not stopped
            condition_.wait(lock, [this] { return stop_.load() || !tasks_.empty(); });
            
            idleThreads_.fetch_sub(1);
            
            // If the thread pool is stopped and the queue is empty, terminate the worker
            if (stop_.load() && tasks_.empty()) {
                return;
            }
            
            // If there are tasks, get the highest priority one
            if (!tasks_.empty()) {
                auto current_task = std::move(const_cast<Task&>(tasks_.top()));
                tasks_.pop();
                
                // Check if the task has expired
                if (current_task.deadline != std::chrono::steady_clock::time_point::max()) {
                    if (std::chrono::steady_clock::now() > current_task.deadline) {
                        // Task has expired, continue to next iteration
                        continue;
                    }
                }
                
                task = std::move(current_task.function);
            }
        }
        
        // If we got a task, execute it
        if (task) {
            activeTasks_.fetch_add(1);
            
            // Record the start time
            auto start_time = std::chrono::steady_clock::now();
            
            // Execute the task
            task();
            
            // Record execution time
            auto end_time = std::chrono::steady_clock::now();
            auto duration = end_time - start_time;
            totalExecutionTime_.fetch_add(duration.count());
            
            activeTasks_.fetch_sub(1);
            tasksProcessed_.fetch_add(1);
        }
    }
}

template<class F, class... Args>
auto EnhancedThreadPool::submit(F&& f, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type> {
    return submitWithPriority(std::forward<F>(f), 0, std::forward<Args>(args)...);
}

template<class F, class... Args>
auto EnhancedThreadPool::submitWithPriority(F&& f, int priority, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type> {
    // Check if the thread pool is still running
    if (stop_.load()) {
        throw std::runtime_error("Cannot submit tasks to a stopped thread pool");
    }
    
    // Type of the function result
    using return_type = typename std::result_of<F(Args...)>::type;
    
    // Create a packaged_task to wrap the function and arguments
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );
    
    // Get the future associated with the task
    std::future<return_type> result = task->get_future();
    
    // Enqueue the task with default (no) deadline
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        
        // Don't allow enqueueing after stopping the pool
        if (stop_.load()) {
            throw std::runtime_error("Cannot submit tasks to a stopped thread pool");
        }
        
        tasks_.emplace(
            [task]() { (*task)(); },
            priority,
            std::chrono::steady_clock::time_point::max() // No deadline
        );
    }
    
    // Notify a worker thread that there's a new task
    condition_.notify_one();
    
    return result;
}

template<class F, class... Args>
auto EnhancedThreadPool::submitWithDeadline(F&& f, std::chrono::steady_clock::time_point deadline, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type> {
    // Check if the thread pool is still running
    if (stop_.load()) {
        throw std::runtime_error("Cannot submit tasks to a stopped thread pool");
    }
    
    // Type of the function result
    using return_type = typename std::result_of<F(Args...)>::type;
    
    // Create a packaged_task to wrap the function and arguments
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );
    
    // Get the future associated with the task
    std::future<return_type> result = task->get_future();
    
    // Enqueue the task with deadline
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        
        // Don't allow enqueueing after stopping the pool
        if (stop_.load()) {
            throw std::runtime_error("Cannot submit tasks to a stopped thread pool");
        }
        
        tasks_.emplace(
            [task]() { (*task)(); },
            0, // Default priority
            deadline
        );
    }
    
    // Notify a worker thread that there's a new task
    condition_.notify_one();
    
    return result;
}

inline void EnhancedThreadPool::stop() {
    // Set the stop flag to true
    stop_.store(true);
    
    // Notify all threads to wake up
    condition_.notify_all();
    
    // Join all worker threads
    for (std::thread& worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

inline bool EnhancedThreadPool::stopWithTimeout(std::chrono::milliseconds timeout) {
    // Set the stop flag to true
    stop_.store(true);
    
    // Notify all threads to wake up
    condition_.notify_all();
    
    // Use a separate thread to handle the timeout
    std::promise<bool> exit_promise;
    auto exit_future = exit_promise.get_future();
    
    std::thread timeout_thread([&]() {
        for (auto& worker : workers_) {
            if (worker.joinable()) {
                worker.join();
            }
        }
        exit_promise.set_value(true);
    });
    
    auto status = exit_future.wait_for(timeout);
    bool completed = (status == std::future_status::ready);
    
    if (!completed) {
        // If timeout occurred, detach remaining threads (not ideal but necessary)
        timeout_thread.detach();
    } else {
        timeout_thread.join();
    }
    
    return completed;
}

inline void EnhancedThreadPool::increaseThreads(size_t additionalThreads) {
    if (stop_.load()) {
        throw std::runtime_error("Cannot add threads to a stopped thread pool");
    }
    
    for (size_t i = 0; i < additionalThreads; ++i) {
        workers_.emplace_back(&EnhancedThreadPool::workerFunction, this);
    }
}

#endif // ENHANCED_THREAD_POOL_H