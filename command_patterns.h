#ifndef COMMAND_PATTERNS_H
#define COMMAND_PATTERNS_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <stack>
#include <functional>
#include <map>

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

// Concrete Receivers
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

// Command interface - defines the interface for executing operations
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual std::string getCommandName() const = 0;
    virtual bool isUndoable() const { return true; }
};

// Concrete Command 1: Draw Command
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
        // In a real system, this might hide or delete the shape
        std::cout << "[COMMAND] Drawing cannot be undone, but state saved" << std::endl;
    }
    
    std::string getCommandName() const override {
        return "DrawCommand";
    }
};

// Concrete Command 2: Move Command
class MoveCommand : public Command {
private:
    ShapeReceiver* shapeReceiver_;
    double deltaX_, deltaY_;
    double originalX_, originalY_;

public:
    MoveCommand(ShapeReceiver* receiver, double dx, double dy) 
        : shapeReceiver_(receiver), deltaX_(dx), deltaY_(dy), originalX_(-1), originalY_(-1) {}
    
    void execute() override {
        // Store original position for undo
        if (originalX_ == -1 && originalY_ == -1) {
            // This is a simple way to get current position - in reality we'd have specific getters
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
        }
        
        std::cout << "[COMMAND] Executing MoveCommand for " << shapeReceiver_->getName() 
                  << " by (" << deltaX_ << "," << deltaY_ << ")" << std::endl;
        shapeReceiver_->move(deltaX_, deltaY_);
    }
    
    void undo() override {
        std::cout << "[COMMAND] Undoing MoveCommand for " << shapeReceiver_->getName() << std::endl;
        // Move back to original position
        if (auto* circle = dynamic_cast<CircleReceiver*>(shapeReceiver_)) {
            shapeReceiver_->move(originalX_ - circle->getX(), originalY_ - circle->getY());
        } else if (auto* rect = dynamic_cast<RectangleReceiver*>(shapeReceiver_)) {
            shapeReceiver_->move(originalX_ - rect->getX(), originalY_ - rect->getY());
        } else if (auto* tri = dynamic_cast<TriangleReceiver*>(shapeReceiver_)) {
            shapeReceiver_->move(originalX_ - tri->getX(), originalY_ - tri->getY());
        }
    }
    
    std::string getCommandName() const override {
        return "MoveCommand";
    }
};

// Concrete Command 3: Resize Command
class ResizeCommand : public Command {
private:
    ShapeReceiver* shapeReceiver_;
    double factor_;
    double originalSize1_, originalSize2_; // To store original dimensions for undo

public:
    ResizeCommand(ShapeReceiver* receiver, double factor) 
        : shapeReceiver_(receiver), factor_(factor), originalSize1_(-1), originalSize2_(-1) {}
    
    void execute() override {
        // Store original dimensions for undo
        if (originalSize1_ == -1) {
            if (auto* circle = dynamic_cast<CircleReceiver*>(shapeReceiver_)) {
                originalSize1_ = circle->getRadius();
            } else if (auto* rect = dynamic_cast<RectangleReceiver*>(shapeReceiver_)) {
                originalSize1_ = rect->getWidth();
                originalSize2_ = rect->getHeight();
            } else if (auto* tri = dynamic_cast<TriangleReceiver*>(shapeReceiver_)) {
                originalSize1_ = tri->getBase();
                originalSize2_ = tri->getHeight();
            }
        }
        
        std::cout << "[COMMAND] Executing ResizeCommand for " << shapeReceiver_->getName() 
                  << " with factor " << factor_ << std::endl;
        shapeReceiver_->resize(factor_);
    }
    
    void undo() override {
        std::cout << "[COMMAND] Undoing ResizeCommand for " << shapeReceiver_->getName() << std::endl;
        
        if (auto* circle = dynamic_cast<CircleReceiver*>(shapeReceiver_)) {
            // Restore original size by computing inverse
            double currentRadius = circle->getRadius();
            double newFactor = originalSize1_ / currentRadius;
            shapeReceiver_->resize(newFactor);
        } else if (auto* rect = dynamic_cast<RectangleReceiver*>(shapeReceiver_)) {
            // In a real system we'd have a more robust way to restore dimensions
            // For this demo, we'll just resize back to original values
            double currentWidth = rect->getWidth();
            double currentHeight = rect->getHeight();
            // Reset to original by resizing proportionally
            shapeReceiver_->resize(originalSize1_/currentWidth); // Approximate
            // A more complete implementation would store the original values and restore them properly
        } else if (auto* tri = dynamic_cast<TriangleReceiver*>(shapeReceiver_)) {
            // Similar restoration for triangle
            double currentBase = tri->getBase();
            double currentHeight = tri->getHeight();
            // Reset to original by resizing proportionally
            shapeReceiver_->resize(originalSize1_/currentBase); // Approximate
        }
    }
    
    std::string getCommandName() const override {
        return "ResizeCommand";
    }
};

// Concrete Command 4: Color Change Command
class ColorChangeCommand : public Command {
private:
    ShapeReceiver* shapeReceiver_;
    std::string newColor_;
    std::string originalColor_;

public:
    ColorChangeCommand(ShapeReceiver* receiver, const std::string& color) 
        : shapeReceiver_(receiver), newColor_(color), originalColor_("unknown") {}
    
    void execute() override {
        // Store original color for undo
        if (auto* circle = dynamic_cast<CircleReceiver*>(shapeReceiver_)) {
            originalColor_ = circle->getColor();
        } else if (auto* rect = dynamic_cast<RectangleReceiver*>(shapeReceiver_)) {
            originalColor_ = rect->getColor();
        } else if (auto* tri = dynamic_cast<TriangleReceiver*>(shapeReceiver_)) {
            originalColor_ = tri->getColor();
        }
        
        std::cout << "[COMMAND] Executing ColorChangeCommand for " << shapeReceiver_->getName() 
                  << " to color " << newColor_ << std::endl;
        shapeReceiver_->changeColor(newColor_);
    }
    
    void undo() override {
        std::cout << "[COMMAND] Undoing ColorChangeCommand for " << shapeReceiver_->getName() << std::endl;
        shapeReceiver_->changeColor(originalColor_);
    }
    
    std::string getCommandName() const override {
        return "ColorChangeCommand";
    }
};

// Concrete Command 5: Rotate Command
class RotateCommand : public Command {
private:
    ShapeReceiver* shapeReceiver_;
    double angle_;

public:
    RotateCommand(ShapeReceiver* receiver, double angle) 
        : shapeReceiver_(receiver), angle_(angle) {}
    
    void execute() override {
        std::cout << "[COMMAND] Executing RotateCommand for " << shapeReceiver_->getName() 
                  << " by " << angle_ << " degrees" << std::endl;
        shapeReceiver_->rotate(angle_);
    }
    
    void undo() override {
        std::cout << "[COMMAND] Undoing RotateCommand for " << shapeReceiver_->getName() << std::endl;
        shapeReceiver_->rotate(-angle_); // Rotate back by negative angle
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

// Null Command - Does nothing, useful for default values
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

// Command with Parameters - Generic command that uses std::function
template<typename... Args>
class FunctionCommand : public Command {
private:
    std::function<void(Args...)> function_;
    std::tuple<Args...> args_;
    std::string commandName_;

public:
    FunctionCommand(std::function<void(Args...)> func, const std::string& name, Args... args) 
        : function_(func), args_(std::make_tuple(args...)), commandName_(name) {}
    
    void execute() override {
        std::cout << "[FUNCTION_COMMAND] Executing: " << commandName_ << std::endl;
        executeImpl(std::index_sequence_for<Args...>{});
    }
    
    void undo() override {
        std::cout << "[FUNCTION_COMMAND] Undo not applicable for function command: " << commandName_ << std::endl;
    }
    
    std::string getCommandName() const override {
        return commandName_;
    }
    
private:
    template<std::size_t... I>
    void executeImpl(std::index_sequence<I...>) {
        function_(std::get<I>(args_)...);
    }
};

// Invoker class - invokes commands
class CommandInvoker {
private:
    std::vector<std::unique_ptr<Command>> commandHistory_;
    std::stack<std::unique_ptr<Command>> undoStack_;
    std::string invokerName_;

public:
    explicit CommandInvoker(const std::string& name = "ShapeInvoker") : invokerName_(name) {}
    
    void setCommand(std::unique_ptr<Command> command) {
        commandHistory_.push_back(std::make_unique<NullCommand>()); // Placeholder
        commandHistory_.back() = std::move(command);
    }
    
    void executeCommand(Command& command) {
        std::cout << "[" << invokerName_ << "] Invoking command: " << command.getCommandName() << std::endl;
        // Push to undo stack before execution (for the latest command)
        // For this implementation we'll store command copy for undo
        command.execute();
        // Note: In a real implementation, we'd store command state for undo
    }
    
    void executeCommands(const std::vector<std::unique_ptr<Command>>& commands) {
        std::cout << "[" << invokerName_ << "] Executing sequence of " << commands.size() << " commands" << std::endl;
        for (auto& cmd : commands) {
            cmd->execute();
            // For undo support, we'd need to track executed commands
        }
    }
    
    void executeWithUndo(std::unique_ptr<Command> command) {
        std::cout << "[" << invokerName_ << "] Executing command with undo support: " 
                  << command->getCommandName() << std::endl;
        command->execute();
        // Store command for potential undo
        undoStack_.push(std::make_unique<DrawCommand>(*static_cast<DrawCommand*>(command.get()))); // This would need to be a more general copy approach
        commandHistory_.push_back(std::move(command));
    }
    
    void undoLastCommand() {
        if (!undoStack_.empty()) {
            std::cout << "[" << invokerName_ << "] Undoing last command" << std::endl;
            // In a real system, we'd need to properly store command state for undo
            undoStack_.top()->undo();
            undoStack_.pop();
        } else {
            std::cout << "[" << invokerName_ << "] No commands to undo" << std::endl;
        }
    }
    
    void executeCommandByName(const std::string& commandName, ShapeReceiver* receiver) {
        std::cout << "[" << invokerName_ << "] Executing command by name: " << commandName << std::endl;
        
        if (commandName == "draw") {
            auto cmd = std::make_unique<DrawCommand>(receiver);
            cmd->execute();
            if (cmd->isUndoable()) {
                undoStack_.push(std::make_unique<DrawCommand>(receiver));
            }
        } else if (commandName == "move") {
            // For demo, using hardcoded values
            auto cmd = std::make_unique<MoveCommand>(receiver, 10.0, 5.0);
            cmd->execute();
            if (cmd->isUndoable()) {
                undoStack_.push(std::make_unique<MoveCommand>(receiver, -10.0, -5.0));
            }
        } else {
            std::cout << "Unknown command: " << commandName << std::endl;
        }
    }
    
    size_t getCommandHistorySize() const { return commandHistory_.size(); }
    size_t getUndoStackSize() const { return undoStack_.size(); }
    
    void printHistory() const {
        std::cout << "[" << invokerName_ << "] Command history (" << commandHistory_.size() << " commands):" << std::endl;
        for (size_t i = 0; i < commandHistory_.size(); ++i) {
            std::cout << "  " << (i + 1) << ". " << commandHistory_[i]->getCommandName() << std::endl;
        }
    }
};

// Advanced Invoker with Macro recording
class AdvancedCommandInvoker {
private:
    std::vector<std::unique_ptr<Command>> commandBuffer_;
    std::unique_ptr<MacroCommand> currentMacro_;
    std::vector<std::unique_ptr<Command>> executedCommands_;
    std::stack<std::unique_ptr<Command>> undoStack_;
    
public:
    AdvancedCommandInvoker() = default;
    
    void beginMacroRecording(const std::string& macroName) {
        std::cout << "[ADVANCED INVOKER] Beginning macro recording: " << macroName << std::endl;
        currentMacro_ = std::make_unique<MacroCommand>(macroName);
    }
    
    void endMacroRecording() {
        if (currentMacro_) {
            std::cout << "[ADVANCED INVOKER] Ending macro recording" << std::endl;
            // Execute the recorded macro
            currentMacro_->execute();
            executedCommands_.push_back(std::move(currentMacro_));
        }
    }
    
    void recordCommand(std::unique_ptr<Command> command) {
        if (currentMacro_) {
            std::cout << "[ADVANCED INVOKER] Recording command: " << command->getCommandName() << " to macro" << std::endl;
            currentMacro_->addCommand(std::move(command));
        } else {
            std::cout << "[ADVANCED_INVOKER] No active macro, executing command directly" << std::endl;
            command->execute();  // Execute directly if not recording
        }
    }
    
    void executeMacro(const std::string& macroName, ShapeReceiver* receiver = nullptr) {
        // In a real system, we'd have named macros stored
        // For demo, we'll create and execute a sample macro
        std::cout << "[ADVANCED INVOKER] Executing macro: " << macroName << std::endl;

        auto macro = std::make_unique<MacroCommand>(macroName);

        if (receiver != nullptr) {
            macro->addCommand(std::make_unique<DrawCommand>(receiver));
        } else {
            // If no receiver, just execute operations with null commands instead
            std::cout << "[ADVANCED INVOKER] No receiver provided for macro execution." << std::endl;
        }
        macro->execute();
    }
    
    void executeCommand(std::unique_ptr<Command> command) {
        command->execute();
        executedCommands_.push_back(std::move(command));
    }
    
    void undoLast() {
        if (!executedCommands_.empty()) {
            auto& lastCmd = executedCommands_.back();
            std::cout << "[ADVANCED INVOKER] Undoing: " << lastCmd->getCommandName() << std::endl;
            lastCmd->undo();
            executedCommands_.pop_back();
        }
    }
};

// Command Manager - centralized command management
class CommandManager {
private:
    std::map<std::string, std::function<std::unique_ptr<Command>(ShapeReceiver*)>> commandFactories_;
    std::vector<std::unique_ptr<Command>> commandHistory_;

public:
    CommandManager() {
        // Register command factories
        commandFactories_["draw"] = [](ShapeReceiver* receiver) {
            return std::make_unique<DrawCommand>(receiver);
        };
        
        commandFactories_["move"] = [](ShapeReceiver* receiver) {
            return std::make_unique<MoveCommand>(receiver, 5.0, 5.0); // Default move
        };
        
        commandFactories_["resize"] = [](ShapeReceiver* receiver) {
            return std::make_unique<ResizeCommand>(receiver, 1.5); // Default resize factor
        };
        
        commandFactories_["color"] = [](ShapeReceiver* receiver) {
            return std::make_unique<ColorChangeCommand>(receiver, "default"); // Would need to take color parameter
        };
        
        commandFactories_["rotate"] = [](ShapeReceiver* receiver) {
            return std::make_unique<RotateCommand>(receiver, 45.0); // Default 45 degrees
        };
    }
    
    std::unique_ptr<Command> createCommand(const std::string& commandType, ShapeReceiver* receiver) {
        auto it = commandFactories_.find(commandType);
        if (it != commandFactories_.end()) {
            return it->second(receiver);
        }
        std::cout << "[COMMAND MANAGER] Unknown command type: " << commandType << std::endl;
        return std::make_unique<NullCommand>();
    }
    
    void registerCommandFactory(const std::string& name, 
                               std::function<std::unique_ptr<Command>(ShapeReceiver*)> factory) {
        commandFactories_[name] = factory;
    }
    
    std::vector<std::string> getAvailableCommands() const {
        std::vector<std::string> commands;
        for (const auto& pair : commandFactories_) {
            commands.push_back(pair.first);
        }
        return commands;
    }
};

#endif // COMMAND_PATTERNS_H