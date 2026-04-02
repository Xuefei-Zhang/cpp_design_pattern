#ifndef BRIDGE_PATTERNS_H
#define BRIDGE_PATTERNS_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <cmath>

// Implementation hierarchy - rendering implementations
class ShapeRenderer {
public:
    virtual ~ShapeRenderer() = default;
    virtual void render(const std::string& shapeType, double size, const std::string& color) const = 0;
    virtual std::string getRendererType() const = 0;
};

// Concrete implementations of rendering
class VectorRenderer : public ShapeRenderer {
public:
    void render(const std::string& shapeType, double size, const std::string& color) const override {
        std::cout << "[VECTOR] Rendering " << shapeType << " with size " << size 
                  << " and color " << color << " using vector graphics" << std::endl;
    }
    
    std::string getRendererType() const override {
        return "VectorRenderer";
    }
};

class RasterRenderer : public ShapeRenderer {
public:
    void render(const std::string& shapeType, double size, const std::string& color) const override {
        std::cout << "[RASTER] Rendering " << shapeType << " with size " << size 
                  << " and color " << color << " using raster graphics" << std::endl;
    }
    
    std::string getRendererType() const override {
        return "RasterRenderer";
    }
};

class SVGRenderer : public ShapeRenderer {
public:
    void render(const std::string& shapeType, double size, const std::string& color) const override {
        std::cout << "[SVG] Rendering " << shapeType << " with size " << size 
                  << " and color " << color << " as SVG" << std::endl;
    }
    
    std::string getRendererType() const override {
        return "SVGRenderer";
    }
};

// Abstraction hierarchy - shape abstractions
class Shape {
protected:
    std::unique_ptr<ShapeRenderer> renderer_;

public:
    explicit Shape(std::unique_ptr<ShapeRenderer> renderer) : renderer_(std::move(renderer)) {}
    
    virtual ~Shape() = default;
    virtual void draw() const = 0;
    virtual double getArea() const = 0;
    virtual std::string getType() const = 0;
    
    // Bridge method - allows changing renderer at runtime
    void setRenderer(std::unique_ptr<ShapeRenderer> newRenderer) {
        renderer_ = std::move(newRenderer);
    }
    
    const ShapeRenderer* getRenderer() const {
        return renderer_.get();
    }
};

// Refined abstractions
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
    
    double getArea() const override {
        return 3.14159 * radius_ * radius_;
    }
    
    std::string getType() const override {
        return "Circle";
    }
    
    double getRadius() const { return radius_; }
    void setRadius(double radius) { radius_ = radius; }
};

class Rectangle : public Shape {
private:
    double width_, height_;
    std::string color_;

public:
    Rectangle(double width, double height, const std::string& color, std::unique_ptr<ShapeRenderer> renderer)
        : Shape(std::move(renderer)), width_(width), height_(height), color_(color) {}

    void draw() const override {
        renderer_->render("Rectangle", width_, color_);
    }
    
    double getArea() const override {
        return width_ * height_;
    }
    
    std::string getType() const override {
        return "Rectangle";
    }
    
    double getWidth() const { return width_; }
    double getHeight() const { return height_; }
    void setDimensions(double width, double height) { 
        width_ = width; 
        height_ = height; 
    }
};

class Triangle : public Shape {
private:
    double base_, height_;
    std::string color_;

public:
    Triangle(double base, double height, const std::string& color, std::unique_ptr<ShapeRenderer> renderer)
        : Shape(std::move(renderer)), base_(base), height_(height), color_(color) {}

    void draw() const override {
        renderer_->render("Triangle", base_, color_);
    }
    
    double getArea() const override {
        return 0.5 * base_ * height_;
    }
    
    std::string getType() const override {
        return "Triangle";
    }
    
    double getBase() const { return base_; }
    double getHeight() const { return height_; }
    void setDimensions(double base, double height) { 
        base_ = base; 
        height_ = height; 
    }
};

// Advanced Bridge: Theme-based rendering
class ThemedShapeRenderer : public ShapeRenderer {
protected:
    std::string themeName_;

public:
    explicit ThemedShapeRenderer(const std::string& theme) : themeName_(theme) {}
    
    std::string getTheme() const { return themeName_; }
    
    void render(const std::string& shapeType, double size, const std::string& color) const override {
        std::cout << "[" << themeName_ << "] Rendering " << shapeType 
                  << " with size " << size << " and color " << color << std::endl;
    }
    
    std::string getRendererType() const override {
        return "ThemedRenderer(" + themeName_ + ")";
    }
};

class ModernRenderer : public ThemedShapeRenderer {
public:
    ModernRenderer() : ThemedShapeRenderer("Modern") {}
    
    void render(const std::string& shapeType, double size, const std::string& color) const override {
        std::cout << "[MODERN] Rendering " << shapeType 
                  << " with sleek design, size=" << size << ", color=" << color << std::endl;
    }
};

class VintageRenderer : public ThemedShapeRenderer {
public:
    VintageRenderer() : ThemedShapeRenderer("Vintage") {}
    
    void render(const std::string& shapeType, double size, const std::string& color) const override {
        std::cout << "[VINTAGE] Rendering " << shapeType 
                  << " with classic design, size=" << size << ", color=" << color << std::endl;
    }
};

// Advanced abstraction with more complex shapes
class ComplexShape : public Shape {
protected:
    std::string shapeName_;

public:
    ComplexShape(const std::string& name, std::unique_ptr<ShapeRenderer> renderer)
        : Shape(std::move(renderer)), shapeName_(name) {}

    std::string getType() const override {
        return shapeName_;
    }
    
    void setShapeName(const std::string& name) {
        shapeName_ = name;
    }
    
    // Method to demonstrate runtime renderer switching
    void switchRendererTo(const std::string& newRendererType) {
        if (newRendererType == "vector") {
            setRenderer(std::make_unique<VectorRenderer>());
        } else if (newRendererType == "raster") {
            setRenderer(std::make_unique<RasterRenderer>());
        } else if (newRendererType == "svg") {
            setRenderer(std::make_unique<SVGRenderer>());
        } else if (newRendererType == "modern") {
            setRenderer(std::make_unique<ModernRenderer>());
        } else if (newRendererType == "vintage") {
            setRenderer(std::make_unique<VintageRenderer>());
        }
    }
};

class Polygon : public ComplexShape {
private:
    int sides_;
    double size_;

public:
    Polygon(int sides, double size, std::unique_ptr<ShapeRenderer> renderer)
        : ComplexShape("Polygon", std::move(renderer)), sides_(sides), size_(size) {}

    void draw() const override {
        renderer_->render("Polygon(" + std::to_string(sides_) + " sides)", size_, "default");
    }
    
    double getArea() const override {
        // Simplified calculation for regular polygon
        return sides_ * size_ * size_ / (4 * std::tan(3.14159 / sides_));
    }
    
    int getSides() const { return sides_; }
    double getSize() const { return size_; }
};

// Bridge for animations (separating animation from shape)
class AnimationRenderer : public ShapeRenderer {
protected:
    double speed_;

public:
    explicit AnimationRenderer(double speed = 1.0) : speed_(speed) {}
    
    void setSpeed(double speed) { speed_ = speed; }
    double getSpeed() const { return speed_; }
    
    std::string getRendererType() const override {
        return "AnimationRenderer";
    }
};

class PulsingAnimation : public AnimationRenderer {
public:
    PulsingAnimation(double speed = 1.0) : AnimationRenderer(speed) {}
    
    void render(const std::string& shapeType, double size, const std::string& color) const override {
        std::cout << "[PULSING ANIMATION] " << shapeType << " pulsing with size=" 
                  << size << ", color=" << color << ", speed=" << speed_ << std::endl;
    }
};

class SpinningAnimation : public AnimationRenderer {
public:
    SpinningAnimation(double speed = 1.0) : AnimationRenderer(speed) {}
    
    void render(const std::string& shapeType, double size, const std::string& color) const override {
        std::cout << "[SPINNING ANIMATION] " << shapeType << " spinning with size=" 
                  << size << ", color=" << color << ", speed=" << speed_ << std::endl;
    }
};

// Shape with animation capability
class AnimatedShape {
protected:
    std::unique_ptr<Shape> baseShape_;

public:
    explicit AnimatedShape(std::unique_ptr<Shape> shape) : baseShape_(std::move(shape)) {}
    
    virtual ~AnimatedShape() = default;
    
    void animate() const {
        // Use the shape's renderer to render with animation
        // Use typeid to check the renderer type without dynamic_cast issues
        if (dynamic_cast<const AnimationRenderer*>(baseShape_->getRenderer())) {
            std::cout << "Animating shape: ";
            baseShape_->draw(); // Will use animation renderer
        } else {
            std::cout << "Shape doesn't have animation capability" << std::endl;
        }
    }
    
    void changeAnimation(std::unique_ptr<AnimationRenderer> newAnim) {
        baseShape_->setRenderer(std::unique_ptr<ShapeRenderer>(std::move(newAnim)));
    }
    
    Shape* getBaseShape() const { return baseShape_.get(); }
};

// Bridge pattern manager/director for complex scenarios
class BridgeManager {
private:
    std::vector<std::unique_ptr<Shape>> shapes_;
    
public:
    void addShape(std::unique_ptr<Shape> shape) {
        shapes_.push_back(std::move(shape));
    }
    
    void renderAll() const {
        std::cout << "Rendering all shapes:" << std::endl;
        for (const auto& shape : shapes_) {
            shape->draw();
        }
    }
    
    void switchAllRenderers(std::unique_ptr<ShapeRenderer> newRenderer) {
        std::string newRendererType = newRenderer->getRendererType();
        for (auto& shape : shapes_) {
            if (newRendererType == "VectorRenderer") {
                shape->setRenderer(std::make_unique<VectorRenderer>());
            } else if (newRendererType == "RasterRenderer") {
                shape->setRenderer(std::make_unique<RasterRenderer>());
            } else if (newRendererType == "SVGRenderer") {
                shape->setRenderer(std::make_unique<SVGRenderer>());
            } else if (newRendererType == "ThemedRenderer(Modern)") {
                shape->setRenderer(std::make_unique<ModernRenderer>());
            } else {
                shape->setRenderer(std::make_unique<VintageRenderer>());
            }
        }
    }
    
    size_t getShapeCount() const { return shapes_.size(); }
    
    void printRendererInfo() const {
        for (size_t i = 0; i < shapes_.size(); ++i) {
            std::cout << "Shape " << i + 1 << " (" << shapes_[i]->getType() 
                      << ") uses: " << shapes_[i]->getRenderer()->getRendererType() << std::endl;
        }
    }
};

#endif // BRIDGE_PATTERNS_H