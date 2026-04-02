#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Base class
class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw() const = 0;
    virtual std::string getType() const = 0;
};

// Concrete implementations
class Circle : public Shape {
public:
    void draw() const override {
        std::cout << "Drawing a circle" << std::endl;
    }
    
    std::string getType() const override {
        return "Circle";
    }
};

class Rectangle : public Shape {
public:
    void draw() const override {
        std::cout << "Drawing a rectangle" << std::endl;
    }
    
    std::string getType() const override {
        return "Rectangle";
    }
};

// Factory using unique_ptr
class UniquePtrShapeFactory {
public:
    static std::unique_ptr<Shape> createShape(const std::string& shapeType) {
        if (shapeType == "circle") {
            return std::make_unique<Circle>();
        } else if (shapeType == "rectangle") {
            return std::make_unique<Rectangle>();
        }
        return nullptr;
    }
};

// Factory returning raw pointers (NOT recommended)
class RawPtrShapeFactory {
public:
    static Shape* createShape(const std::string& shapeType) {
        if (shapeType == "circle") {
            return new Circle();  // Caller must remember to delete!
        } else if (shapeType == "rectangle") {
            return new Rectangle();
        }
        return nullptr;
    }
};

// Factory returning shared_ptr (when sharing ownership is needed)
class SharedPtrShapeFactory {
public:
    static std::shared_ptr<Shape> createShape(const std::string& shapeType) {
        if (shapeType == "circle") {
            return std::make_shared<Circle>();
        } else if (shapeType == "rectangle") {
            return std::make_shared<Rectangle>();
        }
        return nullptr;
    }
};

int main() {
    std::cout << "Using unique_ptr factory:" << std::endl;
    auto shape1 = UniquePtrShapeFactory::createShape("circle");
    if (shape1) {
        shape1->draw();
    }
    // Automatic cleanup when shape1 goes out of scope
    
    std::cout << "\nUsing raw pointer factory (not recommended):" << std::endl;
    Shape* rawShape = RawPtrShapeFactory::createShape("rectangle");
    if (rawShape) {
        rawShape->draw();
        delete rawShape;  // Manual cleanup required - easy to forget!
    }
    
    std::cout << "\nUsing shared_ptr factory:" << std::endl;
    auto shape2 = SharedPtrShapeFactory::createShape("circle");
    if (shape2) {
        shape2->draw();
        // Cleanup happens when last shared_ptr referencing the object is destroyed
    }
    
    // Demonstrate unique_ptr move semantics
    std::cout << "\nDemonstrating unique_ptr move:" << std::endl;
    auto shape3 = UniquePtrShapeFactory::createShape("circle");
    std::vector<std::unique_ptr<Shape>> shapeVector;
    
    // Move shape3 into the vector - no copy, just transfer of ownership
    shapeVector.push_back(std::move(shape3));
    std::cout << "Shape moved to vector. Original shape3 is now null: " 
              << (shape3 ? "false" : "true") << std::endl;
    
    if (shapeVector[0]) {
        shapeVector[0]->draw();
    }
    // All shapes automatically cleaned up when going out of scope
    return 0;
}