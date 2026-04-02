#ifndef COMPOSITE_PATTERNS_H
#define COMPOSITE_PATTERNS_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

// Component interface - defines the interface for all objects in the composition
class ShapeComponent {
public:
    virtual ~ShapeComponent() = default;

    // Common operations for both leaf and composite
    virtual void draw() const = 0;
    virtual std::string getType() const = 0;
    virtual double getArea() const = 0;
    virtual void printInfo() const = 0;

    // Name accessor (can be overridden by implementations)
    virtual std::string getName() const {
        return getType(); // Default implementation
    }
    
    // Operations for composite (these do nothing in leaf implementations)
    virtual void add(std::unique_ptr<ShapeComponent> component) {
        // Default implementation does nothing for leaves
    }
    
    virtual void remove(ShapeComponent* component) {
        // Default implementation does nothing for leaves
    }
    
    virtual bool isComposite() const {
        return false; // Default is false for leaves
    }
    
    virtual const std::vector<std::unique_ptr<ShapeComponent>>& getChildren() const {
        throw std::logic_error("Leaf components don't have children");
    }
    
    virtual bool contains(const ShapeComponent* component) const {
        return false; // Default implementation for leaves
    }
    
    virtual ShapeComponent* findByName(const std::string& name) const {
        return nullptr; // Default implementation
    }
};

// Leaf implementations - individual shapes
class Circle : public ShapeComponent {
private:
    double radius_;
    std::string color_;
    std::string name_;
    double x_, y_;  // Position

public:
    Circle(double radius, const std::string& color = "red", const std::string& name = "Circle", 
           double x = 0, double y = 0) 
        : radius_(radius), color_(color), name_(name), x_(x), y_(y) {}
    
    void draw() const override {
        std::cout << "Drawing Circle: name=" << name_ 
                  << ", radius=" << radius_ 
                  << ", color=" << color_ 
                  << ", position=(" << x_ << "," << y_ << ")" << std::endl;
    }
    
    std::string getType() const override {
        return "Circle";
    }
    
    double getArea() const override {
        return 3.14159 * radius_ * radius_;
    }
    
    void printInfo() const override {
        std::cout << "Circle - Name: " << name_ 
                  << ", Radius: " << radius_ 
                  << ", Color: " << color_
                  << ", Position: (" << x_ << "," << y_ << ")" << std::endl;
    }
    
    // Specific getters
    double getRadius() const { return radius_; }
    const std::string& getColor() const { return color_; }
    std::string getName() const override { return name_; }
    double getX() const { return x_; }
    double getY() const { return y_; }
};

class Rectangle : public ShapeComponent {
private:
    double width_, height_;
    std::string color_;
    std::string name_;
    double x_, y_;  // Position

public:
    Rectangle(double width, double height, const std::string& color = "blue", 
              const std::string& name = "Rectangle", double x = 0, double y = 0) 
        : width_(width), height_(height), color_(color), name_(name), x_(x), y_(y) {}
    
    void draw() const override {
        std::cout << "Drawing Rectangle: name=" << name_ 
                  << ", " << width_ << "x" << height_ 
                  << ", color=" << color_ 
                  << ", position=(" << x_ << "," << y_ << ")" << std::endl;
    }
    
    std::string getType() const override {
        return "Rectangle";
    }
    
    double getArea() const override {
        return width_ * height_;
    }
    
    void printInfo() const override {
        std::cout << "Rectangle - Name: " << name_ 
                  << ", Width: " << width_ 
                  << ", Height: " << height_
                  << ", Color: " << color_
                  << ", Position: (" << x_ << "," << y_ << ")" << std::endl;
    }
    
    // Specific getters
    double getWidth() const { return width_; }
    double getHeight() const { return height_; }
    const std::string& getColor() const { return color_; }
    std::string getName() const override { return name_; }
    double getX() const { return x_; }
    double getY() const { return y_; }
};

class Triangle : public ShapeComponent {
private:
    double base_, height_;
    std::string color_;
    std::string name_;
    double x_, y_;  // Position

public:
    Triangle(double base, double height, const std::string& color = "green", 
             const std::string& name = "Triangle", double x = 0, double y = 0) 
        : base_(base), height_(height), color_(color), name_(name), x_(x), y_(y) {}
    
    void draw() const override {
        std::cout << "Drawing Triangle: name=" << name_ 
                  << ", base=" << base_ << ", height=" << height_ 
                  << ", color=" << color_ 
                  << ", position=(" << x_ << "," << y_ << ")" << std::endl;
    }
    
    std::string getType() const override {
        return "Triangle";
    }
    
    double getArea() const override {
        return 0.5 * base_ * height_;
    }
    
    void printInfo() const override {
        std::cout << "Triangle - Name: " << name_ 
                  << ", Base: " << base_ 
                  << ", Height: " << height_
                  << ", Color: " << color_
                  << ", Position: (" << x_ << "," << y_ << ")" << std::endl;
    }
    
    // Specific getters
    double getBase() const { return base_; }
    double getHeight() const { return height_; }
    const std::string& getColor() const { return color_; }
    std::string getName() const override { return name_; }
    double getX() const { return x_; }
    double getY() const { return y_; }
};

class Line : public ShapeComponent {
private:
    double startX_, startY_, endX_, endY_;
    std::string color_;
    std::string name_;

public:
    Line(double startX, double startY, double endX, double endY, 
         const std::string& color = "black", const std::string& name = "Line") 
        : startX_(startX), startY_(startY), endX_(endX), endY_(endY), 
          color_(color), name_(name) {}
    
    void draw() const override {
        std::cout << "Drawing Line: name=" << name_ 
                  << ", from (" << startX_ << "," << startY_ << ") to (" 
                  << endX_ << "," << endY_ << "), color=" << color_ << std::endl;
    }
    
    std::string getType() const override {
        return "Line";
    }
    
    double getArea() const override {
        return 0.0; // Lines have no area
    }
    
    void printInfo() const override {
        std::cout << "Line - Name: " << name_ 
                  << ", From: (" << startX_ << "," << startY_ << ") to (" 
                  << endX_ << "," << endY_ << "), Color: " << color_ << std::endl;
    }
    
    // Specific getters
    double getStartX() const { return startX_; }
    double getStartY() const { return startY_; }
    double getEndX() const { return endX_; }
    double getEndY() const { return endY_; }
    const std::string& getColor() const { return color_; }
    std::string getName() const override { return name_; }
};

// Composite implementation - groups of shapes
class ShapeComposite : public ShapeComponent {
protected:
    std::vector<std::unique_ptr<ShapeComponent>> children_;
    std::string name_;
    double offsetX_, offsetY_;  // Offset for the entire group

public:
    explicit ShapeComposite(const std::string& name = "Group", double offsetX = 0, double offsetY = 0) 
        : name_(name), offsetX_(offsetX), offsetY_(offsetY) {}
    
    void add(std::unique_ptr<ShapeComponent> component) override {
        children_.push_back(std::move(component));
    }
    
    void remove(ShapeComponent* component) override {
        children_.erase(
            std::remove_if(children_.begin(), children_.end(),
                [component](const std::unique_ptr<ShapeComponent>& child) {
                    return child.get() == component;
                }),
            children_.end()
        );
    }
    
    bool isComposite() const override {
        return true;
    }
    
    const std::vector<std::unique_ptr<ShapeComponent>>& getChildren() const override {
        return children_;
    }
    
    void draw() const override {
        std::cout << "Drawing Composite: " << name_ 
                  << " with offset (" << offsetX_ << "," << offsetY_ << ")" << std::endl;
        
        for (const auto& child : children_) {
            std::cout << "  ";
            child->draw();
        }
    }
    
    std::string getType() const override {
        return "Composite";
    }
    
    double getArea() const override {
        double totalArea = 0.0;
        for (const auto& child : children_) {
            totalArea += child->getArea();
        }
        return totalArea;
    }
    
    void printInfo() const override {
        std::cout << "Composite - Name: " << name_ 
                  << ", Children Count: " << children_.size()
                  << ", Offset: (" << offsetX_ << "," << offsetY_ << ")" << std::endl;
        
        for (const auto& child : children_) {
            std::cout << "  - ";
            child->printInfo();
        }
    }
    
    bool contains(const ShapeComponent* component) const override {
        for (const auto& child : children_) {
            if (child.get() == component) {
                return true;
            }
            // Also check recursively in nested composites
            if (child->isComposite() && child->contains(component)) {
                return true;
            }
        }
        return false;
    }
    
    ShapeComponent* findByName(const std::string& name) const override {
        if (name_ == name) {
            return const_cast<ShapeComposite*>(this);
        }
        
        for (const auto& child : children_) {
            if (child->getName() == name) {
                return child.get();
            }
            
            if (child->isComposite()) {
                auto* found = child->findByName(name);
                if (found) {
                    return found;
                }
            }
        }
        return nullptr;
    }
    
    std::string getName() const override {
        return name_;
    }
    
    size_t getChildCount() const {
        return children_.size();
    }
};

// Advanced Composite: Scene Graph - more sophisticated grouping
class SceneNode : public ShapeComposite {
private:
    double scaleX_, scaleY_;  // Scaling factors
    double rotation_;         // Rotation angle

public:
    SceneNode(const std::string& name, double offsetX = 0, double offsetY = 0, 
              double scaleX = 1.0, double scaleY = 1.0, double rotation = 0.0)
        : ShapeComposite(name, offsetX, offsetY), 
          scaleX_(scaleX), scaleY_(scaleY), rotation_(rotation) {}
    
    void draw() const override {
        std::cout << "Scene Node: " << name_ 
                  << " (Offset: " << offsetX_ << "," << offsetY_ 
                  << ", Scale: " << scaleX_ << "," << scaleY_
                  << ", Rotation: " << rotation_ << "°)" << std::endl;
        
        for (const auto& child : children_) {
            std::cout << "  ";
            child->draw();
        }
    }
    
    void printInfo() const override {
        std::cout << "SceneNode - Name: " << name_ 
                  << ", Offset: (" << offsetX_ << "," << offsetY_ << ")"
                  << ", Scale: (" << scaleX_ << "," << scaleY_ << ")"
                  << ", Rotation: " << rotation_ << "°"
                  << ", Children: " << children_.size() << std::endl;
        
        for (const auto& child : children_) {
            std::cout << "  - ";
            child->printInfo();
        }
    }
    
    std::string getType() const override {
        return "SceneNode";
    }
};

// Concrete composite implementations for specific use cases
class Group : public ShapeComposite {
public:
    explicit Group(const std::string& name = "Group", double offsetX = 0, double offsetY = 0) 
        : ShapeComposite(name, offsetX, offsetY) {}
    
    std::string getType() const override {
        return "Group";
    }
};

class Layer : public ShapeComposite {
private:
    bool visible_;
    double opacity_;

public:
    Layer(const std::string& name, bool visible = true, double opacity = 1.0) 
        : ShapeComposite(name), visible_(visible), opacity_(opacity) {}
    
    void draw() const override {
        if (!visible_) {
            std::cout << "Layer " << name_ << " is invisible, skipping render" << std::endl;
            return;
        }
        
        std::cout << "Layer: " << name_ 
                  << " (Opacity: " << opacity_ << ", Visible: " << (visible_ ? "Yes" : "No") << ")" << std::endl;
        
        for (const auto& child : children_) {
            std::cout << "  ";
            child->draw();
        }
    }
    
    void setVisible(bool visible) { visible_ = visible; }
    bool isVisible() const { return visible_; }
    
    void setOpacity(double opacity) { opacity_ = opacity; }
    double getOpacity() const { return opacity_; }
    
    std::string getType() const override {
        return "Layer";
    }
};

// Advanced composite: Grid layout
class GridLayout : public ShapeComposite {
private:
    int rows_, cols_;

public:
    GridLayout(int rows, int cols, const std::string& name = "Grid Layout") 
        : ShapeComposite(name), rows_(rows), cols_(cols) {}
    
    void arrange() {
        // Simple grid arrangement based on component count
        size_t numComponents = children_.size();
        for (size_t i = 0; i < numComponents && i < static_cast<size_t>(rows_ * cols_); ++i) {
            int row = i / cols_;
            int col = i % cols_;
            
            // Adjust position of child based on grid cell
            // This would typically update the child's position
            std::cout << "Arranging component " << i << " at grid position (" 
                      << row << "," << col << ")" << std::endl;
        }
    }
    
    std::string getType() const override {
        return "GridLayout";
    }
};

// Composite for complex shapes (like a house or car)
class ComplexShapeComposite : public ShapeComposite {
public:
    ComplexShapeComposite(const std::string& name, double offsetX = 0, double offsetY = 0) 
        : ShapeComposite(name, offsetX, offsetY) {}
    
    std::string getType() const override {
        return "ComplexShapeComposite";
    }
    
    void buildHouse() {
        // A predefined house shape composition
        add(std::make_unique<Rectangle>(20.0, 15.0, "brown", "House Body", 0, 0));  // Body
        add(std::make_unique<Triangle>(20.0, 10.0, "red", "Roof", 0, -7.5));       // Roof
        add(std::make_unique<Rectangle>(5.0, 8.0, "blue", "Door", -6.0, 5.0));     // Door
        add(std::make_unique<Circle>(2.0, "yellow", "Window1", -3.0, -3.0));       // Window 1
        add(std::make_unique<Circle>(2.0, "yellow", "Window2", 3.0, -3.0));        // Window 2
    }
    
    void buildCar() {
        // A predefined car shape composition
        add(std::make_unique<Rectangle>(25.0, 10.0, "blue", "Car Body", 0, 0));    // Car body
        add(std::make_unique<Circle>(3.0, "black", "Wheel FL", -8.0, -6.0));       // Front left wheel
        add(std::make_unique<Circle>(3.0, "black", "Wheel FR", 8.0, -6.0));        // Front right wheel
        add(std::make_unique<Circle>(3.0, "black", "Wheel RL", -8.0, 6.0));        // Rear left wheel
        add(std::make_unique<Circle>(3.0, "black", "Wheel RR", 8.0, 6.0));         // Rear right wheel
    }
};


#endif // COMPOSITE_PATTERNS_H