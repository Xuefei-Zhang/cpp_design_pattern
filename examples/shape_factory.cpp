#include <iostream>
#include <memory>
#include <string>

// Base class
class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw() const = 0;
    virtual std::string getType() const = 0;
};

// Concrete implementations
class Circle : public Shape {
public:
    void draw() const override {
        std::cout << "Drawing a circle" << std::endl;
    }
    
    std::string getType() const override {
        return "Circle";
    }
};

class Rectangle : public Shape {
public:
    void draw() const override {
        std::cout << "Drawing a rectangle" << std::endl;
    }
    
    std::string getType() const override {
        return "Rectangle";
    }
};

class Triangle : public Shape {
public:
    void draw() const override {
        std::cout << "Drawing a triangle" << std::endl;
    }
    
    std::string getType() const override {
        return "Triangle";
    }
};

// Factory class
class ShapeFactory {
public:
    static std::unique_ptr<Shape> createShape(const std::string& shapeType) {
        if (shapeType == "circle") {
            return std::make_unique<Circle>();
        } else if (shapeType == "rectangle") {
            return std::make_unique<Rectangle>();
        } else if (shapeType == "triangle") {
            return std::make_unique<Triangle>();
        }
        
        return nullptr; // Return null if type is not recognized
    }
};

// Example usage
int main() {
    // Create different shapes using the factory
    auto circle = ShapeFactory::createShape("circle");
    auto rectangle = ShapeFactory::createShape("rectangle");
    auto triangle = ShapeFactory::createShape("triangle");
    auto unknown = ShapeFactory::createShape("hexagon");  // This will return nullptr
    
    if (circle) {
        std::cout << "Created a " << circle->getType() << ": ";
        circle->draw();
    }
    
    if (rectangle) {
        std::cout << "Created a " << rectangle->getType() << ": ";
        rectangle->draw();
    }
    
    if (triangle) {
        std::cout << "Created a " << triangle->getType() << ": ";
        triangle->draw();
    }
    
    if (!unknown) {
        std::cout << "Could not create shape of type 'hexagon'" << std::endl;
    }
    
    return 0;
}