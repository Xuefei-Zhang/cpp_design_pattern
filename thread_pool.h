#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <atomic>
#include <stdexcept>

/**
 * A simple thread pool implementation for educational purposes
 * Allows tasks to be submitted and executed by a pool of worker threads
 */
class ThreadPool {
public:
    // Constructor: initializes the thread pool with a specified number of threads
    explicit ThreadPool(size_t numThreads = std::thread::hardware_concurrency());

    // Destructor: shuts down the thread pool and joins all worker threads
    ~ThreadPool();

    // Submit a task to the thread pool and return a future for the result
    template<class F, class... Args>
    auto submit(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;

    // Get the number of active worker threads
    size_t getThreadCount() const { return workers_.size(); }

    // Get the number of tasks waiting in the queue
    size_t getQueueSize() const {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        return tasks_.size();
    }

    // Check if the thread pool is currently running
    bool isRunning() const { return !stop_.load(); }

private:
    // Worker function for each thread in the pool
    void workerFunction();

    // Queue of tasks to execute
    std::queue<std::function<void()>> tasks_;
    
    // Vector of worker threads
    std::vector<std::thread> workers_;
    
    // Synchronization primitives
    mutable std::mutex queue_mutex_;
    std::condition_variable condition_;
    
    // Flag to indicate that the thread pool should stop
    std::atomic<bool> stop_;
};

// Implementation
inline ThreadPool::ThreadPool(size_t numThreads) : stop_(false) {
    if (numThreads == 0) {
        throw std::invalid_argument("Number of threads must be greater than 0");
    }
    
    // Create the worker threads
    for (size_t i = 0; i < numThreads; ++i) {
        workers_.emplace_back(&ThreadPool::workerFunction, this);
    }
}

inline ThreadPool::~ThreadPool() {
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

inline void ThreadPool::workerFunction() {
    while (true) {
        std::function<void()> task;
        
        // Wait for a task or until the thread pool is stopped
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            
            // Wait while the queue is empty and the thread pool is not stopped
            condition_.wait(lock, [this] { return stop_.load() || !tasks_.empty(); });
            
            // If the thread pool is stopped and the queue is empty, terminate the worker
            if (stop_.load() && tasks_.empty()) {
                return;
            }
            
            // Get the next task from the queue
            task = std::move(tasks_.front());
            tasks_.pop();
        }
        
        // Execute the task
        task();
    }
}

template<class F, class... Args>
auto ThreadPool::submit(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
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
    
    // Enqueue the task
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        
        // Don't allow enqueueing after stopping the pool
        if (stop_.load()) {
            throw std::runtime_error("Cannot submit tasks to a stopped thread pool");
        }
        
        tasks_.emplace([task](){ (*task)(); });
    }
    
    // Notify a worker thread that there's a new task
    condition_.notify_one();
    
    return result;
}

#endif // THREAD_POOL_H