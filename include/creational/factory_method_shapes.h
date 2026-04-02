#ifndef FACTORY_METHOD_SHAPES_H
#define FACTORY_METHOD_SHAPES_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>

// Abstract base class for shapes
class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw() const = 0;
    virtual std::string getType() const = 0;
    virtual double getArea() const = 0;
    virtual void printInfo() const = 0;
};

// Concrete implementations of Shape
class Circle : public Shape {
private:
    double radius_;
    std::string color_;
    std::string style_;

public:
    Circle(double radius, const std::string& color = "red", const std::string& style = "solid") 
        : radius_(radius), color_(color), style_(style) {}
    
    void draw() const override {
        std::cout << "Drawing Circle: radius=" << radius_ 
                  << ", color=" << color_ 
                  << ", style=" << style_ << std::endl;
    }
    
    std::string getType() const override {
        return "Circle";
    }
    
    double getArea() const override {
        return 3.14159 * radius_ * radius_;
    }
    
    void printInfo() const override {
        std::cout << "Circle - Radius: " << radius_ 
                  << ", Color: " << color_
                  << ", Style: " << style_ << std::endl;
    }
};

class Rectangle : public Shape {
private:
    double width_, height_;
    std::string color_;
    std::string style_;

public:
    Rectangle(double width, double height, const std::string& color = "blue", 
              const std::string& style = "solid") 
        : width_(width), height_(height), color_(color), style_(style) {}
    
    void draw() const override {
        std::cout << "Drawing Rectangle: " << width_ << "x" << height_ 
                  << ", color=" << color_ 
                  << ", style=" << style_ << std::endl;
    }
    
    std::string getType() const override {
        return "Rectangle";
    }
    
    double getArea() const override {
        return width_ * height_;
    }
    
    void printInfo() const override {
        std::cout << "Rectangle - Width: " << width_ 
                  << ", Height: " << height_
                  << ", Color: " << color_
                  << ", Style: " << style_ << std::endl;
    }
};

class Triangle : public Shape {
private:
    double base_, height_;
    std::string color_;
    std::string style_;

public:
    Triangle(double base, double height, const std::string& color = "green", 
             const std::string& style = "solid") 
        : base_(base), height_(height), color_(color), style_(style) {}
    
    void draw() const override {
        std::cout << "Drawing Triangle: base=" << base_ << ", height=" << height_ 
                  << ", color=" << color_ 
                  << ", style=" << style_ << std::endl;
    }
    
    std::string getType() const override {
        return "Triangle";
    }
    
    double getArea() const override {
        return 0.5 * base_ * height_;
    }
    
    void printInfo() const override {
        std::cout << "Triangle - Base: " << base_ 
                  << ", Height: " << height_
                  << ", Color: " << color_
                  << ", Style: " << style_ << std::endl;
    }
};

class Square : public Shape {
private:
    double side_;
    std::string color_;
    std::string style_;

public:
    Square(double side, const std::string& color = "yellow", 
           const std::string& style = "solid") 
        : side_(side), color_(color), style_(style) {}
    
    void draw() const override {
        std::cout << "Drawing Square: side=" << side_ 
                  << ", color=" << color_ 
                  << ", style=" << style_ << std::endl;
    }
    
    std::string getType() const override {
        return "Square";
    }
    
    double getArea() const override {
        return side_ * side_;
    }
    
    void printInfo() const override {
        std::cout << "Square - Side: " << side_ 
                  << ", Color: " << color_
                  << ", Style: " << style_ << std::endl;
    }
};

// Abstract Creator - Defines the factory method
class ShapeFactory {
public:
    virtual ~ShapeFactory() = default;
    
    // Factory method that subclasses must implement
    virtual std::unique_ptr<Shape> createShape(double param1, double param2 = 0.0, 
                                               const std::string& color = "default", 
                                               const std::string& style = "solid") const = 0;
    
    // Client method that uses the factory method
    std::unique_ptr<Shape> makeShape(double param1, double param2 = 0.0, 
                                     const std::string& color = "default", 
                                     const std::string& style = "solid") const {
        auto shape = createShape(param1, param2, color, style);
        if (shape) {
            std::cout << "Created shape using factory: " << shape->getType() << std::endl;
        }
        return shape;
    }
};

// Concrete Creator implementations
class CircleFactory : public ShapeFactory {
public:
    std::unique_ptr<Shape> createShape(double param1, double param2 = 0.0, 
                                       const std::string& color = "default", 
                                       const std::string& style = "solid") const override {
        // param1 is radius
        return std::make_unique<Circle>(param1, color, style);
    }
};

class RectangleFactory : public ShapeFactory {
public:
    std::unique_ptr<Shape> createShape(double param1, double param2 = 0.0, 
                                       const std::string& color = "default", 
                                       const std::string& style = "solid") const override {
        // param1 is width, param2 is height
        return std::make_unique<Rectangle>(param1, param2, color, style);
    }
};

class TriangleFactory : public ShapeFactory {
public:
    std::unique_ptr<Shape> createShape(double param1, double param2 = 0.0, 
                                       const std::string& color = "default", 
                                       const std::string& style = "solid") const override {
        // param1 is base, param2 is height
        return std::make_unique<Triangle>(param1, param2, color, style);
    }
};

class SquareFactory : public ShapeFactory {
public:
    std::unique_ptr<Shape> createShape(double param1, double param2 = 0.0, 
                                       const std::string& color = "default", 
                                       const std::string& style = "solid") const override {
        // param1 is side length
        return std::make_unique<Square>(param1, color, style);
    }
};

// Advanced example: Shape Factory with different themes
class ThemedShapeFactory : public ShapeFactory {
protected:
    std::string defaultColor;
    std::string defaultStyle;

public:
    ThemedShapeFactory(const std::string& color, const std::string& style) 
        : defaultColor(color), defaultStyle(style) {}
    
    virtual std::string getDefaultColor() const { return defaultColor; }
    virtual std::string getDefaultStyle() const { return defaultStyle; }
};

class ModernShapeFactory : public ThemedShapeFactory {
public:
    ModernShapeFactory() : ThemedShapeFactory("white", "dashed") {}

    std::unique_ptr<Shape> createShape(double param1, double param2 = 0.0, 
                                       const std::string& color = "default", 
                                       const std::string& style = "solid") const override {
        std::string actualColor = (color == "default") ? getDefaultColor() : color;
        std::string actualStyle = (style == "solid") ? getDefaultStyle() : style;
        return std::make_unique<Circle>(param1, actualColor, actualStyle);
    }
};

class VintageShapeFactory : public ThemedShapeFactory {
public:
    VintageShapeFactory() : ThemedShapeFactory("brown", "dotted") {}

    std::unique_ptr<Shape> createShape(double param1, double param2 = 0.0, 
                                       const std::string& color = "default", 
                                       const std::string& style = "solid") const override {
        std::string actualColor = (color == "default") ? getDefaultColor() : color;
        std::string actualStyle = (style == "solid") ? getDefaultStyle() : style;
        return std::make_unique<Rectangle>(param1, param2, actualColor, actualStyle);
    }
};

// Factory Manager to demonstrate factory selection
class ShapeFactoryManager {
private:
    std::unique_ptr<ShapeFactory> factory;

public:
    explicit ShapeFactoryManager(std::unique_ptr<ShapeFactory> f) : factory(std::move(f)) {}

    void setFactory(std::unique_ptr<ShapeFactory> newFactory) {
        factory = std::move(newFactory);
    }

    std::unique_ptr<Shape> createShape(double param1, double param2 = 0.0, 
                                       const std::string& color = "default", 
                                       const std::string& style = "solid") const {
        return factory->makeShape(param1, param2, color, style);
    }

    std::string getFactoryType() const {
        // This would be better implemented with RTTI or virtual methods
        // For demo purposes, we'll use a simple approach
        if (dynamic_cast<const CircleFactory*>(factory.get())) {
            return "CircleFactory";
        } else if (dynamic_cast<const RectangleFactory*>(factory.get())) {
            return "RectangleFactory";
        } else if (dynamic_cast<const TriangleFactory*>(factory.get())) {
            return "TriangleFactory";
        } else if (dynamic_cast<const SquareFactory*>(factory.get())) {
            return "SquareFactory";
        } else if (dynamic_cast<const ModernShapeFactory*>(factory.get())) {
            return "ModernShapeFactory";
        } else if (dynamic_cast<const VintageShapeFactory*>(factory.get())) {
            return "VintageShapeFactory";
        }
        return "UnknownFactory";
    }
};

// Abstract Factory that uses Factory Method internally
class ShapeSuiteFactory {
public:
    virtual ~ShapeSuiteFactory() = default;

    // Factory methods for creating whole suites of shapes
    virtual std::vector<std::unique_ptr<Shape>> createBasicSuite() const = 0;
    virtual std::vector<std::unique_ptr<Shape>> createComplexSuite() const = 0;

    // Default implementation that uses individual factory methods
    std::vector<std::unique_ptr<Shape>> createDefaultSuite() const {
        std::vector<std::unique_ptr<Shape>> suite;
        suite.push_back(createCircle(5.0));
        suite.push_back(createRectangle(10.0, 5.0));
        return suite;
    }

protected:
    // Helper methods that should be overridden by subclasses
    virtual std::unique_ptr<Shape> createCircle(double radius) const = 0;
    virtual std::unique_ptr<Shape> createRectangle(double width, double height) const = 0;
};

// Concrete implementation of ShapeSuiteFactory
class ColorfulSuiteFactory : public ShapeSuiteFactory {
protected:
    std::unique_ptr<Shape> createCircle(double radius) const override {
        return std::make_unique<Circle>(radius, "rainbow", "solid");
    }

    std::unique_ptr<Shape> createRectangle(double width, double height) const override {
        return std::make_unique<Rectangle>(width, height, "rainbow", "dashed");
    }

public:
    std::vector<std::unique_ptr<Shape>> createBasicSuite() const override {
        std::vector<std::unique_ptr<Shape>> suite;
        suite.push_back(createCircle(3.0));
        suite.push_back(createRectangle(6.0, 4.0));
        return suite;
    }

    std::vector<std::unique_ptr<Shape>> createComplexSuite() const override {
        std::vector<std::unique_ptr<Shape>> suite;
        suite.push_back(createCircle(5.0));
        suite.push_back(createRectangle(10.0, 8.0));
        suite.push_back(std::make_unique<Triangle>(6.0, 4.0, "rainbow", "dotted"));
        return suite;
    }
};

#endif // FACTORY_METHOD_SHAPES_H