#ifndef FLYWEIGHT_PATTERNS_H
#define FLYWEIGHT_PATTERNS_H

#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <algorithm>

// Flyweight interface - defines the interface for objects that can be shared
class ShapeFlyweight {
public:
    virtual ~ShapeFlyweight() = default;
    
    // Operation that accepts extrinsic state (context-specific data)
    virtual void draw(double x, double y, double scale = 1.0) const = 0;
    virtual std::string getType() const = 0;
    virtual double getArea(double scale = 1.0) const = 0;
    virtual void printInfo() const = 0;
};

// Concrete Flyweights implementing the flyweight interface
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
    
    // Specific getters
    double getRadius() const { return radius_; }
    const std::string& getColor() const { return color_; }
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
    
    // Specific getters
    double getWidth() const { return width_; }
    double getHeight() const { return height_; }
    const std::string& getColor() const { return color_; }
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
    
    // Specific getters
    double getBase() const { return base_; }
    double getHeight() const { return height_; }
    const std::string& getColor() const { return color_; }
};

// Advanced Flyweight Decorator for themed versions
class ThemedShapeFlyweight : public ShapeFlyweight {
protected:
    std::unique_ptr<ShapeFlyweight> baseFlyweight_;
    std::string themeName_;

public:
    ThemedShapeFlyweight(std::unique_ptr<ShapeFlyweight> baseFlyweight, const std::string& theme)
        : baseFlyweight_(std::move(baseFlyweight)), themeName_(theme) {}

    void draw(double x, double y, double scale = 1.0) const override {
        std::cout << "[" << themeName_ << " Theme] ";
        baseFlyweight_->draw(x, y, scale);
    }

    std::string getType() const override {
        return themeName_ + "_" + baseFlyweight_->getType();
    }

    double getArea(double scale = 1.0) const override {
        return baseFlyweight_->getArea(scale);
    }

    void printInfo() const override {
        std::cout << "ThemedFlyweight - Theme: " << themeName_ << std::endl;
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

// Flyweight Factory - manages shared flyweight objects
class ShapeFlyweightFactory {
private:
    static std::unordered_map<std::string, std::unique_ptr<ShapeFlyweight>> flyweights_;

public:
    // Get or create a flyweight based on its intrinsic state (key)
    static ShapeFlyweight* getFlyweight(const std::string& key) {
        auto it = flyweights_.find(key);
        if (it != flyweights_.end()) {
            std::cout << "Reusing existing flyweight for key: " << key << std::endl;
            return it->second.get();
        }

        std::cout << "Creating new flyweight for key: " << key << std::endl;

        // Parse key to determine which flyweight to create
        if (key.find("Circle:") == 0) {
            size_t pos1 = key.find(',', 7); // Find first comma after "Circle:"
            if (pos1 != std::string::npos) {
                size_t pos2 = key.find(',', pos1 + 1);
                if (pos2 != std::string::npos) {
                    std::string radiusStr = key.substr(7, pos1 - 7);
                    std::string colorStr = key.substr(pos1 + 1, pos2 - pos1 - 1);
                    double radius = std::stod(radiusStr);
                    flyweights_[key] = std::make_unique<CircleFlyweight>(radius, colorStr);
                }
            }
        } else if (key.find("Rectangle:") == 0) {
            size_t pos1 = key.find(',', 10); // Find first comma after "Rectangle:"
            if (pos1 != std::string::npos) {
                size_t pos2 = key.find(',', pos1 + 1);
                if (pos2 != std::string::npos) {
                    size_t pos3 = key.find(',', pos2 + 1);
                    if (pos3 != std::string::npos) {
                        std::string widthStr = key.substr(10, pos1 - 10);
                        std::string heightStr = key.substr(pos1 + 1, pos2 - pos1 - 1);
                        std::string colorStr = key.substr(pos2 + 1, pos3 - pos2 - 1);
                        double width = std::stod(widthStr);
                        double height = std::stod(heightStr);
                        flyweights_[key] = std::make_unique<RectangleFlyweight>(width, height, colorStr);
                    }
                }
            }
        } else if (key.find("Triangle:") == 0) {
            size_t pos1 = key.find(',', 9); // Find first comma after "Triangle:"
            if (pos1 != std::string::npos) {
                size_t pos2 = key.find(',', pos1 + 1);
                if (pos2 != std::string::npos) {
                    size_t pos3 = key.find(',', pos2 + 1);
                    if (pos3 != std::string::npos) {
                        std::string baseStr = key.substr(9, pos1 - 9);
                        std::string heightStr = key.substr(pos1 + 1, pos2 - pos1 - 1);
                        std::string colorStr = key.substr(pos2 + 1, pos3 - pos2 - 1);
                        double base = std::stod(baseStr);
                        double height = std::stod(heightStr);
                        flyweights_[key] = std::make_unique<TriangleFlyweight>(base, height, colorStr);
                    }
                }
            }
        }

        return flyweights_[key].get();
    }

    // Helper methods that create standard keys
    static ShapeFlyweight* createCircleFlyweight(double radius, const std::string& color) {
        std::string key = "Circle:" + std::to_string(radius) + "," + color + ",circle";
        return getFlyweight(key);
    }

    static ShapeFlyweight* createRectangleFlyweight(double width, double height, const std::string& color) {
        std::string key = "Rectangle:" + std::to_string(width) + "," + std::to_string(height) + "," + color + ",rectangle";
        return getFlyweight(key);
    }

    static ShapeFlyweight* createTriangleFlyweight(double base, double height, const std::string& color) {
        std::string key = "Triangle:" + std::to_string(base) + "," + std::to_string(height) + "," + color + ",triangle";
        return getFlyweight(key);
    }

    // Getters for factory status
    static size_t getFlyweightCount() {
        return flyweights_.size();
    }

    static void listFlyweights() {
        std::cout << "Currently managed flyweights (" << flyweights_.size() << " total):" << std::endl;
        for (const auto& pair : flyweights_) {
            std::cout << "  Key: " << pair.first << std::endl;
        }
    }

    static void reset() {
        flyweights_.clear();
    }
};

// Initialize static member
std::unordered_map<std::string, std::unique_ptr<ShapeFlyweight>> ShapeFlyweightFactory::flyweights_;

// Wrapper that combines intrinsic state (from flyweight) with extrinsic state (stored locally)
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
                  << "), Scale: " << scale_ 
                  << ", Type: " << getType() << std::endl;
        flyweight_->printInfo();
    }

    // Getters and setters for extrinsic state
    double getX() const { return x_; }
    double getY() const { return y_; }
    double getScale() const { return scale_; }

    void setPosition(double x, double y) { x_ = x; y_ = y; }
    void setScale(double scale) { scale_ = scale; }

    const ShapeFlyweight* getFlyweight() const { return flyweight_; }
};

// Main Facade class - provides simplified interface to the complex subsystem
class GraphicsSystemFacade {
private:
    ShapeFlyweightFactory factory_;

public:
    // High-level method to draw a shape without dealing with flyweight complexity
    void drawShape(const std::string& shapeType, double size, const std::string& color, 
                   double x = 0, double y = 0, double scale = 1.0) {
        std::cout << "[GRAPHICS FACADE] Drawing " << shapeType << " at (" << x << "," << y 
                  << ") with size " << size << " and color " << color << std::endl;

        ShapeFlyweight* flyweight = nullptr;
        if (shapeType == "circle") {
            flyweight = factory_.createCircleFlyweight(size, color);
        } else if (shapeType == "rectangle") {
            flyweight = factory_.createRectangleFlyweight(size, size * 0.75, color);  // Rectangle with aspect ratio
        } else if (shapeType == "triangle") {
            flyweight = factory_.createTriangleFlyweight(size, size * 0.8, color);  // Triangle with height
        }

        if (flyweight) {
            FlyweightShape shape(flyweight, x, y, scale);
            shape.draw();
            std::cout << "[GRAPHICS FACADE] Area: " << shape.getArea() << std::endl;
        }
    }

    // High-level method to create a complex scene with many shapes
    void createScene(const std::string& sceneName, 
                     const std::vector<std::tuple<std::string, double, std::string, double, double, double>>& shapes) {
        std::cout << "[GRAPHICS FACADE] Creating scene: " << sceneName << std::endl;

        for (const auto& shapeSpec : shapes) {
            const std::string& type = std::get<0>(shapeSpec);
            double size = std::get<1>(shapeSpec);
            const std::string& color = std::get<2>(shapeSpec);
            double x = std::get<3>(shapeSpec);
            double y = std::get<4>(shapeSpec);
            double scale = std::get<5>(shapeSpec);

            drawShape(type, size, color, x, y, scale);
        }
    }

    // High-level rendering method that manages the entire rendering process
    void renderScene(const std::vector<std::tuple<std::string, double, std::string, double, double, double>>& shapes) {
        std::cout << "[GRAPHICS FACADE] Rendering scene with " << shapes.size() << " shapes:\n";

        for (const auto& shapeSpec : shapes) {
            const std::string& type = std::get<0>(shapeSpec);
            double size = std::get<1>(shapeSpec);
            const std::string& color = std::get<2>(shapeSpec);
            double x = std::get<3>(shapeSpec);
            double y = std::get<4>(shapeSpec);
            double scale = std::get<5>(shapeSpec);

            ShapeFlyweight* flyweight = nullptr;
            if (type == "circle") {
                flyweight = factory_.createCircleFlyweight(size, color);
            } else if (type == "rectangle") {
                flyweight = factory_.createRectangleFlyweight(size, size * 0.75, color);
            } else if (type == "triangle") {
                flyweight = factory_.createTriangleFlyweight(size, size * 0.8, color);
            }

            if (flyweight) {
                FlyweightShape shape(flyweight, x, y, scale);
                shape.draw();
            }
        }

        std::cout << "[GRAPHICS FACADE] Scene rendering complete\n";
    }

    // High-level method to calculate total area of shapes
    double calculateTotalArea(const std::vector<std::tuple<std::string, double, std::string, double, double, double>>& shapes) {
        double totalArea = 0.0;

        for (const auto& shapeSpec : shapes) {
            const std::string& type = std::get<0>(shapeSpec);
            double size = std::get<1>(shapeSpec);
            const std::string& color = std::get<2>(shapeSpec);
            double scale = std::get<5>(shapeSpec);

            ShapeFlyweight* flyweight = nullptr;
            if (type == "circle") {
                flyweight = factory_.createCircleFlyweight(size, color);
            } else if (type == "rectangle") {
                flyweight = factory_.createRectangleFlyweight(size, size * 0.75, color);
            } else if (type == "triangle") {
                flyweight = factory_.createTriangleFlyweight(size, size * 0.8, color);
            }

            if (flyweight) {
                double area = flyweight->getArea(scale);
                totalArea += area;
            }
        }

        return totalArea;
    }

    // Get the number of unique flyweights currently in use
    size_t getUniqueFlyweightCount() const {
        return ShapeFlyweightFactory::getFlyweightCount();
    }

    // Clear all flyweights
    void clearFlyweights() {
        ShapeFlyweightFactory::reset();
    }
};

// Advanced Facade - For different types of graphics systems
class GameGraphicsFacade {
private:
    GraphicsSystemFacade graphicsFacade_;

public:
    void initializeGameEnvironment() {
        std::cout << "[GAME GRAPHICS FACADE] Initializing game graphics environment...\n";
        std::cout << "[GAME GRAPHICS FACADE] Environment ready!\n\n";
    }

    void createGameObject(const std::string& objectType, double size, const std::string& color, 
                         double x, double y) {
        std::string shapeType = objectType;
        if (objectType == "player" || objectType == "enemy") {
            shapeType = "circle"; // Map game objects to shapes
        } else if (objectType == "building" || objectType == "wall") {
            shapeType = "rectangle";
        } else if (objectType == "trap" || objectType == "obstacle") {
            shapeType = "triangle";
        }

        std::cout << "[GAME GRAPHICS FACADE] Creating game object: " << objectType 
                  << " as " << shapeType << std::endl;
        graphicsFacade_.drawShape(shapeType, size, color, x, y, 1.0);
    }

    void renderGameScene(const std::vector<std::tuple<std::string, double, std::string, double, double>>& gameObjects) {
        std::cout << "[GAME GRAPHICS FACADE] Rendering game scene with " 
                  << gameObjects.size() << " game objects:\n";

        std::vector<std::tuple<std::string, double, std::string, double, double, double>> shapes;
        for (const auto& gameObj : gameObjects) {
            const std::string& objectType = std::get<0>(gameObj);
            double size = std::get<1>(gameObj);
            const std::string& color = std::get<2>(gameObj);
            double x = std::get<3>(gameObj);
            double y = std::get<4>(gameObj);

            std::string shapeType = objectType;
            if (objectType == "player" || objectType == "enemy") {
                shapeType = "circle";
            } else if (objectType == "building" || objectType == "wall") {
                shapeType = "rectangle";
            } else if (objectType == "trap" || objectType == "obstacle") {
                shapeType = "triangle";
            }

            shapes.emplace_back(shapeType, size, color, x, y, 1.0);
        }

        graphicsFacade_.renderScene(shapes);
        std::cout << "[GAME GRAPHICS FACADE] Game scene rendered\n";
    }
};

// UI Facade - For user interface graphics
class UIFacade {
private:
    GraphicsSystemFacade graphicsFacade_;

public:
    void createUIComponent(const std::string& componentType, double width, double height, 
                          const std::string& color, double x, double y) {
        std::string shapeType;
        if (componentType == "button" || componentType == "icon") {
            shapeType = "rectangle"; // Map UI components to shapes
        } else if (componentType == "progress_bar") {
            shapeType = "rectangle"; // Use rectangle for progress bars
        } else if (componentType == "slider_handle") {
            shapeType = "circle"; // Use circle for slider handle
        }

        std::cout << "[UI FACADE] Creating UI component: " << componentType 
                  << " as " << shapeType << std::endl;
        graphicsFacade_.drawShape(shapeType, width, color, x, y, height/width); // Use height/width as scale factor
    }

    void renderUI(const std::vector<std::tuple<std::string, double, double, std::string, double, double>>& uiComponents) {
        std::cout << "[UI FACADE] Rendering UI with " << uiComponents.size() << " components:\n";

        std::vector<std::tuple<std::string, double, std::string, double, double, double>> shapes;
        for (const auto& uiComp : uiComponents) {
            const std::string& type = std::get<0>(uiComp);
            double width = std::get<1>(uiComp);
            double height = std::get<2>(uiComp);
            const std::string& color = std::get<3>(uiComp);
            double x = std::get<4>(uiComp);
            double y = std::get<5>(uiComp);

            std::string shapeType = "rectangle"; // Default to rectangle
            if (type == "slider_handle" || type == "indicator") {
                shapeType = "circle";
            }

            shapes.emplace_back(shapeType, width, color, x, y, height/width);
        }

        graphicsFacade_.renderScene(shapes);
        std::cout << "[UI FACADE] UI rendered\n";
    }
};

// Animation Facade - For animated graphics
class AnimationFacade {
private:
    GraphicsSystemFacade graphicsFacade_;

public:
    void createAnimatedShape(const std::string& shapeType, double size, const std::string& color,
                            double startX, double startY, double endX, double endY, 
                            double duration) {
        std::cout << "[ANIMATION FACADE] Creating animated " << shapeType 
                  << " moving from (" << startX << "," << startY << ") to (" 
                  << endX << "," << endY << ") over " << duration << " seconds\n";

        // Draw the shape at the start position initially
        graphicsFacade_.drawShape(shapeType, size, color, startX, startY, 1.0);

        // In a real system, we would animate over time
        std::cout << "[ANIMATION FACADE] Animation path prepared\n";
    }

    void createRotatingShape(const std::string& shapeType, double size, const std::string& color,
                            double centerX, double centerY, double radius, double duration) {
        std::cout << "[ANIMATION FACADE] Creating rotating " << shapeType 
                  << " around (" << centerX << "," << centerY << ") with radius " 
                  << radius << " over " << duration << " seconds\n";

        // Draw the shape at its initial position
        graphicsFacade_.drawShape(shapeType, size, color, centerX + radius, centerY, 1.0);

        std::cout << "[ANIMATION FACADE] Rotation animation prepared\n";
    }
};

#endif // FLYWEIGHT_PATTERNS_H