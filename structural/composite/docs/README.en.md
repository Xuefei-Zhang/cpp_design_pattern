# Composite Pattern in C++

This project demonstrates the Composite design pattern in C++ using the shape grouping context for consistency with other patterns.

## Overview

The Composite pattern is a structural design pattern that allows composing objects into tree structures to represent part-whole hierarchies. It enables treating individual objects and compositions of objects uniformly, making it easy to work with hierarchical structures.

## Key Concepts

### When to use Composite pattern:
- When you want to represent part-whole hierarchies of objects
- When you want clients to be able to treat individual objects and compositions uniformly
- When you need to create hierarchical tree structures
- When you want to add new composite or leaf components without changing existing code
- When your application deals with nested structures like trees or hierarchies

### Components of Composite:
- **Component**: Declares the interface for objects in the composition and implements default behavior (ShapeComponent)
- **Leaf**: Represents leaf objects in the composition (Circle, Rectangle, Triangle, Line)
- **Composite**: Stores child components and implements child-related operations (ShapeComposite, Group, Layer)
- **Client**: Manipulates objects in the composition through the Component interface

## Implementation Variations

### 1. Basic Composite
- Simple tree structure with leaf and composite components
- Uniform interface for both individual objects and groups
- Basic addition/removal operations

### 2. Nested Composites
- Composites that can contain other composites
- Deep hierarchical structures
- Recursive operations throughout the tree

### 3. Themed Composites
- Specialized composites for specific purposes (Layers, SceneNodes, Groups)
- Enhanced functionality for specific use cases
- Property inheritance and management

### 4. Predefined Complex Shapes
- Composites with predefined structures (House, Car)
- Factory methods for common compositions
- Structured groupings of related elements

## Files

- `composite_patterns.h` - All Composite pattern implementations
- `test_composite_pattern.cpp` - Comprehensive test program
- `COMPOSITE_PATTERN_README.md` - This documentation

## Key Examples

### 1. Basic Composite Implementation
```cpp
// Component interface
class ShapeComponent {
public:
    virtual ~ShapeComponent() = default;
    virtual void draw() const = 0;
    virtual std::string getType() const = 0;
    virtual double getArea() const = 0;
    virtual std::string getName() const = 0;
    
    // Default operations that do nothing for leaves
    virtual void add(std::unique_ptr<ShapeComponent> component) {}
    virtual bool isComposite() const { return false; }
    virtual const std::vector<std::unique_ptr<ShapeComponent>>& getChildren() const {
        throw std::logic_error("Not a composite");
    }
};

// Leaf implementation
class Circle : public ShapeComponent {
private:
    double radius_;
    std::string color_;
    std::string name_;

public:
    Circle(double radius, const std::string& color, const std::string& name) 
        : radius_(radius), color_(color), name_(name) {}

    void draw() const override {
        std::cout << "Drawing Circle: " << name_ << ", radius=" << radius_ << std::endl;
    }

    std::string getType() const override { return "Circle"; }
    double getArea() const override { return 3.14159 * radius_ * radius_; }
    std::string getName() const override { return name_; }
    bool isComposite() const override { return false; }
};

// Composite implementation
class ShapeComposite : public ShapeComponent {
protected:
    std::vector<std::unique_ptr<ShapeComponent>> children_;
    std::string name_;

public:
    explicit ShapeComposite(const std::string& name) : name_(name) {}

    void add(std::unique_ptr<ShapeComponent> component) override {
        children_.push_back(std::move(component));
    }

    void draw() const override {
        std::cout << "Drawing Composite: " << name_ << std::endl;
        for (const auto& child : children_) {
            std::cout << "  ";
            child->draw();
        }
    }

    std::string getType() const override { return "Composite"; }
    std::string getName() const override { return name_; }
    bool isComposite() const override { return true; }
    double getArea() const override {
        double total = 0;
        for (const auto& child : children_) {
            total += child->getArea();
        }
        return total;
    }
};
```

### 2. Using Composite Pattern
```cpp
// Create individual shapes
auto circle = std::make_unique<Circle>(5.0, "red", "Red Circle");
auto rectangle = std::make_unique<Rectangle>(10.0, 8.0, "blue", "Blue Rectangle");

// Create a composite group
auto group = std::make_unique<ShapeComposite>("Main Group");
group->add(std::make_unique<Circle>(3.0, "yellow", "Small Circle"));
group->add(std::make_unique<Rectangle>(5.0, 3.0, "green", "Small Rectangle"));

// Treat individual shapes and composites uniformly
std::vector<std::unique_ptr<ShapeComponent>> components;
components.push_back(std::move(circle));
components.push_back(std::move(rectangle));
components.push_back(std::move(group));

// Process all components uniformly
for (const auto& component : components) {
    component->draw();  // Works for both individual shapes and composites
    std::cout << "Area: " << component->getArea() << std::endl;
}
```

## Compile and Run

```bash
g++ -std=c++14 -o test_composite_pattern test_composite_pattern.cpp
./test_composite_pattern
```

## Educational Value

This implementation demonstrates:
- Complete separation of concerns between leaf and composite components
- How to structure code to enable uniform treatment of individual and composite objects
- Recursive tree traversal and operations
- Hierarchical data structure management
- Real-world scenarios like UI components, graphics systems, and scene graphs
- Performance considerations with deep hierarchies
- Flexible, extensible design patterns

## Composite vs Other Patterns

- **Decorator**: Adds responsibilities to objects without changing their interface, Composite creates tree structures
- **Iterator**: Provides access to elements in aggregate objects, Composite organizes objects in tree structures
- **Visitor**: Adds new operations to existing object structures, Composite creates hierarchical structures

## Important Notes

- Both leaves and composites should implement the same interface
- Client code treats all components uniformly without knowing which is a leaf or composite
- Be careful about deep hierarchies that could cause stack overflow during recursion
- Memory management in tree structures requires attention to ownership patterns
- Consider using smart pointers to manage object lifetimes

## Benefits

- **Uniform Interface**: Treat individual objects and compositions the same way
- **Hierarchical Composition**: Natural representation of tree structures
- **Extensibility**: Easy to add new component types without changing existing code
- **Recursive Operations**: Operations can be applied to entire trees automatically
- **Transparency**: Clients can't tell the difference between leaves and composites
- **Flexibility**: Structure can be changed at runtime by adding or removing components