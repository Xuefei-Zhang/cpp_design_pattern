#ifndef BUILDER_SHAPES_H
#define BUILDER_SHAPES_H

#include "shape_builder.h"
#include <memory>

// Director class that uses a builder to create complex shapes
class ShapeDirector {
private:
    std::unique_ptr<ShapeBuilder> builder_;

public:
    explicit ShapeDirector(std::unique_ptr<ShapeBuilder> b) : builder_(std::move(b)) {}
    
    void setBuilder(std::unique_ptr<ShapeBuilder> b) {
        builder_ = std::move(b);
    }
    
    // Create a standard circle with predefined specifications
    std::unique_ptr<Shape> createStandardCircle() {
        auto circleBuilder = dynamic_cast<CircleBuilder*>(builder_.get());
        if (circleBuilder) {
            circleBuilder->setRadius(5.0)
                         ->setColor("blue")
                         ->setStyle("dashed")
                         ->setFilled(false);
            return builder_->getResult();
        }
        return nullptr;
    }
    
    // Create a large filled circle
    std::unique_ptr<Shape> createLargeFilledCircle() {
        auto circleBuilder = dynamic_cast<CircleBuilder*>(builder_.get());
        if (circleBuilder) {
            circleBuilder->setRadius(10.0)
                         ->setColor("red")
                         ->setStyle("solid")
                         ->setFilled(true);
            return builder_->getResult();
        }
        return nullptr;
    }
    
    // Create a complex rectangle
    std::unique_ptr<Shape> createComplexRectangle() {
        auto rectBuilder = dynamic_cast<RectangleBuilder*>(builder_.get());
        if (rectBuilder) {
            rectBuilder->setWidth(15.0)
                       ->setHeight(8.0)
                       ->setColor("yellow")
                       ->setStyle("dotted")
                       ->setFilled(true)
                       ->setBorderWidth(3);
            return builder_->getResult();
        }
        return nullptr;
    }
    
    // Create a labeled triangle
    std::unique_ptr<Shape> createLabeledTriangle() {
        auto triBuilder = dynamic_cast<TriangleBuilder*>(builder_.get());
        if (triBuilder) {
            triBuilder->setBase(12.0)
                      ->setHeight(9.0)
                      ->setColor("green")
                      ->setStyle("solid")
                      ->setFilled(false)
                      ->setLabel("Right Triangle");
            return builder_->getResult();
        }
        return nullptr;
    }
};

// Fluent Builder implementation for easier use
class FluentShapeBuilder {
private:
    std::unique_ptr<Shape> currentShape_;

public:
    // Build a customized circle fluently
    std::unique_ptr<Shape> buildCircle(double radius, const std::string& color = "red", 
                                       const std::string& style = "solid", bool filled = true) {
        currentShape_ = std::make_unique<Circle>(radius, color, style, filled);
        return std::move(currentShape_);
    }
    
    // Build a customized rectangle fluently
    std::unique_ptr<Shape> buildRectangle(double width, double height, const std::string& color = "blue", 
                                          const std::string& style = "solid", bool filled = true, int borderWidth = 1) {
        currentShape_ = std::make_unique<Rectangle>(width, height, color, style, filled, borderWidth);
        return std::move(currentShape_);
    }
    
    // Build a customized triangle fluently
    std::unique_ptr<Shape> buildTriangle(double base, double height, const std::string& color = "green", 
                                         const std::string& style = "solid", bool filled = true, 
                                         const std::string& label = "") {
        currentShape_ = std::make_unique<Triangle>(base, height, color, style, filled, label);
        return std::move(currentShape_);
    }
    
    // Advanced fluent builder for Circle
    class CircleFluentBuilder {
    private:
        CircleBuilder builder_;
        
    public:
        CircleFluentBuilder& radius(double r) {
            builder_.setRadius(r);
            return *this;
        }
        
        CircleFluentBuilder& color(const std::string& c) {
            builder_.setColor(c);
            return *this;
        }
        
        CircleFluentBuilder& style(const std::string& s) {
            builder_.setStyle(s);
            return *this;
        }
        
        CircleFluentBuilder& filled(bool f) {
            builder_.setFilled(f);
            return *this;
        }
        
        std::unique_ptr<Shape> build() {
            return builder_.getResult();
        }
    };
    
    // Advanced fluent builder for Rectangle
    class RectangleFluentBuilder {
    private:
        RectangleBuilder builder_;
        
    public:
        RectangleFluentBuilder& width(double w) {
            builder_.setWidth(w);
            return *this;
        }
        
        RectangleFluentBuilder& height(double h) {
            builder_.setHeight(h);
            return *this;
        }
        
        RectangleFluentBuilder& color(const std::string& c) {
            builder_.setColor(c);
            return *this;
        }
        
        RectangleFluentBuilder& style(const std::string& s) {
            builder_.setStyle(s);
            return *this;
        }
        
        RectangleFluentBuilder& filled(bool f) {
            builder_.setFilled(f);
            return *this;
        }
        
        RectangleFluentBuilder& borderWidth(int w) {
            builder_.setBorderWidth(w);
            return *this;
        }
        
        std::unique_ptr<Shape> build() {
            return builder_.getResult();
        }
    };
    
    // Advanced fluent builder for Triangle
    class TriangleFluentBuilder {
    private:
        TriangleBuilder builder_;
        
    public:
        TriangleFluentBuilder& base(double b) {
            builder_.setBase(b);
            return *this;
        }
        
        TriangleFluentBuilder& height(double h) {
            builder_.setHeight(h);
            return *this;
        }
        
        TriangleFluentBuilder& color(const std::string& c) {
            builder_.setColor(c);
            return *this;
        }
        
        TriangleFluentBuilder& style(const std::string& s) {
            builder_.setStyle(s);
            return *this;
        }
        
        TriangleFluentBuilder& filled(bool f) {
            builder_.setFilled(f);
            return *this;
        }
        
        TriangleFluentBuilder& label(const std::string& l) {
            builder_.setLabel(l);
            return *this;
        }
        
        std::unique_ptr<Shape> build() {
            return builder_.getResult();
        }
    };
    
    // Get fluent builders
    CircleFluentBuilder createCircleBuilder() { return CircleFluentBuilder(); }
    RectangleFluentBuilder createRectangleBuilder() { return RectangleFluentBuilder(); }
    TriangleFluentBuilder createTriangleBuilder() { return TriangleFluentBuilder(); }
};

// Builder for complex shapes - composite shapes made of multiple simple shapes
class ComplexShapeBuilder : public ShapeBuilder {
private:
    std::vector<std::unique_ptr<Shape>> shapes_;
    std::string name_;

public:
    ComplexShapeBuilder(const std::string& name = "Complex Shape") : name_(name) {}
    
    void buildShape() override {
        // Complex shape doesn't need building, it's just a container
    }
    
    std::unique_ptr<Shape> getResult() override {
        // For this example, we'll return a placeholder since we can't have composite shapes directly
        // In practice, you might want to implement a CompositeShape class
        std::cout << "Building complex shape: " << name_ << " with " << shapes_.size() << " components" << std::endl;
        for (auto& shape : shapes_) {
            shape->draw();
        }
        return nullptr; // Placeholder for complex shape
    }
    
    ComplexShapeBuilder* addShape(std::unique_ptr<Shape> shape) {
        shapes_.push_back(std::move(shape));
        return this;
    }
    
    ComplexShapeBuilder* setName(const std::string& name) {
        name_ = name;
        return this;
    }
    
    size_t getShapeCount() const { return shapes_.size(); }
};

// Abstract Factory that can work with Builder pattern (combining patterns)
class ShapeFactoryBuilder {
public:
    static std::unique_ptr<CircleBuilder> createCircleBuilder() {
        return std::make_unique<CircleBuilder>();
    }
    
    static std::unique_ptr<RectangleBuilder> createRectangleBuilder() {
        return std::make_unique<RectangleBuilder>();
    }
    
    static std::unique_ptr<TriangleBuilder> createTriangleBuilder() {
        return std::make_unique<TriangleBuilder>();
    }
};

#endif // BUILDER_SHAPES_H