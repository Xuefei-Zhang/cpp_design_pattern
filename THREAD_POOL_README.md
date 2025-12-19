# Thread Pool Implementation in C++

This project contains two implementations of a thread pool in C++ for educational purposes:

1. Basic Thread Pool (`thread_pool.h`)
2. Enhanced Thread Pool (`enhanced_thread_pool.h`)

## Overview

A thread pool is a design pattern that maintains a pool of threads to execute tasks. This approach:

- Reduces thread creation overhead
- Controls resource consumption
- Improves application performance
- Provides better thread management

## Components

### Basic Thread Pool
- Fixed-size thread pool
- Task submission with futures
- Thread-safe operations using mutex and condition variables
- Simple queue-based task scheduling

### Enhanced Thread Pool
- All features of the basic pool plus:
- Task priority scheduling
- Deadline-based task execution
- Dynamic thread scaling
- Comprehensive statistics tracking
- Timeout-based shutdown
- Performance monitoring

## Files

- `thread_pool.h` - Basic thread pool implementation
- `test_thread_pool.cpp` - Test program for basic pool
- `enhanced_thread_pool.h` - Enhanced thread pool implementation
- `test_enhanced_thread_pool.cpp` - Test program for enhanced pool
- `THREAD_POOL_README.md` - This documentation

## Key Features

- **Task Submission**: Submit functions with parameters using futures
- **Thread Safety**: Proper synchronization with mutexes and condition variables
- **Graceful Shutdown**: Safe cleanup of worker threads
- **Statistics**: Track task execution and performance metrics
- **RAII**: Automatic cleanup in destructors

## Usage

```cpp
#include "thread_pool.h"

// Create a pool with 4 threads
ThreadPool pool(4);

// Submit a task and get a future
auto future = pool.submit([](int x) { return x * x; }, 5);

// Wait for result
int result = future.get(); // Will be 25
```

## Compile and Run

```bash
# Basic thread pool
g++ -std=c++14 -pthread -o test_thread_pool test_thread_pool.cpp
./test_thread_pool

# Enhanced thread pool
g++ -std=c++14 -pthread -o test_enhanced_thread_pool test_enhanced_thread_pool.cpp
./test_enhanced_thread_pool
```

## Educational Value

This implementation demonstrates:
- Thread management and synchronization
- Template programming with variadic templates
- Futures and promises for task results
- RAII principles
- Condition variables and mutex usage
- Priority queues for task scheduling
- Performance monitoring techniques