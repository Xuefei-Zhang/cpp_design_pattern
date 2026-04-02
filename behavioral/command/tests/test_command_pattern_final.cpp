#include "../include/command_patterns.h"
#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
#include <functional>
#include <algorithm>

int main() {
    std::cout << "=== Command Pattern Demo: Shape Command System ===\n\n";
    
    // Test 1: Basic Command Usage
    std::cout << "1. Basic Command Pattern Usage:\n";
    
    auto circle = std::make_unique<CircleReceiver>(5.0, 10.0, 10.0, "red");
    ShapeReceiver* circlePtr = circle.get();
    
    // Create and execute a draw command
    auto drawCommand = std::make_unique<DrawCommand>(circlePtr);
    drawCommand->execute();
    std::cout << std::endl;
    
    // Test 2: Different Commands on Same Shape
    std::cout << "2. Different Commands on Same Shape:\n";
    
    auto moveCommand = std::make_unique<MoveCommand>(circlePtr, 20.0, 15.0);
    auto colorCommand = std::make_unique<ColorChangeCommand>(circlePtr, "blue");
    
    moveCommand->execute();
    colorCommand->execute();
    std::cout << std::endl;
    
    // Test 3: Rectangle with Commands
    std::cout << "3. Rectangle with Commands:\n";
    
    auto rectangle = std::make_unique<RectangleReceiver>(8.0, 6.0, 0.0, 0.0, "blue");
    ShapeReceiver* rectPtr = rectangle.get();
    
    auto rectCommands = std::make_unique<DrawCommand>(rectPtr);
    rectCommands->execute();
    
    auto resizeCommand = std::make_unique<ResizeCommand>(rectPtr, 1.2);
    resizeCommand->execute();
    std::cout << std::endl;
    
    // Test 4: Triangle with Commands
    std::cout << "4. Triangle with Commands:\n";
    
    auto triangle = std::make_unique<TriangleReceiver>(10.0, 8.0, 5.0, 5.0, "green");
    ShapeReceiver* triPtr = triangle.get();
    
    auto triCommands = std::make_unique<DrawCommand>(triPtr);
    triCommands->execute();
    
    auto rotateCommand = std::make_unique<RotateCommand>(triPtr, 45.0);
    rotateCommand->execute();
    std::cout << std::endl;
    
    // Test 5: Macro Command Usage
    std::cout << "5. Macro Command Usage:\n";
    
    auto macro = std::make_unique<MacroCommand>("Shape Transformation");
    
    auto rectForMacro = std::make_unique<RectangleReceiver>(5.0, 4.0, 100.0, 100.0, "macro_color");
    ShapeReceiver* macroPtr = rectForMacro.get();
    
    macro->addCommand(std::make_unique<DrawCommand>(macroPtr));
    macro->addCommand(std::make_unique<MoveCommand>(macroPtr, 10.0, 10.0));
    macro->addCommand(std::make_unique<ColorChangeCommand>(macroPtr, "lightblue"));
    macro->addCommand(std::make_unique<RotateCommand>(macroPtr, 90.0));
    
    macro->execute();
    std::cout << std::endl;
    
    // Test 6: Command Invoker Usage
    std::cout << "6. Command Invoker Usage:\n";
    
    CommandInvoker invoker("GraphicsInvoker");
    
    auto circleForInvoker = std::make_unique<CircleReceiver>(3.0, 200.0, 200.0, "invoker_color");
    ShapeReceiver* invokerPtr = circleForInvoker.get();
    
    // Create commands and execute through invoker
    auto drawCmd = std::make_unique<DrawCommand>(invokerPtr);
    auto colorCmd = std::make_unique<ColorChangeCommand>(invokerPtr, "orange");
    auto moveCmd = std::make_unique<MoveCommand>(invokerPtr, 50.0, 25.0);

    invoker.executeCommand(*drawCmd);
    invoker.executeCommand(*colorCmd);
    invoker.executeCommand(*moveCmd);
    
    invoker.printHistory();
    std::cout << std::endl;
    
    // Test 7: Fluent Command Builder
    std::cout << "7. Fluent Command Builder:\n";
    
    auto rectForFluent = std::make_unique<RectangleReceiver>(6.0, 4.0, 300.0, 300.0, "fluent_color");
    ShapeReceiver* fluentPtr = rectForFluent.get();
    
    std::cout << "Fluent command builder functionality would be demonstrated here if implemented\n";
    std::cout << "Instead, manually creating and executing commands:\n";

    auto drawCmd2 = std::make_unique<DrawCommand>(fluentPtr);
    auto moveCmd2 = std::make_unique<MoveCommand>(fluentPtr, 20.0, 10.0);
    auto resizeCmd2 = std::make_unique<ResizeCommand>(fluentPtr, 1.5);
    auto colorCmd2 = std::make_unique<ColorChangeCommand>(fluentPtr, "yellow");

    std::cout << "Executing commands in sequence:\n";
    drawCmd2->execute();
    moveCmd2->execute();
    resizeCmd2->execute();
    colorCmd2->execute();
    std::cout << std::endl;
    
    // Test 8: Command Pattern with Different Shape Types
    std::cout << "8. Command Pattern with Different Shape Types:\n";
    
    std::vector<std::unique_ptr<ShapeReceiver>> shapes;
    shapes.push_back(std::make_unique<CircleReceiver>(4.0, 0.0, 0.0, "multi_circle"));
    shapes.push_back(std::make_unique<RectangleReceiver>(7.0, 5.0, 0.0, 0.0, "multi_rectangle"));
    shapes.push_back(std::make_unique<TriangleReceiver>(8.0, 6.0, 0.0, 0.0, "multi_triangle"));
    
    for (size_t i = 0; i < shapes.size(); ++i) {
        std::cout << "Applying commands to shape " << (i + 1) << " (" << shapes[i]->getName() << "):\n";
        
        // Same commands can be applied to different shape types
        auto drawCmd = std::make_unique<DrawCommand>(shapes[i].get());
        auto moveCmd = std::make_unique<MoveCommand>(shapes[i].get(), 15.0, 10.0);
        auto colorCmd = std::make_unique<ColorChangeCommand>(shapes[i].get(), 
                                                             i == 0 ? "red" : (i == 1 ? "green" : "blue"));
        
        drawCmd->execute();
        moveCmd->execute();
        colorCmd->execute();
        std::cout << "Area: " << shapes[i]->getArea() << std::endl;
        std::cout << std::endl;
    }
    
    // Test 9: Null Command Usage
    std::cout << "9. Null Command Usage:\n";
    
    auto nullCommand = std::make_unique<NullCommand>();
    nullCommand->execute();
    nullCommand->undo();
    std::cout << std::endl;
    
    // Test 10: Command Manager
    std::cout << "10. Command Manager Usage:\n";
    
    CommandManager manager;
    auto availableCommands = manager.getAvailableCommands();
    std::cout << "Available commands in manager: ";
    for (const auto& cmd : availableCommands) {
        std::cout << cmd << " ";
    }
    std::cout << std::endl;
    
    auto managedCircle = std::make_unique<CircleReceiver>(2.5, 5.0, 5.0, "managed_color");
    ShapeReceiver* managedPtr = managedCircle.get();
    
    // Create commands through the manager (with correct interface)
    auto managedDraw = manager.createCommand("draw", managedPtr);
    auto managedColor = manager.createCommand("color", managedPtr);
    
    std::cout << "Executing manager-created commands:\n";
    managedDraw->execute();
    managedColor->execute();
    std::cout << std::endl;
    
    // Test 11: Real-world Scenario - UI Control System
    std::cout << "11. Real-world Scenario - UI Control System:\n";
    
    // Simulate creating UI components with commands
    auto buttonShape = std::make_unique<RectangleReceiver>(100.0, 30.0, 10.0, 10.0, "ui_button");
    auto iconShape = std::make_unique<CircleReceiver>(15.0, 120.0, 20.0, "ui_icon");
    auto panelShape = std::make_unique<RectangleReceiver>(300.0, 200.0, 10.0, 50.0, "ui_panel");
    
    ShapeReceiver* buttonPtr = buttonShape.get();
    ShapeReceiver* iconPtr = iconShape.get();
    ShapeReceiver* panelPtr = panelShape.get();
    
    std::cout << "Creating UI with command pattern:\n";
    
    // Button creation commands
    auto buttonMacro = std::make_unique<MacroCommand>("Button Creation");
    buttonMacro->addCommand(std::make_unique<DrawCommand>(buttonPtr));
    buttonMacro->addCommand(std::make_unique<ColorChangeCommand>(buttonPtr, "lightgray"));
    buttonMacro->execute();
    
    // Icon creation commands
    auto iconMacro = std::make_unique<MacroCommand>("Icon Creation");
    iconMacro->addCommand(std::make_unique<DrawCommand>(iconPtr));
    iconMacro->addCommand(std::make_unique<ColorChangeCommand>(iconPtr, "darkblue"));
    iconMacro->execute();
    
    // Panel creation commands
    auto panelMacro = std::make_unique<MacroCommand>("Panel Creation");
    panelMacro->addCommand(std::make_unique<DrawCommand>(panelPtr));
    panelMacro->addCommand(std::make_unique<ColorChangeCommand>(panelPtr, "white"));
    panelMacro->execute();
    std::cout << std::endl;
    
    // Test 12: Performance Test - Creating Many Commands
    std::cout << "12. Performance Test - Creating Many Commands:\n";
    
    auto start = std::chrono::high_resolution_clock::now();
    
    std::vector<std::unique_ptr<Command>> performanceCommands;
    auto perfCircle = std::make_unique<CircleReceiver>(1.0, 0.0, 0.0, "perf_color");
    ShapeReceiver* perfPtr = perfCircle.get();
    
    for (int i = 0; i < 5000; ++i) {
        // Alternate between different command types
        if (i % 4 == 0) {
            performanceCommands.push_back(std::make_unique<DrawCommand>(perfPtr));
        } else if (i % 4 == 1) {
            performanceCommands.push_back(std::make_unique<MoveCommand>(perfPtr, i % 10, i % 5));
        } else if (i % 4 == 2) {
            performanceCommands.push_back(std::make_unique<ColorChangeCommand>(perfPtr, "perf_color_" + std::to_string(i % 10)));
        } else {
            performanceCommands.push_back(std::make_unique<RotateCommand>(perfPtr, i % 360));
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Created " << performanceCommands.size() << " commands in: " << duration.count() << " microseconds\n";
    std::cout << "Command pattern allows for efficient command creation\n";
    std::cout << std::endl;
    
    // Test 13: Command Pattern Benefits Demonstration
    std::cout << "13. Command Pattern Benefits Demonstration:\n";
    std::cout << "- Encapsulation: Each command encapsulates a specific action\n";
    std::cout << "- Parameterization: Objects can be configured with different commands\n";
    std::cout << "- Queueing: Commands can be queued for later execution\n";
    std::cout << "- Logging: Command execution can be easily logged\n";
    std::cout << "- Undo/Redo: Commands can support undo operations\n";
    std::cout << "- Transactional: Macro commands provide transactional operations\n";
    std::cout << "- Extensibility: New commands can be added without changing existing code\n";
    std::cout << "- Transparency: Same interface for all commands\n";
    std::cout << std::endl;
    
    // Test 14: Dynamic Command Creation and Assignment
    std::cout << "14. Dynamic Command Creation and Assignment:\n";
    
    std::vector<std::function<std::unique_ptr<Command>(ShapeReceiver*)>> commandCreators = {
        [](ShapeReceiver* receiver) { return std::make_unique<DrawCommand>(receiver); },
        [](ShapeReceiver* receiver) { return std::make_unique<MoveCommand>(receiver, 10.0, 10.0); },
        [](ShapeReceiver* receiver) { return std::make_unique<ColorChangeCommand>(receiver, "dynamic_color"); }
    };
    
    auto dynamicCircle = std::make_unique<CircleReceiver>(3.0, 400.0, 400.0, "dynamic");
    ShapeReceiver* dynamicPtr = dynamicCircle.get();
    
    for (size_t i = 0; i < commandCreators.size(); ++i) {
        std::cout << "Creating command dynamically (" << i + 1 << "): ";
        auto command = commandCreators[i](dynamicPtr);
        std::cout << command->getCommandName() << std::endl;
        command->execute();
    }
    std::cout << std::endl;
    
    // Test 15: Multiple Receivers with Same Commands
    std::cout << "15. Same Commands on Different Receivers:\n";
    
    auto circ2 = std::make_unique<CircleReceiver>(2.5, 0.0, 0.0, "common");
    auto rect2 = std::make_unique<RectangleReceiver>(5.0, 3.0, 0.0, 0.0, "common");
    auto tri2 = std::make_unique<TriangleReceiver>(6.0, 4.0, 0.0, 0.0, "common");

    ShapeReceiver* circPtr2 = circ2.get();
    ShapeReceiver* rectPtr2 = rect2.get();
    ShapeReceiver* triPtr2 = tri2.get();

    // Apply the same move command to different shapes
    auto moveCirc = std::make_unique<MoveCommand>(circPtr2, 5.0, 10.0);
    auto moveRect = std::make_unique<MoveCommand>(rectPtr2, 5.0, 10.0);
    auto moveTri = std::make_unique<MoveCommand>(triPtr2, 5.0, 10.0);

    std::cout << "Applying same move command (5,10) to different shapes:\n";
    moveCirc->execute();
    moveRect->execute();
    moveTri->execute();
    std::cout << std::endl;
    
    // Test 16: Macro Command with Different Shapes
    std::cout << "16. Macro Command with Different Shapes:\n";
    
    auto complexMacro = std::make_unique<MacroCommand>("Complex Shape Operations");
    
    // Add operations for different shapes to the same macro
    complexMacro->addCommand(std::make_unique<DrawCommand>(circPtr2));
    complexMacro->addCommand(std::make_unique<ColorChangeCommand>(rectPtr2, "macro_blue"));
    complexMacro->addCommand(std::make_unique<MoveCommand>(triPtr2, 20.0, 15.0));
    complexMacro->addCommand(std::make_unique<RotateCommand>(rectPtr2, 180.0));
    
    std::cout << "Executing complex macro with operations on multiple shapes:\n";
    complexMacro->execute();
    std::cout << std::endl;
    
    // Test 17: Command Pattern vs Direct Method Calls
    std::cout << "17. Command Pattern vs Direct Method Calls:\n";
    
    std::cout << "\nDirect Method Calls:\n";
    CircleReceiver directCircle(3.0, 5.0, 5.0, "direct");
    directCircle.draw();
    directCircle.move(10.0, 10.0);
    directCircle.changeColor("bright_blue");
    
    std::cout << "\nCommand Pattern (Encapsulated Operations):\n";
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
    std::cout << "- Enables undo/redo functionality\n";
    std::cout << "- Supports macro commands for complex operations\n";
    std::cout << "- Provides uniform interface for different operations\n";
    std::cout << "- Allows for command parameterization and delegation\n";
    std::cout << std::endl;
    
    // Test 18: Command Chaining Concept (though not typical for Command pattern)
    std::cout << "18. Command Pattern Integration with Other Patterns:\n";
    
    // Demonstrating how commands could be integrated with other patterns
    // For example, combining with factory pattern for dynamic command creation
    std::cout << "Combining Command pattern with Factory concept:\n";
    
    auto factoryCircle = std::make_unique<CircleReceiver>(4.0, 600.0, 600.0, "factory_integration");
    ShapeReceiver* factoryPtr = factoryCircle.get();
    
    // This simulates a command factory concept
    std::vector<std::string> commandTypes = {"draw", "move", "color", "rotate"};
    for (const auto& cmdType : commandTypes) {
        std::cout << "  Executing " << cmdType << " command from factory\n";
        if (cmdType == "draw") {
            auto cmd = std::make_unique<DrawCommand>(factoryPtr);
            cmd->execute();
        } else if (cmdType == "move") {
            auto cmd = std::make_unique<MoveCommand>(factoryPtr, 5.0, 5.0);
            cmd->execute();
        } else if (cmdType == "color") {
            auto cmd = std::make_unique<ColorChangeCommand>(factoryPtr, "factory_color");
            cmd->execute();
        } else if (cmdType == "rotate") {
            auto cmd = std::make_unique<RotateCommand>(factoryPtr, 45.0);
            cmd->execute();
        }
    }
    std::cout << std::endl;
    
    // Test 19: Extensibility - Adding New Commands
    std::cout << "19. Extensibility - Easy to Add New Commands:\n";
    std::cout << "New command types can be added by implementing the Command interface\n";
    std::cout << "No changes needed to existing code\n";
    std::cout << "Existing commands and invokers work with new commands automatically\n";
    std::cout << std::endl;
    
    // Test 20: Command Pattern Summary
    std::cout << "20. Command Pattern Summary:\n";
    std::cout << "The Command pattern successfully demonstrates:\n";
    std::cout << "- Encapsulation of operations as objects\n";
    std::cout << "- Storage and queuing of operations\n";
    std::cout << "- Support for undoable operations\n";
    std::cout << "- Parameterization of objects with operations\n";
    std::cout << "- Transactional operations through macro commands\n";
    std::cout << "- Dynamic command creation and execution\n";
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