#ifndef PROTOTYPE_SHAPES_H
#define PROTOTYPE_SHAPES_H

#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

// Abstract Prototype Interface
class ShapePrototype {
public:
    virtual ~ShapePrototype() = default;
    
    // Pure virtual clone method - each subclass must implement its own clone
    virtual std::unique_ptr<ShapePrototype> clone() const = 0;
    
    // Common interface methods
    virtual void draw() const = 0;
    virtual std::string getType() const = 0;
    virtual double getArea() const = 0;
    virtual void printInfo() const = 0;
    
    // Virtual assignment operator for deep copying properties
    virtual void copyProperties(const ShapePrototype& other) = 0;
};

// Concrete Prototype: Circle
class CirclePrototype : public ShapePrototype {
protected:
    double radius_;
    std::string color_;
    std::string style_;
    double x_, y_;  // Position coordinates

public:
    CirclePrototype(double radius = 1.0, const std::string& color = "red", 
                   const std::string& style = "solid", double x = 0.0, double y = 0.0)
        : radius_(radius), color_(color), style_(style), x_(x), y_(y) {}
    
    // Virtual copy constructor (clone method)
    std::unique_ptr<ShapePrototype> clone() const override {
        return std::make_unique<CirclePrototype>(*this);
    }
    
    // Copy properties from another shape
    void copyProperties(const ShapePrototype& other) override {
        // Attempt to cast to CirclePrototype to copy specific properties
        const CirclePrototype* otherCircle = dynamic_cast<const CirclePrototype*>(&other);
        if (otherCircle) {
            radius_ = otherCircle->radius_;
            color_ = otherCircle->color_;
            style_ = otherCircle->style_;
            x_ = otherCircle->x_;
            y_ = otherCircle->y_;
        }
    }
    
    void draw() const override {
        std::cout << "Drawing Circle: radius=" << radius_ 
                  << ", color=" << color_ 
                  << ", style=" << style_
                  << ", position=(" << x_ << "," << y_ << ")" << std::endl;
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
                  << ", Style: " << style_
                  << ", Position: (" << x_ << "," << y_ << ")" << std::endl;
    }
    
    // Specific getters and setters for Circle
    double getRadius() const { return radius_; }
    void setRadius(double radius) { radius_ = radius; }
    const std::string& getColor() const { return color_; }
    void setColor(const std::string& color) { color_ = color; }
    double getX() const { return x_; }
    void setX(double x) { x_ = x; }
    double getY() const { return y_; }
    void setY(double y) { y_ = y; }
};

// Concrete Prototype: Rectangle
class RectanglePrototype : public ShapePrototype {
protected:
    double width_, height_;
    std::string color_;
    std::string style_;
    double x_, y_;  // Position coordinates

public:
    RectanglePrototype(double width = 1.0, double height = 1.0, 
                      const std::string& color = "blue", 
                      const std::string& style = "solid", 
                      double x = 0.0, double y = 0.0)
        : width_(width), height_(height), color_(color), style_(style), x_(x), y_(y) {}
    
    // Virtual copy constructor (clone method)
    std::unique_ptr<ShapePrototype> clone() const override {
        return std::make_unique<RectanglePrototype>(*this);
    }
    
    // Copy properties from another shape
    void copyProperties(const ShapePrototype& other) override {
        // Attempt to cast to RectanglePrototype to copy specific properties
        const RectanglePrototype* otherRect = dynamic_cast<const RectanglePrototype*>(&other);
        if (otherRect) {
            width_ = otherRect->width_;
            height_ = otherRect->height_;
            color_ = otherRect->color_;
            style_ = otherRect->style_;
            x_ = otherRect->x_;
            y_ = otherRect->y_;
        }
    }
    
    void draw() const override {
        std::cout << "Drawing Rectangle: " << width_ << "x" << height_ 
                  << ", color=" << color_ 
                  << ", style=" << style_
                  << ", position=(" << x_ << "," << y_ << ")" << std::endl;
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
                  << ", Style: " << style_
                  << ", Position: (" << x_ << "," << y_ << ")" << std::endl;
    }
    
    // Specific getters and setters for Rectangle
    double getWidth() const { return width_; }
    void setWidth(double width) { width_ = width; }
    double getHeight() const { return height_; }
    void setHeight(double height) { height_ = height; }
    const std::string& getColor() const { return color_; }
    void setColor(const std::string& color) { color_ = color; }
    double getX() const { return x_; }
    void setX(double x) { x_ = x; }
    double getY() const { return y_; }
    void setY(double y) { y_ = y; }
};

// Concrete Prototype: Triangle
class TrianglePrototype : public ShapePrototype {
protected:
    double base_, height_;
    std::string color_;
    std::string style_;
    double x_, y_;  // Position coordinates

public:
    TrianglePrototype(double base = 1.0, double height = 1.0, 
                     const std::string& color = "green", 
                     const std::string& style = "solid", 
                     double x = 0.0, double y = 0.0)
        : base_(base), height_(height), color_(color), style_(style), x_(x), y_(y) {}
    
    // Virtual copy constructor (clone method)
    std::unique_ptr<ShapePrototype> clone() const override {
        return std::make_unique<TrianglePrototype>(*this);
    }
    
    // Copy properties from another shape
    void copyProperties(const ShapePrototype& other) override {
        // Attempt to cast to TrianglePrototype to copy specific properties
        const TrianglePrototype* otherTri = dynamic_cast<const TrianglePrototype*>(&other);
        if (otherTri) {
            base_ = otherTri->base_;
            height_ = otherTri->height_;
            color_ = otherTri->color_;
            style_ = otherTri->style_;
            x_ = otherTri->x_;
            y_ = otherTri->y_;
        }
    }
    
    void draw() const override {
        std::cout << "Drawing Triangle: base=" << base_ << ", height=" << height_ 
                  << ", color=" << color_ 
                  << ", style=" << style_
                  << ", position=(" << x_ << "," << y_ << ")" << std::endl;
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
                  << ", Style: " << style_
                  << ", Position: (" << x_ << "," << y_ << ")" << std::endl;
    }
    
    // Specific getters and setters for Triangle
    double getBase() const { return base_; }
    void setBase(double base) { base_ = base; }
    double getHeight() const { return height_; }
    void setHeight(double height) { height_ = height; }
    const std::string& getColor() const { return color_; }
    void setColor(const std::string& color) { color_ = color; }
    double getX() const { return x_; }
    void setX(double x) { x_ = x; }
    double getY() const { return y_; }
    void setY(double y) { y_ = y; }
};

// Advanced Prototype: Complex Shape that includes multiple other shapes
class ComplexShapePrototype : public ShapePrototype {
protected:
    std::vector<std::unique_ptr<ShapePrototype>> components_;
    std::string name_;
    double x_, y_;  // Position of the complex shape

public:
    ComplexShapePrototype(const std::string& name = "ComplexShape", 
                         double x = 0.0, double y = 0.0)
        : name_(name), x_(x), y_(y) {}
    
    // Virtual copy constructor (clone method) - deep copy
    std::unique_ptr<ShapePrototype> clone() const override {
        auto clone = std::make_unique<ComplexShapePrototype>(name_, x_, y_);
        
        // Deep copy all components
        for (const auto& component : components_) {
            clone->addComponent(component->clone());
        }
        
        return std::unique_ptr<ShapePrototype>(clone.release());
    }
    
    // Copy properties from another shape
    void copyProperties(const ShapePrototype& other) override {
        const ComplexShapePrototype* otherComplex = dynamic_cast<const ComplexShapePrototype*>(&other);
        if (otherComplex) {
            name_ = otherComplex->name_;
            x_ = otherComplex->x_;
            y_ = otherComplex->y_;
            
            // Clear existing components and copy new ones
            components_.clear();
            for (const auto& component : otherComplex->components_) {
                components_.push_back(component->clone());
            }
        }
    }
    
    void addComponent(std::unique_ptr<ShapePrototype> component) {
        components_.push_back(std::move(component));
    }
    
    void draw() const override {
        std::cout << "Drawing Complex Shape: " << name_ 
                  << " at position (" << x_ << "," << y_ << ")" << std::endl;
        for (const auto& component : components_) {
            std::cout << "  ";
            component->draw();
        }
    }
    
    std::string getType() const override {
        return name_;
    }
    
    double getArea() const override {
        double totalArea = 0.0;
        for (const auto& component : components_) {
            totalArea += component->getArea();
        }
        return totalArea;
    }
    
    void printInfo() const override {
        std::cout << "Complex Shape - Name: " << name_
                  << ", Position: (" << x_ << "," << y_ << ")"
                  << ", Components: " << components_.size() << std::endl;
        for (const auto& component : components_) {
            std::cout << "  - ";
            component->printInfo();
        }
    }
    
    size_t getComponentCount() const { return components_.size(); }
    void setName(const std::string& name) { name_ = name; }
};

// Prototype Registry - Centralized repository for prototype objects
class PrototypeRegistry {
private:
    std::unordered_map<std::string, std::unique_ptr<ShapePrototype>> prototypes_;

public:
    // Register a prototype with a specific ID
    void registerPrototype(const std::string& id, std::unique_ptr<ShapePrototype> prototype) {
        prototypes_[id] = std::move(prototype);
    }
    
    // Unregister a prototype
    void unregisterPrototype(const std::string& id) {
        prototypes_.erase(id);
    }
    
    // Clone a prototype by ID
    std::unique_ptr<ShapePrototype> create(const std::string& id) const {
        auto it = prototypes_.find(id);
        if (it != prototypes_.end()) {
            return it->second->clone();
        }
        return nullptr; // Prototype not found
    }
    
    // Check if a prototype exists
    bool hasPrototype(const std::string& id) const {
        return prototypes_.find(id) != prototypes_.end();
    }
    
    // Get list of available prototype IDs
    std::vector<std::string> getAvailableIds() const {
        std::vector<std::string> ids;
        for (const auto& pair : prototypes_) {
            ids.push_back(pair.first);
        }
        return ids;
    }
    
    // Get the prototype without cloning (for inspection)
    const ShapePrototype* getPrototype(const std::string& id) const {
        auto it = prototypes_.find(id);
        return (it != prototypes_.end()) ? it->second.get() : nullptr;
    }
};

// Factory using the Prototype Registry
class ShapeFactory {
private:
    static PrototypeRegistry& getRegistry() {
        static PrototypeRegistry registry;
        return registry;
    }

public:
    // Register a prototype with the factory
    static void registerPrototype(const std::string& id, std::unique_ptr<ShapePrototype> prototype) {
        getRegistry().registerPrototype(id, std::move(prototype));
    }
    
    // Create a shape using the prototype
    static std::unique_ptr<ShapePrototype> createShape(const std::string& id) {
        return getRegistry().create(id);
    }
    
    // Check if a shape type is available
    static bool isShapeAvailable(const std::string& id) {
        return getRegistry().hasPrototype(id);
    }
    
    // Get available shape types
    static std::vector<std::string> getAvailableShapeTypes() {
        return getRegistry().getAvailableIds();
    }
};

// Advanced Prototype with Configuration
class ConfigurableShapePrototype : public ShapePrototype {
protected:
    std::unique_ptr<ShapePrototype> baseShape_;
    std::string customName_;
    std::unordered_map<std::string, std::string> properties_;

public:
    ConfigurableShapePrototype(std::unique_ptr<ShapePrototype> base, const std::string& customName = "")
        : baseShape_(std::move(base)), customName_(customName) {}
    
    // Clone method - creates a new configurable shape with cloned base
    std::unique_ptr<ShapePrototype> clone() const override {
        auto clonedBase = baseShape_->clone();
        auto clone = std::make_unique<ConfigurableShapePrototype>(std::move(clonedBase), customName_);
        
        // Copy properties
        clone->properties_ = properties_;
        
        return std::unique_ptr<ShapePrototype>(clone.release());
    }
    
    void copyProperties(const ShapePrototype& other) override {
        const ConfigurableShapePrototype* otherConfig = dynamic_cast<const ConfigurableShapePrototype*>(&other);
        if (otherConfig) {
            if (otherConfig->baseShape_) {
                baseShape_ = otherConfig->baseShape_->clone();
            }
            customName_ = otherConfig->customName_;
            properties_ = otherConfig->properties_;
        }
    }
    
    void setProperty(const std::string& key, const std::string& value) {
        properties_[key] = value;
    }
    
    std::string getProperty(const std::string& key) const {
        auto it = properties_.find(key);
        return (it != properties_.end()) ? it->second : "";
    }
    
    void draw() const override {
        std::cout << "Configurable Shape: " << customName_ << " - ";
        baseShape_->draw();
        
        // Print properties if any
        if (!properties_.empty()) {
            std::cout << "  Properties: ";
            for (const auto& prop : properties_) {
                std::cout << prop.first << "=" << prop.second << " ";
            }
            std::cout << std::endl;
        }
    }
    
    std::string getType() const override {
        return customName_.empty() ? baseShape_->getType() : customName_;
    }
    
    double getArea() const override {
        return baseShape_->getArea();
    }
    
    void printInfo() const override {
        std::cout << "Configurable " << baseShape_->getType() << " - ";
        if (!customName_.empty()) {
            std::cout << "Custom Name: " << customName_ << ", ";
        }
        baseShape_->printInfo();
        
        // Print properties if any
        if (!properties_.empty()) {
            std::cout << "  Properties: ";
            for (const auto& prop : properties_) {
                std::cout << prop.first << "=" << prop.second << " ";
            }
            std::cout << std::endl;
        }
    }
};

#endif // PROTOTYPE_SHAPES_H