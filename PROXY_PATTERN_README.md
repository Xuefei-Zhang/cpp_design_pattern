# Proxy Pattern in C++

This project demonstrates the Proxy design pattern in C++ with multiple implementations and real-world scenarios using the shape context for consistency with other patterns.

## Overview

The Proxy pattern is a structural design pattern that provides a surrogate or placeholder for another object to control access to it. It's useful for adding extra functionality like caching, logging, access control, or lazy initialization without changing the original object.

## Key Concepts

### When to use Proxy pattern:
- When you need to control access to an object
- When you want to add additional functionality without changing the original object
- When you need to implement lazy initialization of expensive objects
- When you need to provide a local representative for an object in a different address space
- When you want to add security layers or access control
- When you need to implement caching mechanisms

### Types of Proxies:
- **Remote Proxy**: Represents an object in a different address space
- **Virtual Proxy**: Provides lazy initialization of expensive objects
- **Protection Proxy**: Controls access rights to an object
- **Caching Proxy**: Caches expensive operations
- **Logging Proxy**: Adds transparent logging functionality
- **Smart Proxy**: Adds reference counting or other management capabilities

## Implementation Types

### 1. Remote Proxy
- Simulates access to a remote object
- Handles network communication
- Provides transparent access to remote services

### 2. Logging Proxy
- Adds logging functionality to method calls
- Transparent to the client code
- Records operations without changing object behavior

### 3. Caching Proxy
- Caches expensive operations
- Improves performance by avoiding repeated calculations
- Transparently handles caching logic

### 4. Protection Proxy
- Controls access based on permissions
- Validates client credentials
- Restricts operations based on authorization level

## Files

- `proxy_shapes.h` - All Proxy pattern implementations
- `test_proxy_pattern.cpp` - Comprehensive test program demonstrating various proxy types
- `PROXY_PATTERN_README.md` - This documentation

## Examples

### 1. Basic Proxy Implementation
```cpp
// Base class for all shapes
class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw() const = 0;
    virtual std::string getType() const = 0;
    virtual double getArea() const = 0;
};

// Proxy implementation
class LoggingShapeProxy : public Shape {
private:
    std::unique_ptr<Shape> realShape_;

public:
    explicit LoggingShapeProxy(std::unique_ptr<Shape> shape) 
        : realShape_(std::move(shape)) {}

    void draw() const override {
        std::cout << "[LOG] Calling draw() on " << realShape_->getType() << std::endl;
        realShape_->draw();
        std::cout << "[LOG] Completed draw() on " << realShape_->getType() << std::endl;
    }

    std::string getType() const override {
        std::cout << "[LOG] Calling getType()" << std::endl;
        return realShape_->getType();
    }

    double getArea() const override {
        std::cout << "[LOG] Calling getArea()" << std::endl;
        auto area = realShape_->getArea();
        std::cout << "[LOG] Area of " << realShape_->getType() << " is " << area << std::endl;
        return area;
    }
};
```

### 2. Using Proxies
```cpp
// Create a circle
auto circle = std::make_unique<Circle>(5.0, "red");

// Wrap it in a logging proxy
auto loggedCircle = std::make_unique<LoggingShapeProxy>(std::move(circle));

// Use as normal - logging happens transparently
loggedCircle->draw();  // This will print log messages
loggedCircle->getArea();  // This will print log messages
```

### 3. Chaining Proxies
```cpp
// Create a chain of proxies
auto shape = std::make_unique<LoggingShapeProxy>(
    std::make_unique<CachingShapeProxy>(
        std::make_unique<ProtectedShapeProxy>(
            std::make_unique<Circle>(3.0, "blue"),
            "admin"
        )
    )
);
```

## Compile and Run

```bash
g++ -std=c++14 -o test_proxy_pattern test_proxy_pattern.cpp
./test_proxy_pattern
```

## Educational Value

This implementation demonstrates:
- How to implement different types of proxies
- Proper use of inheritance and composition
- Transparent interface to clients
- How to enhance functionality without modifying original objects
- Performance improvements through caching
- Security and access control layers
- Real-world scenarios where proxies are useful
- Proper memory management with smart pointers
- Proxy chaining and composition

## Proxy vs Other Patterns

- **Decorator**: Adds functionality, Proxy controls access
- **Adapter**: Changes interface, Proxy maintains the same interface
- **Facade**: Simplifies a complex interface, Proxy represents an object

## Important Notes

- Proxies should maintain the same interface as the target object
- Proxy implementations should be transparent to clients
- Memory management is crucial when using proxy patterns with smart pointers
- Consider performance implications of adding proxy layers
- Proxies can be chained together for combined functionality

## Benefits

- **Controlled Access**: Provides fine-grained control over object access
- **Lazy Initialization**: Virtual proxies can defer expensive initialization
- **Remote Access**: Transparent access to remote objects
- **Security**: Protection proxies can enforce access control
- **Performance**: Caching proxies can improve performance for expensive operations
- **Logging**: Transparent logging without modifying the target object
- **Transparency**: Clients don't need to know whether they're using the proxy or real object
- **Flexibility**: Different proxy types can be combined as needed