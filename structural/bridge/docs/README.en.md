# Bridge Pattern in C++

This project demonstrates the Bridge design pattern in C++ using the shape context for consistency with other patterns.

## Overview

The Bridge pattern is a structural design pattern that separates an object's abstraction from its implementation so that the two can vary independently. It's like having two parallel inheritance hierarchies where one controls how something is done (implementation) and the other controls what is being done (abstraction).

## Key Concepts

### When to use Bridge pattern:
- When you want to avoid permanent binding between an abstraction and its implementation
- When both the abstractions and their implementations need to extend in subclasses independently
- When changes to the implementation shouldn't impact the client code
- When you want to share an implementation among multiple objects
- When you need to provide multiple implementations for the same abstraction

### Components of Bridge:
- **Abstraction**: Defines the interface for the "control" part of the object (Shape hierarchy)
- **Refined Abstraction**: Extends the interface defined by Abstraction (Circle, Rectangle, Triangle)
- **Implementation**: Defines the interface for implementation classes (ShapeRenderer hierarchy)
- **Concrete Implementation**: Implements the Implementation interface (VectorRenderer, RasterRenderer, etc.)

## Implementation Variations

### 1. Basic Bridge
- Simple separation of abstraction (Shape) and implementation (ShapeRenderer)
- Runtime switching of implementations
- Multiple concrete implementations

### 2. Themed Renderers
- Extends implementation hierarchy with themed rendering
- Shows how implementation hierarchy can be extended

### 3. Animation Bridge
- Advanced bridge that combines animation with shape rendering
- Demonstrates complex bridge usage

### 4. Bridge Manager
- Higher-level abstraction that manages multiple bridge objects
- Shows real-world scenario of bridge pattern usage

## Files

- `bridge_patterns.h` - All Bridge pattern implementations
- `test_bridge_pattern.cpp` - Comprehensive test program
- `BRIDGE_PATTERN_README.md` - This documentation

## Key Examples

### 1. Basic Bridge Implementation
```cpp
// Implementation hierarchy
class ShapeRenderer {
public:
    virtual ~ShapeRenderer() = default;
    virtual void render(const std::string& shapeType, double size, const std::string& color) const = 0;
};

class VectorRenderer : public ShapeRenderer {
public:
    void render(const std::string& shapeType, double size, const std::string& color) const override {
        std::cout << "[VECTOR] Rendering " << shapeType << " with size " << size << std::endl;
    }
};

// Abstraction hierarchy
class Shape {
protected:
    std::unique_ptr<ShapeRenderer> renderer_;

public:
    explicit Shape(std::unique_ptr<ShapeRenderer> renderer) : renderer_(std::move(renderer)) {}
    
    virtual ~Shape() = default;
    virtual void draw() const = 0;
    
    void setRenderer(std::unique_ptr<ShapeRenderer> newRenderer) {
        renderer_ = std::move(newRenderer);
    }
};

class Circle : public Shape {
private:
    double radius_;
    std::string color_;

public:
    Circle(double radius, const std::string& color, std::unique_ptr<ShapeRenderer> renderer)
        : Shape(std::move(renderer)), radius_(radius), color_(color) {}

    void draw() const override {
        renderer_->render("Circle", radius_, color_);
    }
};
```

### 2. Runtime Implementation Switching
```cpp
// Create a circle with vector renderer
auto circle = std::make_unique<Circle>(5.0, "red", std::make_unique<VectorRenderer>());
circle->draw();  // Uses vector renderer

// Switch to raster renderer at runtime
circle->setRenderer(std::make_unique<RasterRenderer>());
circle->draw();  // Now uses raster renderer
```

## Compile and Run

```bash
g++ -std=c++14 -o test_bridge_pattern test_bridge_pattern.cpp
./test_bridge_pattern
```

## Educational Value

This implementation demonstrates:
- Complete separation of abstraction and implementation hierarchies
- How to structure code to allow independent evolution of both hierarchies
- Runtime switching of implementations
- Themed rendering systems
- Animation bridges
- Real-world graphics library integration scenarios

## Bridge vs Other Patterns

- **Adapter**: Makes unrelated interfaces work together, Bridge separates interface from implementation
- **Abstract Factory**: Creates families of related objects, Bridge decouples abstraction from implementation
- **Strategy**: Defines a family of algorithms, Bridge separates abstraction from implementation

## Important Notes

- Both hierarchies can vary independently
- Implementation can be changed at runtime
- Promotes composition over inheritance
- Improves code maintainability and extensibility
- Can add new implementations without changing abstractions

## Benefits

- **Decoupling**: Abstraction and implementation are independent
- **Extensibility**: Easy to add new implementations or abstractions
- **Runtime Flexibility**: Implementations can be switched at runtime
- **Maintainability**: Changes in one hierarchy don't affect the other
- **Testing**: Both hierarchies can be tested independently