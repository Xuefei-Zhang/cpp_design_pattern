# Abstract Factory Pattern in C++

This project demonstrates the Abstract Factory design pattern in C++ using the shape/rendering context for consistency with other patterns.

## Overview

The Abstract Factory pattern is a creational design pattern that provides an interface for creating families of related or dependent objects without specifying their concrete classes. It's like a Factory of Factories.

## Key Concepts

### When to use Abstract Factory pattern:
- When you want to create families of related objects
- When you want to ensure compatibility between objects in a family
- When you need to switch between product families at runtime
- When you want to provide a class library of products and reveal only their interfaces, not their implementations

### Components of Abstract Factory:
- **AbstractFactory**: Declares an interface for operations that create abstract products (ShapeAbstractFactory)
- **ConcreteFactory**: Implements the operations to create concrete products (BasicThemeFactory, AdvancedThemeFactory, etc.)
- **AbstractProduct**: Declares an interface for a type of product object (Shape, ShapeRenderer, ShapeAnimator)
- **ConcreteProduct**: Defines a product object to be created by the corresponding concrete factory (BasicCircle, AdvancedShapeRenderer, etc.)
- **Client**: Uses only interfaces declared by AbstractFactory and AbstractProduct

## Implementation Variations

### 1. Basic Abstract Factory
- Abstract factory interface with multiple product creation methods
- Concrete factories implement all product creation methods
- Simple and straightforward implementation

### 2. Themed Factories
- Factories with consistent styling across all products
- Each theme provides a cohesive look and feel
- Shows how related products maintain consistency

### 3. Product Families
- Multiple product types that work together
- Each factory creates compatible products
- Ensures consistency within a family

### 4. Factory Selector
- Runtime factory selection mechanism
- Centralized factory management
- Easy switching between factories

## Files

- `abstract_factory_shapes.h` - All Abstract Factory implementations
- `test_abstract_factory.cpp` - Comprehensive test program
- `ABSTRACT_FACTORY_README.md` - This documentation

## Key Examples

### 1. Abstract Factory Interface
```cpp
class ShapeAbstractFactory {
public:
    virtual ~ShapeAbstractFactory() = default;
    
    // Product A Creation Methods
    virtual std::unique_ptr<Shape> createCircle(double radius, const std::string& color = "red") const = 0;
    virtual std::unique_ptr<Shape> createRectangle(double width, double height, const std::string& color = "blue") const = 0;
    virtual std::unique_ptr<Shape> createTriangle(double base, double height, const std::string& color = "green") const = 0;
    
    // Product B Creation Methods
    virtual std::unique_ptr<ShapeRenderer> createRenderer(const std::string& quality = "standard") const = 0;
    
    // Product C Creation Methods
    virtual std::unique_ptr<ShapeAnimator> createAnimator(double speed = 1.0) const = 0;
};
```

### 2. Concrete Factory Implementation
```cpp
class BasicThemeFactory : public ShapeAbstractFactory {
public:
    std::unique_ptr<Shape> createCircle(double radius, const std::string& color) const override {
        return std::make_unique<BasicCircle>(radius, color);
    }
    
    std::unique_ptr<ShapeRenderer> createRenderer(const std::string& quality) const override {
        return std::make_unique<BasicShapeRenderer>(quality);
    }
    
    std::unique_ptr<ShapeAnimator> createAnimator(double speed) const override {
        return std::make_unique<BasicShapeAnimator>(speed);
    }
};
```

### 3. Client Usage
```cpp
ShapeSystemClient client(std::make_unique<BasicThemeFactory>());
client.createAndDisplayShape("circle", 5.0, "red");

// Change factory at runtime
client.setFactory(std::make_unique<ModernThemeFactory>());
client.createAndDisplayShape("circle", 5.0, "blue");
```

## Compile and Run

```bash
g++ -std=c++14 -o test_abstract_factory test_abstract_factory.cpp
./test_abstract_factory
```

## Educational Value

This implementation demonstrates:
- How to create families of related objects using abstract factories
- Proper use of polymorphism for flexible product creation
- How to maintain consistency within product families
- Runtime factory selection and switching
- The difference between Abstract Factory and Factory Method
- Real-world applications of the pattern

## Abstract Factory vs Other Patterns

- **Factory Method**: Creates single type, Abstract Factory creates families of related types
- **Simple Factory**: More rigid, Abstract Factory allows for family creation
- **Builder**: More complex construction, Abstract Factory for family creation

## Important Notes

- Abstract Factory promotes consistency within product families
- It provides a way to switch entire product families at runtime
- The pattern can be complex to implement but provides great flexibility
- It follows the Dependency Inversion Principle
- New product families can be added without changing existing client code

## Benefits

- **Product Family Consistency**: Ensures related products are used together
- **Easy Switching**: Can easily switch between product families
- **Open/Closed Principle**: Open for new product families, closed for modification
- **Loose Coupling**: Client code is not tied to specific product classes
- **Dependency Inversion**: Clients depend on abstractions, not concretions