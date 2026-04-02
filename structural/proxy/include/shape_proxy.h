#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>
#include <string>
#include <memory>
#include <functional>

// Abstract base class for shapes
class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw() const = 0;
    virtual std::string getType() const = 0;
    virtual double getArea() const = 0;
};

// Concrete implementations of Shape
class Circle : public Shape {
private:
    double radius_;
    std::string color_;

public:
    Circle(double radius, const std::string& color = "red") 
        : radius_(radius), color_(color) {}
    
    void draw() const override {
        std::cout << "Drawing Circle: radius=" << radius_ << ", color=" << color_ << std::endl;
    }
    
    std::string getType() const override {
        return "Circle";
    }
    
    double getArea() const override {
        return 3.14159 * radius_ * radius_;
    }
};

class Rectangle : public Shape {
private:
    double width_, height_;
    std::string color_;

public:
    Rectangle(double width, double height, const std::string& color = "blue") 
        : width_(width), height_(height), color_(color) {}
    
    void draw() const override {
        std::cout << "Drawing Rectangle: " << width_ << "x" << height_ 
                  << ", color=" << color_ << std::endl;
    }
    
    std::string getType() const override {
        return "Rectangle";
    }
    
    double getArea() const override {
        return width_ * height_;
    }
};

class Triangle : public Shape {
private:
    double base_, height_;
    std::string color_;

public:
    Triangle(double base, double height, const std::string& color = "green") 
        : base_(base), height_(height), color_(color) {}
    
    void draw() const override {
        std::cout << "Drawing Triangle: base=" << base_ << ", height=" << height_ 
                  << ", color=" << color_ << std::endl;
    }
    
    std::string getType() const override {
        return "Triangle";
    }
    
    double getArea() const override {
        return 0.5 * base_ * height_;
    }
};

// Virtual Proxy for shape - delays creation until actually needed
class LazyShapeProxy : public Shape {
private:
    std::function<std::unique_ptr<Shape>()> shapeFactory_;
    mutable std::unique_ptr<Shape> realShape_;
    mutable bool isRealShapeCreated_;

public:
    // Constructor that takes a factory function
    explicit LazyShapeProxy(std::function<std::unique_ptr<Shape>()> factory)
        : shapeFactory_(std::move(factory)), isRealShapeCreated_(false) {}

    void draw() const override {
        createRealShapeIfNeeded();
        realShape_->draw();
    }

    std::string getType() const override {
        createRealShapeIfNeeded();
        return realShape_->getType();
    }

    double getArea() const override {
        createRealShapeIfNeeded();
        return realShape_->getArea();
    }

private:
    void createRealShapeIfNeeded() const {
        if (!isRealShapeCreated_) {
            std::cout << "[LAZY] Creating shape on demand..." << std::endl;
            realShape_ = shapeFactory_();
            isRealShapeCreated_ = true;
        }
    }
};

#endif // SHAPE_H