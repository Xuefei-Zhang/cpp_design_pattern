#include "behavioral/command_patterns.h"
#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
#include <functional>
#include <algorithm>

int main() {
    std::cout << "=== Command Pattern Demo: Shape Graphics Control System ===\n\n";
    
    // Test 1: Basic Command Pattern Usage
    std::cout << "1. Basic Command Pattern Usage:\n";
    
    auto circle = std::make_unique<CircleReceiver>(5.0, 10.0, 10.0, "red");
    
    // Create commands for the circle
    auto drawCommand = std::make_unique<DrawCommand>(circle.get());
    auto moveCommand = std::make_unique<MoveCommand>(circle.get(), 20.0, 15.0);
    auto resizeCommand = std::make_unique<ResizeCommand>(circle.get(), 1.5);
    auto colorCommand = std::make_unique<ColorChangeCommand>(circle.get(), "blue");
    
    // Execute commands
    drawCommand->execute();
    moveCommand->execute();
    resizeCommand->execute();
    colorCommand->execute();
    std::cout << std::endl;
    
    // Test 2: Undo Operations
    std::cout << "2. Undo Operations:\n";
    
    std::cout << "Undoing color change...\n";
    colorCommand->undo();
    
    std::cout << "Undoing resize (approximately)...\n";
    resizeCommand->undo();  // Note: In a real implementation, this would restore exact values
    
    std::cout << std::endl;
    
    // Test 3: Command Invoker Usage
    std::cout << "3. Command Invoker Usage:\n";
    
    CommandInvoker invoker("GraphicsInvoker");
    invoker.initializeGraphicsSystem();
    
    // Execute commands through invoker
    std::vector<std::unique_ptr<Command>> commands;
    commands.push_back(std::make_unique<DrawCommand>(circle.get()));
    commands.push_back(std::make_unique<MoveCommand>(circle.get(), 5.0, 5.0));
    commands.push_back(std::make_unique<ColorChangeCommand>(circle.get(), "green"));
    
    invoker.executeCommands(commands);
    std::cout << std::endl;
    
    // Test 4: Rectangle Commands
    std::cout << "4. Rectangle Commands:\n";
    
    auto rectangle = std::make_unique<RectangleReceiver>(8.0, 6.0, 0.0, 0.0, "blue");
    
    auto rectDrawCommand = std::make_unique<DrawCommand>(rectangle.get());
    auto rectMoveCommand = std::make_unique<MoveCommand>(rectangle.get(), 30.0, 20.0);
    auto rectResizeCommand = std::make_unique<ResizeCommand>(rectangle.get(), 1.2);
    auto rectColorCommand = std::make_unique<ColorChangeCommand>(rectangle.get(), "yellow");
    auto rectRotateCommand = std::make_unique<RotateCommand>(rectangle.get(), 45.0);
    
    std::cout << "Executing rectangle operations:\n";
    rectDrawCommand->execute();
    rectMoveCommand->execute();
    rectResizeCommand->execute();
    rectColorCommand->execute();
    rectRotateCommand->execute();
    std::cout << std::endl;
    
    // Test 5: Triangle Commands
    std::cout << "5. Triangle Commands:\n";
    
    auto triangle = std::make_unique<TriangleReceiver>(10.0, 8.0, 5.0, 5.0, "green");
    
    auto triDrawCommand = std::make_unique<DrawCommand>(triangle.get());
    auto triMoveCommand = std::make_unique<MoveCommand>(triangle.get(), 15.0, 10.0);
    auto triColorCommand = std::make_unique<ColorChangeCommand>(triangle.get(), "orange");
    
    std::cout << "Executing triangle operations:\n";
    triDrawCommand->execute();
    triMoveCommand->execute();
    triColorCommand->execute();
    std::cout << std::endl;
    
    // Test 6: Macro Command Usage
    std::cout << "6. Macro Command Usage:\n";
    
    auto macro = std::make_unique<MacroCommand>("Shape Transformation Sequence");
    macro->addCommand(std::make_unique<DrawCommand>(circle.get()));
    macro->addCommand(std::make_unique<MoveCommand>(circle.get(), 5.0, 5.0));
    macro->addCommand(std::make_unique<ResizeCommand>(circle.get(), 1.1));
    macro->addCommand(std::make_unique<ColorChangeCommand>(circle.get(), "purple"));
    
    std::cout << "Executing macro command:\n";
    macro->execute();
    std::cout << "Undoing macro command (reversing actions):\n";
    macro->undo();
    std::cout << std::endl;
    
    // Test 7: Protection Proxy Integration
    std::cout << "7. Protection Proxy Integration:\n";
    
    auto protectedCircle = std::make_unique<ProtectedShapeProxy>(
        std::make_unique<CircleReceiver>(3.0, 1.0, 1.0, "protected_color"), 
        "admin"
    );
    auto protectedDraw = std::make_unique<DrawCommand>(protectedCircle.get());
    protectedDraw->execute();
    
    auto unauthorizedCircle = std::make_unique<ProtectedShapeProxy>(
        std::make_unique<CircleReceiver>(3.0, 1.0, 1.0, "protected_color"), 
        "guest"
    );
    auto unauthorizedDraw = std::make_unique<DrawCommand>(unauthorizedCircle.get());
    unauthorizedDraw->execute();
    std::cout << std::endl;
    
    // Test 8: Advanced CommandInvoker with Macro Recording
    std::cout << "8. Advanced CommandInvoker with Macro Recording:\n";
    
    AdvancedCommandInvoker advancedInvoker;
    
    // Begin macro recording
    advancedInvoker.beginMacroRecording("Drawing Sequence");
    
    // Execute several commands that will be recorded
    auto simpleCircle = std::make_unique<CircleReceiver>(1.0, 0.0, 0.0, "recording");
    (void)simpleCircle; // Suppress unused warning, would be used if we had receivers in the recordCommand method
    
    // End macro recording and execute it
    advancedInvoker.endMacroRecording();
    std::cout << std::endl;
    
    // Test 9: Command Manager Usage
    std::cout << "9. Command Manager Usage:\n";
    
    CommandManager manager;
    auto circleForManager = std::make_unique<CircleReceiver>(4.0, 2.0, 2.0, "manager_color");
    ShapeReceiver* circlePtr = circleForManager.get();
    
    auto availableCommands = manager.getAvailableCommands();
    std::cout << "Available commands: ";
    for (const auto& cmd : availableCommands) {
        std::cout << cmd << " ";
    }
    std::cout << std::endl;
    
    auto drawCmd = manager.createCommand("draw", circlePtr);
    auto moveCmd = manager.createCommand("move", circlePtr);
    auto resizeCmd = manager.createCommand("resize", circlePtr);
    
    std::cout << "Executing commands from manager:\n";
    drawCmd->execute();
    moveCmd->execute();
    resizeCmd->execute();
    std::cout << std::endl;
    
    // Test 10: Real-world Scenario - Drawing Application
    std::cout << "10. Real-world Scenario - Drawing Application:\n";
    
    std::vector<std::unique_ptr<ShapeReceiver>> canvas;
    
    // Create shapes on canvas
    canvas.push_back(std::make_unique<CircleReceiver>(5.0, 50.0, 50.0, "canvas_red"));
    canvas.push_back(std::make_unique<RectangleReceiver>(10.0, 8.0, 100.0, 100.0, "canvas_blue"));
    canvas.push_back(std::make_unique<TriangleReceiver>(8.0, 6.0, 150.0, 150.0, "canvas_green"));
    
    // Create commands for canvas operations
    std::vector<std::unique_ptr<Command>> canvasCommands;
    
    for (size_t i = 0; i < canvas.size(); ++i) {
        canvasCommands.push_back(std::make_unique<DrawCommand>(canvas[i].get()));
        canvasCommands.push_back(std::make_unique<MoveCommand>(canvas[i].get(), 10.0, 10.0));
    }
    
    std::cout << "Canvas with " << canvas.size() << " shapes:\n";
    for (auto& cmd : canvasCommands) {
        cmd->execute();
    }
    std::cout << std::endl;
    
    // Test 11: Animation Sequence with Command Pattern
    std::cout << "11. Animation Sequence with Command Pattern:\n";
    
    auto animatedShape = std::make_unique<RectangleReceiver>(4.0, 3.0, 200.0, 200.0, "animated");
    ShapeReceiver* animPtr = animatedShape.get();
    
    auto animationMacro = std::make_unique<MacroCommand>("Animation Sequence");
    animationMacro->addCommand(std::make_unique<MoveCommand>(animPtr, 5.0, 0.0));
    animationMacro->addCommand(std::make_unique<ResizeCommand>(animPtr, 1.1));
    animationMacro->addCommand(std::make_unique<MoveCommand>(animPtr, 0.0, 5.0));
    animationMacro->addCommand(std::make_unique<ResizeCommand>(animPtr, 0.9));
    animationMacro->addCommand(std::make_unique<ColorChangeCommand>(animPtr, "lightblue"));
    
    std::cout << "Playing animation sequence:\n";
    animationMacro->execute();
    std::cout << std::endl;
    
    // Test 12: Null Command Usage
    std::cout << "12. Null Command Usage:\n";
    
    auto nullCmd = std::make_unique<NullCommand>();
    nullCmd->execute();
    nullCmd->undo();
    std::cout << std::endl;
    
    // Test 13: Composite Pattern Integration
    std::cout << "13. Composite Pattern Integration:\n";
    
    // This would involve creating a composite shape with commands, but for brevity, 
    // we'll simulate the concept
    auto compositeMacro = std::make_unique<MacroCommand>("Composite Shape Operations");
    auto compositeCircle = std::make_unique<CircleReceiver>(3.0, 10.0, 10.0, "composite");
    auto compositeRect = std::make_unique<RectangleReceiver>(5.0, 4.0, 50.0, 50.0, "composite");
    
    compositeMacro->addCommand(std::make_unique<DrawCommand>(compositeCircle.get()));
    compositeMacro->addCommand(std::make_unique<ColorChangeCommand>(compositeCircle.get(), "lightgreen"));
    compositeMacro->addCommand(std::make_unique<DrawCommand>(compositeRect.get()));
    compositeMacro->addCommand(std::make_unique<ColorChangeCommand>(compositeRect.get(), "lightyellow"));
    
    compositeMacro->execute();
    std::cout << std::endl;
    
    // Test 14: Performance Considerations
    std::cout << "14. Performance Test - Creating Many Commands:\n";
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Create and execute many commands
    std::vector<std::unique_ptr<Command>> performanceCommands;
    for (int i = 0; i < 1000; ++i) {
        auto tempCircle = std::make_unique<CircleReceiver>(1.0, i, i, "perf_color" + std::to_string(i));
        performanceCommands.push_back(std::make_unique<DrawCommand>(tempCircle.get()));
        // Note: This is just for demonstration - in practice, we'd keep references to shapes
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Created 1000 commands in: " << duration.count() << " microseconds\n";
    std::cout << "Command pattern overhead is minimal for creation\n";
    std::cout << std::endl;
    
    // Test 15: Command Pattern Benefits Demonstration
    std::cout << "15. Command Pattern Benefits Demonstration:\n";
    
    std::cout << "- Encapsulation: Each command encapsulates a specific action\n";
    std::cout << "- Parameterization: Objects can be parameterized with different commands\n";
    std::cout << "- Queueing: Commands can be queued for later execution\n";
    std::cout << "- Logging: Commands can be logged for debugging or recovery\n";
    std::cout << "- Undo/Redo: Commands can often be undone or reversed\n";
    std::cout << "- Transactional: Sequences of commands can be treated as transactions\n";
    std::cout << "- Late Binding: Command execution can be delayed\n";
    std::cout << std::endl;
    
    // Test 16: Advanced Features - Chaining Commands
    std::cout << "16. Advanced Feature - Command Chaining Concept:\n";
    
    // In a real system, we might implement command chaining, but for this demo
    // we'll show how multiple commands can be applied in sequence
    auto chainingShape = std::make_unique<CircleReceiver>(2.0, 0.0, 0.0, "chaining");
    ShapeReceiver* chainPtr = chainingShape.get();
    
    std::cout << "Applying a sequence of operations:\n";
    auto drawOp = std::make_unique<DrawCommand>(chainPtr);
    auto colorOp = std::make_unique<ColorChangeCommand>(chainPtr, "rainbow");
    auto moveOp = std::make_unique<MoveCommand>(chainPtr, 5.0, 5.0);
    auto scaleOp = std::make_unique<ResizeCommand>(chainPtr, 1.5);
    
    drawOp->execute();
    colorOp->execute();
    moveOp->execute();
    scaleOp->execute();
    
    std::cout << "Each operation was encapsulated in its own command object\n";
    std::cout << std::endl;
    
    // Test 17: Command History and Undo Stack
    std::cout << "17. Command History and Undo Capability:\n";
    
    CommandInvoker historyInvoker("HistoryInvoker");
    historyInvoker.initializeGraphicsSystem();
    
    auto historyShape = std::make_unique<RectangleReceiver>(6.0, 4.0, 25.0, 25.0, "history_color");
    ShapeReceiver* histPtr = historyShape.get();
    
    // Execute some commands
    historyInvoker.executeCommand(std::make_unique<DrawCommand>(histPtr));
    historyInvoker.executeCommand(std::make_unique<MoveCommand>(histPtr, 10.0, 5.0));
    historyInvoker.executeCommand(std::make_unique<ColorChangeCommand>(histPtr, "golden"));
    
    std::cout << "History size: " << historyInvoker.getCommandHistorySize() << std::endl;
    std::cout << "Undo stack size: " << historyInvoker.getUndoStackSize() << std::endl;
    historyInvoker.printHistory();
    std::cout << std::endl;
    
    // Test 18: Multiple Receivers with Same Commands
    std::cout << "18. Same Commands on Different Receivers:\n";
    
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
    
    // Test 19: Remote Command Simulation
    std::cout << "19. Remote Command Simulation:\n";
    
    auto remoteCircle = std::make_unique<CircleReceiver>(4.0, 30.0, 30.0, "remote");
    auto remoteCmd = std::make_unique<RemoteShapeProxy>(std::unique_ptr<Shape>(remoteCircle.get()), "remote_server");
    
    // This would use the RemoteShapeProxy created earlier
    std::cout << "Remote operations would happen through network\n";
    std::cout << std::endl;
    
    // Test 20: Command Pattern Comparison with Direct Method Calls
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
    std::cout << "- Supports transactional operations\n";
    std::cout << "- Enables macro commands for complex operations\n";
    std::cout << "- Provides uniform interface for different operations\n";
    std::cout << std::endl;
    
    // Test 21: Cleanup
    std::cout << "21. Cleanup and Shutdown:\n";
    
    invoker.shutdownGraphicsSystem();
    
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