#include <iostream>
#include <memory>
#include <string>

// Interface class that defines the contract
class IDrawable {
public:
    virtual ~IDrawable() = default;
    virtual void draw() const = 0;
    virtual void resize(double factor) = 0;
    virtual double getArea() const = 0;
};

// Base class that implements the interface with common functionality
class Shape : public IDrawable {
protected:
    double x = 0.0, y = 0.0;  // Position coordinates
    double scale = 1.0;       // Scale factor

public:
    Shape(double pos_x = 0.0, double pos_y = 0.0) : x(pos_x), y(pos_y) {}
    
    // Common functionality for all shapes
    void setPosition(double pos_x, double pos_y) {
        x = pos_x;
        y = pos_y;
    }
    
    void getPosition(double& pos_x, double& pos_y) const {
        pos_x = x;
        pos_y = y;
    }
    
    virtual std::string getType() const = 0;
    
    // Default implementation for resize
    void resize(double factor) override {
        scale *= factor;
    }
    
    // Pure virtual functions that derived classes must implement
    virtual void draw() const = 0;
    virtual double getArea() const = 0;
};

// Concrete implementations
class Circle : public Shape {
private:
    double radius;

public:
    Circle(double r = 1.0, double pos_x = 0.0, double pos_y = 0.0) 
        : Shape(pos_x, pos_y), radius(r) {}
    
    void draw() const override {
        std::cout << "Drawing a " << getType() << " with radius " << radius 
                  << " at position (" << x << ", " << y << ")" << std::endl;
    }
    
    std::string getType() const override {
        return "Circle";
    }
    
    double getArea() const override {
        return 3.14159 * radius * radius * scale * scale;
    }
};

class Rectangle : public Shape {
private:
    double width, height;

public:
    Rectangle(double w = 1.0, double h = 1.0, double pos_x = 0.0, double pos_y = 0.0) 
        : Shape(pos_x, pos_y), width(w), height(h) {}
    
    void draw() const override {
        std::cout << "Drawing a " << getType() << " with dimensions " 
                  << width << "x" << height << " at position (" << x << ", " << y << ")" << std::endl;
    }
    
    std::string getType() const override {
        return "Rectangle";
    }
    
    double getArea() const override {
        return width * height * scale * scale;
    }
};

class Triangle : public Shape {
private:
    double base, height;

public:
    Triangle(double b = 1.0, double h = 1.0, double pos_x = 0.0, double pos_y = 0.0) 
        : Shape(pos_x, pos_y), base(b), height(h) {}
    
    void draw() const override {
        std::cout << "Drawing a " << getType() << " with base " << base 
                  << " and height " << height << " at position (" << x << ", " << y << ")" << std::endl;
    }
    
    std::string getType() const override {
        return "Triangle";
    }
    
    double getArea() const override {
        return 0.5 * base * height * scale * scale;
    }
};

// Enhanced Factory class
class ShapeFactory {
public:
    enum class ShapeType {
        CIRCLE,
        RECTANGLE,
        TRIANGLE
    };

    static std::unique_ptr<Shape> createShape(ShapeType type, 
                                              double param1 = 1.0, 
                                              double param2 = 1.0, 
                                              double pos_x = 0.0, 
                                              double pos_y = 0.0) {
        switch (type) {
            case ShapeType::CIRCLE:
                return std::make_unique<Circle>(param1, pos_x, pos_y);
            case ShapeType::RECTANGLE:
                return std::make_unique<Rectangle>(param1, param2, pos_x, pos_y);
            case ShapeType::TRIANGLE:
                return std::make_unique<Triangle>(param1, param2, pos_x, pos_y);
            default:
                return nullptr;
        }
    }

    static std::unique_ptr<Shape> createShape(const std::string& shapeType, 
                                              double param1 = 1.0, 
                                              double param2 = 1.0, 
                                              double pos_x = 0.0, 
                                              double pos_y = 0.0) {
        if (shapeType == "circle") {
            return std::make_unique<Circle>(param1, pos_x, pos_y);
        } else if (shapeType == "rectangle") {
            return std::make_unique<Rectangle>(param1, param2, pos_x, pos_y);
        } else if (shapeType == "triangle") {
            return std::make_unique<Triangle>(param1, param2, pos_x, pos_y);
        }
        
        return nullptr;
    }
};

// Example usage
int main() {
    std::cout << "Creating shapes using the enhanced factory pattern:\n" << std::endl;
    
    // Create different shapes
    auto shapes = {
        ShapeFactory::createShape("circle", 2.0, 1.0, 5.0, 3.0),
        ShapeFactory::createShape("rectangle", 3.0, 4.0, 1.0, 2.0),
        ShapeFactory::createShape("triangle", 6.0, 4.0, 0.0, 0.0)
    };
    
    int i = 1;
    for (const auto& shape : shapes) {
        if (shape) {
            std::cout << "Shape " << i++ << ":" << std::endl;
            shape->draw();
            std::cout << "Area: " << shape->getArea() << std::endl;
            
            // Test the resize functionality (from interface)
            shape->resize(2.0);  // Double the size
            std::cout << "After resizing by factor 2.0, new area: " << shape->getArea() << std::endl;
            std::cout << std::endl;
        }
    }
    
    // Demonstrate polymorphism with interface
    std::cout << "Demonstrating polymorphism with interface:" << std::endl;
    IDrawable* drawable = ShapeFactory::createShape(ShapeFactory::ShapeType::CIRCLE, 1.5).release();
    if (drawable) {
        drawable->draw();
        std::cout << "Area: " << static_cast<Shape*>(drawable)->getArea() << std::endl;
        delete drawable;  // Manual cleanup since we used release()
    }
    
    return 0;
}