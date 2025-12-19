# Singleton Pattern in C++

This project demonstrates the Singleton design pattern in C++ with multiple implementation variations to show different approaches and considerations.

## Overview

The Singleton pattern is a creational design pattern that ensures a class has only one instance and provides a global point of access to that instance. This implementation includes various approaches to handle different scenarios including thread safety.

## Key Concepts

### When to use Singleton pattern:
- When you need exactly one instance of a class
- When you need controlled access to that instance
- When you want to avoid global variables
- For resource management (logging, configuration, caching, thread pools, etc.)

### Important Considerations:
- Can create tight coupling
- Can make unit testing difficult
- Can hide bad design (global state)
- Should be thread-safe in multi-threaded environments

## Implementation Variations

### 1. Basic Singleton
- Simple implementation without thread safety
- Uses static pointer to hold the instance
- Lazy initialization on first access

### 2. Thread-Safe Singleton (Meyers Singleton)
- Uses C++11's thread-safe static initialization
- Most efficient for modern C++ (C++11 and later)
- Automatic cleanup

### 3. Double-Checked Locking Singleton
- For older C++ standards or when Meyers isn't suitable
- Uses mutex for thread safety
- Checks instance twice to avoid mutex overhead on every access

### 4. Lazy-Initialized Singleton
- Uses std::call_once for guaranteed one-time initialization
- Thread-safe and efficient
- Good for older C++ standards

### 5. Template-Based Singleton
- Generic approach using templates
- Can be inherited by other classes
- Provides a base for creating singletons

### 6. Destroyable Singleton
- Allows for explicit cleanup
- Useful when you need to control the lifecycle
- Includes destruction method

### 7. Configuration Manager Example
- Practical example of Singleton usage
- Demonstrates real-world application
- Extends the GenericSingleton base

## Files

- `singleton_patterns.h` - All Singleton implementations
- `test_singleton_pattern.cpp` - Comprehensive test program
- `SINGLETON_PATTERN_README.md` - This documentation

## Key Examples

### 1. Using Basic Singleton
```cpp
auto* instance1 = BasicSingleton::getInstance();
auto* instance2 = BasicSingleton::getInstance();
// instance1 and instance2 are the same object
```

### 2. Using Thread-Safe Singleton (Recommended for modern C++)
```cpp
auto& instance = ThreadSafeSingleton::getInstance();
// Thread-safe, efficient, and follows RAII
```

### 3. Using Template-Based Singleton
```cpp
class Logger : public GenericSingleton<Logger> {
    // Custom methods
};
auto& logger = Logger::getInstance();
```

## Compile and Run

```bash
g++ -std=c++14 -pthread -o test_singleton_pattern test_singleton_pattern.cpp
./test_singleton_pattern
```

## Educational Value

This implementation demonstrates:
- Multiple approaches to Singleton implementation
- Thread safety considerations in Singleton pattern
- Proper use of C++ features (mutex, static variables, etc.)
- When different approaches are appropriate
- How to make singletons inheritable using templates
- Real-world examples of Singleton usage
- Performance implications of different approaches

## Patterns Compared

- **Basic Singleton**: Simple but not thread-safe
- **Meyers Singleton**: Thread-safe, efficient, modern approach
- **Double-Checked Locking**: Thread-safe for older C++ standards
- **Template Singleton**: Reusable approach for multiple singleton classes

## Important Notes

1. **Thread Safety**: In C++11 and later, the Meyers Singleton is the preferred approach due to built-in thread safety for static variable initialization.

2. **Memory Management**: Some Singleton implementations may cause memory leaks if not properly managed. The Meyers approach uses static storage duration which is automatically cleaned up.

3. **Testing**: Singletons can make unit testing difficult. Consider dependency injection as an alternative.

4. **Performance**: The Meyers approach is generally the most efficient as it has minimal overhead after initialization.

5. **Alternatives**: Often dependency injection or other patterns provide better design than Singletons.