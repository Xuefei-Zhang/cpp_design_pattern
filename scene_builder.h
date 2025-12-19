#ifndef SCENE_BUILDER_H
#define SCENE_BUILDER_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <algorithm>

// Forward declaration for circular reference handling
class SceneNode;

// Basic Shape interface (reusing from previous examples)
class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw() const = 0;
    virtual std::string getType() const = 0;
    virtual double getArea() const = 0;
    virtual void printInfo() const = 0;
    virtual void setTransform(double x, double y, double scale = 1.0, double rotation = 0.0) = 0;
};

// Concrete Shape implementations
class Circle : public Shape {
private:
    double radius_, x_, y_, scale_, rotation_;
    std::string color_;
    std::string style_;

public:
    Circle(double radius, const std::string& color = "red", const std::string& style = "solid") 
        : radius_(radius), x_(0), y_(0), scale_(1.0), rotation_(0.0), color_(color), style_(style) {}
    
    void draw() const override {
        std::cout << "Drawing Circle at (" << x_ << "," << y_ << "): radius=" << radius_ 
                  << ", scale=" << scale_ << ", rotation=" << rotation_
                  << ", color=" << color_ << ", style=" << style_ << std::endl;
    }
    
    std::string getType() const override {
        return "Circle";
    }
    
    double getArea() const override {
        return 3.14159 * radius_ * radius_ * scale_ * scale_;
    }
    
    void printInfo() const override {
        std::cout << "Circle - Radius: " << radius_ 
                  << ", Position: (" << x_ << "," << y_ << ")"
                  << ", Scale: " << scale_
                  << ", Color: " << color_
                  << ", Style: " << style_ << std::endl;
    }
    
    void setTransform(double x, double y, double scale = 1.0, double rotation = 0.0) override {
        x_ = x;
        y_ = y;
        scale_ = scale;
        rotation_ = rotation;
    }
    
    // Specific getters for builder access
    double getRadius() const { return radius_; }
    double getX() const { return x_; }
    double getY() const { return y_; }
    double getScale() const { return scale_; }
    double getRotation() const { return rotation_; }
    const std::string& getColor() const { return color_; }
    const std::string& getStyle() const { return style_; }
};

class Rectangle : public Shape {
private:
    double width_, height_, x_, y_, scale_, rotation_;
    std::string color_;
    std::string style_;

public:
    Rectangle(double width, double height, const std::string& color = "blue", 
              const std::string& style = "solid") 
        : width_(width), height_(height), x_(0), y_(0), scale_(1.0), rotation_(0.0), 
          color_(color), style_(style) {}
    
    void draw() const override {
        std::cout << "Drawing Rectangle at (" << x_ << "," << y_ << "): " << width_ << "x" << height_ 
                  << ", scale=" << scale_ << ", rotation=" << rotation_
                  << ", color=" << color_ << ", style=" << style_ << std::endl;
    }
    
    std::string getType() const override {
        return "Rectangle";
    }
    
    double getArea() const override {
        return width_ * height_ * scale_ * scale_;
    }
    
    void printInfo() const override {
        std::cout << "Rectangle - Width: " << width_ 
                  << ", Height: " << height_
                  << ", Position: (" << x_ << "," << y_ << ")"
                  << ", Scale: " << scale_
                  << ", Color: " << color_
                  << ", Style: " << style_ << std::endl;
    }
    
    void setTransform(double x, double y, double scale = 1.0, double rotation = 0.0) override {
        x_ = x;
        y_ = y;
        scale_ = scale;
        rotation_ = rotation;
    }
    
    // Specific getters for builder access
    double getWidth() const { return width_; }
    double getHeight() const { return height_; }
    double getX() const { return x_; }
    double getY() const { return y_; }
    double getScale() const { return scale_; }
    double getRotation() const { return rotation_; }
    const std::string& getColor() const { return color_; }
    const std::string& getStyle() const { return style_; }
};

// SceneNode represents an element in the scene graph
class SceneNode {
public:
    std::string name;
    std::unique_ptr<Shape> shape;
    std::vector<std::unique_ptr<SceneNode>> children;
    SceneNode* parent;
    double localX, localY;  // Position relative to parent
    double localScale;
    double rotation;

    SceneNode(const std::string& nodeName, std::unique_ptr<Shape> s) 
        : name(nodeName), shape(std::move(s)), parent(nullptr), 
          localX(0), localY(0), localScale(1.0), rotation(0.0) {}

    void addChild(std::unique_ptr<SceneNode> child) {
        child->parent = this;
        children.push_back(std::move(child));
    }

    void setLocalTransform(double x, double y, double scale = 1.0, double rot = 0.0) {
        localX = x;
        localY = y;
        localScale = scale;
        rotation = rot;
    }

    // Calculate global transform based on parent hierarchy
    void getGlobalTransform(double& globalX, double& globalY, double& globalScale) const {
        globalX = localX;
        globalY = localY;
        globalScale = localScale;

        SceneNode* current = parent;
        while (current != nullptr) {
            globalX += current->localX;
            globalY += current->localY;
            globalScale *= current->localScale;
            current = current->parent;
        }
    }

    void draw() const {
        if (shape) {
            double globalX, globalY, globalScale;
            getGlobalTransform(globalX, globalY, globalScale);
            shape->setTransform(globalX, globalY, globalScale, rotation);
            shape->draw();
        }

        // Draw children
        for (const auto& child : children) {
            child->draw();
        }
    }

    void printStructure(int depth = 0) const {
        std::string indent(depth * 2, ' ');
        std::cout << indent << name << " (";
        if (shape) {
            std::cout << shape->getType() << ")";
        } else {
            std::cout << "Group)";
        }
        std::cout << " at (" << localX << "," << localY << ")" << std::endl;

        for (const auto& child : children) {
            child->printStructure(depth + 1);
        }
    }
};

// Scene represents a complete graphical scene
class Scene {
private:
    std::unique_ptr<SceneNode> root;
    std::string sceneName;
    std::string backgroundColor;

public:
    Scene(const std::string& name, const std::string& bgColor = "black") 
        : sceneName(name), backgroundColor(bgColor) {
        root = std::make_unique<SceneNode>("Root", nullptr);
    }

    SceneNode* getRoot() { return root.get(); }

    void draw() const {
        std::cout << "Drawing Scene: " << sceneName << " with background: " << backgroundColor << std::endl;
        root->draw();
    }

    void printSceneStructure() const {
        std::cout << "Scene: " << sceneName << " Structure:" << std::endl;
        root->printStructure();
    }
    
    const std::string& getName() const { return sceneName; }
    const std::string& getBackgroundColor() const { return backgroundColor; }
};

// Abstract Builder Interface for Scene
class SceneBuilder {
public:
    virtual ~SceneBuilder() = default;
    virtual SceneBuilder* reset() = 0;
    virtual SceneBuilder* setName(const std::string& name) = 0;
    virtual SceneBuilder* setBackgroundColor(const std::string& color) = 0;
    virtual SceneBuilder* addShape(const std::string& name, std::unique_ptr<Shape> shape) = 0;
    virtual SceneBuilder* addGroup(const std::string& name) = 0;
    virtual SceneBuilder* addShapeToGroup(const std::string& groupName, const std::string& shapeName, std::unique_ptr<Shape> shape) = 0;
    virtual SceneBuilder* setTransform(const std::string& name, double x, double y, double scale = 1.0, double rotation = 0.0) = 0;
    virtual std::unique_ptr<Scene> getResult() = 0;
};

// Concrete Builder Implementation
class SceneBuilderImpl : public SceneBuilder {
private:
    std::unique_ptr<Scene> scene;
    std::map<std::string, SceneNode*> nodeMap; // For quick access to named nodes

public:
    SceneBuilderImpl() {
        reset();
    }

    SceneBuilder* reset() override {
        scene = std::make_unique<Scene>("DefaultScene");
        nodeMap.clear();
        nodeMap["Root"] = scene->getRoot();
        return this;
    }

    SceneBuilder* setName(const std::string& name) override {
        scene->getRoot()->name = name;
        return this;
    }

    SceneBuilder* setBackgroundColor(const std::string& color) override {
        // For the purpose of this builder, we'll store it in the root's name
        // A more complete implementation would have a proper scene metadata system
        return this;
    }

    SceneBuilder* addShape(const std::string& name, std::unique_ptr<Shape> shape) override {
        auto node = std::make_unique<SceneNode>(name, std::move(shape));
        scene->getRoot()->addChild(std::move(node));
        
        // Update node map - find the last child we just added
        SceneNode* addedNode = scene->getRoot()->children.back().get();
        nodeMap[name] = addedNode;
        
        return this;
    }

    SceneBuilder* addGroup(const std::string& name) override {
        auto node = std::make_unique<SceneNode>(name, nullptr);
        scene->getRoot()->addChild(std::move(node));
        
        // Update node map - find the last child we just added
        SceneNode* addedNode = scene->getRoot()->children.back().get();
        nodeMap[name] = addedNode;
        
        return this;
    }

    SceneBuilder* addShapeToGroup(const std::string& groupName, const std::string& shapeName, std::unique_ptr<Shape> shape) override {
        auto it = nodeMap.find(groupName);
        if (it != nodeMap.end()) {
            auto childNode = std::make_unique<SceneNode>(shapeName, std::move(shape));
            it->second->addChild(std::move(childNode));
            
            // Add to node map
            SceneNode* addedNode = it->second->children.back().get();
            nodeMap[shapeName] = addedNode;
        }
        return this;
    }

    SceneBuilder* setTransform(const std::string& name, double x, double y, double scale, double rotation) override {
        auto it = nodeMap.find(name);
        if (it != nodeMap.end()) {
            it->second->setLocalTransform(x, y, scale, rotation);
        }
        return this;
    }

    std::unique_ptr<Scene> getResult() override {
        auto result = std::move(scene);
        reset(); // Prepare for next use
        return result;
    }
};

// Director class that uses the builder to create predefined scenes
class SceneDirector {
private:
    std::unique_ptr<SceneBuilder> builder;

public:
    explicit SceneDirector(std::unique_ptr<SceneBuilder> b) : builder(std::move(b)) {}

    void setBuilder(std::unique_ptr<SceneBuilder> b) {
        builder = std::move(b);
    }

    std::unique_ptr<Scene> createSimpleScene() {
        builder->reset()
               ->setName("Simple Scene")
               ->addShape("MainCircle", std::make_unique<Circle>(5.0, "red"))
               ->setTransform("MainCircle", 10.0, 10.0, 1.0, 0.0);
        return builder->getResult();
    }

    std::unique_ptr<Scene> createComplexScene() {
        builder->reset()
               ->setName("Complex Scene")
               ->setBackgroundColor("lightblue")
               ->addShape("BackgroundCircle", std::make_unique<Circle>(20.0, "blue"))
               ->setTransform("BackgroundCircle", 0.0, 0.0, 1.0, 0.0)
               ->addGroup("CharacterGroup")
               ->setTransform("CharacterGroup", 5.0, 5.0, 1.0, 0.0)
               ->addShapeToGroup("CharacterGroup", "Head", std::make_unique<Circle>(3.0, "pink"))
               ->setTransform("Head", 0.0, 0.0, 1.0, 0.0)
               ->addShapeToGroup("CharacterGroup", "Body", std::make_unique<Rectangle>(6.0, 8.0, "blue"))
               ->setTransform("Body", 0.0, -5.0, 1.0, 0.0);
        return builder->getResult();
    }

    std::unique_ptr<Scene> createHierarchicalScene() {
        builder->reset()
               ->setName("Hierarchical Scene")
               ->setBackgroundColor("darkgray")
               ->addGroup("SolarSystem")
               ->setTransform("SolarSystem", 0.0, 0.0, 1.0, 0.0)
               ->addShapeToGroup("SolarSystem", "Sun", std::make_unique<Circle>(8.0, "yellow"))
               ->setTransform("Sun", 0.0, 0.0, 1.0, 0.0)
               ->addGroup("EarthOrbit")
               ->setTransform("EarthOrbit", 0.0, 0.0, 1.0, 0.0)
               ->addShapeToGroup("EarthOrbit", "Earth", std::make_unique<Circle>(3.0, "blue"))
               ->setTransform("Earth", 25.0, 0.0, 1.0, 0.0)
               ->addGroup("MoonOrbit")
               ->setTransform("MoonOrbit", 25.0, 0.0, 1.0, 0.0)
               ->addShapeToGroup("MoonOrbit", "Moon", std::make_unique<Circle>(1.0, "gray"))
               ->setTransform("Moon", 5.0, 0.0, 1.0, 0.0);
        return builder->getResult();
    }
};

// Fluent Builder interface for more readable construction
class FluentSceneBuilder {
private:
    std::unique_ptr<SceneBuilder> builder;

public:
    FluentSceneBuilder() : builder(std::make_unique<SceneBuilderImpl>()) {}

    FluentSceneBuilder& name(const std::string& sceneName) {
        builder->setName(sceneName);
        return *this;
    }

    FluentSceneBuilder& background(const std::string& color) {
        // Note: In a real implementation, this would set the actual background
        return *this;
    }

    FluentSceneBuilder& addCircle(const std::string& name, double radius, const std::string& color = "red") {
        builder->addShape(name, std::make_unique<Circle>(radius, color));
        return *this;
    }

    FluentSceneBuilder& addRectangle(const std::string& name, double width, double height, const std::string& color = "blue") {
        builder->addShape(name, std::make_unique<Rectangle>(width, height, color));
        return *this;
    }

    FluentSceneBuilder& group(const std::string& name) {
        builder->addGroup(name);
        return *this;
    }

    FluentSceneBuilder& addCircleToGroup(const std::string& groupName, const std::string& name,
                                         double radius, const std::string& color = "red") {
        builder->addShapeToGroup(groupName, name, std::make_unique<Circle>(radius, color));
        return *this;
    }

    FluentSceneBuilder& addRectangleToGroup(const std::string& groupName, const std::string& name,
                                            double width, double height, const std::string& color = "blue") {
        builder->addShapeToGroup(groupName, name, std::make_unique<Rectangle>(width, height, color));
        return *this;
    }

    FluentSceneBuilder& transform(const std::string& name, double x, double y, 
                                  double scale = 1.0, double rotation = 0.0) {
        builder->setTransform(name, x, y, scale, rotation);
        return *this;
    }

    std::unique_ptr<Scene> build() {
        return builder->getResult();
    }
};

#endif // SCENE_BUILDER_H