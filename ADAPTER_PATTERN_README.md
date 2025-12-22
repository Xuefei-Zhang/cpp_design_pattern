# Adapter Pattern in C++

This project demonstrates the Adapter design pattern in C++ using the shape context for consistency with other patterns.

## Overview

The Adapter pattern is a structural design pattern that allows objects with incompatible interfaces to collaborate. It converts the interface of a class into another interface that clients expect.

## Key Concepts

### When to use Adapter pattern:
- When you want to use an existing class but its interface doesn't match the one you need
- When you want to create a reusable class that cooperates with unrelated classes that don't have compatible interfaces
- When you need to work with legacy code or third-party libraries with different interfaces
- To integrate new functionality with existing code without modifying either

### Components of Adapter:
- **Target**: Defines the domain-specific interface that Client uses (Shape interface)
- **Client**: Collaborates with objects conforming to the Target interface
- **Adaptee**: Defines an existing interface that needs adapting (LegacyShape, ThirdPartyShape, etc.)
- **Adapter**: Adapts the Adaptee's interface to the Target interface

## Implementation Variations

### 1. Object Adapter
- Uses composition to adapt the Adaptee
- More flexible, can adapt classes and their subclasses
- Requires additional forwarding methods

### 2. Class Adapter
- Uses multiple inheritance to adapt the Adaptee
- Only adapts the Adaptee class (not its subclasses)
- Simpler to implement but less flexible

### 3. Multiple Adapters
- Shows how different incompatible systems can be adapted to a common interface
- Demonstrates real-world integration scenarios

## Files

- `adapter_patterns.h` - All Adapter pattern implementations
- `test_adapter_pattern.cpp` - Comprehensive test program
- `ADAPTER_PATTERN_README.md` - This documentation

## Key Examples

### 1. Object Adapter Implementation
```cpp
class LegacyShapeAdapter : public Shape {
private:
    std::unique_ptr<LegacyShape> legacyShape_;

public:
    explicit LegacyShapeAdapter(std::unique_ptr<LegacyShape> legacy) 
        : legacyShape_(std::move(legacy)) {}
    
    void draw() const override {
        legacyShape_->renderLegacy();  // Adapt the call
    }
    
    std::string getType() const override {
        return "Adapted " + legacyShape_->getName();
    }
    
    double getArea() const override {
        return legacyShape_->calculateLegacyArea();
    }
    
    void printInfo() const override {
        legacyShape_->displayLegacyInfo();
    }
};
```

### 2. Client Code Using Adapted Objects
```cpp
// Client code works with any class that implements Shape interface
std::vector<std::unique_ptr<Shape>> shapes;
shapes.push_back(std::make_unique<Circle>(5.0, "red"));  // Native
shapes.push_back(std::make_unique<LegacyShapeAdapter>(   // Adapted legacy
    std::make_unique<LegacyCircle>(4.0)
));
shapes.push_back(std::make_unique<ThirdPartyShapeAdapter>(  // Adapted third-party
    std::make_unique<ThirdPartyEllipse>(6.0, 4.0)
));

// Process all shapes the same way regardless of origin
for (const auto& shape : shapes) {
    shape->draw();  // Works for all types
    std::cout << "Area: " << shape->getArea() << std::endl;
}
```

## Compile and Run

```bash
g++ -std=c++14 -o test_adapter_pattern test_adapter_pattern.cpp
./test_adapter_pattern
```

## Educational Value

This implementation demonstrates:
- Object adapter pattern using composition
- How to adapt incompatible interfaces to work together
- Integration of legacy systems with modern code
- Integration of third-party libraries with existing code
- Real-world scenario of graphics system consolidation
- Performance implications of adaptation
- Different approaches to adaptation (object vs class adapter)

## Adapter vs Other Patterns

- **Decorator**: Adds responsibilities to objects without changing their interface
- **Bridge**: Separates abstraction from implementation, Adapter makes incompatible interfaces work together
- **Facade**: Provides simplified interface to complex system, Adapter changes interface to match what's needed

## Important Notes

- Object Adapter is more flexible than Class Adapter
- Adapters can introduce some performance overhead
- Adapters maintain encapsulation between systems
- Can be used to integrate legacy code without modification
- Supports the Open/Closed Principle (open for extension)

## Benefits

- **Interface Compatibility**: Allows incompatible interfaces to work together
- **Reusability**: Legacy and third-party code can be reused without modification
- **Flexibility**: Easy to integrate new systems with common interface
- **Maintainability**: Clean separation between different system interfaces
- **Extensibility**: New adapters can be added without changing existing code