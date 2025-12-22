# Prototype Pattern in C++

This project demonstrates the Prototype design pattern in C++ using the shape context for consistency with other patterns.

## Overview

The Prototype pattern is a creational design pattern that allows creating new objects by copying existing objects (prototypes) instead of creating them from scratch. This pattern is particularly useful when object creation is expensive or complex.

## Key Concepts

### When to use Prototype pattern:
- When the cost of creating a new object is expensive
- When you need to avoid subclasses of an object creator in the common framework
- When instances of a class can have one of only a few different combinations of state
- When you want to implement dynamic loading of classes

### Components of Prototype:
- **Prototype**: Declares an interface for cloning itself (ShapePrototype)
- **ConcretePrototype**: Implements the clone operation (CirclePrototype, RectanglePrototype, etc.)
- **Client**: Creates new objects by asking a prototype to clone itself
- **Registry**: Centralized repository for prototype objects

## Implementation Variations

### 1. Basic Prototype
- Abstract interface with virtual clone method
- Concrete implementations with proper deep copying
- Standard copy properties functionality

### 2. Prototype Registry
- Centralized repository for prototype objects
- Runtime registration and retrieval of prototypes
- Efficient object creation from stored templates

### 3. Factory Integration
- Shape factory using prototype registry
- Centralized object creation system
- Easy integration with existing systems

### 4. Complex Prototypes
- Composite patterns with prototypes
- Deep cloning of complex object hierarchies
- Configurable prototypes with additional properties

## Files

- `prototype_shapes.h` - All Prototype pattern implementations
- `test_prototype_pattern.cpp` - Comprehensive test program
- `PROTOTYPE_PATTERN_README.md` - This documentation

## Key Examples

### 1. Basic Prototype Usage
```cpp
class ShapePrototype {
public:
    virtual ~ShapePrototype() = default;
    virtual std::unique_ptr<ShapePrototype> clone() const = 0;
    virtual void draw() const = 0;
    // ... other methods
};

class CirclePrototype : public ShapePrototype {
public:
    std::unique_ptr<ShapePrototype> clone() const override {
        return std::make_unique<CirclePrototype>(*this);  // Uses copy constructor
    }
    // ... other methods
};
```

### 2. Registry Usage
```cpp
PrototypeRegistry registry;
registry.registerPrototype("basic_circle", std::make_unique<CirclePrototype>(5.0, "red"));

// Create new objects by cloning the registered prototype
auto newCircle = registry.create("basic_circle");
```

### 3. Factory with Prototypes
```cpp
ShapeFactory::registerPrototype("circle", std::make_unique<CirclePrototype>(3.0, "blue"));
auto circle = ShapeFactory::createShape("circle");  // Creates from registered prototype
```

## Compile and Run

```bash
g++ -std=c++14 -o test_prototype_pattern test_prototype_pattern.cpp
./test_prototype_pattern
```

## Educational Value

This implementation demonstrates:
- Proper implementation of the clone() method with deep copying
- Virtual copy constructor pattern in C++
- Prototype registry for centralized object management
- Integration with Factory patterns
- Complex object cloning (composite prototypes)
- Configurable prototypes with additional properties
- Performance considerations of prototype vs direct creation

## Prototype vs Other Patterns

- **Factory Method**: Creates objects from scratch, Prototype clones existing objects
- **Abstract Factory**: Creates families of related objects, Prototype clones individual objects
- **Builder**: Constructs complex objects step-by-step, Prototype copies existing objects

## Important Notes

- Proper deep copying is essential to avoid shared references
- The clone method should return the correct type while maintaining polymorphism
- Complex objects require careful implementation of deep copy logic
- Registry pattern can be used for runtime configuration
- Performance varies depending on object complexity

## Benefits

- **Performance**: Often faster than creating complex objects from scratch
- **Flexibility**: Runtime addition of new object types
- **Simplicity**: Reduces the need for subclassing
- **Configurability**: Easy to create slight variations of existing objects
- **Decoupling**: Client code doesn't need to know concrete classes