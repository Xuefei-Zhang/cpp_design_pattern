# Abstract Factory Pattern in C++

This project demonstrates the Abstract Factory design pattern in C++ using the shape and rendering context for consistency with other patterns.

## Overview

The Abstract Factory pattern is a creational design pattern that provides an interface for creating families of related or dependent objects without specifying their concrete classes. It's a factory of factories, where each factory in the family creates objects of a specific "theme" or "variant".

## Key Concepts

### When to use Abstract Factory pattern:
- When you need to create families of related objects
- When you want to ensure that objects are compatible with each other
- When you want to make your code independent of how objects are created and represented
- When you need to provide multiple "themes" or "variants" of objects
- When you want to switch between different product families at runtime

### Components of Abstract Factory:
- **AbstractFactory**: Declares an interface for operations that create abstract products
- **ConcreteFactory**: Implements the operations to create concrete products 
- **AbstractProduct**: Declares an interface for a type of product object
- **ConcreteProduct**: Defines a product object to be created by the corresponding concrete factory
- **Client**: Uses only interfaces declared by AbstractFactory and AbstractProduct

## Implementation Variations

### 1. Basic Abstract Factory
- Abstract factory interface defining product creation methods
- Multiple concrete factories implementing the interface
- Consistent product family creation

### 2. Themed Factories (Basic, Advanced, Modern, Vintage)
- Each factory creates a complete set of related products
- Different styles/treatments for the same object types
- Consistent look and feel within each theme

### 3. Factory Selector/Manager
- Runtime factory selection mechanism
- Centralized factory management
- Easy factory switching

### 4. Client Integration
- Client code that uses the abstract factory
- Demonstrates factory independence
- Shows runtime factory switching

## Files

- `abstract_factory_shapes.h` - All Abstract Factory pattern implementations
- `test_abstract_factory.cpp` - Comprehensive test program
- `ABSTRACT_FACTORY_README.md` - This documentation

## Key Examples

### 1. Basic Abstract Factory Implementation
```cpp
// Abstract Factory Interface
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

// Concrete Factory Implementation
class BasicThemeFactory : public ShapeAbstractFactory {
public:
    std::unique_ptr<Shape> createCircle(double radius, const std::string& color = "red") const override {
        return std::make_unique<BasicCircle>(radius, color);
    }

    std::unique_ptr<Shape> createRectangle(double width, double height, const std::string& color = "blue") const override {
        return std::make_unique<BasicRectangle>(width, height, color);
    }

    std::unique_ptr<ShapeRenderer> createRenderer(const std::string& quality = "standard") const override {
        return std::make_unique<BasicShapeRenderer>(quality);
    }

    std::unique_ptr<ShapeAnimator> createAnimator(double speed = 1.0) const override {
        return std::make_unique<BasicShapeAnimator>(speed);
    }
};
```

### 2. Using the Abstract Factory
```cpp
// Create a factory
auto factory = std::make_unique<BasicThemeFactory>();

// Create a complete shape system using the factory
auto circle = factory->createCircle(5.0, "red");
auto renderer = factory->createRenderer("high");
auto animator = factory->createAnimator(1.5);

renderer->render(*circle);
animator->animate(*circle);
```

### 3. Factory Selector Usage
```cpp
// Select factory at runtime
auto factory = AbstractFactorySelector::getFactory("modern");
auto [shape, renderer, animator] = factory->createShapeSystem(6.0, "circle", "themed_color");

renderer->render(*shape);
animator->animate(*shape);
```

## Compile and Run

```bash
g++ -std=c++14 -o test_abstract_factory test_abstract_factory.cpp
./test_abstract_factory
```

## Educational Value

This implementation demonstrates:
- How to create families of related objects without specifying their concrete classes
- The difference between Abstract Factory and Factory Method patterns
- How to ensure compatibility between objects in the same family
- Runtime selection of object families
- Proper use of polymorphism with abstract factories
- How different "themes" (Basic, Advanced, Modern, Vintage) represent different families of objects
- The Open/Closed Principle (new themes can be added without modifying existing code)

## Abstract Factory vs Other Patterns

- **Factory Method**: Creates single objects, Abstract Factory creates families of related objects
- **Simple Factory**: Creates objects with if/else logic, Abstract Factory provides an interface
- **Builder**: More complex construction process, Abstract Factory creates families of objects

## Important Notes

- Products created by the same factory are guaranteed to be compatible
- Abstract Factory enforces consistency among products
- Adding new product families (themes) is easy, but adding new product types requires changing all factories
- Memory management is important when using factory patterns with smart pointers
- The pattern is useful when you need to support pluggable UI toolkits or themes

## Benefits

- **Isolation of concrete classes**: Client code only interacts with the abstract interfaces
- **Easy product family switching**: The whole family of products can be swapped at once
- **Consistency**: Forces creation of compatible objects
- **Platform independence**: Can create platform-specific object families
- **Open/Closed Principle**: Open for extension (new themes), closed for modification

## Potential Drawbacks

- The pattern is complex and harder to implement than simpler factory patterns
- Adding new product types requires changes to all concrete factories
- Can lead to a large number of small classes