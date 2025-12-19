# Proxy Pattern Implementation in C++

This project demonstrates the Proxy design pattern in C++ using a Shape rendering context. The Proxy pattern provides a surrogate or placeholder for another object to control access to it.

## Overview

The Proxy pattern is used to create a representative object that controls access to another object. This implementation includes several types of proxies:

- **Virtual Proxy**: Delays the creation of expensive objects until needed (LazyShapeProxy)
- **Logging Proxy**: Adds logging functionality to operations (LoggingShapeProxy) 
- **Caching Proxy**: Caches expensive operations (CachingShapeProxy)
- **Protection Proxy**: Controls access based on permissions (ProtectedShapeProxy)
- **Remote Proxy**: Simulates remote object access (RemoteShapeProxy)

## Components

### Shape Interface and Concrete Implementations
- `Shape`: Abstract base class defining the interface
- `Circle`, `Rectangle`, `Triangle`: Concrete shape implementations
- Each shape has draw(), getType(), and getArea() methods

### Proxy Implementations
- `LazyShapeProxy`: Virtual proxy that creates the real object on demand
- `LoggingShapeProxy`: Adds logging before and after method calls
- `CachingShapeProxy`: Caches results of expensive operations
- `ProtectedShapeProxy`: Controls access based on authorization level
- `RemoteShapeProxy`: Simulates remote object access with network delays

## Files

- `shape_proxy.h` - Basic Shape interface and concrete implementations
- `proxy_shapes.h` - All Proxy implementations
- `test_proxy_pattern.cpp` - Test program demonstrating Proxy usage
- `PROXY_PATTERN_README.md` - This documentation

## Key Features

- **Virtual Proxy**: Lazy initialization of expensive objects
- **Logging Proxy**: Automatic logging of method calls
- **Caching Proxy**: Performance optimization through caching
- **Protection Proxy**: Access control enforcement
- **Remote Proxy**: Simulation of remote access
- **Chain of Proxies**: Multiple proxies can be stacked together

## Usage

```cpp
#include "proxy_shapes.h"

// Create a basic shape
auto circle = std::make_unique<Circle>(5.0, "red");

// Wrap with logging proxy
auto loggingCircle = std::make_unique<LoggingShapeProxy>(std::move(circle));

// Use the proxy - logging will happen automatically
loggingCircle->draw();  // This will log the operation
```

## Compile and Run

```bash
g++ -std=c++14 -o test_proxy_pattern test_proxy_pattern.cpp
./test_proxy_pattern
```

## Educational Value

This implementation demonstrates:
- Different types of Proxy patterns (Virtual, Logging, Caching, Protection, Remote)
- Template and function pointer usage for object creation
- RAII principles for resource management
- Inheritance and polymorphism
- Performance optimization techniques
- Access control mechanisms
- Design pattern composition (chaining multiple proxies)