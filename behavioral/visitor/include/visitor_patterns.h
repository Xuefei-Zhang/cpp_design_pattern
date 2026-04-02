#ifndef VISITOR_PATTERNS_H
#define VISITOR_PATTERNS_H

#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Forward declarations
class Circle;
class Rectangle;
class Triangle;

// Visitor interface
class ShapeVisitor {
public:
    virtual ~ShapeVisitor() = default;
    virtual void visitCircle(Circle* circle) = 0;
    virtual void visitRectangle(Rectangle* rectangle) = 0;
    virtual void visitTriangle(Triangle* triangle) = 0;
};

// Element interface
class Shape {
public:
    virtual ~Shape() = default;
    virtual void accept(ShapeVisitor* visitor) = 0;
    virtual std::string getName() const = 0;
};

// Concrete Elements
class Circle : public Shape {
private:
    double radius_;

public:
    explicit Circle(double radius) : radius_(radius) {}

    void accept(ShapeVisitor* visitor) override {
        visitor->visitCircle(this);
    }

    std::string getName() const override {
        return "Circle";
    }

    double getRadius() const {
        return radius_;
    }

    void setRadius(double radius) {
        radius_ = radius;
    }
};

class Rectangle : public Shape {
private:
    double width_;
    double height_;

public:
    Rectangle(double width, double height)
        : width_(width), height_(height) {}

    void accept(ShapeVisitor* visitor) override {
        visitor->visitRectangle(this);
    }

    std::string getName() const override {
        return "Rectangle";
    }

    double getWidth() const {
        return width_;
    }

    double getHeight() const {
        return height_;
    }

    void setWidth(double width) {
        width_ = width;
    }

    void setHeight(double height) {
        height_ = height;
    }
};

class Triangle : public Shape {
private:
    double base_;
    double height_;

public:
    Triangle(double base, double height)
        : base_(base), height_(height) {}

    void accept(ShapeVisitor* visitor) override {
        visitor->visitTriangle(this);
    }

    std::string getName() const override {
        return "Triangle";
    }

    double getBase() const {
        return base_;
    }

    double getHeight() const {
        return height_;
    }

    void setBase(double base) {
        base_ = base;
    }

    void setHeight(double height) {
        height_ = height;
    }
};

// Concrete Visitor - Area Calculator
class AreaCalculator : public ShapeVisitor {
private:
    double totalArea_;

public:
    AreaCalculator() : totalArea_(0.0) {}

    void visitCircle(Circle* circle) override {
        double area = 3.14159 * circle->getRadius() * circle->getRadius();
        totalArea_ += area;
        std::cout << "   Circle area: " << area << "\n";
    }

    void visitRectangle(Rectangle* rectangle) override {
        double area = rectangle->getWidth() * rectangle->getHeight();
        totalArea_ += area;
        std::cout << "   Rectangle area: " << area << "\n";
    }

    void visitTriangle(Triangle* triangle) override {
        double area = 0.5 * triangle->getBase() * triangle->getHeight();
        totalArea_ += area;
        std::cout << "   Triangle area: " << area << "\n";
    }

    double getTotalArea() const {
        return totalArea_;
    }

    void reset() {
        totalArea_ = 0.0;
    }
};

// Concrete Visitor - Perimeter Calculator
class PerimeterCalculator : public ShapeVisitor {
private:
    double totalPerimeter_;

public:
    PerimeterCalculator() : totalPerimeter_(0.0) {}

    void visitCircle(Circle* circle) override {
        double perimeter = 2.0 * 3.14159 * circle->getRadius();
        totalPerimeter_ += perimeter;
        std::cout << "   Circle perimeter: " << perimeter << "\n";
    }

    void visitRectangle(Rectangle* rectangle) override {
        double perimeter = 2.0 * (rectangle->getWidth() + rectangle->getHeight());
        totalPerimeter_ += perimeter;
        std::cout << "   Rectangle perimeter: " << perimeter << "\n";
    }

    void visitTriangle(Triangle* triangle) override {
        // Simplified - assumes right triangle for demo
        double hypotenuse = std::sqrt(
            triangle->getBase() * triangle->getBase() +
            triangle->getHeight() * triangle->getHeight()
        );
        double perimeter = triangle->getBase() + triangle->getHeight() + hypotenuse;
        totalPerimeter_ += perimeter;
        std::cout << "   Triangle perimeter: " << perimeter << "\n";
    }

    double getTotalPerimeter() const {
        return totalPerimeter_;
    }

    void reset() {
        totalPerimeter_ = 0.0;
    }
};

// Concrete Visitor - XML Exporter
class XMLExporter : public ShapeVisitor {
private:
    std::string xml_;

public:
    XMLExporter() : xml_("<shapes>\n") {}

    void visitCircle(Circle* circle) override {
        xml_ += "  <circle radius=\"" + std::to_string(circle->getRadius()) + "\"/>\n";
    }

    void visitRectangle(Rectangle* rectangle) override {
        xml_ += "  <rectangle width=\"" + std::to_string(rectangle->getWidth()) +
                "\" height=\"" + std::to_string(rectangle->getHeight()) + "\"/>\n";
    }

    void visitTriangle(Triangle* triangle) override {
        xml_ += "  <triangle base=\"" + std::to_string(triangle->getBase()) +
                "\" height=\"" + std::to_string(triangle->getHeight()) + "\"/>\n";
    }

    std::string getXML() {
        return xml_ + "</shapes>\n";
    }

    void reset() {
        xml_ = "<shapes>\n";
    }
};

// Concrete Visitor - JSON Exporter
class JSONExporter : public ShapeVisitor {
private:
    std::vector<std::string> jsonObjects_;

public:
    void visitCircle(Circle* circle) override {
        std::string json = "{\"type\":\"circle\",\"radius\":" +
                          std::to_string(circle->getRadius()) + "}";
        jsonObjects_.push_back(json);
    }

    void visitRectangle(Rectangle* rectangle) override {
        std::string json = "{\"type\":\"rectangle\",\"width\":" +
                          std::to_string(rectangle->getWidth()) +
                          ",\"height\":" + std::to_string(rectangle->getHeight()) + "}";
        jsonObjects_.push_back(json);
    }

    void visitTriangle(Triangle* triangle) override {
        std::string json = "{\"type\":\"triangle\",\"base\":" +
                          std::to_string(triangle->getBase()) +
                          ",\"height\":" + std::to_string(triangle->getHeight()) + "}";
        jsonObjects_.push_back(json);
    }

    std::string getJSON() {
        if (jsonObjects_.empty()) {
            return "[]";
        }

        std::string result = "[";
        for (std::size_t i = 0; i < jsonObjects_.size(); ++i) {
            result += jsonObjects_[i];
            if (i < jsonObjects_.size() - 1) {
                result += ",";
            }
        }
        result += "]";
        return result;
    }

    void reset() {
        jsonObjects_.clear();
    }

    std::size_t getCount() const {
        return jsonObjects_.size();
    }
};

// Concrete Visitor - Drawing Renderer
class DrawingRenderer : public ShapeVisitor {
private:
    std::vector<std::string> drawCommands_;

public:
    void visitCircle(Circle* circle) override {
        std::string cmd = "DRAW_CIRCLE(radius=" + std::to_string(circle->getRadius()) + ")";
        drawCommands_.push_back(cmd);
        std::cout << "   " << cmd << "\n";
    }

    void visitRectangle(Rectangle* rectangle) override {
        std::string cmd = "DRAW_RECTANGLE(w=" + std::to_string(rectangle->getWidth()) +
                         ", h=" + std::to_string(rectangle->getHeight()) + ")";
        drawCommands_.push_back(cmd);
        std::cout << "   " << cmd << "\n";
    }

    void visitTriangle(Triangle* triangle) override {
        std::string cmd = "DRAW_TRIANGLE(base=" + std::to_string(triangle->getBase()) +
                         ", h=" + std::to_string(triangle->getHeight()) + ")";
        drawCommands_.push_back(cmd);
        std::cout << "   " << cmd << "\n";
    }

    std::size_t getCommandCount() const {
        return drawCommands_.size();
    }

    const std::vector<std::string>& getCommands() const {
        return drawCommands_;
    }

    void reset() {
        drawCommands_.clear();
    }
};

// Concrete Visitor - Scale Transformer
class ScaleTransformer : public ShapeVisitor {
private:
    double scaleFactor_;

public:
    explicit ScaleTransformer(double factor) : scaleFactor_(factor) {}

    void visitCircle(Circle* circle) override {
        circle->setRadius(circle->getRadius() * scaleFactor_);
        std::cout << "   Scaled circle to radius: " << circle->getRadius() << "\n";
    }

    void visitRectangle(Rectangle* rectangle) override {
        rectangle->setWidth(rectangle->getWidth() * scaleFactor_);
        rectangle->setHeight(rectangle->getHeight() * scaleFactor_);
        std::cout << "   Scaled rectangle to " << rectangle->getWidth()
                  << "x" << rectangle->getHeight() << "\n";
    }

    void visitTriangle(Triangle* triangle) override {
        triangle->setBase(triangle->getBase() * scaleFactor_);
        triangle->setHeight(triangle->getHeight() * scaleFactor_);
        std::cout << "   Scaled triangle to base=" << triangle->getBase()
                  << ", height=" << triangle->getHeight() << "\n";
    }

    void setScaleFactor(double factor) {
        scaleFactor_ = factor;
    }
};

// Object structure that holds elements
class Drawing {
private:
    std::vector<std::unique_ptr<Shape>> shapes_;

public:
    void addShape(std::unique_ptr<Shape> shape) {
        shapes_.push_back(std::move(shape));
    }

    void accept(ShapeVisitor* visitor) {
        for (auto& shape : shapes_) {
            shape->accept(visitor);
        }
    }

    std::size_t getShapeCount() const {
        return shapes_.size();
    }
};

// Inline usage examples:
// Drawing drawing;
// drawing.addShape(std::make_unique<Circle>(5.0));
// drawing.addShape(std::make_unique<Rectangle>(4.0, 3.0));
// AreaCalculator areaCalc;
// drawing.accept(&areaCalc);
// double total = areaCalc.getTotalArea();

#endif // VISITOR_PATTERNS_H
