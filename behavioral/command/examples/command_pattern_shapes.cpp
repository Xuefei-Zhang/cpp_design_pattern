#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <stack>
#include <functional>

// Shape hierarchy (same as before)
class IDrawable {
public:
    virtual ~IDrawable() = default;
    virtual void draw() const = 0;
    virtual void resize(double factor) = 0;
    virtual double getArea() const = 0;
};

class Shape : public IDrawable {
protected:
    double x = 0.0, y = 0.0;
    double scale = 1.0;
    std::string id;

public:
    Shape(double pos_x = 0.0, double pos_y = 0.0, const std::string& shape_id = "") 
        : x(pos_x), y(pos_y), id(shape_id) {}

    void setPosition(double pos_x, double pos_y) {
        x = pos_x;
        y = pos_y;
    }

    void getPosition(double& pos_x, double& pos_y) const {
        pos_x = x;
        pos_y = y;
    }

    virtual std::string getType() const = 0;

    void resize(double factor) override {
        scale *= factor;
    }

    virtual void draw() const = 0;
    virtual double getArea() const = 0;
    
    const std::string& getId() const { return id; }
    void setId(const std::string& newId) { id = newId; }
};

class Circle : public Shape {
private:
    double radius;

public:
    Circle(double r = 1.0, double pos_x = 0.0, double pos_y = 0.0, const std::string& shape_id = "")
        : Shape(pos_x, pos_y, shape_id), radius(r) {}

    void draw() const override {
        std::cout << "Drawing a " << getType() << " with radius " << radius
                  << " at position (" << x << ", " << y << ") [ID: " << id << "]" << std::endl;
    }

    std::string getType() const override {
        return "Circle";
    }

    double getArea() const override {
        return 3.14159 * radius * radius * scale * scale;
    }
};

class Rectangle : public Shape {
private:
    double width, height;

public:
    Rectangle(double w = 1.0, double h = 1.0, double pos_x = 0.0, double pos_y = 0.0, const std::string& shape_id = "")
        : Shape(pos_x, pos_y, shape_id), width(w), height(h) {}

    void draw() const override {
        std::cout << "Drawing a " << getType() << " with dimensions "
                  << width << "x" << height << " at position (" << x << ", " << y << ") [ID: " << id << "]" << std::endl;
    }

    std::string getType() const override {
        return "Rectangle";
    }

    double getArea() const override {
        return width * height * scale * scale;
    }
};

class Triangle : public Shape {
private:
    double base, height;

public:
    Triangle(double b = 1.0, double h = 1.0, double pos_x = 0.0, double pos_y = 0.0, const std::string& shape_id = "")
        : Shape(pos_x, pos_y, shape_id), base(b), height(h) {}

    void draw() const override {
        std::cout << "Drawing a " << getType() << " with base " << base
                  << " and height " << height << " at position (" << x << ", " << y << ") [ID: " << id << "]" << std::endl;
    }

    std::string getType() const override {
        return "Triangle";
    }

    double getArea() const override {
        return 0.5 * base * height * scale * scale;
    }
};

// Command interface
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual std::string getName() const = 0;
};

// Concrete command: Move Shape
class MoveShapeCommand : public Command {
private:
    std::shared_ptr<Shape> shape;
    double oldX, oldY;
    double newX, newY;
    bool executed = false;

public:
    MoveShapeCommand(std::shared_ptr<Shape> s, double toX, double toY) 
        : shape(s), newX(toX), newY(toY) {
        if (shape) {
            shape->getPosition(oldX, oldY);
        }
    }

    void execute() override {
        if (shape) {
            shape->setPosition(newX, newY);
            executed = true;
            std::cout << "Executed: Moving " << shape->getType() 
                     << " from (" << oldX << ", " << oldY << ") to (" 
                     << newX << ", " << newY << ") [ID: " << shape->getId() << "]" << std::endl;
        }
    }

    void undo() override {
        if (shape && executed) {
            shape->setPosition(oldX, oldY);
            std::cout << "Undid: Moving " << shape->getType() 
                     << " back to (" << oldX << ", " << oldY << ") [ID: " << shape->getId() << "]" << std::endl;
        }
    }

    std::string getName() const override {
        return "Move Shape";
    }
};

// Concrete command: Resize Shape
class ResizeShapeCommand : public Command {
private:
    std::shared_ptr<Shape> shape;
    double oldScaleFactor;
    double newScaleFactor;
    bool executed = false;

public:
    ResizeShapeCommand(std::shared_ptr<Shape> s, double newFactor)
        : shape(s), newScaleFactor(newFactor) {
        if (shape) {
            // Store the original scale by calculating what it was before any resizes
            // For this example, we'll assume the original scale factor was 1.0
            oldScaleFactor = 1.0;
        }
    }

    void execute() override {
        if (shape) {
            double currentScale = shape->getArea(); // Get current area as reference
            shape->resize(newScaleFactor);
            executed = true;
            std::cout << "Executed: Resizing " << shape->getType()
                     << " with factor " << newScaleFactor << " [ID: " << shape->getId() << "]" << std::endl;
        }
    }

    void undo() override {
        if (shape && executed) {
            shape->resize(1.0/newScaleFactor); // Undo by applying inverse of the scale factor
            std::cout << "Undid: Resizing " << shape->getType()
                     << " back using inverse factor [ID: " << shape->getId() << "]" << std::endl;
        }
    }

    std::string getName() const override {
        return "Resize Shape";
    }
};

// Concrete command: Create Shape
class CreateShapeCommand : public Command {
private:
    std::shared_ptr<Shape> shape;
    std::function<std::shared_ptr<Shape>()> factory;
    bool executed = false;

public:
    CreateShapeCommand(std::function<std::shared_ptr<Shape>()> shapeFactory)
        : factory(shapeFactory) {}

    void execute() override {
        shape = factory();
        executed = true;
        if (shape) {
            std::cout << "Executed: Creating " << shape->getType()
                     << " [ID: " << shape->getId() << "]" << std::endl;
        }
    }

    void undo() override {
        if (shape && executed) {
            std::cout << "Undid: Deleting " << shape->getType()
                     << " [ID: " << shape->getId() << "]" << std::endl;
            shape.reset(); // Delete the shape
        }
    }

    std::string getName() const override {
        return "Create Shape";
    }

    std::shared_ptr<Shape> getCreatedShape() const {
        return shape;
    }
};

// Concrete command: Delete Shape
class DeleteShapeCommand : public Command {
private:
    std::shared_ptr<Shape> shape;
    bool executed = false;

public:
    DeleteShapeCommand(std::shared_ptr<Shape> s) : shape(s) {}

    void execute() override {
        if (shape) {
            executed = true;
            std::cout << "Executed: Deleting " << shape->getType() 
                     << " [ID: " << shape->getId() << "]" << std::endl;
            shape.reset(); // Actually delete the shape
        }
    }

    void undo() override {
        std::cout << "Cannot undo: Shape deletion is not reversible" << std::endl;
    }

    std::string getName() const override {
        return "Delete Shape";
    }
};

// Command invoker / manager
class CommandManager {
private:
    std::vector<std::shared_ptr<Command>> commandHistory;
    std::stack<std::shared_ptr<Command>> undoStack;
    std::stack<std::shared_ptr<Command>> redoStack;

public:
    void executeCommand(std::shared_ptr<Command> command) {
        if (command) {
            command->execute();
            commandHistory.push_back(command);
            undoStack.push(command);
            
            // Clear redo stack when new command is executed
            while (!redoStack.empty()) {
                redoStack.pop();
            }
            
            std::cout << "Command executed: " << command->getName() << std::endl;
        }
    }

    void undoLastCommand() {
        if (!undoStack.empty()) {
            auto command = undoStack.top();
            command->undo();
            redoStack.push(command);
            undoStack.pop();
            std::cout << "Command undone: " << command->getName() << std::endl;
        } else {
            std::cout << "No commands to undo" << std::endl;
        }
    }

    void redoLastCommand() {
        if (!redoStack.empty()) {
            auto command = redoStack.top();
            command->execute();
            undoStack.push(command);
            redoStack.pop();
            std::cout << "Command redone: " << command->getName() << std::endl;
        } else {
            std::cout << "No commands to redo" << std::endl;
        }
    }

    void showHistory() const {
        std::cout << "\nCommand History:" << std::endl;
        for (size_t i = 0; i < commandHistory.size(); ++i) {
            std::cout << (i + 1) << ". " << commandHistory[i]->getName() << std::endl;
        }
    }

    void clearHistory() {
        commandHistory.clear();
        while (!undoStack.empty()) undoStack.pop();
        while (!redoStack.empty()) redoStack.pop();
    }
};

// Shape factory class
class ShapeFactory {
public:
    enum class ShapeType {
        CIRCLE,
        RECTANGLE,
        TRIANGLE
    };

    static std::shared_ptr<Shape> createShape(ShapeType type,
                                              double param1 = 1.0,
                                              double param2 = 1.0,
                                              double pos_x = 0.0,
                                              double pos_y = 0.0,
                                              const std::string& id = "") {
        switch (type) {
            case ShapeType::CIRCLE: {
                auto circle = std::make_unique<Circle>(param1, pos_x, pos_y, id);
                return std::shared_ptr<Shape>(circle.release());
            }
            case ShapeType::RECTANGLE: {
                auto rect = std::make_unique<Rectangle>(param1, param2, pos_x, pos_y, id);
                return std::shared_ptr<Shape>(rect.release());
            }
            case ShapeType::TRIANGLE: {
                auto tri = std::make_unique<Triangle>(param1, param2, pos_x, pos_y, id);
                return std::shared_ptr<Shape>(tri.release());
            }
            default:
                return nullptr;
        }
    }
};

int main() {
    std::cout << "Command Pattern with Shapes Example\n" << std::endl;

    CommandManager manager;

    std::cout << "Creating a circle..." << std::endl;
    auto createCircleCmd = std::make_shared<CreateShapeCommand>(
        []() { return ShapeFactory::createShape(ShapeFactory::ShapeType::CIRCLE, 2.0, 0.0, 5.0, 5.0, "circle_1"); }
    );
    manager.executeCommand(createCircleCmd);

    // Get the created shape
    auto shape = std::dynamic_pointer_cast<CreateShapeCommand>(createCircleCmd)->getCreatedShape();

    std::cout << "\nMoving the circle..." << std::endl;
    auto moveCmd = std::make_shared<MoveShapeCommand>(shape, 10.0, 15.0);
    manager.executeCommand(moveCmd);

    std::cout << "\nResizing the circle..." << std::endl;
    auto resizeCmd = std::make_shared<ResizeShapeCommand>(shape, 2.0);
    manager.executeCommand(resizeCmd);

    std::cout << "\nCreating a rectangle..." << std::endl;
    auto createRectCmd = std::make_shared<CreateShapeCommand>(
        []() { return ShapeFactory::createShape(ShapeFactory::ShapeType::RECTANGLE, 3.0, 4.0, 0.0, 0.0, "rect_1"); }
    );
    manager.executeCommand(createRectCmd);

    auto rectShape = std::dynamic_pointer_cast<CreateShapeCommand>(createRectCmd)->getCreatedShape();

    std::cout << "\nMoving the rectangle..." << std::endl;
    auto moveRectCmd = std::make_shared<MoveShapeCommand>(rectShape, 20.0, 30.0);
    manager.executeCommand(moveRectCmd);
    
    std::cout << "\n" << std::string(50, '-') << std::endl;
    manager.showHistory();

    std::cout << "\n" << std::string(50, '-') << std::endl;
    std::cout << "Undoing last command (move rectangle)..." << std::endl;
    manager.undoLastCommand();

    std::cout << "\nUndoing last command (resize circle)..." << std::endl;
    manager.undoLastCommand();

    std::cout << "\nRedoing last command (resize circle)..." << std::endl;
    manager.redoLastCommand();

    std::cout << "\n" << std::string(50, '-') << std::endl;
    std::cout << "Creating and manipulating a triangle..." << std::endl;

    // Create a triangle
    auto createTriCmd = std::make_shared<CreateShapeCommand>(
        []() { return ShapeFactory::createShape(ShapeFactory::ShapeType::TRIANGLE, 3.0, 4.0, 1.0, 1.0, "triangle_1"); }
    );
    manager.executeCommand(createTriCmd);

    auto triShape = std::dynamic_pointer_cast<CreateShapeCommand>(createTriCmd)->getCreatedShape();

    // Move the triangle
    auto moveTriCmd = std::make_shared<MoveShapeCommand>(triShape, 5.0, 5.0);
    manager.executeCommand(moveTriCmd);

    // Resize the triangle
    auto resizeTriCmd = std::make_shared<ResizeShapeCommand>(triShape, 1.5);
    manager.executeCommand(resizeTriCmd);

    std::cout << "\nFinal state after all operations:" << std::endl;
    manager.showHistory();

    std::cout << "\nCommand Pattern demonstration completed!" << std::endl;

    return 0;
}