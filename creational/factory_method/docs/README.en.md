# Factory Method Pattern in C++

This project demonstrates the Factory Method design pattern in C++ using the shape/rendering context for consistency with other patterns.

## Overview

The Factory Method pattern is a creational design pattern that provides an interface for creating objects in a superclass, but allows subclasses to alter the type of objects that will be created. It delegates the instantiation to subclasses.

## Key Concepts

### When to use Factory Method pattern:
- When a class can't anticipate the class of objects it must create
- When you want to delegate the responsibility of object creation to subclasses
- When you want to follow the Open/Closed Principle (open for extension, closed for modification)
- When you need to centralize object creation logic

### Components of Factory Method:
- **Product**: The object that gets created (Shape classes)
- **Creator**: Declares the factory method which returns a Product object (ShapeFactory)
- **ConcreteCreator**: Overrides the factory method to return an instance of a ConcreteProduct (CircleFactory, RectangleFactory, etc.)

## Implementation Variations

### 1. Basic Factory Method
- Abstract base factory with virtual factory method
- Concrete factories implement the factory method
- Simple and straightforward implementation

### 2. Factory Manager
- Uses composition to manage different factories
- Allows runtime factory switching
- Demonstrates dependency injection principles

### 3. Themed Factories
- Factories with default configurations
- Shows inheritance in factory pattern
- Different themes provide different defaults

### 4. Suite Factories
- Creates collections of related objects
- Advanced factory pattern usage
- Demonstrates how factories can create complex object structures

## Files

- `factory_method_shapes.h` - All Factory Method implementations
- `test_factory_method.cpp` - Comprehensive test program
- `FACTORY_METHOD_README.md` - This documentation

## Key Examples

### 1. Basic Factory Method Implementation
```cpp
class ShapeFactory {
public:
    virtual std::unique_ptr<Shape> createShape(double param1, double param2, 
                                               const std::string& color, 
                                               const std::string& style) const = 0;
    
    std::unique_ptr<Shape> makeShape(double param1, double param2, 
                                     const std::string& color, 
                                     const std::string& style) const {
        auto shape = createShape(param1, param2, color, style);
        return shape;
    }
};

class CircleFactory : public ShapeFactory {
public:
    std::unique_ptr<Shape> createShape(double param1, double param2, 
                                       const std::string& color, 
                                       const std::string& style) const override {
        return std::make_unique<Circle>(param1, color, style);
    }
};
```

### 2. Using Factory Manager
```cpp
ShapeFactoryManager manager(std::make_unique<CircleFactory>());
auto shape = manager.createShape(5.0, 0.0, "red", "solid");
```

### 3. Themed Factories
```cpp
class ModernShapeFactory : public ThemedShapeFactory {
public:
    ModernShapeFactory() : ThemedShapeFactory("white", "dashed") {}
    
    std::unique_ptr<Shape> createShape(double param1, double param2, 
                                       const std::string& color, 
                                       const std::string& style) const override {
        // Use default theme if not overridden
        std::string actualColor = (color == "default") ? getDefaultColor() : color;
        std::string actualStyle = (style == "solid") ? getDefaultStyle() : style;
        return std::make_unique<Circle>(param1, actualColor, actualStyle);
    }
};
```

## Compile and Run

```bash
g++ -std=c++14 -o test_factory_method test_factory_method.cpp
./test_factory_method
```

## Educational Value

This implementation demonstrates:
- How to implement the Factory Method pattern with virtual methods
- Proper use of polymorphism for flexible object creation
- How to structure code following the Open/Closed Principle
- Runtime factory selection using Factory Manager
- Advanced patterns like themed factories and suite creation
- The difference between Factory Method and direct construction
- Real-world applications of the pattern

## Factory Method vs Other Patterns

- **Simple Factory**: Factory Method uses inheritance, Simple Factory uses if/else
- **Abstract Factory**: Creates families of related objects, Factory Method creates single objects
- **Builder**: More complex construction, Factory Method for simple creation

## Important Notes

- Factory Method promotes loose coupling between classes
- It makes code more extensible (easy to add new product types)
- Subclasses can override the creation logic
- The pattern can be combined with other patterns for more complex systems
- Runtime selection of factories allows for dynamic behavior

## Benefits

- **Extensibility**: Easy to add new product types by creating new concrete factories
- **Maintainability**: Creation logic centralized in factory methods
- **Flexibility**: Runtime selection of object types
- **Testability**: Easy to mock factories for testing
- **Open/Closed Principle**: Closed for modification, open for extension