# Abstract Factory Pattern in C++

This project demonstrates the Abstract Factory design pattern in C++ using a graphics shape system context for consistency with other patterns.

## Overview

The Abstract Factory pattern is a creational design pattern that provides an interface for creating families of related or dependent objects without specifying their concrete classes. It's essentially a factory of factories, where each concrete factory produces a family of related objects.

## Key Concepts

### When to use Abstract Factory pattern:
- When you need to create families of related objects
- When you want to ensure that objects are compatible with each other
- When you want to provide a library of products and reveal only their interfaces, not their implementations
- When the system needs to be independent of how products are created, composed, and represented
- When you need multiple families of products to choose from at runtime

### Components of Abstract Factory:
- **AbstractFactory**: Declares an interface for operations that create abstract products
- **ConcreteFactory**: Implements the operations to create concrete product objects
- **AbstractProduct**: Declares an interface for a type of product object  
- **ConcreteProduct**: Defines a product object to be created by the corresponding concrete factory
- **Client**: Uses only interfaces declared by AbstractFactory and AbstractProduct

## Implementation Variations

### 1. Basic Abstract Factory
- Abstract factory interface with multiple product creation methods
- Concrete factories implementing the full interface
- Consistent product family creation

### 2. Theme-Based Factories
- Different factories for different styles/themes (Modern, Vintage, Basic, Advanced)
- Each factory creates a complete set of themed products
- Ensures visual consistency within themes

### 3. Factory Selector/Manager
- Runtime factory selection system
- Centralized factory management
- Easy switching between product families

### 4. Complex Factory Systems
- Advanced factories that create product combinations
- Factory methods for complete object ecosystems
- Integration with other patterns

## Files

- `abstract_factory_shapes.h` - All Abstract Factory implementations
- `test_abstract_factory.cpp` - Comprehensive test program
- `ABSTRACT_FACTORY_README.md` - This documentation

## Key Examples

### 1. Basic Abstract Factory Implementation
```cpp
// Abstract Factory interface
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
// Client code works with abstract interfaces
std::vector<std::unique_ptr<ShapeAbstractFactory>> factories;
factories.push_back(std::make_unique<BasicThemeFactory>());
factories.push_back(std::make_unique<AdvancedThemeFactory>());
factories.push_back(std::make_unique<ModernThemeFactory>());
factories.push_back(std::make_unique<VintageThemeFactory>());

for (auto& factory : factories) {
    // All factories create compatible sets of shapes, renderers, and animators
    auto shape = factory->createCircle(5.0, "family_color");
    auto renderer = factory->createRenderer();
    auto animator = factory->createAnimator(1.5);
    
    renderer->render(*shape);
    animator->animate(*shape);
}
```

### 3. Factory Selector Usage
```cpp
class AbstractFactorySelector {
public:
    static std::unique_ptr<ShapeAbstractFactory> getFactory(const std::string& theme) {
        if (theme == "basic") {
            return std::make_unique<BasicThemeFactory>();
        } else if (theme == "advanced") {
            return std::make_unique<AdvancedThemeFactory>();
        } else if (theme == "modern") {
            return std::make_unique<ModernThemeFactory>();
        } else if (theme == "vintage") {
            return std::make_unique<VintageThemeFactory>();
        } else {
            return std::make_unique<BasicThemeFactory>(); // default
        }
    }
    
    static std::vector<std::string> getAvailableThemes() {
        return {"basic", "advanced", "modern", "vintage"};
    }
};
```

## Compile and Run

```bash
g++ -std=c++14 -o test_abstract_factory test_abstract_factory.cpp
./test_abstract_factory
```

## Educational Value

This implementation demonstrates:
- How to create families of related objects without specifying concrete classes
- The importance of ensuring compatibility between products in the same family
- Runtime selection of product families
- Proper use of polymorphism with abstract factories
- How different "themes" represent different families of products
- The Open/Closed Principle (open for extension, closed for modification)
- How to structure code to maintain consistency across product families

## Abstract Factory vs Factory Method

- **Factory Method**: Creates single objects, uses inheritance to decide which class to instantiate
- **Abstract Factory**: Creates families of related objects, focused on object families rather than individual objects
- **Abstract Factory**: Generally uses Factory Methods internally to create individual objects

## Important Notes

- Products from the same factory are guaranteed to be compatible 
- Abstract Factory enforces consistency among product families
- Adding new product families is easy (just create new concrete factory)
- Adding new product types requires changing all factory interfaces
- Memory management is important when using factory patterns with smart pointers
- The pattern works well with other creational patterns

## Benefits

- **Isolation**: Client code is isolated from concrete classes
- **Product Family Consistency**: Ensures compatibility between products in family
- **Easy Switching**: Can easily switch between different product families 
- **Platform Independence**: Can create families of platform-specific objects
- **Open/Closed Principle**: New families can be added without changing existing code
- **Encapsulation**: Creation logic is encapsulated in factory implementations