# Decorator Pattern in C++

This project demonstrates the Decorator design pattern in C++ using the shape enhancement context for consistency with other patterns.

## Overview

The Decorator pattern is a structural design pattern that allows adding behaviors to objects dynamically by placing them inside wrapping objects. It provides a flexible alternative to subclassing for extending functionality.

## Key Concepts

### When to use Decorator pattern:
- When you want to add responsibilities to objects without breaking encapsulation
- When subclassing would result in a proliferation of classes
- When you need to add or remove responsibilities from an object at runtime
- When you want to combine several "enhancements" to an object
- When you need different and independent "enhancements" that can be applied in any order

### Components of Decorator:
- **Component**: Defines the interface for objects that can have responsibilities added to them (Shape interface)
- **ConcreteComponent**: Defines an object to which additional responsibilities can be attached (Circle, Rectangle, Triangle)
- **Decorator**: Maintains a reference to a Component object and defines an interface that conforms to Component's interface (ShapeDecorator)
- **ConcreteDecorator**: Adds responsibilities to the component (BorderDecorator, FillDecorator, etc.)

## Implementation Variations

### 1. Basic Decorator
- Simple decorator that wraps and extends a component
- Maintains the same interface as the component
- Can be chained with other decorators

### 2. Multiple Concrete Decorators
- Different decorators for different functionalities (Border, Fill, Shadow, Gradient, etc.)
- Each enhances the behavior in a specific way
- Can be combined in any order

### 3. Fluent Decorator
- Provides a fluent interface for chaining decorations
- Allows for readable, method-chaining syntax
- Provides a clean API for applying multiple decorators

### 4. Compound Decorator
- Applies multiple effects in a single decorator
- More efficient for combining related effects
- Cleaner output for complex styling

## Files

- `decorator_patterns.h` - All Decorator pattern implementations
- `test_decorator_pattern.cpp` - Comprehensive test program
- `DECORATOR_PATTERN_README.md` - This documentation

## Key Examples

### 1. Basic Decorator Implementation
```cpp
// Decorator base class
class ShapeDecorator : public Shape {
protected:
    std::unique_ptr<Shape> shape_;

public:
    explicit ShapeDecorator(std::unique_ptr<Shape> shape) : shape_(std::move(shape)) {}

    void draw() const override {
        shape_->draw();  // Call wrapped component
    }
    
    // Delegate other methods to the wrapped component
    std::string getType() const override {
        return shape_->getType();
    }
    
    double getArea() const override {
        return shape_->getArea();
    }
    
    std::string getDescription() const override {
        return shape_->getDescription();
    }
};

// Concrete decorator
class BorderDecorator : public ShapeDecorator {
private:
    int borderWidth_;
    std::string borderColor_;

public:
    BorderDecorator(std::unique_ptr<Shape> shape, int borderWidth = 2, const std::string& borderColor = "black")
        : ShapeDecorator(std::move(shape)), borderWidth_(borderWidth), borderColor_(borderColor) {}
    
    void draw() const override {
        std::cout << "Applying " << borderColor_ << " border (width: " << borderWidth_ << ") to ";
        shape_->draw();  // Add behavior and delegate to the wrapped object
    }
    
    std::string getDescription() const override {
        return "Border(" + borderColor_ + " width=" + std::to_string(borderWidth_) + ") around " + 
               shape_->getDescription();
    }
};
```

### 2. Using Decorators
```cpp
// Create a basic circle
auto circle = std::make_unique<Circle>(3.0, "red");

// Add a border to the circle
auto borderedCircle = std::make_unique<BorderDecorator>(std::move(circle), 2, "black");

// Add fill to the bordered circle (decorator chaining)
auto filledBorderedCircle = std::make_unique<FillDecorator>(std::move(borderedCircle), "blue", true);

// All behave like a Shape but with enhanced functionality
filledBorderedCircle->draw();  // Will draw with border and fill
```

### 3. Fluent Decorator Usage
```cpp
// Using the fluent interface for clean syntax
auto decoratedCircle = FluentDecorator(std::make_unique<Circle>(2.0, "yellow"))
                            .border(2, "red")
                            .fill("orange", true)
                            .shadow(3, "lightgray")
                            .scale(1.5, 1.5)
                            .build();
decoratedCircle->draw();
```

## Compile and Run

```bash
g++ -std=c++14 -o test_decorator_pattern test_decorator_pattern.cpp
./test_decorator_pattern
```

## Educational Value

This implementation demonstrates:
- How to maintain the same interface while adding functionality
- Proper use of composition over inheritance
- How to create a hierarchy of decorators
- Fluent interface design for clean API usage
- Performance considerations with deep decorator chains
- Real-world scenarios where decorators are useful
- Dynamic composition of behaviors at runtime

## Decorator vs Other Patterns

- **Adapter**: Changes interface, Decorator maintains the same interface but extends functionality
- **Composite**: Forms tree structures, Decorator adds responsibilities to an object
- **Strategy**: Encapsulates interchangeable behaviors, Decorator adds responsibilities to an object
- **Bridge**: Separates abstraction from implementation, Decorator adds functionality dynamically

## Important Notes

- Decorators must maintain the same interface as the component
- Be careful about deep decorator chains which can impact performance
- Memory management is important when using smart pointers with decorators
- The order of decorators can matter for certain operations
- Decorators are transparent to the client code

## Benefits

- **Flexibility**: Can add/remove responsibilities at runtime
- **Composition**: Combine behaviors without complex inheritance hierarchies
- **Transparency**: Decorated objects are indistinguishable from original objects
- **Open/Closed Principle**: Open for extension of functionality, closed for modification
- **Single Responsibility**: Each decorator focuses on a single enhancement
- **Reusability**: Same decorators can be applied to different component types
- **Method Chaining**: Clean, readable code when using fluent decorators