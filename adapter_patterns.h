#ifndef ADAPTER_PATTERNS_H
#define ADAPTER_PATTERNS_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <cmath>
#include <functional>

// Target interface - what the client expects
class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw() const = 0;
    virtual std::string getType() const = 0;
    virtual double getArea() const = 0;
    virtual void printInfo() const = 0;
};

// Concrete implementations of target interface
class Circle : public Shape {
private:
    double radius_;
    std::string color_;

public:
    Circle(double radius = 1.0, const std::string& color = "red") 
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
};

class Rectangle : public Shape {
private:
    double width_, height_;
    std::string color_;

public:
    Rectangle(double width = 1.0, double height = 1.0, const std::string& color = "blue") 
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
};

// Adaptee 1: Legacy Drawing System (incompatible interface)
class LegacyShape {
protected:
    std::string name_;
    double size_;

public:
    LegacyShape(const std::string& name, double size) 
        : name_(name), size_(size) {}
    
    // Legacy methods with different interface
    virtual void renderLegacy() const {
        std::cout << "Legacy rendering: " << name_ << " with size " << size_ << std::endl;
    }
    
    virtual double calculateLegacyArea() const {
        // Simplified area calculation
        return size_ * size_ * 0.785; // Approximation
    }
    
    virtual void displayLegacyInfo() const {
        std::cout << "LegacyShape - Name: " << name_ << ", Size: " << size_ << std::endl;
    }
    
    const std::string& getName() const { return name_; }
    double getSize() const { return size_; }
};

class LegacyCircle : public LegacyShape {
public:
    LegacyCircle(double radius) : LegacyShape("LegacyCircle", radius) {}
    
    void renderLegacy() const override {
        std::cout << "Legacy Circle rendering: radius=" << size_ 
                  << ", name=" << name_ << std::endl;
    }
    
    double calculateLegacyArea() const override {
        return 3.14159 * size_ * size_;
    }
};

class LegacyRectangle : public LegacyShape {
public:
    LegacyRectangle(double width, double height) 
        : LegacyShape("LegacyRectangle", (width + height) / 2.0) {} // Average for size
    
    void renderLegacy() const override {
        std::cout << "Legacy Rectangle rendering: 2D shape with average size=" << size_ 
                  << ", name=" << name_ << std::endl;
    }
    
    double calculateLegacyArea() const override {
        // In legacy system, we have width and height stored differently
        return size_ * size_ * 1.5; // Approximation
    }
};

// Adaptee 2: Third-party Graphics Library
class ThirdPartyShape {
protected:
    double param1_, param2_;  // Different parameters than target
    std::string style_;

public:
    ThirdPartyShape(double p1, double p2, const std::string& style) 
        : param1_(p1), param2_(p2), style_(style) {}
    
    // Third-party methods with completely different interface
    virtual void renderAsSVG() const {
        std::cout << "SVG rendering with params (" << param1_ << ", " << param2_ 
                  << "), style=" << style_ << std::endl;
    }
    
    virtual double getCalculatedArea() const {
        return param1_ * param2_ * 1.2;  // Third-party calculation
    }
    
    virtual void showDetails() const {
        std::cout << "ThirdPartyShape - Params: (" << param1_ << ", " << param2_ 
                  << "), Style: " << style_ << std::endl;
    }
    
    double getParam1() const { return param1_; }
    double getParam2() const { return param2_; }
    const std::string& getStyle() const { return style_; }
};

class ThirdPartyEllipse : public ThirdPartyShape {
public:
    ThirdPartyEllipse(double radiusX, double radiusY) 
        : ThirdPartyShape(radiusX, radiusY, "ellipse") {}
    
    void renderAsSVG() const override {
        std::cout << "SVG Ellipse: rx=" << param1_ << ", ry=" << param2_ 
                  << ", style=" << style_ << std::endl;
    }
    
    double getCalculatedArea() const override {
        return 3.14159 * param1_ * param2_;  // More accurate for ellipse
    }
};

class ThirdPartyBox : public ThirdPartyShape {
public:
    ThirdPartyBox(double width, double height) 
        : ThirdPartyShape(width, height, "box") {}
    
    void renderAsSVG() const override {
        std::cout << "SVG Box: width=" << param1_ << ", height=" << param2_ 
                  << ", style=" << style_ << std::endl;
    }
    
    double getCalculatedArea() const override {
        return param1_ * param2_;  // Rectangle area
    }
};

// Adaptee 3: New Modern Shape System
struct ModernShapeData {
    std::string type;
    double x, y, scale;
    std::vector<double> parameters;
};

class ModernShapeProcessor {
public:
    static void drawModernShape(const ModernShapeData& data) {
        std::cout << "Modern rendering: " << data.type 
                  << " at (" << data.x << "," << data.y << "), scale=" << data.scale 
                  << ", params=" << data.parameters.size() << std::endl;
    }
    
    static double calculateModernArea(const ModernShapeData& data) {
        if (data.type == "circle" && !data.parameters.empty()) {
            return 3.14159 * data.parameters[0] * data.parameters[0];
        } else if (data.type == "rectangle" && data.parameters.size() >= 2) {
            return data.parameters[0] * data.parameters[1];
        }
        return 0.0;
    }
    
    static void displayModernInfo(const ModernShapeData& data) {
        std::cout << "ModernShape: type=" << data.type 
                  << ", pos=(" << data.x << "," << data.y << "), scale=" << data.scale << std::endl;
    }
};

// Adapter 1: Object Adapter for LegacyShape
class LegacyShapeAdapter : public Shape {
protected:
    std::unique_ptr<LegacyShape> legacyShape_;

public:
    explicit LegacyShapeAdapter(std::unique_ptr<LegacyShape> legacy) 
        : legacyShape_(std::move(legacy)) {}
    
    void draw() const override {
        legacyShape_->renderLegacy();
    }
    
    std::string getType() const override {
        return "Adapted " + legacyShape_->getName();
    }
    
    double getArea() const override {
        return legacyShape_->calculateLegacyArea();
    }
    
    void printInfo() const override {
        legacyShape_->displayLegacyInfo();
    }
};

// Adapter 2: Object Adapter for ThirdPartyShape
class ThirdPartyShapeAdapter : public Shape {
protected:
    std::unique_ptr<ThirdPartyShape> thirdPartyShape_;

public:
    explicit ThirdPartyShapeAdapter(std::unique_ptr<ThirdPartyShape> thirdParty) 
        : thirdPartyShape_(std::move(thirdParty)) {}
    
    void draw() const override {
        thirdPartyShape_->renderAsSVG();
    }
    
    std::string getType() const override {
        return "Adapted ThirdParty " + thirdPartyShape_->getStyle();
    }
    
    double getArea() const override {
        return thirdPartyShape_->getCalculatedArea();
    }
    
    void printInfo() const override {
        thirdPartyShape_->showDetails();
    }
};

// Adapter 3: Class Adapter for LegacyShape (would require multiple inheritance)
// Since C++ supports multiple inheritance, we can create a class adapter, 
// but for shape consistency, we'll stick with object adapter approach

// Adapter 4: Adapter for ModernShapeProcessor
class ModernShapeAdapter : public Shape {
private:
    ModernShapeData data_;

public:
    ModernShapeAdapter(const ModernShapeData& data) : data_(data) {}
    
    void draw() const override {
        ModernShapeProcessor::drawModernShape(data_);
    }
    
    std::string getType() const override {
        return data_.type;
    }
    
    double getArea() const override {
        return ModernShapeProcessor::calculateModernArea(data_);
    }
    
    void printInfo() const override {
        ModernShapeProcessor::displayModernInfo(data_);
    }
};

// For C++14 compatibility, let's use a simpler approach instead of the universal adapter
// that had issues with non-copyable lambdas

// Client code that works with the Shape interface
class ShapeRenderer {
public:
    static void renderAll(const std::vector<std::unique_ptr<Shape>>& shapes) {
        for (const auto& shape : shapes) {
            std::cout << "Rendering shape: " << shape->getType() << std::endl;
            shape->draw();
            std::cout << "Area: " << shape->getArea() << std::endl;
            shape->printInfo();
            std::cout << std::endl;
        }
    }
    
    static double calculateTotalArea(const std::vector<std::unique_ptr<Shape>>& shapes) {
        double total = 0.0;
        for (const auto& shape : shapes) {
            total += shape->getArea();
        }
        return total;
    }
};

#endif // ADAPTER_PATTERNS_H