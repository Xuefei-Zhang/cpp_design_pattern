#ifndef FLYWEIGHT_PATTERNS_H
#define FLYWEIGHT_PATTERNS_H

#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <functional>

// Flyweight Interface - defines the interface for flyweight objects
class ShapeFlyweight {
public:
    virtual ~ShapeFlyweight() = default;

    // Operation that accepts extrinsic state (context-specific data)
    virtual void draw(double x, double y, double scale = 1.0) const = 0;
    virtual std::string getType() const = 0;
    virtual double getArea(double scale = 1.0) const = 0;
    virtual void printInfo() const = 0;
};

// Concrete Flyweights - lightweight implementations that store intrinsic state
class CircleFlyweight : public ShapeFlyweight {
private:
    double radius_;  // Intrinsic state (shared)
    std::string color_;  // Intrinsic state (shared)

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
    
    // Specific getters
    double getRadius() const { return radius_; }
    const std::string& getColor() const { return color_; }
};

class RectangleFlyweight : public ShapeFlyweight {
private:
    double width_, height_;  // Intrinsic state (shared)
    std::string color_;  // Intrinsic state (shared)

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
    
    // Specific getters
    double getWidth() const { return width_; }
    double getHeight() const { return height_; }
    const std::string& getColor() const { return color_; }
};

class TriangleFlyweight : public ShapeFlyweight {
private:
    double base_, height_;  // Intrinsic state (shared)
    std::string color_;  // Intrinsic state (shared)

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
    
    // Specific getters
    double getBase() const { return base_; }
    double getHeight() const { return height_; }
    const std::string& getColor() const { return color_; }
};

class LineFlyweight : public ShapeFlyweight {
private:
    double thickness_;  // Intrinsic state (shared)
    std::string color_;  // Intrinsic state (shared)

public:
    LineFlyweight(double thickness, const std::string& color)
        : thickness_(thickness), color_(color) {}
    
    void draw(double x, double y, double scale = 1.0) const override {
        // Draw a line from current position with some direction
        double endX = x + (5 * scale);
        double endY = y + (5 * scale);
        std::cout << "Drawing Line: thickness=" << (thickness_ * scale)
                  << " from (" << x << "," << y << ") to (" << endX << "," << endY 
                  << ") with color=" << color_ << std::endl;
    }
    
    std::string getType() const override {
        return "Line";
    }
    
    double getArea(double scale = 1.0) const override {
        // Lines have no area
        return 0.0;
    }
    
    void printInfo() const override {
        std::cout << "LineFlyweight - Thickness: " << thickness_ 
                  << ", Color: " << color_ << std::endl;
    }
    
    // Specific getters
    double getThickness() const { return thickness_; }
    const std::string& getColor() const { return color_; }
};

// Flyweight Factory - manages flyweight objects and ensures sharing
class ShapeFlyweightFactory {
private:
    static std::unordered_map<std::string, std::unique_ptr<ShapeFlyweight>> flyweights_;

public:
    // Get or create a flyweight based on intrinsic state
    static ShapeFlyweight* getFlyweight(const std::string& key) {
        auto it = flyweights_.find(key);
        if (it != flyweights_.end()) {
            std::cout << "Reusing existing flyweight for key: " << key << std::endl;
            return it->second.get();
        }
        
        std::cout << "Creating new flyweight for key: " << key << std::endl;
        
        // Parse the key to extract shape type and attributes
        if (key.find("Circle") == 0) {
            // Extract parameters from key (format: "Circle_radius_color")
            size_t first_underscore = key.find('_', 6); // Skip "Circle_"
            if (first_underscore != std::string::npos) {
                size_t second_underscore = key.find('_', first_underscore + 1);
                if (second_underscore != std::string::npos) {
                    std::string radiusStr = key.substr(6, first_underscore - 6);
                    std::string colorStr = key.substr(first_underscore + 1);
                    double radius = std::stod(radiusStr);
                    flyweights_[key] = std::make_unique<CircleFlyweight>(radius, colorStr);
                }
            }
        } else if (key.find("Rectangle") == 0) {
            // Extract parameters from key (format: "Rectangle_width_height_color")
            size_t first_underscore = key.find('_', 9); // Skip "Rectangle_"
            size_t second_underscore = key.npos;
            size_t third_underscore = key.npos;

            if (first_underscore != std::string::npos) {
                second_underscore = key.find('_', first_underscore + 1);
                if (second_underscore != std::string::npos) {
                    third_underscore = key.find('_', second_underscore + 1);
                    if (third_underscore != std::string::npos) {
                        std::string widthStr = key.substr(9, first_underscore - 9);
                        std::string heightStr = key.substr(first_underscore + 1, second_underscore - first_underscore - 1);
                        std::string colorStr = key.substr(second_underscore + 1);
                        double width = std::stod(widthStr);
                        double height = std::stod(heightStr);
                        flyweights_[key] = std::make_unique<RectangleFlyweight>(width, height, colorStr);
                    }
                }
            }
        } else if (key.find("Triangle") == 0) {
            // Extract parameters from key (format: "Triangle_base_height_color")
            size_t first_underscore = key.find('_', 8); // Skip "Triangle_"
            size_t second_underscore = key.npos;
            size_t third_underscore = key.npos;

            if (first_underscore != std::string::npos) {
                second_underscore = key.find('_', first_underscore + 1);
                if (second_underscore != std::string::npos) {
                    third_underscore = key.find('_', second_underscore + 1);
                    if (third_underscore != std::string::npos) {
                        std::string baseStr = key.substr(8, first_underscore - 8);
                        std::string heightStr = key.substr(first_underscore + 1, second_underscore - first_underscore - 1);
                        std::string colorStr = key.substr(second_underscore + 1);
                        double base = std::stod(baseStr);
                        double height = std::stod(heightStr);
                        flyweights_[key] = std::make_unique<TriangleFlyweight>(base, height, colorStr);
                    }
                }
            }
        } else if (key.find("Line") == 0) {
            // Extract parameters from key (format: "Line_thickness_color")
            size_t first_underscore = key.find('_', 4); // Skip "Line_"
            size_t second_underscore = key.npos;

            if (first_underscore != std::string::npos) {
                second_underscore = key.find('_', first_underscore + 1);
                if (second_underscore != std::string::npos) {
                    std::string thicknessStr = key.substr(4, first_underscore - 4);
                    std::string colorStr = key.substr(first_underscore + 1);
                    double thickness = std::stod(thicknessStr);
                    flyweights_[key] = std::make_unique<LineFlyweight>(thickness, colorStr);
                }
            }
        }

        return flyweights_[key].get();
    }

    // Create a flyweight with specific parameters
    static ShapeFlyweight* createCircleFlyweight(double radius, const std::string& color) {
        std::string key = "Circle_" + std::to_string(radius) + "_" + color;
        return getFlyweight(key);
    }

    static ShapeFlyweight* createRectangleFlyweight(double width, double height, const std::string& color) {
        std::string key = "Rectangle_" + std::to_string(width) + "_" + std::to_string(height) + "_" + color;
        return getFlyweight(key);
    }

    static ShapeFlyweight* createTriangleFlyweight(double base, double height, const std::string& color) {
        std::string key = "Triangle_" + std::to_string(base) + "_" + std::to_string(height) + "_" + color;
        return getFlyweight(key);
    }

    static ShapeFlyweight* createLineFlyweight(double thickness, const std::string& color) {
        std::string key = "Line_" + std::to_string(thickness) + "_" + color;
        return getFlyweight(key);
    }
    
    // Get count of currently managed flyweights
    static size_t getFlyweightCount() {
        return flyweights_.size();
    }
    
    // List all managed flyweights
    static void listFlyweights() {
        std::cout << "Currently managed flyweights (" << flyweights_.size() << " total):" << std::endl;
        for (const auto& pair : flyweights_) {
            std::cout << "  Key: " << pair.first << " -> ";
            pair.second->printInfo();
        }
    }
    
    // Reset and clear all flyweights
    static void reset() {
        flyweights_.clear();
    }

    // Additional utility methods for testing
    static size_t getFlyweightCount() {
        return flyweights_.size();
    }

    static void listFlyweights() {
        std::cout << "Available flyweights (" << flyweights_.size() << " total):" << std::endl;
        for (const auto& pair : flyweights_) {
            std::cout << "  " << pair.first << std::endl;
        }
    }
};

// Initialize static member
std::unordered_map<std::string, std::unique_ptr<ShapeFlyweight>> ShapeFlyweightFactory::flyweights_;

// Flyweight wrapper that holds extrinsic state
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
        std::cout << "FlyweightShape - Position: (" << x_ << "," << y_ 
                  << "), Scale: " << scale_ << ", Type: " << getType() << std::endl;
        std::cout << "  ";
        flyweight_->printInfo();
    }
    
    // Getters and setters for extrinsic state
    double getX() const { return x_; }
    double getY() const { return y_; }
    double getScale() const { return scale_; }
    
    void setPosition(double x, double y) { x_ = x; y_ = y; }
    void setScale(double scale) { scale_ = scale; }
    
    // Get reference to intrinsic data
    const ShapeFlyweight* getFlyweight() const { return flyweight_; }
};

// Advanced Flyweight: Themed Flyweights
class ThemedFlyweight : public ShapeFlyweight {
protected:
    std::unique_ptr<ShapeFlyweight> baseFlyweight_;
    std::string themeStyle_;

public:
    ThemedFlyweight(std::unique_ptr<ShapeFlyweight> base, const std::string& themeStyle)
        : baseFlyweight_(std::move(base)), themeStyle_(themeStyle) {}
    
    void draw(double x, double y, double scale = 1.0) const override {
        std::cout << "[" << themeStyle_ << " THEME] ";
        baseFlyweight_->draw(x, y, scale);
    }
    
    std::string getType() const override {
        return themeStyle_ + "_" + baseFlyweight_->getType();
    }
    
    double getArea(double scale = 1.0) const override {
        return baseFlyweight_->getArea(scale);
    }
    
    void printInfo() const override {
        std::cout << "ThemedFlyweight - Theme: " << themeStyle_ << std::endl;
        std::cout << "  ";
        baseFlyweight_->printInfo();
    }
};

class ModernThemedFlyweight : public ThemedFlyweight {
public:
    ModernThemedFlyweight(std::unique_ptr<ShapeFlyweight> base)
        : ThemedFlyweight(std::move(base), "Modern") {}
    
    void draw(double x, double y, double scale = 1.0) const override {
        std::cout << "[MODERN STYLE] ";
        baseFlyweight_->draw(x, y, scale);
    }
};

class VintageThemedFlyweight : public ThemedFlyweight {
public:
    VintageThemedFlyweight(std::unique_ptr<ShapeFlyweight> base)
        : ThemedFlyweight(std::move(base), "Vintage") {}
    
    void draw(double x, double y, double scale = 1.0) const override {
        std::cout << "[VINTAGE STYLE] ";
        baseFlyweight_->draw(x, y, scale);
    }
};

// Flyweight Registry - Advanced factory with more sophisticated management
class ShapeFlyweightRegistry {
private:
    std::unordered_map<std::string, std::unique_ptr<ShapeFlyweight>> registry_;
    std::function<std::unique_ptr<ShapeFlyweight>(const std::string&)> factoryFunction_;

public:
    ShapeFlyweightRegistry() = default;
    
    // Set a custom factory function for creating flyweights
    void setFactoryFunction(std::function<std::unique_ptr<ShapeFlyweight>(const std::string&)> func) {
        factoryFunction_ = func;
    }
    
    // Register a custom flyweight
    ShapeFlyweight* registerFlyweight(const std::string& key, std::unique_ptr<ShapeFlyweight> flyweight) {
        registry_[key] = std::move(flyweight);
        return registry_[key].get();
    }
    
    // Get or create a flyweight with the factory function
    ShapeFlyweight* getOrCreateFlyweight(const std::string& key) {
        auto it = registry_.find(key);
        if (it != registry_.end()) {
            return it->second.get();
        }
        
        if (factoryFunction_) {
            auto flyweight = factoryFunction_(key);
            if (flyweight) {
                registry_[key] = std::move(flyweight);
                return registry_[key].get();
            }
        }
        
        return nullptr;
    }
    
    // Create a themed flyweight
    ShapeFlyweight* getThemedFlyweight(const std::string& baseKey, const std::string& theme) {
        std::string themedKey = "THEMED_" + theme + "_" + baseKey;
        
        auto it = registry_.find(themedKey);
        if (it != registry_.end()) {
            return it->second.get();
        }
        
        // Get the base flyweight
        ShapeFlyweight* baseFlyweight = getOrCreateFlyweight(baseKey);
        if (!baseFlyweight) {
            return nullptr;
        }
        
        // Create themed version
        std::unique_ptr<ShapeFlyweight> themedFlyweight;
        if (theme == "Modern") {
            // For this example, we'll create a new modern-themed shape
            themedFlyweight = std::make_unique<ModernThemedFlyweight>(
                std::make_unique<CircleFlyweight>(
                    5.0, // Default radius, in a real impl would use actual values
                    "white"
                )
            );
        } else if (theme == "Vintage") {
            themedFlyweight = std::make_unique<VintageThemedFlyweight>(
                std::make_unique<RectangleFlyweight>(
                    8.0, 6.0, // Default dimensions
                    "beige"
                )
            );
        }
        
        if (themedFlyweight) {
            registry_[themedKey] = std::move(themedFlyweight);
            return registry_[themedKey].get();
        }
        
        return nullptr;
    }
    
    size_t getRegistrySize() const {
        return registry_.size();
    }
    
    void clear() {
        registry_.clear();
    }
};

// Flyweight Manager - High-level interface for creating shape collections
class FlyweightShapeManager {
private:
    std::vector<std::unique_ptr<FlyweightShape>> shapes_;
    ShapeFlyweightFactory factory_;

public:
    // Add a shape using flyweight pattern
    void addCircle(double x, double y, double radius, const std::string& color, double scale = 1.0) {
        ShapeFlyweight* flyweight = factory_.createCircleFlyweight(radius, color);
        shapes_.push_back(std::make_unique<FlyweightShape>(flyweight, x, y, scale));
    }

    void addRectangle(double x, double y, double width, double height, const std::string& color, double scale = 1.0) {
        ShapeFlyweight* flyweight = factory_.createRectangleFlyweight(width, height, color);
        shapes_.push_back(std::make_unique<FlyweightShape>(flyweight, x, y, scale));
    }

    void addTriangle(double x, double y, double base, double height, const std::string& color, double scale = 1.0) {
        ShapeFlyweight* flyweight = factory_.createTriangleFlyweight(base, height, color);
        shapes_.push_back(std::make_unique<FlyweightShape>(flyweight, x, y, scale));
    }

    void addLine(double x, double y, double thickness, const std::string& color, double scale = 1.0) {
        ShapeFlyweight* flyweight = factory_.createLineFlyweight(thickness, color);
        shapes_.push_back(std::make_unique<FlyweightShape>(flyweight, x, y, scale));
    }
    
    // Add a shape with a specific flyweight
    void addShape(ShapeFlyweight* flyweight, double x, double y, double scale = 1.0) {
        shapes_.push_back(std::make_unique<FlyweightShape>(flyweight, x, y, scale));
    }
    
    // Render all shapes
    void renderAll() const {
        std::cout << "Rendering scene with " << shapes_.size() << " shapes:\n";
        for (const auto& shape : shapes_) {
            shape->draw();
        }
    }
    
    // Calculate total area of all shapes
    double calculateTotalArea() const {
        double total_area = 0.0;
        for (const auto& shape : shapes_) {
            total_area += shape->getArea();
        }
        return total_area;
    }
    
    // Get count of shapes
    size_t getShapeCount() const {
        return shapes_.size();
    }
    
    // Get count of unique flyweights being used
    size_t getUniqueFlyweightCount() const {
        std::unordered_map<ShapeFlyweight*, bool> uniqueFlyweights;
        for (const auto& shape : shapes_) {
            uniqueFlyweights[shape->getFlyweight()] = true;
        }
        return uniqueFlyweights.size();
    }
    
    // Get flyweight factory reference for access to shared objects
    ShapeFlyweightFactory& getFactory() { return factory_; }
    
    // Clear all shapes
    void clear() { shapes_.clear(); }
    
    // Draw and print info for all shapes
    void printAllInfo() const {
        for (size_t i = 0; i < shapes_.size(); ++i) {
            std::cout << "Shape " << (i + 1) << ": ";
            shapes_[i]->printInfo();
        }
    }
};

// Template-based Flyweight for automatic type-safe flyweight creation
template<typename T>
class TypedFlyweightFactory {
private:
    static std::unordered_map<std::string, std::unique_ptr<T>> flyweights_;

public:
    template<typename... Args>
    static T* getFlyweight(Args&&... args) {
        // Generate a key based on the constructor arguments
        std::string key = generateKey(args...);
        
        auto it = flyweights_.find(key);
        if (it != flyweights_.end()) {
            return it->second.get();
        }
        
        flyweights_[key] = std::make_unique<T>(std::forward<Args>(args)...);
        return flyweights_[key].get();
    }
    
    static size_t getCount() {
        return flyweights_.size();
    }
    
    template<typename... Args>
    static std::string generateKey(Args&&... args) {
        return std::string(typeid(T).name()) + "_" + concatenateArgs(args...);
    }
    
private:
    // Helper functions to generate keys
    static std::string concatenateArgs(double val) {
        return std::to_string(val);
    }
    
    static std::string concatenateArgs(const std::string& str) {
        return str;
    }
    
    template<typename T1, typename T2, typename... Rest>
    static std::string concatenateArgs(T1&& first, T2&& second, Rest&&... rest) {
        return concatenateArgs(first) + "_" + concatenateArgs(second, rest...);
    }
};

// Initialize static member
template<typename T>
std::unordered_map<std::string, std::unique_ptr<T>> TypedFlyweightFactory<T>::flyweights_;

#endif // FLYWEIGHT_PATTERNS_H