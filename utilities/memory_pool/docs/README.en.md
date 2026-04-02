# Memory Pool Implementation in C++

This project contains two implementations of a memory pool in C++ for educational purposes:

1. Basic Memory Pool (`memory_pool.h`)
2. Enhanced Memory Pool (`enhanced_memory_pool.h`)

## Overview

A memory pool is a memory allocation technique that allocates a large block of memory at once and then manages allocation and deallocation within that block. This approach:

- Reduces dynamic memory allocation overhead
- Improves cache locality
- Provides faster allocation/deallocation
- Helps prevent memory fragmentation

## Components

### Basic Memory Pool
- Fixed-size block allocation
- Thread-safe operations using mutex
- Simple linked-list free block management
- Basic statistics tracking

### Enhanced Memory Pool
- All features of the basic pool plus:
- Lock-free operations using atomic compare-and-swap
- Bulk allocation/deallocation for better performance
- Integrity validation
- Pool reset functionality
- More robust thread safety

## Files

- `memory_pool.h` - Basic memory pool implementation
- `test_memory_pool.cpp` - Test program for basic pool
- `enhanced_memory_pool.h` - Enhanced memory pool implementation  
- `test_enhanced_memory_pool.cpp` - Test program for enhanced pool
- `MEMORY_POOL_README.md` - This documentation

## Key Features

- **Fixed Size Allocation**: All blocks in the pool are the same size
- **Alignment**: Proper memory alignment for different data types
- **Threading Support**: Thread-safe operations
- **Statistics**: Track allocated and free blocks
- **RAII**: Automatic cleanup in destructors

## Usage

```cpp
#include "memory_pool.h"

// Create a pool for objects of size 64 with 100 blocks
MemoryPool<64> pool(100);

// Allocate memory
void* ptr = pool.allocate();

// Use the memory
// ...

// Return memory to pool
pool.deallocate(ptr);
```

## Compile and Run

```bash
# Basic memory pool
g++ -std=c++14 -pthread -o test_memory_pool test_memory_pool.cpp
./test_memory_pool

# Enhanced memory pool
g++ -std=c++14 -pthread -o test_enhanced_memory_pool test_enhanced_memory_pool.cpp
./test_enhanced_memory_pool
```

## Educational Value

This implementation demonstrates:
- Template programming
- Memory management techniques
- Thread safety concepts
- RAII principles
- Atomic operations
- Performance optimization strategies