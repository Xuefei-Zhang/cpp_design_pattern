# Builder Pattern in C++

This project demonstrates the Builder design pattern in C++ with comprehensive examples using the shape/render context, similar to factory pattern implementations.

## Overview

The Builder pattern is a creational design pattern that allows constructing complex objects step by step. It separates the construction of an object from its representation, allowing the same construction process to create different representations.

## Key Concepts

### When to use Builder pattern:
- When you need to build complex objects with many optional parameters
- When the construction algorithm should be independent of the parts that make up the object
- When you want to create different representations of an object using the same construction process
- When you want to provide a fluent interface for object construction

### Components of Builder Pattern:
- **Product**: The complex object being built (Shape classes)
- **Builder**: Abstract interface for creating parts of the product (ShapeBuilder)
- **ConcreteBuilder**: Implements the Builder interface to construct and assemble parts (CircleBuilder, RectangleBuilder, TriangleBuilder)
- **Director**: Constructs the object using the Builder interface (ShapeDirector)
- **Client**: Uses the Director and Builder to create objects

## Components

### Basic Shape Classes (`shape_builder.h`)
- **Shape**: Abstract base class defining the interface
- **Circle, Rectangle, Triangle**: Concrete shape implementations with complex construction parameters
- **ShapeBuilder**: Abstract builder interface
- **CircleBuilder, RectangleBuilder, TriangleBuilder**: Concrete builders with fluent interfaces

### Advanced Builder Features (`builder_shapes.h`)
- **ShapeDirector**: Manages the building process using different builders
- **FluentShapeBuilder**: Provides fluent interfaces for easy construction
- **ComplexShapeBuilder**: Builds complex shapes from multiple simple shapes
- **ShapeFactoryBuilder**: Combines factory and builder patterns

### Test Program (`test_builder_pattern.cpp`)
- Comprehensive demonstration of all Builder patterns
- Practical examples of builder usage
- Comparison with direct construction
- Director pattern usage
- Fluent builder demonstrations

## Files

- `shape_builder.h` - Basic Shape classes with Builder implementations
- `builder_shapes.h` - Advanced Builder patterns and Director
- `test_builder_pattern.cpp` - Test program demonstrating Builder functionality
- `BUILDER_PATTERN_README.md` - This documentation

## Key Examples

### 1. Basic Builder Usage
```cpp
auto circleBuilder = std::make_unique<CircleBuilder>();
auto circle = circleBuilder->setRadius(5.0)
                           ->setColor("red")
                           ->setStyle("solid")
                           ->setFilled(true)
                           ->getResult();
```

### 2. Director Pattern
```cpp
auto director = std::make_unique<ShapeDirector>(std::make_unique<CircleBuilder>());
auto standardCircle = director->createStandardCircle();
```

### 3. Fluent Builder
```cpp
auto circle = FluentShapeBuilder::CircleFluentBuilder()
                     .radius(7.0)
                     .color("blue")
                     .style("dashed")
                     .filled(false)
                     .build();
```

## Compile and Run

```bash
g++ -std=c++14 -o test_builder_pattern test_builder_pattern.cpp
./test_builder_pattern
```

## Educational Value

This implementation demonstrates:
- Basic Builder pattern concepts and implementation
- Fluent interfaces for easy object construction
- Director pattern for managing the building process
- Method chaining for readable code
- How to handle complex objects with multiple optional parameters
- The difference between Builder and Factory patterns
- Proper encapsulation while allowing builders access to private members
- Reset functionality for reusing builders

## Builder vs Factory Pattern

- **Factory**: Creates objects in a single call, good for simple objects or when creation logic needs to be centralized
- **Builder**: Constructs objects step-by-step, good for complex objects with many parameters or when the construction process is complex

The Builder pattern is particularly useful when:
- Objects have many optional parameters
- The construction process is complex
- You need to create different representations of an object
- You want to provide a clean, readable API for object creation

## Important Notes

- The builder uses friendship to access private members of the shape classes
- Builders maintain their own state between builds
- The reset functionality ensures clean state for each build
- Fluent interfaces make the API more readable and user-friendly
- Director pattern helps to separate construction algorithm from actual construction