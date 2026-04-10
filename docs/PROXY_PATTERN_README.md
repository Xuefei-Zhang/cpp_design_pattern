# Proxy Pattern in C++

This project demonstrates the Proxy design pattern in C++ using a graphics/shape system context for consistency with other patterns.

## Overview

The Proxy pattern is a structural design pattern that provides a surrogate or placeholder for another object to control access to it. It allows an object to control access to another object without the client realizing it's dealing with a proxy rather than the real object.

## Key Concepts

### When to use Proxy pattern:
- When you need to provide a surrogate or placeholder for another object to control access to it
- When you want to add additional functionality to an existing object without changing its structure
- When you need to implement lazy initialization of expensive objects (Virtual Proxy)
- When you want to provide remote access to an object (Remote Proxy)
- When you need to control object access (Protection Proxy)
- When you want to cache expensive operations (Caching Proxy)
- When you need to add logging/monitoring capabilities (Logging Proxy)

### Types of Proxies:
- **Virtual Proxy**: Delays the creation of expensive objects until needed
- **Remote Proxy**: Represents an object in a different address space
- **Protection Proxy**: Controls access rights to an object
- **Caching Proxy**: Caches expensive operations 
- **Logging Proxy**: Logs operations on the target object
- **Smart Proxy**: Adds reference counting or other management features

### Components of Proxy:
- **Subject**: Defines the common interface for RealSubject and Proxy
- **RealSubject**: Defines the real object that the proxy represents
- **Proxy**: Maintains a reference to the RealSubject and implements the Subject interface
- **Client**: Manipulates the Proxy and RealSubject through the same interface

## Implementation Variations

### 1. Basic Proxy Implementation
- Simple proxy that adds functionality to the real subject
- Maintains the same interface as the real subject
- Basic forwarding of operations

### 2. Virtual Proxy (Lazy Initialization)
- Delays expensive object creation until needed
- Creates the real object only when first accessed
- Saves resources when objects might not be used

### 3. Remote Proxy
- Simulates access to remote objects
- Handles network communication transparently
- Adds network delay simulation

### 4. Protection Proxy
- Controls access based on permission levels
- Validates client permissions before operations
- Simulates security checks

### 5. Caching Proxy
- Caches expensive operations
- Improves performance by avoiding recomputation
- Stores results of costly operations

### 6. Logging Proxy
- Adds transparent logging to operations
- Tracks all access to the real subject
- Provides audit trails

## Files

- `proxy_shapes.h` - All Proxy pattern implementations
- `test_proxy_pattern.cpp` - Comprehensive test program
- `PROXY_PATTERN_README.md` - This documentation

## Key Examples

### 1. Basic Proxy Implementation
```cpp
// Subject interface
class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw() const = 0;
    virtual std::string getType() const = 0;
    virtual double getArea() const = 0;
    virtual void printInfo() const = 0;
};

// Proxy implementation
class ShapeProxy : public Shape {
protected:
    std::unique_ptr<Shape> realShape_;
    std::string proxyType_;

public:
    explicit ShapeProxy(std::unique_ptr<Shape> shape, const std::string& type)
        : realShape_(std::move(shape)), proxyType_(type) {}
    
    virtual void draw() const override {
        std::cout << "[" << proxyType_ << "] Proxying draw operation\n";
        realShape_->draw();
    }
    
    virtual std::string getType() const override {
        return realShape_->getType();
    }
    
    virtual double getArea() const override {
        return realShape_->getArea();
    }
    
    virtual void printInfo() const override {
        realShape_->printInfo();
    }
};
```

### 2. Using Different Proxy Types
```cpp
// Create real shape
auto circle = std::make_unique<Circle>(5.0, "red");

// Use different proxy types
auto loggingCircle = std::make_unique<LoggingShapeProxy>(std::move(circle));
auto cachingCircle = std::make_unique<CachingShapeProxy>(std::make_unique<Rectangle>(8.0, 6.0, "blue"));
auto protectedCircle = std::make_unique<ProtectedShapeProxy>(
    std::make_unique<Circle>(3.0, "purple"), 
    "admin", "admin"  // Requires admin permission, user has admin role
);

// All work with the same interface
loggingCircle->draw();
cachingCircle->draw();
protectedCircle->draw();
```

### 3. Proxy Chaining
```cpp
// Chain proxies together to combine behaviors
auto chainedProxy = std::make_unique<LoggingShapeProxy>(
    std::make_unique<CachingShapeProxy>(
        std::make_unique<ProtectedShapeProxy>(
            std::make_unique<Circle>(4.0, "chained"), 
            "user", "user"  // Permission granted
        )
    )
);

// The chain provides logging + caching + protection
chainedProxy->draw();
double area = chainedProxy->getArea();
```

## Compile and Run

```bash
g++ -std=c++14 -o test_proxy_pattern test_proxy_pattern.cpp
./test_proxy_pattern
```

## Educational Value

This implementation demonstrates:
- How to maintain the same interface in proxy classes as their real counterparts
- Different types of proxies and their specific use cases
- How to chain multiple proxies together
- Proper resource management with smart pointers
- Runtime flexibility to switch between different proxy configurations
- Real-world scenarios where proxies are useful
- Performance implications of proxy patterns
- The difference between Proxy and other structural patterns

## Proxy vs Other Patterns

- **Decorator**: Adds functionality, Proxy controls access (though they have similar structure)
- **Adapter**: Changes interface, Proxy maintains the same interface
- **Facade**: Provides simple interface to complex system, Proxy provides control over another object

## Important Notes

- Proxy and RealSubject should share the same interface
- Proxy can add functionality without changing the RealSubject
- Proxy should be transparent to clients
- Be careful with proxy chains which can add performance overhead
- Memory management is important when using proxies with smart pointers

## Benefits

- **Controlled Access**: Provides various mechanisms to control access to the target object
- **Lazy Initialization**: Virtual proxies can defer expensive object creation
- **Remote Access**: Transparent access to remote objects
- **Security**: Protection proxies can add security layers
- **Performance**: Caching proxies can improve performance
- **Monitoring**: Logging proxies can provide detailed operation logs
- **Flexibility**: Easy to add additional functionality without modifying original objects
- **Transparency**: Clients work with interfaces without knowing if they're using a proxy
- **Resource Management**: Optimizes resource usage through virtual proxies