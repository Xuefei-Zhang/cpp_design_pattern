#ifndef DECORATOR_PATTERNS_H
#define DECORATOR_PATTERNS_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <cmath>

// Component interface - defines the interface for objects that can be decorated
class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw() const = 0;
    virtual std::string getType() const = 0;
    virtual double getArea() const = 0;
    virtual void printInfo() const = 0;
    virtual std::string getDescription() const = 0;
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
    
    void printInfo() const override {
        std::cout << "Circle - Radius: " << radius_ << ", Color: " << color_ << std::endl;
    }
    
    std::string getDescription() const override {
        return "Circle with radius " + std::to_string(radius_) + " and color " + color_;
    }
    
    // Specific getters
    double getRadius() const { return radius_; }
    const std::string& getColor() const { return color_; }
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
    
    void printInfo() const override {
        std::cout << "Rectangle - Width: " << width_ 
                  << ", Height: " << height_
                  << ", Color: " << color_ << std::endl;
    }
    
    std::string getDescription() const override {
        return "Rectangle " + std::to_string(width_) + "x" + std::to_string(height_) + " with color " + color_;
    }
    
    // Specific getters
    double getWidth() const { return width_; }
    double getHeight() const { return height_; }
    const std::string& getColor() const { return color_; }
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
    
    void printInfo() const override {
        std::cout << "Triangle - Base: " << base_ 
                  << ", Height: " << height_
                  << ", Color: " << color_ << std::endl;
    }
    
    std::string getDescription() const override {
        return "Triangle with base " + std::to_string(base_) + " and height " + std::to_string(height_) + ", color " + color_;
    }
    
    // Specific getters
    double getBase() const { return base_; }
    double getHeight() const { return height_; }
    const std::string& getColor() const { return color_; }
};

// Decorator base class - inherits from Shape to maintain the same interface
class ShapeDecorator : public Shape {
protected:
    std::unique_ptr<Shape> shape_;

public:
    explicit ShapeDecorator(std::unique_ptr<Shape> shape) : shape_(std::move(shape)) {}
    
    virtual ~ShapeDecorator() = default;
    
    void draw() const override {
        shape_->draw();
    }
    
    std::string getType() const override {
        return shape_->getType();
    }
    
    double getArea() const override {
        return shape_->getArea();
    }
    
    void printInfo() const override {
        shape_->printInfo();
    }
    
    std::string getDescription() const override {
        return shape_->getDescription();
    }
};

// Concrete Decorators

// Border Decorator
class BorderDecorator : public ShapeDecorator {
private:
    int borderWidth_;
    std::string borderColor_;

public:
    BorderDecorator(std::unique_ptr<Shape> shape, int borderWidth = 2, const std::string& borderColor = "black")
        : ShapeDecorator(std::move(shape)), borderWidth_(borderWidth), borderColor_(borderColor) {}
    
    void draw() const override {
        std::cout << "Applying " << borderColor_ << " border (width: " << borderWidth_ << ") to ";
        shape_->draw();
    }
    
    std::string getDescription() const override {
        return "Border(" + borderColor_ + " width=" + std::to_string(borderWidth_) + ") around " + 
               shape_->getDescription();
    }
    
    void printInfo() const override {
        std::cout << "Border Decorator - Width: " << borderWidth_ 
                  << ", Color: " << borderColor_ << std::endl;
        shape_->printInfo();
    }
};

// Fill Decorator
class FillDecorator : public ShapeDecorator {
private:
    std::string fillColor_;
    bool filled_;

public:
    FillDecorator(std::unique_ptr<Shape> shape, const std::string& fillColor = "none", bool filled = true)
        : ShapeDecorator(std::move(shape)), fillColor_(fillColor), filled_(filled) {}
    
    void draw() const override {
        if (filled_) {
            std::cout << "Filling with " << fillColor_ << " before ";
        } else {
            std::cout << "Stroke only (no fill) before ";
        }
        shape_->draw();
    }
    
    std::string getDescription() const override {
        return "Fill(" + fillColor_ + ", filled=" + (filled_ ? "true" : "false") + ") for " + 
               shape_->getDescription();
    }
    
    void printInfo() const override {
        std::cout << "Fill Decorator - Color: " << fillColor_ 
                  << ", Filled: " << (filled_ ? "Yes" : "No") << std::endl;
        shape_->printInfo();
    }
};

// Shadow Decorator
class ShadowDecorator : public ShapeDecorator {
private:
    int shadowDistance_;
    std::string shadowColor_;

public:
    ShadowDecorator(std::unique_ptr<Shape> shape, int distance = 5, const std::string& color = "gray")
        : ShapeDecorator(std::move(shape)), shadowDistance_(distance), shadowColor_(color) {}
    
    void draw() const override {
        std::cout << "Drawing shadow (" << shadowColor_ << " at distance " << shadowDistance_ << ") for ";
        shape_->draw();
    }
    
    std::string getDescription() const override {
        return "Shadow(" + shadowColor_ + " distance=" + std::to_string(shadowDistance_) + ") under " + 
               shape_->getDescription();
    }
    
    void printInfo() const override {
        std::cout << "Shadow Decorator - Distance: " << shadowDistance_ 
                  << ", Color: " << shadowColor_ << std::endl;
        shape_->printInfo();
    }
};

// Gradient Decorator
class GradientDecorator : public ShapeDecorator {
private:
    std::string startColor_;
    std::string endColor_;
    std::string direction_; // horizontal, vertical, radial

public:
    GradientDecorator(std::unique_ptr<Shape> shape, const std::string& start = "white", 
                     const std::string& end = "black", const std::string& dir = "horizontal")
        : ShapeDecorator(std::move(shape)), startColor_(start), endColor_(end), direction_(dir) {}
    
    void draw() const override {
        std::cout << "Applying " << direction_ << " gradient from " << startColor_ 
                  << " to " << endColor_ << " on ";
        shape_->draw();
    }
    
    std::string getDescription() const override {
        return "Gradient(" + direction_ + " from " + startColor_ + " to " + endColor_ + ") for " + 
               shape_->getDescription();
    }
    
    void printInfo() const override {
        std::cout << "Gradient Decorator - Direction: " << direction_ 
                  << ", From: " << startColor_ << ", To: " << endColor_ << std::endl;
        shape_->printInfo();
    }
};

// Animation Decorator
class AnimationDecorator : public ShapeDecorator {
private:
    std::string animationType_;
    double animationSpeed_;

public:
    AnimationDecorator(std::unique_ptr<Shape> shape, const std::string& type = "none", double speed = 1.0)
        : ShapeDecorator(std::move(shape)), animationType_(type), animationSpeed_(speed) {}
    
    void draw() const override {
        if (animationType_ != "none") {
            std::cout << "Animating (" << animationType_ << " at " << animationSpeed_ << "x speed) ";
        }
        shape_->draw();
    }
    
    std::string getDescription() const override {
        return "Animation(" + animationType_ + " @" + std::to_string(animationSpeed_) + "x) for " + 
               shape_->getDescription();
    }
    
    void printInfo() const override {
        std::cout << "Animation Decorator - Type: " << animationType_ 
                  << ", Speed: " << animationSpeed_ << "x" << std::endl;
        shape_->printInfo();
    }
};

// Scale Decorator
class ScaleDecorator : public ShapeDecorator {
private:
    double scaleX_, scaleY_;

public:
    ScaleDecorator(std::unique_ptr<Shape> shape, double sx = 1.0, double sy = 1.0)
        : ShapeDecorator(std::move(shape)), scaleX_(sx), scaleY_(sy) {}
    
    void draw() const override {
        std::cout << "Scaling by (" << scaleX_ << "," << scaleY_ << ") the ";
        shape_->draw();
    }
    
    double getArea() const override {
        return shape_->getArea() * scaleX_ * scaleY_;
    }
    
    std::string getDescription() const override {
        return "Scaled(" + std::to_string(scaleX_) + "," + std::to_string(scaleY_) + ") " + 
               shape_->getDescription();
    }
    
    void printInfo() const override {
        std::cout << "Scale Decorator - Scale X: " << scaleX_ 
                  << ", Scale Y: " << scaleY_ << std::endl;
        shape_->printInfo();
    }
};

// Multiple Decorator Base for combining multiple effects
class MultiDecorator : public ShapeDecorator {
protected:
    std::vector<std::string> effects_;

public:
    explicit MultiDecorator(std::unique_ptr<Shape> shape) : ShapeDecorator(std::move(shape)) {}
    
    std::string getDescription() const override {
        std::string baseDesc = shape_->getDescription();
        std::string effectStr = "";
        for (const auto& effect : effects_) {
            effectStr += effect + " + ";
        }
        if (!effectStr.empty()) {
            effectStr = effectStr.substr(0, effectStr.length() - 3); // Remove trailing " + "
            return "MultiDecorated(" + effectStr + ") " + baseDesc;
        }
        return baseDesc;
    }
    
    void addEffect(const std::string& effect) {
        effects_.push_back(effect);
    }
};

// Fluent Decorator - allows chaining decorations fluently
class FluentDecorator {
private:
    std::unique_ptr<Shape> shape_;

public:
    explicit FluentDecorator(std::unique_ptr<Shape> shape) : shape_(std::move(shape)) {}

    FluentDecorator& border(int width = 2, const std::string& color = "black") {
        shape_ = std::make_unique<BorderDecorator>(std::move(shape_), width, color);
        return *this;
    }

    FluentDecorator& fill(const std::string& color = "blue", bool filled = true) {
        shape_ = std::make_unique<FillDecorator>(std::move(shape_), color, filled);
        return *this;
    }

    FluentDecorator& shadow(int distance = 5, const std::string& color = "gray") {
        shape_ = std::make_unique<ShadowDecorator>(std::move(shape_), distance, color);
        return *this;
    }

    FluentDecorator& gradient(const std::string& start = "white",
                             const std::string& end = "black",
                             const std::string& dir = "horizontal") {
        shape_ = std::make_unique<GradientDecorator>(std::move(shape_), start, end, dir);
        return *this;
    }

    FluentDecorator& scale(double sx = 1.0, double sy = 1.0) {
        shape_ = std::make_unique<ScaleDecorator>(std::move(shape_), sx, sy);
        return *this;
    }

    FluentDecorator& animate(const std::string& type = "pulse", double speed = 1.0) {
        shape_ = std::make_unique<AnimationDecorator>(std::move(shape_), type, speed);
        return *this;
    }

    std::unique_ptr<Shape> build() {
        return std::move(shape_);
    }

    void draw() const {
        if (shape_) {
            shape_->draw();
        }
    }

    std::string getDescription() const {
        if (shape_) {
            return shape_->getDescription();
        }
        return "Empty Shape";
    }
};

// Advanced Decorator: Compound Decorator that applies multiple decorators at once
class CompoundDecorator : public ShapeDecorator {
private:
    bool hasBorder_, hasFill_, hasShadow_, hasAnimation_;
    int borderWidth_;
    std::string borderColor_, fillColor_, shadowColor_, animationType_;
    double shadowDistance_, animationSpeed_;
    bool filled_;

public:
    CompoundDecorator(std::unique_ptr<Shape> shape,
                     bool border = false, int borderWidth = 2, const std::string& borderColor = "black",
                     bool fill = false, const std::string& fillColor = "white", bool filled = true,
                     bool shadow = false, int shadowDistance = 5, const std::string& shadowColor = "gray",
                     bool animation = false, const std::string& animationType = "pulse", double animationSpeed = 1.0)
        : ShapeDecorator(std::move(shape)),
          hasBorder_(border), hasFill_(fill), hasShadow_(shadow), hasAnimation_(animation),
          borderWidth_(borderWidth), borderColor_(borderColor), fillColor_(fillColor),
          shadowColor_(shadowColor), animationType_(animationType), animationSpeed_(animationSpeed),
          filled_(filled) {}
    
    void draw() const override {
        // Apply decorators in a logical order
        if (hasShadow_) {
            std::cout << "[SHADOW: " << shadowColor_ << "@" << shadowDistance_ << "] ";
        }
        if (hasFill_ && filled_) {
            std::cout << "[FILL: " << fillColor_ << "] ";
        }
        if (hasBorder_) {
            std::cout << "[BORDER: " << borderColor_ << "#" << borderWidth_ << "] ";
        }
        if (hasAnimation_) {
            std::cout << "[ANIM: " << animationType_ << "@" << animationSpeed_ << "x] ";
        }
        
        // Draw the base shape
        shape_->draw();
    }
    
    std::string getDescription() const override {
        std::string desc = shape_->getDescription();
        if (hasFill_) desc = "Filled(" + fillColor_ + ") " + desc;
        if (hasBorder_) desc = "Bordered(" + borderColor_ + "#" + std::to_string(borderWidth_) + ") " + desc;
        if (hasShadow_) desc = "Shadowed(" + shadowColor_ + "@" + std::to_string(shadowDistance_) + ") " + desc;
        if (hasAnimation_) desc = "Animated(" + animationType_ + "@" + std::to_string(animationSpeed_) + "x) " + desc;
        return desc;
    }
    
    void printInfo() const override {
        if (hasBorder_) {
            std::cout << "Compound Decorator - Border: " << borderColor_ << " width: " << borderWidth_ << std::endl;
        }
        if (hasFill_) {
            std::cout << "Compound Decorator - Fill: " << fillColor_ << " filled: " << (filled_ ? "true" : "false") << std::endl;
        }
        if (hasShadow_) {
            std::cout << "Compound Decorator - Shadow: " << shadowColor_ << " distance: " << shadowDistance_ << std::endl;
        }
        if (hasAnimation_) {
            std::cout << "Compound Decorator - Animation: " << animationType_ << " speed: " << animationSpeed_ << "x" << std::endl;
        }
        shape_->printInfo();
    }
    
    double getArea() const override {
        // Only scale affects area calculation significantly
        return shape_->getArea(); // For compound, basic area from core shape
    }
};

// Decorator Factory for easy decorator creation
class DecoratorFactory {
public:
    static std::unique_ptr<Shape> addBorder(std::unique_ptr<Shape> shape, int width = 2, const std::string& color = "black") {
        return std::make_unique<BorderDecorator>(std::move(shape), width, color);
    }
    
    static std::unique_ptr<Shape> addFill(std::unique_ptr<Shape> shape, const std::string& color = "white", bool filled = true) {
        return std::make_unique<FillDecorator>(std::move(shape), color, filled);
    }
    
    static std::unique_ptr<Shape> addShadow(std::unique_ptr<Shape> shape, int distance = 5, const std::string& color = "gray") {
        return std::make_unique<ShadowDecorator>(std::move(shape), distance, color);
    }
    
    static std::unique_ptr<Shape> addScale(std::unique_ptr<Shape> shape, double sx = 1.0, double sy = 1.0) {
        return std::make_unique<ScaleDecorator>(std::move(shape), sx, sy);
    }
    
    static std::unique_ptr<Shape> addGradient(std::unique_ptr<Shape> shape, const std::string& start, const std::string& end, const std::string& dir) {
        return std::make_unique<GradientDecorator>(std::move(shape), start, end, dir);
    }
    
    static std::unique_ptr<Shape> addAnimation(std::unique_ptr<Shape> shape, const std::string& type, double speed) {
        return std::make_unique<AnimationDecorator>(std::move(shape), type, speed);
    }
    
    // Compound decorator that adds multiple effects
    static std::unique_ptr<Shape> addEffects(std::unique_ptr<Shape> shape,
                                            bool border = false, int borderWidth = 2, const std::string& borderCol = "black",
                                            bool fill = false, const std::string& fillCol = "white", bool filled = true,
                                            bool shadow = false, int shadowDist = 5, const std::string& shadowCol = "gray") {
        std::unique_ptr<Shape> decoratedShape = std::move(shape);
        
        if (fill) {
            decoratedShape = std::make_unique<FillDecorator>(std::move(decoratedShape), fillCol, filled);
        }
        if (border) {
            decoratedShape = std::make_unique<BorderDecorator>(std::move(decoratedShape), borderWidth, borderCol);
        }
        if (shadow) {
            decoratedShape = std::make_unique<ShadowDecorator>(std::move(decoratedShape), shadowDist, shadowCol);
        }
        
        return decoratedShape;
    }
};

#endif // DECORATOR_PATTERNS_H