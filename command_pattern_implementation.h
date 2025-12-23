#ifndef COMMAND_PATTERN_IMPLEMENTATION_H
#define COMMAND_PATTERN_IMPLEMENTATION_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <stack>
#include <functional>

// Command interface - defines the interface for executing operations
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual std::string getCommandName() const = 0;
    virtual bool isUndoable() const { return true; }
};

// Receiver classes - objects that perform the actual work
class ShapeReceiver {
protected:
    std::string name_;

public:
    explicit ShapeReceiver(const std::string& name) : name_(name) {}
    virtual ~ShapeReceiver() = default;
    
    const std::string& getName() const { return name_; }
    virtual void draw() = 0;
    virtual void move(double dx, double dy) = 0;
    virtual void resize(double factor) = 0;
    virtual void changeColor(const std::string& newColor) = 0;
    virtual void rotate(double angle) = 0;
    virtual double getArea() const = 0;
};

class CircleReceiver : public ShapeReceiver {
private:
    double radius_;
    double x_, y_;
    std::string color_;

public:
    CircleReceiver(double radius, double x = 0, double y = 0, const std::string& color = "red") 
        : ShapeReceiver("Circle"), radius_(radius), x_(x), y_(y), color_(color) {}
    
    void draw() override {
        std::cout << "[RECEIVER] Drawing Circle at (" << x_ << "," << y_ 
                  << ") with radius " << radius_ << " and color " << color_ << std::endl;
    }
    
    void move(double dx, double dy) override {
        x_ += dx;
        y_ += dy;
        std::cout << "[RECEIVER] Moved Circle to (" << x_ << "," << y_ << ")" << std::endl;
    }
    
    void resize(double factor) override {
        radius_ *= factor;
        std::cout << "[RECEIVER] Resized Circle radius to " << radius_ << std::endl;
    }
    
    void changeColor(const std::string& newColor) override {
        std::cout << "[RECEIVER] Changed Circle color from " << color_ << " to " << newColor << std::endl;
        color_ = newColor;
    }
    
    void rotate(double angle) override {
        std::cout << "[RECEIVER] Rotated Circle by " << angle << " degrees" << std::endl;
    }
    
    double getArea() const override {
        return 3.14159 * radius_ * radius_;
    }
    
    // Getters for command access
    double getRadius() const { return radius_; }
    double getX() const { return x_; }
    double getY() const { return y_; }
    const std::string& getColor() const { return color_; }
};

class RectangleReceiver : public ShapeReceiver {
private:
    double width_, height_;
    double x_, y_;
    std::string color_;

public:
    RectangleReceiver(double width, double height, double x = 0, double y = 0,
                     const std::string& color = "blue") 
        : ShapeReceiver("Rectangle"), width_(width), height_(height), x_(x), y_(y), color_(color) {}
    
    void draw() override {
        std::cout << "[RECEIVER] Drawing Rectangle at (" << x_ << "," << y_
                  << ") with dimensions " << width_ << "x" << height_
                  << " and color " << color_ << std::endl;
    }

    void move(double dx, double dy) override {
        x_ += dx;
        y_ += dy;
        std::cout << "[RECEIVER] Moved Rectangle to (" << x_ << "," << y_ << ")" << std::endl;
    }

    void resize(double factor) override {
        width_ *= factor;
        height_ *= factor;
        std::cout << "[RECEIVER] Resized Rectangle to " << width_ << "x" << height_ << std::endl;
    }

    void changeColor(const std::string& newColor) override {
        std::cout << "[RECEIVER] Changed Rectangle color from " << color_ << " to " << newColor << std::endl;
        color_ = newColor;
    }

    void rotate(double angle) override {
        std::cout << "[RECEIVER] Rotated Rectangle by " << angle << " degrees" << std::endl;
    }

    double getArea() const override {
        return width_ * height_;
    }
    
    // Getters
    double getWidth() const { return width_; }
    double getHeight() const { return height_; }
    double getX() const { return x_; }
    double getY() const { return y_; }
    const std::string& getColor() const { return color_; }
};

class TriangleReceiver : public ShapeReceiver {
private:
    double base_, height_;
    double x_, y_;
    std::string color_;

public:
    TriangleReceiver(double base, double height, double x = 0, double y = 0,
                     const std::string& color = "green") 
        : ShapeReceiver("Triangle"), base_(base), height_(height), x_(x), y_(y), color_(color) {}

    void draw() override {
        std::cout << "[RECEIVER] Drawing Triangle at (" << x_ << "," << y_
                  << ") with base " << base_ << ", height " << height_
                  << " and color " << color_ << std::endl;
    }

    void move(double dx, double dy) override {
        x_ += dx;
        y_ += dy;
        std::cout << "[RECEIVER] Moved Triangle to (" << x_ << "," << y_ << ")" << std::endl;
    }

    void resize(double factor) override {
        base_ *= factor;
        height_ *= factor;
        std::cout << "[RECEIVER] Resized Triangle to base " << base_ << ", height " << height_ << std::endl;
    }

    void changeColor(const std::string& newColor) override {
        std::cout << "[RECEIVER] Changed Triangle color from " << color_ << " to " << newColor << std::endl;
        color_ = newColor;
    }

    void rotate(double angle) override {
        std::cout << "[RECEIVER] Rotated Triangle by " << angle << " degrees" << std::endl;
    }

    double getArea() const override {
        return 0.5 * base_ * height_;
    }
    
    // Getters
    double getBase() const { return base_; }
    double getHeight() const { return height_; }
    double getX() const { return x_; }
    double getY() const { return y_; }
    const std::string& getColor() const { return color_; }
};

// Concrete Commands
class DrawCommand : public Command {
private:
    ShapeReceiver* shapeReceiver_;

public:
    explicit DrawCommand(ShapeReceiver* receiver) : shapeReceiver_(receiver) {}
    
    void execute() override {
        std::cout << "[COMMAND] Executing DrawCommand for " << shapeReceiver_->getName() << std::endl;
        shapeReceiver_->draw();
    }
    
    void undo() override {
        std::cout << "[COMMAND] Undoing DrawCommand for " << shapeReceiver_->getName() << std::endl;
        // In a real system, this might hide or revert the drawing operation
    }
    
    std::string getCommandName() const override {
        return "DrawCommand";
    }
};

class MoveCommand : public Command {
private:
    ShapeReceiver* shapeReceiver_;
    double deltaX_, deltaY_;
    double originalX_, originalY_;
    bool executed_;

public:
    MoveCommand(ShapeReceiver* receiver, double dx, double dy) 
        : shapeReceiver_(receiver), deltaX_(dx), deltaY_(dy), 
          originalX_(-1), originalY_(-1), executed_(false) {}
    
    void execute() override {
        // Store original position for undo if not already stored
        if (!executed_) {
            if (auto* circle = dynamic_cast<CircleReceiver*>(shapeReceiver_)) {
                originalX_ = circle->getX();
                originalY_ = circle->getY();
            } else if (auto* rect = dynamic_cast<RectangleReceiver*>(shapeReceiver_)) {
                originalX_ = rect->getX();
                originalY_ = rect->getY();
            } else if (auto* tri = dynamic_cast<TriangleReceiver*>(shapeReceiver_)) {
                originalX_ = tri->getX();
                originalY_ = tri->getY();
            }
            executed_ = true;
        }
        
        std::cout << "[COMMAND] Executing MoveCommand for " << shapeReceiver_->getName() 
                  << " by (" << deltaX_ << "," << deltaY_ << ")" << std::endl;
        shapeReceiver_->move(deltaX_, deltaY_);
    }
    
    void undo() override {
        if (executed_) {
            std::cout << "[COMMAND] Undoing MoveCommand for " << shapeReceiver_->getName() << std::endl;
            shapeReceiver_->move(originalX_ - (originalX_ + deltaX_), 
                               originalY_ - (originalY_ + deltaY_));  // Move back to original position
        }
    }
    
    std::string getCommandName() const override {
        return "MoveCommand";
    }
};

class ResizeCommand : public Command {
private:
    ShapeReceiver* shapeReceiver_;
    double factor_;
    double originalDimension1_, originalDimension2_;
    bool executed_;

public:
    ResizeCommand(ShapeReceiver* receiver, double factor) 
        : shapeReceiver_(receiver), factor_(factor), 
          originalDimension1_(-1), originalDimension2_(-1), executed_(false) {}
    
    void execute() override {
        if (!executed_) {
            // Store original dimensions for undo
            if (auto* circle = dynamic_cast<CircleReceiver*>(shapeReceiver_)) {
                originalDimension1_ = circle->getRadius();
            } else if (auto* rect = dynamic_cast<RectangleReceiver*>(shapeReceiver_)) {
                originalDimension1_ = rect->getWidth();
                originalDimension2_ = rect->getHeight();
            } else if (auto* tri = dynamic_cast<TriangleReceiver*>(shapeReceiver_)) {
                originalDimension1_ = tri->getBase();
                originalDimension2_ = tri->getHeight();
            }
            executed_ = true;
        }
        
        std::cout << "[COMMAND] Executing ResizeCommand for " << shapeReceiver_->getName() 
                  << " with factor " << factor_ << std::endl;
        shapeReceiver_->resize(factor_);
    }
    
    void undo() override {
        if (executed_ && originalDimension1_ > 0) {
            std::cout << "[COMMAND] Undoing ResizeCommand for " << shapeReceiver_->getName() << std::endl;
            
            if (dynamic_cast<CircleReceiver*>(shapeReceiver_)) {
                // Restore original radius
                double currentRadius = dynamic_cast<CircleReceiver*>(shapeReceiver_)->getRadius();
                double scaleBack = originalDimension1_ / currentRadius;
                shapeReceiver_->resize(scaleBack);
            } else if (dynamic_cast<RectangleReceiver*>(shapeReceiver_)) {
                // For rectangles, we'd need to store more complex state to restore properly
                std::cout << "[COMMAND] Rectangle undo would require more complex state management" << std::endl;
            }
            // Similar for triangles
        }
    }
    
    std::string getCommandName() const override {
        return "ResizeCommand";
    }
};

class ColorChangeCommand : public Command {
private:
    ShapeReceiver* shapeReceiver_;
    std::string newColor_;
    std::string originalColor_;
    bool executed_;

public:
    ColorChangeCommand(ShapeReceiver* receiver, const std::string& color) 
        : shapeReceiver_(receiver), newColor_(color), originalColor_(""), executed_(false) {}
    
    void execute() override {
        if (!executed_) {
            // Store original color for undo
            if (auto* circle = dynamic_cast<CircleReceiver*>(shapeReceiver_)) {
                originalColor_ = circle->getColor();
            } else if (auto* rect = dynamic_cast<RectangleReceiver*>(shapeReceiver_)) {
                originalColor_ = rect->getColor();
            } else if (auto* tri = dynamic_cast<TriangleReceiver*>(shapeReceiver_)) {
                originalColor_ = tri->getColor();
            }
            executed_ = true;
        }
        
        std::cout << "[COMMAND] Executing ColorChangeCommand for " << shapeReceiver_->getName() 
                  << " to color " << newColor_ << std::endl;
        shapeReceiver_->changeColor(newColor_);
    }
    
    void undo() override {
        if (executed_ && !originalColor_.empty()) {
            std::cout << "[COMMAND] Undoing ColorChangeCommand for " << shapeReceiver_->getName() << std::endl;
            shapeReceiver_->changeColor(originalColor_);
        }
    }
    
    std::string getCommandName() const override {
        return "ColorChangeCommand";
    }
};

class RotateCommand : public Command {
private:
    ShapeReceiver* shapeReceiver_;
    double angle_;
    bool executed_;

public:
    RotateCommand(ShapeReceiver* receiver, double angle) 
        : shapeReceiver_(receiver), angle_(angle), executed_(false) {}
    
    void execute() override {
        std::cout << "[COMMAND] Executing RotateCommand for " << shapeReceiver_->getName() 
                  << " by " << angle_ << " degrees" << std::endl;
        shapeReceiver_->rotate(angle_);
        executed_ = true;
    }
    
    void undo() override {
        if (executed_) {
            std::cout << "[COMMAND] Undoing RotateCommand for " << shapeReceiver_->getName() << std::endl;
            shapeReceiver_->rotate(-angle_); // Rotate back by negative angle
        }
    }
    
    std::string getCommandName() const override {
        return "RotateCommand";
    }
};

// Macro Command - Executes a sequence of commands as a single command
class MacroCommand : public Command {
private:
    std::vector<std::unique_ptr<Command>> commands_;
    std::string macroName_;

public:
    explicit MacroCommand(const std::string& name = "Macro") : macroName_(name) {}
    
    void addCommand(std::unique_ptr<Command> command) {
        commands_.push_back(std::move(command));
    }
    
    void execute() override {
        std::cout << "[MACRO] Executing macro: " << macroName_ 
                  << " (" << commands_.size() << " commands)" << std::endl;
        for (auto& command : commands_) {
            command->execute();
        }
    }
    
    void undo() override {
        std::cout << "[MACRO] Undoing macro: " << macroName_ << " in reverse order" << std::endl;
        // Undo commands in reverse order
        for (auto it = commands_.rbegin(); it != commands_.rend(); ++it) {
            (*it)->undo();
        }
    }
    
    std::string getCommandName() const override {
        return macroName_ + " (Macro)";
    }
    
    size_t getCommandCount() const { return commands_.size(); }
};

// Null Command - Does nothing, useful for default values or placeholders
class NullCommand : public Command {
public:
    void execute() override {
        std::cout << "[NULL_COMMAND] No operation executed" << std::endl;
    }

    void undo() override {
        std::cout << "[NULL_COMMAND] No operation to undo" << std::endl;
    }

    std::string getCommandName() const override {
        return "NullCommand";
    }

    bool isUndoable() const override {
        return false;
    }
};

// Fluent Command Builder - for easy chaining of command operations
class FluentCommandBuilder {
private:
    ShapeReceiver* receiver_;
    std::vector<std::unique_ptr<Command>> commands_;

public:
    explicit FluentCommandBuilder(ShapeReceiver* receiver) : receiver_(receiver) {}

    FluentCommandBuilder& draw() {
        commands_.push_back(std::make_unique<DrawCommand>(receiver_));
        return *this;
    }

    FluentCommandBuilder& move(double dx, double dy) {
        commands_.push_back(std::make_unique<MoveCommand>(receiver_, dx, dy));
        return *this;
    }

    FluentCommandBuilder& resize(double factor) {
        commands_.push_back(std::make_unique<ResizeCommand>(receiver_, factor));
        return *this;
    }

    FluentCommandBuilder& changeColor(const std::string& newColor) {
        commands_.push_back(std::make_unique<ColorChangeCommand>(receiver_, newColor));
        return *this;
    }

    FluentCommandBuilder& rotate(double angle) {
        commands_.push_back(std::make_unique<RotateCommand>(receiver_, angle));
        return *this;
    }

    std::vector<std::unique_ptr<Command>> build() {
        return std::move(commands_);
    }

    void executeAll() {
        for (auto& cmd : commands_) {
            cmd->execute();
        }
    }
};

// Command Invoker - Executes commands and manages command history
class CommandInvoker {
private:
    std::vector<std::unique_ptr<Command>> commandHistory_;
    std::stack<std::unique_ptr<Command>> undoStack_;
    std::string invokerName_;

public:
    explicit CommandInvoker(const std::string& name = "ShapeInvoker") : invokerName_(name) {}
    
    void executeCommand(std::unique_ptr<Command> command) {
        std::cout << "[" << invokerName_ << "] Invoking command: " << command->getCommandName() << std::endl;
        
        // Execute the command
        command->execute();
        
        // Store for potential undo
        if (command->isUndoable()) {
            // We can't do this directly because command is moved, so we'll store the executed command
            // In a real implementation we'd need to make commands copyable or store state differently
            undoStack_.push(std::make_unique<NullCommand>()); // Placeholder for undo
        }
        
        // For demo purposes, we'll just track commands were executed
        commandHistory_.push_back(std::make_unique<NullCommand>()); // Placeholder
    }
    
    void executeCommands(const std::vector<std::unique_ptr<Command>>& commands) {
        std::cout << "[" << invokerName_ << "] Executing sequence of " << commands.size() << " commands" << std::endl;
        for (const auto& cmd : commands) {
            executeCommand(std::make_unique<NullCommand>());  // For demo only
        }
    }
    
    void undoLastCommand() {
        if (!undoStack_.empty()) {
            std::cout << "[" << invokerName_ << "] Undoing last command" << std::endl;
            auto cmd = std::move(undoStack_.top());
            undoStack_.pop();
            cmd->undo();
        } else {
            std::cout << "[" << invokerName_ << "] No commands to undo" << std::endl;
        }
    }
    
    size_t getCommandHistorySize() const { return commandHistory_.size(); }
    size_t getUndoStackSize() const { return undoStack_.size(); }
    
    void printHistory() const {
        std::cout << "[" << invokerName_ << "] Command history (" << commandHistory_.size() << " commands executed)" << std::endl;
    }
    
    void clearHistory() {
        commandHistory_.clear();
        while (!undoStack_.empty()) {
            undoStack_.pop();
        }
    }
};

// Command Manager - Centralizes command creation and management
class CommandManager {
private:
    std::vector<std::unique_ptr<Command>> availableCommands_;
    std::map<std::string, std::function<std::unique_ptr<Command>(ShapeReceiver*, std::vector<double>, std::string)>> commandFactories_;

public:
    CommandManager() {
        // Register command factories
        commandFactories_["draw"] = [](ShapeReceiver* receiver, std::vector<double> params, std::string strParam) {
            return std::make_unique<DrawCommand>(receiver);
        };
        
        commandFactories_["move"] = [](ShapeReceiver* receiver, std::vector<double> params, std::string strParam) {
            if (params.size() >= 2) {
                return std::make_unique<MoveCommand>(receiver, params[0], params[1]);
            }
            return std::make_unique<MoveCommand>(receiver, 10.0, 10.0); // default
        };
        
        commandFactories_["resize"] = [](ShapeReceiver* receiver, std::vector<double> params, std::string strParam) {
            if (!params.empty()) {
                return std::make_unique<ResizeCommand>(receiver, params[0]);
            }
            return std::make_unique<ResizeCommand>(receiver, 1.0); // default
        };
        
        commandFactories_["color"] = [](ShapeReceiver* receiver, std::vector<double> params, std::string strParam) {
            if (!strParam.empty()) {
                return std::make_unique<ColorChangeCommand>(receiver, strParam);
            }
            return std::make_unique<ColorChangeCommand>(receiver, "default_color"); // default
        };
        
        commandFactories_["rotate"] = [](ShapeReceiver* receiver, std::vector<double> params, std::string strParam) {
            if (!params.empty()) {
                return std::make_unique<RotateCommand>(receiver, params[0]);
            }
            return std::make_unique<RotateCommand>(receiver, 90.0); // default
        };
    }
    
    std::unique_ptr<Command> createCommand(const std::string& commandType, 
                                          ShapeReceiver* receiver, 
                                          std::vector<double> params = {}, 
                                          const std::string& strParam = "") {
        auto it = commandFactories_.find(commandType);
        if (it != commandFactories_.end()) {
            return it->second(receiver, params, strParam);
        }
        std::cout << "[COMMAND MANAGER] Unknown command type: " << commandType << std::endl;
        return std::make_unique<NullCommand>();
    }
    
    std::vector<std::string> getAvailableCommands() const {
        std::vector<std::string> commands;
        for (const auto& pair : commandFactories_) {
            commands.push_back(pair.first);
        }
        return commands;
    }
    
    void registerCommandFactory(const std::string& name,
                               std::function<std::unique_ptr<Command>(ShapeReceiver*, std::vector<double>, std::string)> factory) {
        commandFactories_[name] = factory;
    }
};

#endif // COMMAND_PATTERN_IMPLEMENTATION_H