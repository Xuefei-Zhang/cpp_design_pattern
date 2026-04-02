#ifndef SHAPE_BUILDER_H
#define SHAPE_BUILDER_H

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
    bool filled_;

public:
    Circle(double radius, const std::string& color = "red",
           const std::string& style = "solid", bool filled = true)
        : radius_(radius), color_(color), style_(style), filled_(filled) {}

    void draw() const override {
        std::cout << "Drawing Circle: radius=" << radius_
                  << ", color=" << color_
                  << ", style=" << style_
                  << ", filled=" << (filled_ ? "yes" : "no") << std::endl;
    }

    std::string getType() const override {
        return "Circle";
    }

    double getArea() const override {
        return 3.14159 * radius_ * radius_;
    }

    void printInfo() const override {
        std::cout << "Circle Info - Radius: " << radius_
                  << ", Color: " << color_
                  << ", Style: " << style_
                  << ", Filled: " << (filled_ ? "yes" : "no") << std::endl;
    }

    // Getters
    double getRadius() const { return radius_; }
    const std::string& getColor() const { return color_; }
    const std::string& getStyle() const { return style_; }
    bool isFilled() const { return filled_; }

    // Friend class declaration to allow builder access
    friend class CircleBuilder;
};

class Rectangle : public Shape {
private:
    double width_, height_;
    std::string color_;
    std::string style_;
    bool filled_;
    int borderWidth_;

public:
    Rectangle(double width, double height, const std::string& color = "blue",
              const std::string& style = "solid", bool filled = true, int borderWidth = 1)
        : width_(width), height_(height), color_(color), style_(style), filled_(filled), borderWidth_(borderWidth) {}

    void draw() const override {
        std::cout << "Drawing Rectangle: " << width_ << "x" << height_
                  << ", color=" << color_
                  << ", style=" << style_
                  << ", filled=" << (filled_ ? "yes" : "no")
                  << ", border=" << borderWidth_ << std::endl;
    }

    std::string getType() const override {
        return "Rectangle";
    }

    double getArea() const override {
        return width_ * height_;
    }

    void printInfo() const override {
        std::cout << "Rectangle Info - Width: " << width_
                  << ", Height: " << height_
                  << ", Color: " << color_
                  << ", Style: " << style_
                  << ", Filled: " << (filled_ ? "yes" : "no")
                  << ", Border Width: " << borderWidth_ << std::endl;
    }

    // Getters
    double getWidth() const { return width_; }
    double getHeight() const { return height_; }
    const std::string& getColor() const { return color_; }
    const std::string& getStyle() const { return style_; }
    bool isFilled() const { return filled_; }
    int getBorderWidth() const { return borderWidth_; }

    // Friend class declaration to allow builder access
    friend class RectangleBuilder;
};

class Triangle : public Shape {
private:
    double base_, height_;
    std::string color_;
    std::string style_;
    bool filled_;
    std::string label_;

public:
    Triangle(double base, double height, const std::string& color = "green",
             const std::string& style = "solid", bool filled = true, const std::string& label = "")
        : base_(base), height_(height), color_(color), style_(style), filled_(filled), label_(label) {}

    void draw() const override {
        std::cout << "Drawing Triangle: base=" << base_ << ", height=" << height_
                  << ", color=" << color_
                  << ", style=" << style_
                  << ", filled=" << (filled_ ? "yes" : "no")
                  << ", label=" << label_ << std::endl;
    }

    std::string getType() const override {
        return "Triangle";
    }

    double getArea() const override {
        return 0.5 * base_ * height_;
    }

    void printInfo() const override {
        std::cout << "Triangle Info - Base: " << base_
                  << ", Height: " << height_
                  << ", Color: " << color_
                  << ", Style: " << style_
                  << ", Filled: " << (filled_ ? "yes" : "no")
                  << ", Label: " << label_ << std::endl;
    }

    // Getters
    double getBase() const { return base_; }
    double getHeight() const { return height_; }
    const std::string& getColor() const { return color_; }
    const std::string& getStyle() const { return style_; }
    bool isFilled() const { return filled_; }
    const std::string& getLabel() const { return label_; }

    // Friend class declaration to allow builder access
    friend class TriangleBuilder;
};

// Abstract Builder interface
class ShapeBuilder {
public:
    virtual ~ShapeBuilder() = default;
    virtual void buildShape() = 0;
    virtual std::unique_ptr<Shape> getResult() = 0;
};

// Concrete Builder for Circle
class CircleBuilder : public ShapeBuilder {
private:
    std::unique_ptr<Circle> circle_;

public:
    CircleBuilder() {
        reset();
    }
    
    void reset() {
        circle_ = std::make_unique<Circle>(1.0, "red", "solid", true);  // Default circle with all parameters
    }
    
    void buildShape() override {
        // Nothing to do here since we're building incrementally
    }
    
    std::unique_ptr<Shape> getResult() override {
        auto result = std::move(circle_);
        reset(); // Prepare for next build
        return std::unique_ptr<Shape>(result.release());
    }
    
    CircleBuilder* setRadius(double radius) {
        circle_->radius_ = radius;
        return this;
    }

    CircleBuilder* setColor(const std::string& color) {
        circle_->color_ = color;
        return this;
    }

    CircleBuilder* setStyle(const std::string& style) {
        circle_->style_ = style;
        return this;
    }

    CircleBuilder* setFilled(bool filled) {
        circle_->filled_ = filled;
        return this;
    }
};

// Concrete Builder for Rectangle
class RectangleBuilder : public ShapeBuilder {
private:
    std::unique_ptr<Rectangle> rectangle_;

public:
    RectangleBuilder() {
        reset();
    }
    
    void reset() {
        rectangle_ = std::make_unique<Rectangle>(1.0, 1.0, "blue", "solid", true, 1);  // Default rectangle with all parameters
    }
    
    void buildShape() override {
        // Nothing to do here since we're building incrementally
    }
    
    std::unique_ptr<Shape> getResult() override {
        auto result = std::move(rectangle_);
        reset(); // Prepare for next build
        return std::unique_ptr<Shape>(result.release());
    }
    
    RectangleBuilder* setWidth(double width) {
        rectangle_->width_ = width;
        return this;
    }

    RectangleBuilder* setHeight(double height) {
        rectangle_->height_ = height;
        return this;
    }

    RectangleBuilder* setColor(const std::string& color) {
        rectangle_->color_ = color;
        return this;
    }

    RectangleBuilder* setStyle(const std::string& style) {
        rectangle_->style_ = style;
        return this;
    }

    RectangleBuilder* setFilled(bool filled) {
        rectangle_->filled_ = filled;
        return this;
    }

    RectangleBuilder* setBorderWidth(int borderWidth) {
        rectangle_->borderWidth_ = borderWidth;
        return this;
    }
};

// Concrete Builder for Triangle
class TriangleBuilder : public ShapeBuilder {
private:
    std::unique_ptr<Triangle> triangle_;

public:
    TriangleBuilder() {
        reset();
    }
    
    void reset() {
        triangle_ = std::make_unique<Triangle>(1.0, 1.0, "green", "solid", true, "");  // Default triangle with all parameters
    }
    
    void buildShape() override {
        // Nothing to do here since we're building incrementally
    }
    
    std::unique_ptr<Shape> getResult() override {
        auto result = std::move(triangle_);
        reset(); // Prepare for next build
        return std::unique_ptr<Shape>(result.release());
    }
    
    TriangleBuilder* setBase(double base) {
        triangle_->base_ = base;
        return this;
    }

    TriangleBuilder* setHeight(double height) {
        triangle_->height_ = height;
        return this;
    }

    TriangleBuilder* setColor(const std::string& color) {
        triangle_->color_ = color;
        return this;
    }

    TriangleBuilder* setStyle(const std::string& style) {
        triangle_->style_ = style;
        return this;
    }

    TriangleBuilder* setFilled(bool filled) {
        triangle_->filled_ = filled;
        return this;
    }

    TriangleBuilder* setLabel(const std::string& label) {
        triangle_->label_ = label;
        return this;
    }
};

#endif // SHAPE_BUILDER_H