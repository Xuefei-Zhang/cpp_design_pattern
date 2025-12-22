#ifndef FLYWEIGHT_PATTERNS_H
#define FLYWEIGHT_PATTERNS_H

#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <algorithm>

// Flyweight interface - defines the interface for flyweight objects
class ShapeFlyweight {
public:
    virtual ~ShapeFlyweight() = default;
    
    // Operation that accepts extrinsic state (context-specific data)
    virtual void draw(double x, double y, double scale = 1.0) const = 0;
    virtual std::string getType() const = 0;
    virtual double getArea(double scale = 1.0) const = 0;
    virtual void printInfo() const = 0;
};

// Concrete Flyweights - store intrinsic state (shared/common properties)
class CircleFlyweight : public ShapeFlyweight {
private:
    double radius_;
    std::string color_;

public:
    CircleFlyweight(double radius, const std::string& color) 
        : radius_(radius), color_(color) {}
    
    void draw(double x, double y, double scale = 1.0) const override {
        std::cout << "Drawing Circle: radius=" << (radius_ * scale) 
                  << " at (" << x << "," << y << ") with color=" << color_ << std::endl;
    }
    
    std::string getType() const override {
        return "Circle";
    }
    
    double getArea(double scale = 1.0) const override {
        return 3.14159 * radius_ * radius_ * scale * scale;
    }
    
    void printInfo() const override {
        std::cout << "CircleFlyweight - Radius: " << radius_ 
                  << ", Color: " << color_ << std::endl;
    }
};

class RectangleFlyweight : public ShapeFlyweight {
private:
    double width_, height_;
    std::string color_;

public:
    RectangleFlyweight(double width, double height, const std::string& color) 
        : width_(width), height_(height), color_(color) {}
    
    void draw(double x, double y, double scale = 1.0) const override {
        std::cout << "Drawing Rectangle: " << (width_ * scale) << "x" << (height_ * scale)
                  << " at (" << x << "," << y << ") with color=" << color_ << std::endl;
    }
    
    std::string getType() const override {
        return "Rectangle";
    }
    
    double getArea(double scale = 1.0) const override {
        return width_ * height_ * scale * scale;
    }
    
    void printInfo() const override {
        std::cout << "RectangleFlyweight - Width: " << width_ 
                  << ", Height: " << height_
                  << ", Color: " << color_ << std::endl;
    }
};

class TriangleFlyweight : public ShapeFlyweight {
private:
    double base_, height_;
    std::string color_;

public:
    TriangleFlyweight(double base, double height, const std::string& color) 
        : base_(base), height_(height), color_(color) {}
    
    void draw(double x, double y, double scale = 1.0) const override {
        std::cout << "Drawing Triangle: base=" << (base_ * scale) 
                  << ", height=" << (height_ * scale)
                  << " at (" << x << "," << y << ") with color=" << color_ << std::endl;
    }
    
    std::string getType() const override {
        return "Triangle";
    }
    
    double getArea(double scale = 1.0) const override {
        return 0.5 * base_ * height_ * scale * scale;
    }
    
    void printInfo() const override {
        std::cout << "TriangleFlyweight - Base: " << base_ 
                  << ", Height: " << height_
                  << ", Color: " << color_ << std::endl;
    }
};

// Flyweight Factory - manages shared flyweight objects
class ShapeFlyweightFactory {
private:
    static std::unordered_map<std::string, std::unique_ptr<ShapeFlyweight>> flyweights_;

public:
    // Get or create a flyweight based on its intrinsic properties (key)
    static ShapeFlyweight* getFlyweight(const std::string& key) {
        auto it = flyweights_.find(key);
        if (it != flyweights_.end()) {
            std::cout << "[FACTOR] Reusing flyweight with key: " << key << std::endl;
            return it->second.get();
        }

        std::cout << "[FACTOR] Creating new flyweight with key: " << key << std::endl;

        // Parse key to determine which flyweight to create
        if (key.find("Circle:") == 0) {
            // Format: "Circle:radius,color"
            size_t pos = key.find(',', 7); // Find comma after "Circle:"
            if (pos != std::string::npos) {
                std::string radiusStr = key.substr(7, pos - 7);
                std::string colorStr = key.substr(pos + 1);
                double radius = std::stod(radiusStr);
                flyweights_[key] = std::make_unique<CircleFlyweight>(radius, colorStr);
            }
        } else if (key.find("Rectangle:") == 0) {
            // Format: "Rectangle:width,height,color"
            size_t pos1 = key.find(',', 10); // Find first comma after "Rectangle:"
            if (pos1 != std::string::npos) {
                size_t pos2 = key.find(',', pos1 + 1);
                if (pos2 != std::string::npos) {
                    std::string widthStr = key.substr(10, pos1 - 10);
                    std::string heightStr = key.substr(pos1 + 1, pos2 - pos1 - 1);
                    std::string colorStr = key.substr(pos2 + 1);
                    double width = std::stod(widthStr);
                    double height = std::stod(heightStr);
                    flyweights_[key] = std::make_unique<RectangleFlyweight>(width, height, colorStr);
                }
            }
        } else if (key.find("Triangle:") == 0) {
            // Format: "Triangle:base,height,color"
            size_t pos1 = key.find(',', 9); // Find first comma after "Triangle:"
            if (pos1 != std::string::npos) {
                size_t pos2 = key.find(',', pos1 + 1);
                if (pos2 != std::string::npos) {
                    std::string baseStr = key.substr(9, pos1 - 9);
                    std::string heightStr = key.substr(pos1 + 1, pos2 - pos1 - 1);
                    std::string colorStr = key.substr(pos2 + 1);
                    double base = std::stod(baseStr);
                    double height = std::stod(heightStr);
                    flyweights_[key] = std::make_unique<TriangleFlyweight>(base, height, colorStr);
                }
            }
        }

        return flyweights_[key].get();
    }

    // Convenience methods for common flyweight creation
    static ShapeFlyweight* createCircle(double radius, const std::string& color) {
        std::string key = "Circle:" + std::to_string(radius) + "," + color + ",circle";
        return getFlyweight(key);
    }

    static ShapeFlyweight* createRectangle(double width, double height, const std::string& color) {
        std::string key = "Rectangle:" + std::to_string(width) + "," + std::to_string(height) + "," + color + ",rectangle";
        return getFlyweight(key);
    }

    static ShapeFlyweight* createTriangle(double base, double height, const std::string& color) {
        std::string key = "Triangle:" + std::to_string(base) + "," + std::to_string(height) + "," + color + ",triangle";
        return getFlyweight(key);
    }

    // Utility methods
    static size_t getFlyweightCount() {
        return flyweights_.size();
    }

    static void listFlyweights() {
        std::cout << "Managed flyweights (" << flyweights_.size() << " total):" << std::endl;
        for (const auto& pair : flyweights_) {
            std::cout << "  " << pair.first << std::endl;
        }
    }

    static void reset() {
        flyweights_.clear();
    }
};

// Initialize static member
std::unordered_map<std::string, std::unique_ptr<ShapeFlyweight>> ShapeFlyweightFactory::flyweights_;

// Flyweight wrapper that combines intrinsic state (from flyweight) with extrinsic state (local)
class FlyweightShape {
private:
    ShapeFlyweight* flyweight_;  // Shared intrinsic state
    double x_, y_;              // Extrinsic state (unique to each instance)
    double scale_;

public:
    FlyweightShape(ShapeFlyweight* flyweight, double x, double y, double scale = 1.0)
        : flyweight_(flyweight), x_(x), y_(y), scale_(scale) {}

    void draw() const {
        flyweight_->draw(x_, y_, scale_);
    }

    double getArea() const {
        return flyweight_->getArea(scale_);
    }

    std::string getType() const {
        return flyweight_->getType();
    }

    void printInfo() const {
        std::cout << "FlyweightShape: " << getType() 
                  << " at (" << x_ << "," << y_ << ") with scale " << scale_ << std::endl;
        flyweight_->printInfo();
    }

    // Getters and setters for extrinsic state
    double getX() const { return x_; }
    double getY() const { return y_; }
    double getScale() const { return scale_; }
    void setPosition(double x, double y) { x_ = x; y_ = y; }
    void setScale(double scale) { scale_ = scale; }
    ShapeFlyweight* getFlyweight() const { return flyweight_; }
};

// Advanced Flyweight - Themed shapes
class ThemedShapeFlyweight : public ShapeFlyweight {
protected:
    std::unique_ptr<ShapeFlyweight> baseFlyweight_;
    std::string themeName_;

public:
    ThemedShapeFlyweight(std::unique_ptr<ShapeFlyweight> base, const std::string& theme)
        : baseFlyweight_(std::move(base)), themeName_(theme) {}

    void draw(double x, double y, double scale = 1.0) const override {
        std::cout << "[" << themeName_ << "] ";
        baseFlyweight_->draw(x, y, scale);
    }

    std::string getType() const override {
        return themeName_ + "_" + baseFlyweight_->getType();
    }

    double getArea(double scale = 1.0) const override {
        return baseFlyweight_->getArea(scale);
    }

    void printInfo() const override {
        std::cout << "ThemedShapeFlyweight - Theme: " << themeName_ << std::endl;
        baseFlyweight_->printInfo();
    }
};

class ModernThemedFlyweight : public ThemedShapeFlyweight {
public:
    ModernThemedFlyweight(std::unique_ptr<ShapeFlyweight> base)
        : ThemedShapeFlyweight(std::move(base), "Modern") {}

    void draw(double x, double y, double scale = 1.0) const override {
        std::cout << "[MODERN STYLE] ";
        baseFlyweight_->draw(x, y, scale);
    }
};

class VintageThemedFlyweight : public ThemedShapeFlyweight {
public:
    VintageThemedFlyweight(std::unique_ptr<ShapeFlyweight> base)
        : ThemedShapeFlyweight(std::move(base), "Vintage") {}

    void draw(double x, double y, double scale = 1.0) const override {
        std::cout << "[VINTAGE STYLE] ";
        baseFlyweight_->draw(x, y, scale);
    }
};

// Shape System Manager - High-level interface for creating shape collections using flyweights
class FlyweightShapeSystem {
private:
    std::vector<std::unique_ptr<FlyweightShape>> shapes_;

public:
    void addCircle(double x, double y, double radius, const std::string& color, double scale = 1.0) {
        ShapeFlyweight* flyweight = ShapeFlyweightFactory::createCircle(radius, color);
        shapes_.push_back(std::make_unique<FlyweightShape>(flyweight, x, y, scale));
    }

    void addRectangle(double x, double y, double width, double height, const std::string& color, double scale = 1.0) {
        ShapeFlyweight* flyweight = ShapeFlyweightFactory::createRectangle(width, height, color);
        shapes_.push_back(std::make_unique<FlyweightShape>(flyweight, x, y, scale));
    }

    void addTriangle(double x, double y, double base, double height, const std::string& color, double scale = 1.0) {
        ShapeFlyweight* flyweight = ShapeFlyweightFactory::createTriangle(base, height, color);
        shapes_.push_back(std::make_unique<FlyweightShape>(flyweight, x, y, scale));
    }

    void renderAll() const {
        std::cout << "Rendering system with " << shapes_.size() << " shapes:\n";
        for (const auto& shape : shapes_) {
            shape->draw();
        }
    }

    double calculateTotalArea() const {
        double total = 0.0;
        for (const auto& shape : shapes_) {
            total += shape->getArea();
        }
        return total;
    }

    size_t getShapeCount() const {
        return shapes_.size();
    }

    size_t getUniqueFlyweightCount() const {
        std::unordered_map<ShapeFlyweight*, bool> uniqueFlyweights;
        for (const auto& shape : shapes_) {
            uniqueFlyweights[shape->getFlyweight()] = true;
        }
        return uniqueFlyweights.size();
    }

    void printAllInfo() const {
        for (size_t i = 0; i < shapes_.size(); ++i) {
            std::cout << "Shape " << (i + 1) << ": ";
            shapes_[i]->printInfo();
            std::cout << std::endl;
        }
    }

    void clear() {
        shapes_.clear();
    }
};

// Performance optimized Flyweight Manager
class OptimizedFlyweightManager {
private:
    std::vector<std::unique_ptr<FlyweightShape>> shapes_;
    // Cache of recently used flyweights for faster access
    std::unordered_map<std::string, ShapeFlyweight*> recentFlyweights_;

public:
    // Add shape with automatic flyweight caching
    void addShape(const std::string& type, double x, double y, 
                  double param1, double param2, const std::string& color, double scale = 1.0) {
        std::string cacheKey = type + "_" + color;
        ShapeFlyweight* flyweight = nullptr;
        
        // Try to use cached flyweight first
        auto cacheIt = recentFlyweights_.find(cacheKey);
        if (cacheIt != recentFlyweights_.end()) {
            std::cout << "[MANAGER] Using cached flyweight for " << type << " with " << color << std::endl;
            flyweight = cacheIt->second;
        } else {
            // Create new flyweight through factory
            if (type == "circle") {
                flyweight = ShapeFlyweightFactory::createCircle(param1, color);
            } else if (type == "rectangle") {
                flyweight = ShapeFlyweightFactory::createRectangle(param1, param2, color);
            } else if (type == "triangle") {
                flyweight = ShapeFlyweightFactory::createTriangle(param1, param2, color);
            }
            
            // Cache the flyweight for future use
            recentFlyweights_[cacheKey] = flyweight;
        }

        shapes_.push_back(std::make_unique<FlyweightShape>(flyweight, x, y, scale));
    }

    // High-performance rendering with batch processing
    void batchRender() const {
        std::cout << "[OPTIMIZED MANAGER] Batch rendering " << shapes_.size() << " shapes:\n";
        for (const auto& shape : shapes_) {
            shape->draw();
        }
    }

    size_t getShapeCount() const { return shapes_.size(); }
    size_t getUniqueFlyweightCount() const { return recentFlyweights_.size(); }
    
    void clear() {
        shapes_.clear();
        recentFlyweights_.clear();
    }
};

#endif // FLYWEIGHT_PATTERNS_H