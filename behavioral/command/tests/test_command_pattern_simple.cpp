#include "../include/command_patterns.h"
#include <iostream>
#include <memory>
#include <vector>
#include <chrono>

int main() {
    std::cout << "=== Command Pattern Demo: Shape Graphics Command System ===\n\n";
    
    // Test 1: Basic Command Usage
    std::cout << "1. Basic Command Pattern Usage:\n";
    
    auto circle = std::make_unique<CircleReceiver>(5.0, 10.0, 10.0, "red");
    
    // Create and execute draw command
    auto drawCommand = std::make_unique<DrawCommand>(circle.get());
    drawCommand->execute();
    std::cout << std::endl;
    
    // Test 2: Move Command
    std::cout << "2. Move Command:\n";
    auto moveCommand = std::make_unique<MoveCommand>(circle.get(), 20.0, 15.0);
    moveCommand->execute();
    std::cout << std::endl;
    
    // Test 3: Resize Command
    std::cout << "3. Resize Command:\n";
    auto resizeCommand = std::make_unique<ResizeCommand>(circle.get(), 1.5);
    resizeCommand->execute();
    std::cout << std::endl;
    
    // Test 4: Color Change Command
    std::cout << "4. Color Change Command:\n";
    auto colorCommand = std::make_unique<ColorChangeCommand>(circle.get(), "blue");
    colorCommand->execute();
    std::cout << std::endl;
    
    // Test 5: Rectangle with Various Commands
    std::cout << "5. Rectangle with Various Commands:\n";
    
    auto rectangle = std::make_unique<RectangleReceiver>(8.0, 6.0, 0.0, 0.0, "blue");
    
    auto rectDrawCommand = std::make_unique<DrawCommand>(rectangle.get());
    auto rectMoveCommand = std::make_unique<MoveCommand>(rectangle.get(), 30.0, 20.0);
    auto rectColorCommand = std::make_unique<ColorChangeCommand>(rectangle.get(), "yellow");
    auto rectRotateCommand = std::make_unique<RotateCommand>(rectangle.get(), 45.0);
    
    std::cout << "Executing rectangle operations:\n";
    rectDrawCommand->execute();
    rectMoveCommand->execute();
    rectColorCommand->execute();
    rectRotateCommand->execute();
    std::cout << std::endl;
    
    // Test 6: Triangle Commands
    std::cout << "6. Triangle Commands:\n";
    
    auto triangle = std::make_unique<TriangleReceiver>(10.0, 8.0, 5.0, 5.0, "green");
    
    auto triDrawCommand = std::make_unique<DrawCommand>(triangle.get());
    auto triMoveCommand = std::make_unique<MoveCommand>(triangle.get(), 15.0, 10.0);
    auto triColorCommand = std::make_unique<ColorChangeCommand>(triangle.get(), "orange");
    
    std::cout << "Executing triangle operations:\n";
    triDrawCommand->execute();
    triMoveCommand->execute();
    triColorCommand->execute();
    std::cout << std::endl;
    
    // Test 7: Macro Command Usage
    std::cout << "7. Macro Command Usage:\n";
    
    auto macro = std::make_unique<MacroCommand>("Shape Transformation Sequence");
    macro->addCommand(std::make_unique<DrawCommand>(circle.get()));
    macro->addCommand(std::make_unique<MoveCommand>(circle.get(), 5.0, 5.0));
    macro->addCommand(std::make_unique<ColorChangeCommand>(circle.get(), "purple"));
    
    std::cout << "Executing macro command:\n";
    macro->execute();
    std::cout << std::endl;
    
    // Test 8: Null Command Usage
    std::cout << "8. Null Command Usage:\n";
    
    auto nullCmd = std::make_unique<NullCommand>();
    nullCmd->execute();
    nullCmd->undo();
    std::cout << std::endl;
    
    // Test 9: Command Invoker Usage
    std::cout << "9. Command Invoker Usage:\n";
    
    CommandInvoker invoker("GraphicsInvoker");
    
    // Execute commands through invoker (simplified interface)
    std::vector<std::unique_ptr<Command>> commands;
    commands.push_back(std::make_unique<DrawCommand>(circle.get()));
    commands.push_back(std::make_unique<ColorChangeCommand>(circle.get(), "magenta"));
    commands.push_back(std::make_unique<MoveCommand>(circle.get(), 10.0, 10.0));
    
    std::cout << "Executing sequence of commands via invoker:\n";
    for (auto& cmd : commands) {
        invoker.executeCommand(*cmd);
    }
    std::cout << std::endl;
    
    // Test 10: Undo Operations
    std::cout << "10. Undo Operations:\n";
    
    // In a real implementation, we'd have the invoker track executed commands
    // For this demo, we'll demonstrate the concept with manual undo calls
    std::cout << "Demonstrating undo for the last color change:\n";
    std::cout << "  The color command would restore the original color if properly implemented\n";
    std::cout << std::endl;
    
    // Test 11: Command Manager Usage
    std::cout << "11. Command Manager Usage:\n";
    
    CommandManager manager;
    auto availableCommands = manager.getAvailableCommands();
    std::cout << "Available commands in manager: ";
    for (const auto& cmd : availableCommands) {
        std::cout << cmd << " ";
    }
    std::cout << std::endl;
    
    // Use manager to create a command
    auto circleForManager = std::make_unique<CircleReceiver>(4.0, 2.0, 2.0, "manager_color");
    ShapeReceiver* circlePtr = circleForManager.get();
    
    auto managedCommand = manager.createCommand("draw", circlePtr);
    std::cout << "Executing command from manager: ";
    managedCommand->execute();
    std::cout << std::endl;
    
    // Test 12: Real-world Scenario - Graphics Pipeline
    std::cout << "12. Real-world Scenario - Graphics Pipeline:\n";
    
    std::vector<std::unique_ptr<ShapeReceiver>> graphicsPipeline;
    
    // Add different shapes to the pipeline
    graphicsPipeline.push_back(std::make_unique<CircleReceiver>(3.0, 50.0, 50.0, "pipeline_red"));
    graphicsPipeline.push_back(std::make_unique<RectangleReceiver>(6.0, 4.0, 100.0, 100.0, "pipeline_blue"));
    graphicsPipeline.push_back(std::make_unique<TriangleReceiver>(5.0, 4.0, 150.0, 150.0, "pipeline_green"));
    
    std::cout << "Processing " << graphicsPipeline.size() << " shapes in pipeline:\n";
    
    for (size_t i = 0; i < graphicsPipeline.size(); ++i) {
        auto drawCmd = std::make_unique<DrawCommand>(graphicsPipeline[i].get());
        drawCmd->execute();
        
        auto colorCmd = std::make_unique<ColorChangeCommand>(graphicsPipeline[i].get(), 
                                                           i % 2 == 0 ? "light_red" : "light_blue");
        colorCmd->execute();
    }
    std::cout << std::endl;
    
    // Test 13: Animation Sequence with Macro Command
    std::cout << "13. Animation Sequence with Macro Command:\n";
    
    auto animatedRect = std::make_unique<RectangleReceiver>(4.0, 3.0, 200.0, 200.0, "animated");
    ShapeReceiver* animPtr = animatedRect.get();
    
    auto animationMacro = std::make_unique<MacroCommand>("Animation Sequence");
    animationMacro->addCommand(std::make_unique<MoveCommand>(animPtr, 10.0, 5.0));
    animationMacro->addCommand(std::make_unique<ColorChangeCommand>(animPtr, "lightblue"));
    animationMacro->addCommand(std::make_unique<RotateCommand>(animPtr, 90.0));
    
    std::cout << "Playing animation sequence:\n";
    animationMacro->execute();
    std::cout << std::endl;
    
    // Test 14: Physics Simulation Commands
    std::cout << "14. Physics Simulation Commands:\n";
    
    auto physicsCircle = std::make_unique<CircleReceiver>(2.5, 10.0, 10.0, "physics_orange");
    ShapeReceiver* physicsPtr = physicsCircle.get();
    
    // Create commands that might trigger physics simulation
    auto physicsMove = std::make_unique<MoveCommand>(physicsPtr, 15.0, 20.0);
    auto physicsRotate = std::make_unique<RotateCommand>(physicsPtr, 30.0);
    
    std::cout << "Executing physics-enabled operations:\n";
    physicsMove->execute();
    physicsRotate->execute();
    std::cout << std::endl;
    
    // Test 15: Performance Test
    std::cout << "15. Performance Test - Creating Many Commands:\n";
    
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::unique_ptr<ShapeReceiver>> shapes;
    std::vector<std::unique_ptr<Command>> performanceCommands;

    // Create shapes first
    for (int i = 0; i < 1000; ++i) {
        shapes.push_back(std::make_unique<CircleReceiver>(1.0, 0.0, 0.0, "performance"));
    }

    // Now create commands for each shape
    for (int i = 0; i < 1000; ++i) {
        performanceCommands.push_back(std::make_unique<DrawCommand>(shapes[i].get()));
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Created 10,000 commands in: " << duration.count() << " microseconds\n";
    std::cout << "Command pattern allows for efficient command creation\n";
    std::cout << std::endl;
    
    // Test 16: Dynamic Command Creation
    std::cout << "16. Dynamic Command Creation:\n";
    
    std::vector<std::string> commandTypes = {"draw", "move", "color", "rotate"};
    ShapeReceiver* testShape = circle.get();
    
    for (const auto& cmdType : commandTypes) {
        std::cout << "Creating " << cmdType << " command dynamically\n";
        // In a real system, we would use the manager to create commands
        if (cmdType == "draw") {
            auto cmd = std::make_unique<DrawCommand>(testShape);
            cmd->execute();
        } else if (cmdType == "move") {
            auto cmd = std::make_unique<MoveCommand>(testShape, 5.0, 5.0);
            cmd->execute();
        } else if (cmdType == "color") {
            auto cmd = std::make_unique<ColorChangeCommand>(testShape, "dynamic");
            cmd->execute();
        } else if (cmdType == "rotate") {
            auto cmd = std::make_unique<RotateCommand>(testShape, 15.0);
            cmd->execute();
        }
    }
    std::cout << std::endl;
    
    // Test 17: Command Pattern Benefits Demonstration
    std::cout << "17. Command Pattern Benefits Demonstration:\n";
    std::cout << "- Encapsulation: Each action is encapsulated in a command object\n";
    std::cout << "- Extensibility: Easy to add new commands without changing existing code\n";
    std::cout << "- Parameterization: Objects can be configured with different commands\n";
    std::cout << "- Queuing: Commands can be stored and executed later\n";
    std::cout << "- Logging: Command execution can be easily logged\n";
    std::cout << "- Undo/Redo: Commands can be designed to support undo operations\n";
    std::cout << "- Transactional: Macros can group commands into transactions\n";
    std::cout << "- Late Binding: Command execution can be deferred\n";
    std::cout << std::endl;
    
    // Test 18: Different Shapes with Same Commands
    std::cout << "18. Same Commands on Different Shapes:\n";
    
    auto circ = std::make_unique<CircleReceiver>(3.0, 0.0, 0.0, "common");
    auto rect = std::make_unique<RectangleReceiver>(4.0, 3.0, 0.0, 0.0, "common");
    auto tri = std::make_unique<TriangleReceiver>(5.0, 3.0, 0.0, 0.0, "common");
    
    // Same move command applied to different shapes
    auto moveCirc = std::make_unique<MoveCommand>(circ.get(), 5.0, 10.0);
    auto moveRect = std::make_unique<MoveCommand>(rect.get(), 5.0, 10.0);
    auto moveTri = std::make_unique<MoveCommand>(tri.get(), 5.0, 10.0);
    
    std::cout << "Same move command (5,10) applied to different shapes:\n";
    moveCirc->execute();
    moveRect->execute();
    moveTri->execute();
    std::cout << std::endl;
    
    // Test 19: Command Composition with Macro
    std::cout << "19. Command Composition with Macro Command:\n";
    
    auto complexMacro = std::make_unique<MacroCommand>("Complex Shape Manipulation");
    
    // Add various operations to the macro
    complexMacro->addCommand(std::make_unique<DrawCommand>(circle.get()));
    complexMacro->addCommand(std::make_unique<ColorChangeCommand>(circle.get(), "rainbow"));
    complexMacro->addCommand(std::make_unique<MoveCommand>(circle.get(), 25.0, 25.0));
    complexMacro->addCommand(std::make_unique<RotateCommand>(circle.get(), 180.0));
    
    std::cout << "Executing complex macro with multiple operations:\n";
    complexMacro->execute();
    std::cout << "Macro contains " << complexMacro->getCommandCount() << " commands\n";
    std::cout << std::endl;
    
    // Test 20: Command Pattern vs Direct Method Calls Comparison
    std::cout << "20. Command Pattern vs Direct Method Calls Comparison:\n";
    
    std::cout << "\nDirect Method Calls:\n";
    CircleReceiver directCircle(3.0, 5.0, 5.0, "direct");
    directCircle.draw();
    directCircle.move(10.0, 10.0);
    directCircle.changeColor("bright_blue");
    
    std::cout << "\nCommand Pattern (encapsulated operations):\n";
    auto cmdCircle = std::make_unique<CircleReceiver>(3.0, 5.0, 5.0, "command");
    ShapeReceiver* cmdPtr = cmdCircle.get();
    
    auto cmdDraw = std::make_unique<DrawCommand>(cmdPtr);
    auto cmdMove = std::make_unique<MoveCommand>(cmdPtr, 10.0, 10.0);
    auto cmdColor = std::make_unique<ColorChangeCommand>(cmdPtr, "bright_blue");
    
    cmdDraw->execute();
    cmdMove->execute();
    cmdColor->execute();
    
    std::cout << "\nBenefits of Command Pattern:\n";
    std::cout << "- Operations can be stored, queued, and executed later\n";
    std::cout << "- Easy to add undo/redo functionality\n";
    std::cout << "- Enables macro commands for complex operations\n";
    std::cout << "- Provides uniform interface for different operations\n";
    std::cout << "- Supports command history and logging\n";
    std::cout << "- Allows for command parameterization\n";
    std::cout << std::endl;
    
    std::cout << "Command pattern demo completed successfully!\n";
    std::cout << "\nKey Benefits of Command Pattern:\n";
    std::cout << "- Encapsulates a request as an object\n";
    std::cout << "- Allows for parameterization of clients with different requests\n";
    std::cout << "- Supports queuing of requests and undo operations\n";
    std::cout << "- Enables logging of operations for system recovery\n";
    std::cout << "- Permits macro commands (sequences of commands)\n";
    std::cout << "- Promotes loose coupling between sender and receiver\n";
    std::cout << "- Makes it easy to extend with new commands\n";
    std::cout << "- Supports the Open/Closed Principle\n";
    
    return 0;
}