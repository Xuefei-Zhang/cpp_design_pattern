#ifndef ABSTRACT_FACTORY_SHAPES_H
#define ABSTRACT_FACTORY_SHAPES_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>

// Abstract Product A: Basic Shapes
class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw() const = 0;
    virtual std::string getType() const = 0;
    virtual double getArea() const = 0;
    virtual void printInfo() const = 0;
};

// Abstract Product B: Renderers for shapes
class ShapeRenderer {
public:
    virtual ~ShapeRenderer() = default;
    virtual void render(const Shape& shape) const = 0;
    virtual std::string getRendererType() const = 0;
    virtual void setRenderQuality(const std::string& quality) = 0;
};

// Abstract Product C: Shape Animators
class ShapeAnimator {
public:
    virtual ~ShapeAnimator() = default;
    virtual void animate(const Shape& shape) const = 0;
    virtual std::string getAnimationType() const = 0;
    virtual void setAnimationSpeed(double speed) = 0;
};

// Concrete Product A1: Basic Circle
class BasicCircle : public Shape {
private:
    double radius_;
    std::string color_;

public:
    BasicCircle(double radius, const std::string& color = "red") 
        : radius_(radius), color_(color) {}
    
    void draw() const override {
        std::cout << "Drawing Basic Circle: radius=" << radius_ << ", color=" << color_ << std::endl;
    }
    
    std::string getType() const override {
        return "BasicCircle";
    }
    
    double getArea() const override {
        return 3.14159 * radius_ * radius_;
    }
    
    void printInfo() const override {
        std::cout << "BasicCircle - Radius: " << radius_ << ", Color: " << color_ << std::endl;
    }
};

// Concrete Product A2: Basic Rectangle
class BasicRectangle : public Shape {
private:
    double width_, height_;
    std::string color_;

public:
    BasicRectangle(double width, double height, const std::string& color = "blue") 
        : width_(width), height_(height), color_(color) {}
    
    void draw() const override {
        std::cout << "Drawing Basic Rectangle: " << width_ << "x" << height_ 
                  << ", color=" << color_ << std::endl;
    }
    
    std::string getType() const override {
        return "BasicRectangle";
    }
    
    double getArea() const override {
        return width_ * height_;
    }
    
    void printInfo() const override {
        std::cout << "BasicRectangle - Width: " << width_ 
                  << ", Height: " << height_
                  << ", Color: " << color_ << std::endl;
    }
};

// Concrete Product A3: Basic Triangle
class BasicTriangle : public Shape {
private:
    double base_, height_;
    std::string color_;

public:
    BasicTriangle(double base, double height, const std::string& color = "green") 
        : base_(base), height_(height), color_(color) {}
    
    void draw() const override {
        std::cout << "Drawing Basic Triangle: base=" << base_ << ", height=" << height_ 
                  << ", color=" << color_ << std::endl;
    }
    
    std::string getType() const override {
        return "BasicTriangle";
    }
    
    double getArea() const override {
        return 0.5 * base_ * height_;
    }
    
    void printInfo() const override {
        std::cout << "BasicTriangle - Base: " << base_ 
                  << ", Height: " << height_
                  << ", Color: " << color_ << std::endl;
    }
};

// Concrete Product B1: Basic Renderer
class BasicShapeRenderer : public ShapeRenderer {
private:
    std::string quality_;

public:
    BasicShapeRenderer(const std::string& quality = "standard") : quality_(quality) {}
    
    void render(const Shape& shape) const override {
        std::cout << "Basic rendering of " << shape.getType() 
                  << " with quality: " << quality_ << std::endl;
        shape.draw();
    }
    
    std::string getRendererType() const override {
        return "BasicRenderer";
    }
    
    void setRenderQuality(const std::string& quality) override {
        quality_ = quality;
    }
};

// Concrete Product B2: Advanced Renderer
class AdvancedShapeRenderer : public ShapeRenderer {
private:
    std::string quality_;

public:
    AdvancedShapeRenderer(const std::string& quality = "high") : quality_(quality) {}
    
    void render(const Shape& shape) const override {
        std::cout << "Advanced rendering of " << shape.getType() 
                  << " with quality: " << quality_ << " (shadows, textures)" << std::endl;
        shape.draw();
    }
    
    std::string getRendererType() const override {
        return "AdvancedRenderer";
    }
    
    void setRenderQuality(const std::string& quality) override {
        quality_ = quality;
    }
};

// Concrete Product C1: Basic Animator
class BasicShapeAnimator : public ShapeAnimator {
private:
    double speed_;

public:
    BasicShapeAnimator(double speed = 1.0) : speed_(speed) {}
    
    void animate(const Shape& shape) const override {
        std::cout << "Basic animation of " << shape.getType() 
                  << " at speed: " << speed_ << "x" << std::endl;
    }
    
    std::string getAnimationType() const override {
        return "BasicAnimation";
    }
    
    void setAnimationSpeed(double speed) override {
        speed_ = speed;
    }
};

// Concrete Product C2: Smooth Animator
class SmoothShapeAnimator : public ShapeAnimator {
private:
    double speed_;

public:
    SmoothShapeAnimator(double speed = 1.0) : speed_(speed) {}
    
    void animate(const Shape& shape) const override {
        std::cout << "Smooth animation of " << shape.getType() 
                  << " at speed: " << speed_ << "x (with easing)" << std::endl;
    }
    
    std::string getAnimationType() const override {
        return "SmoothAnimation";
    }
    
    void setAnimationSpeed(double speed) override {
        speed_ = speed;
    }
};

// Abstract Factory Interface
class ShapeAbstractFactory {
public:
    virtual ~ShapeAbstractFactory() = default;
    
    // Product A Creation Methods
    virtual std::unique_ptr<Shape> createCircle(double radius, const std::string& color = "red") const = 0;
    virtual std::unique_ptr<Shape> createRectangle(double width, double height, const std::string& color = "blue") const = 0;
    virtual std::unique_ptr<Shape> createTriangle(double base, double height, const std::string& color = "green") const = 0;
    
    // Product B Creation Methods
    virtual std::unique_ptr<ShapeRenderer> createRenderer(const std::string& quality = "standard") const = 0;
    
    // Product C Creation Methods
    virtual std::unique_ptr<ShapeAnimator> createAnimator(double speed = 1.0) const = 0;
    
    // Convenience method to create a complete shape system
    virtual std::tuple<std::unique_ptr<Shape>, std::unique_ptr<ShapeRenderer>, std::unique_ptr<ShapeAnimator>>
    createShapeSystem(double size, const std::string& type, const std::string& color = "default") const {
        std::unique_ptr<Shape> shape;
        if (type == "circle") {
            shape = createCircle(size, color);
        } else if (type == "rectangle") {
            shape = createRectangle(size, size * 0.8, color);
        } else if (type == "triangle") {
            shape = createTriangle(size, size * 0.7, color);
        } else {
            shape = createCircle(size, color);
        }

        auto renderer = createRenderer();
        auto animator = createAnimator();

        return std::make_tuple(std::move(shape), std::move(renderer), std::move(animator));
    }

    // Alternative method without structured bindings for C++14 compatibility
    virtual void createShapeSystem(double size, const std::string& type,
                                   std::unique_ptr<Shape>& out_shape,
                                   std::unique_ptr<ShapeRenderer>& out_renderer,
                                   std::unique_ptr<ShapeAnimator>& out_animator,
                                   const std::string& color = "default") const {
        if (type == "circle") {
            out_shape = createCircle(size, color);
        } else if (type == "rectangle") {
            out_shape = createRectangle(size, size * 0.8, color);
        } else if (type == "triangle") {
            out_shape = createTriangle(size, size * 0.7, color);
        } else {
            out_shape = createCircle(size, color);
        }

        out_renderer = createRenderer();
        out_animator = createAnimator();
    }
};

// Concrete Factory 1: Basic Theme Factory
class BasicThemeFactory : public ShapeAbstractFactory {
public:
    std::unique_ptr<Shape> createCircle(double radius, const std::string& color = "red") const override {
        return std::make_unique<BasicCircle>(radius, color);
    }
    
    std::unique_ptr<Shape> createRectangle(double width, double height, const std::string& color = "blue") const override {
        return std::make_unique<BasicRectangle>(width, height, color);
    }
    
    std::unique_ptr<Shape> createTriangle(double base, double height, const std::string& color = "green") const override {
        return std::make_unique<BasicTriangle>(base, height, color);
    }
    
    std::unique_ptr<ShapeRenderer> createRenderer(const std::string& quality = "standard") const override {
        return std::make_unique<BasicShapeRenderer>(quality);
    }
    
    std::unique_ptr<ShapeAnimator> createAnimator(double speed = 1.0) const override {
        return std::make_unique<BasicShapeAnimator>(speed);
    }
};

// Concrete Factory 2: Advanced Theme Factory
class AdvancedThemeFactory : public ShapeAbstractFactory {
public:
    std::unique_ptr<Shape> createCircle(double radius, const std::string& color = "red") const override {
        // Advanced circles might have special properties
        auto circle = std::make_unique<BasicCircle>(radius * 1.1, color); // Slightly larger for advanced effect
        return std::move(circle);
    }
    
    std::unique_ptr<Shape> createRectangle(double width, double height, const std::string& color = "blue") const override {
        auto rect = std::make_unique<BasicRectangle>(width * 1.1, height * 1.1, color); // Slightly larger
        return std::move(rect);
    }
    
    std::unique_ptr<Shape> createTriangle(double base, double height, const std::string& color = "green") const override {
        auto tri = std::make_unique<BasicTriangle>(base * 1.1, height * 1.1, color); // Slightly larger
        return std::move(tri);
    }
    
    std::unique_ptr<ShapeRenderer> createRenderer(const std::string& quality = "high") const override {
        return std::make_unique<AdvancedShapeRenderer>(quality);
    }
    
    std::unique_ptr<ShapeAnimator> createAnimator(double speed = 1.2) const override {
        return std::make_unique<SmoothShapeAnimator>(speed);
    }
};

// Concrete Factory 3: Modern Theme Factory
class ModernThemeFactory : public ShapeAbstractFactory {
public:
    std::unique_ptr<Shape> createCircle(double radius, const std::string& color = "gray") const override {
        // Modern circles with different default color
        return std::make_unique<BasicCircle>(radius, color.empty() ? "white" : color);
    }
    
    std::unique_ptr<Shape> createRectangle(double width, double height, const std::string& color = "gray") const override {
        return std::make_unique<BasicRectangle>(width, height, color.empty() ? "white" : color);
    }
    
    std::unique_ptr<Shape> createTriangle(double base, double height, const std::string& color = "gray") const override {
        return std::make_unique<BasicTriangle>(base, height, color.empty() ? "white" : color);
    }
    
    std::unique_ptr<ShapeRenderer> createRenderer(const std::string& quality = "medium") const override {
        return std::make_unique<AdvancedShapeRenderer>(quality);
    }
    
    std::unique_ptr<ShapeAnimator> createAnimator(double speed = 0.8) const override {
        return std::make_unique<SmoothShapeAnimator>(speed);
    }
};

// Concrete Factory 4: Vintage Theme Factory
class VintageThemeFactory : public ShapeAbstractFactory {
public:
    std::unique_ptr<Shape> createCircle(double radius, const std::string& color = "brown") const override {
        return std::make_unique<BasicCircle>(radius, color.empty() ? "beige" : color);
    }
    
    std::unique_ptr<Shape> createRectangle(double width, double height, const std::string& color = "brown") const override {
        return std::make_unique<BasicRectangle>(width, height, color.empty() ? "beige" : color);
    }
    
    std::unique_ptr<Shape> createTriangle(double base, double height, const std::string& color = "brown") const override {
        return std::make_unique<BasicTriangle>(base, height, color.empty() ? "beige" : color);
    }
    
    std::unique_ptr<ShapeRenderer> createRenderer(const std::string& quality = "standard") const override {
        return std::make_unique<BasicShapeRenderer>(quality);
    }
    
    std::unique_ptr<ShapeAnimator> createAnimator(double speed = 0.5) const override {
        return std::make_unique<BasicShapeAnimator>(speed);
    }
};

// Factory Selector/Manager to demonstrate factory selection
class AbstractFactorySelector {
public:
    static std::unique_ptr<ShapeAbstractFactory> getFactory(const std::string& theme) {
        if (theme == "basic") {
            return std::make_unique<BasicThemeFactory>();
        } else if (theme == "advanced") {
            return std::make_unique<AdvancedThemeFactory>();
        } else if (theme == "modern") {
            return std::make_unique<ModernThemeFactory>();
        } else if (theme == "vintage") {
            return std::make_unique<VintageThemeFactory>();
        } else {
            return std::make_unique<BasicThemeFactory>(); // default
        }
    }
    
    static std::vector<std::string> getAvailableThemes() {
        return {"basic", "advanced", "modern", "vintage"};
    }
};

// Client code that uses the abstract factory
class ShapeSystemClient {
private:
    std::unique_ptr<ShapeAbstractFactory> factory_;

public:
    explicit ShapeSystemClient(std::unique_ptr<ShapeAbstractFactory> factory) 
        : factory_(std::move(factory)) {}
    
    void setFactory(std::unique_ptr<ShapeAbstractFactory> factory) {
        factory_ = std::move(factory);
    }
    
    void createAndDisplayShape(const std::string& shapeType, double size, const std::string& color = "") const {
        std::unique_ptr<Shape> shape;
        std::unique_ptr<ShapeRenderer> renderer = factory_->createRenderer();
        std::unique_ptr<ShapeAnimator> animator = factory_->createAnimator();
        
        if (shapeType == "circle") {
            shape = factory_->createCircle(size, color.empty() ? "red" : color);
        } else if (shapeType == "rectangle") {
            shape = factory_->createRectangle(size, size * 0.7, color.empty() ? "blue" : color);
        } else if (shapeType == "triangle") {
            shape = factory_->createTriangle(size, size * 0.6, color.empty() ? "green" : color);
        }
        
        if (shape) {
            std::cout << "Created with " << getFactoryType() << " factory: ";
            renderer->render(*shape);
            animator->animate(*shape);
            std::cout << "Area: " << shape->getArea() << std::endl << std::endl;
        }
    }
    
    std::string getFactoryType() const {
        // This would normally be implemented with virtual methods or RTTI
        // For demo purposes, using a simple approach
        if (dynamic_cast<BasicThemeFactory*>(factory_.get())) {
            return "BasicTheme";
        } else if (dynamic_cast<AdvancedThemeFactory*>(factory_.get())) {
            return "AdvancedTheme";
        } else if (dynamic_cast<ModernThemeFactory*>(factory_.get())) {
            return "ModernTheme";
        } else if (dynamic_cast<VintageThemeFactory*>(factory_.get())) {
            return "VintageTheme";
        }
        return "Unknown";
    }
    
    void createCompleteShapeSystem(double size, const std::string& type, const std::string& color = "") const {
        std::unique_ptr<Shape> shape;
        std::unique_ptr<ShapeRenderer> renderer;
        std::unique_ptr<ShapeAnimator> animator;

        factory_->createShapeSystem(size, type, shape, renderer, animator, color);

        std::cout << "Complete system from " << getFactoryType() << " factory:\n";
        renderer->render(*shape);
        animator->animate(*shape);
        std::cout << "Area: " << shape->getArea() << "\n" << std::endl;
    }
};

#endif // ABSTRACT_FACTORY_SHAPES_H