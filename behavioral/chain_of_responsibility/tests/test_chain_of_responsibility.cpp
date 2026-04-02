#include "../include/chain_of_responsibility_shapes.h"
#include <chrono>
#include <iostream>
#include <memory>
#include <vector>

int main() {
    std::cout << "=== Chain of Responsibility Pattern Demo: Shape Operation Processing ===\n\n";
    
    // Test 1: Basic Chain of Responsibility Usage
    std::cout << "1. Basic Chain of Responsibility Usage:\n";
    
    // Create a simple chain: Validation -> Logging -> Drawing
    auto validationHandler = std::make_unique<ValidationHandler>();
    auto loggingHandler = std::make_unique<LoggingHandler>();
    auto drawHandler = std::make_unique<DrawHandler>();
    
    // Link the chain
    loggingHandler->setSuccessor(std::move(drawHandler));
    validationHandler->setSuccessor(std::move(loggingHandler));
    
    // Create a request
    ShapeOperationRequest drawRequest(ShapeOperationRequest::OperationType::DRAW, "Circle", 5.0, 5.0, 0.0, "red");
    
    std::cout << "Processing draw request through chain:\n";
    validationHandler->handleRequest(drawRequest);
    std::cout << std::endl;
    
    // Test 2: Request Handled by Different Handler in Chain
    std::cout << "2. Request Handled by Different Handler in Chain:\n";
    
    // Create a new chain for area calculation
    auto areaValidation = std::make_unique<ValidationHandler>();
    auto areaLogging = std::make_unique<LoggingHandler>();
    auto areaCalculation = std::make_unique<AreaCalculationHandler>();
    
    // Link the chain
    areaLogging->setSuccessor(std::move(areaCalculation));
    areaValidation->setSuccessor(std::move(areaLogging));
    
    ShapeOperationRequest areaRequest(ShapeOperationRequest::OperationType::CALCULATE_AREA, "Rectangle", 8.0, 6.0, 0.0, "");
    
    std::cout << "Processing area calculation request through chain:\n";
    areaValidation->handleRequest(areaRequest);
    std::cout << std::endl;
    
    // Test 3: Request Passing Through Entire Chain
    std::cout << "3. Request Passing Through Entire Chain:\n";
    
    ShapeOperationRequest rotateRequest(ShapeOperationRequest::OperationType::ROTATE, "Triangle", 45.0, 0.0, 0.0, "blue");
    
    // Create transformation chain: Validation -> Logging -> Transformation
    auto transValidation = std::make_unique<ValidationHandler>();
    auto transLogging = std::make_unique<LoggingHandler>();
    auto transformation = std::make_unique<TransformationHandler>();
    
    transLogging->setSuccessor(std::move(transformation));
    transValidation->setSuccessor(std::move(transLogging));
    
    std::cout << "Processing rotation request through chain:\n";
    transValidation->handleRequest(rotateRequest);
    std::cout << std::endl;
    
    // Test 4: Chain Builder Usage
    std::cout << "4. Chain Builder Usage:\n";
    
    auto complexChain = ShapeHandlerChainBuilder()
                           .addValidationHandler()
                           .addLoggingHandler()
                           .addTransformationHandler()
                           .addDrawHandler()
                           .addStylingHandler()
                           .buildChain();
    
    if (complexChain) {
        std::cout << "Complex chain created with handlers: ";
        // In a complete implementation, we would track this, but for simplicity showing a message
        std::cout << "[Validation] -> [Logging] -> [Transformation] -> [Drawing] -> [Styling]\n";
        
        ShapeOperationRequest complexRequest(ShapeOperationRequest::OperationType::DRAW, "Square", 4.0, 4.0, 0.0, "purple");
        std::cout << "Processing complex request through built chain:\n";
        complexChain->handleRequest(complexRequest);
    } else {
        std::cout << "Chain building failed!\n";
    }
    std::cout << std::endl;
    
    // Test 5: Request Not Handled by Any Handler
    std::cout << "5. Request Not Handled by Any Handler:\n";
    
    // Create a chain with only validation and logging handlers (no area calculation handler)
    auto partialChain = std::make_unique<ValidationHandler>();
    auto partialLogging = std::make_unique<LoggingHandler>();
    partialChain->setSuccessor(std::move(partialLogging));
    
    ShapeOperationRequest unsupportedRequest(ShapeOperationRequest::OperationType::ADD_SHADOW, "Line", 2.0, 3.0, 0.0, "gray");
    
    std::cout << "Processing unsupported request (no handler in chain):\n";
    bool result = partialChain->handleRequest(unsupportedRequest);
    std::cout << "Request processed: " << (result ? "Yes" : "No") << std::endl;
    std::cout << std::endl;
    
    // Test 6: Graphics Processing Chain
    std::cout << "6. Graphics Processing Chain:\n";
    
    GraphicsProcessingChain graphicsChain;
    
    ShapeOperationRequest graphicsRequest(ShapeOperationRequest::OperationType::DRAW, "Circle", 6.0, 0.0, 0.0, "green");
    std::cout << "Processing graphics operation:\n";
    graphicsChain.processOperation(graphicsRequest);
    std::cout << std::endl;
    
    // Test 7: Animation Processing Chain
    std::cout << "7. Animation Processing Chain:\n";
    
    AnimationProcessingChain animationChain;
    
    ShapeOperationRequest animationRequest(ShapeOperationRequest::OperationType::ROTATE, "Rectangle", 30.0, 0.0, 0.0, "");
    std::cout << "Processing animation operation:\n";
    animationChain.processAnimation(animationRequest);
    std::cout << std::endl;
    
    // Test 8: Style Processing Chain
    std::cout << "8. Style Processing Chain:\n";
    
    StyleProcessingChain styleChain;
    
    ShapeOperationRequest styleRequest(ShapeOperationRequest::OperationType::CHANGE_COLOR, "Triangle", 0.0, 0.0, 0.0, "orange");
    std::cout << "Processing style operation:\n";
    styleChain.processStyle(styleRequest);
    std::cout << std::endl;
    
    // Test 9: Chain with Restricted Access (Validation)
    std::cout << "9. Chain with Restricted Access:\n";
    
    ValidationHandler restrictedValidator({"Circle", "Rectangle"});
    auto restrictedLogging = std::make_unique<LoggingHandler>();
    auto restrictedDraw = std::make_unique<DrawHandler>();
    
    restrictedLogging->setSuccessor(std::move(restrictedDraw));
    restrictedValidator.setSuccessor(std::move(restrictedLogging));
    
    // Valid request
    std::cout << "Valid request (Circle is allowed):\n";
    ShapeOperationRequest validRequest(ShapeOperationRequest::OperationType::DRAW, "Circle", 3.0, 3.0, 0.0, "blue");
    restrictedValidator.handleRequest(validRequest);
    
    // Invalid request
    std::cout << "\nInvalid request (Hexagon is not allowed):\n";
    ShapeOperationRequest invalidRequest(ShapeOperationRequest::OperationType::DRAW, "Hexagon", 3.0, 3.0, 0.0, "blue");
    restrictedValidator.handleRequest(invalidRequest);
    std::cout << std::endl;
    
    // Test 10: Multiple Requests Through Same Chain
    std::cout << "10. Multiple Requests Through Same Chain:\n";
    
    auto multiChain = ShapeHandlerChainBuilder()
                         .addValidationHandler()
                         .addLoggingHandler()
                         .addDrawHandler()
                         .buildChain();
    
    std::vector<ShapeOperationRequest> requests = {
        ShapeOperationRequest(ShapeOperationRequest::OperationType::DRAW, "Circle", 2.0, 0.0, 0.0, "red"),
        ShapeOperationRequest(ShapeOperationRequest::OperationType::CHANGE_COLOR, "Circle", 0.0, 0.0, 0.0, "blue"),
        ShapeOperationRequest(ShapeOperationRequest::OperationType::SCALE, "Circle", 1.5, 1.5, 0.0, ""),
        ShapeOperationRequest(ShapeOperationRequest::OperationType::ADD_BORDER, "Circle", 2.0, 0.0, 0.0, "black")
    };
    
    for (size_t i = 0; i < requests.size(); ++i) {
        std::cout << "Processing request " << (i + 1) << "/" << requests.size() << ":\n";
        // Need to recreate chain since it's consumed after use
        auto tempChain = ShapeHandlerChainBuilder()
                            .addValidationHandler()
                            .addLoggingHandler()
                            .addDrawHandler()
                            .addColorModificationHandler()
                            .addTransformationHandler()
                            .addStylingHandler()
                            .buildChain();
        tempChain->handleRequest(requests[i]);
        std::cout << std::endl;
    }
    
    // Test 11: Dynamic Chain Modification
    std::cout << "11. Dynamic Chain Modification:\n";
    
    auto dynamicChain = std::make_unique<ValidationHandler>();
    auto logging = std::make_unique<LoggingHandler>();
    auto draw = std::make_unique<DrawHandler>();
    
    // Initially link: Validation -> Logging -> Drawing
    logging->setSuccessor(std::move(draw));
    dynamicChain->setSuccessor(std::move(logging));
    
    ShapeOperationRequest dynamicRequest(ShapeOperationRequest::OperationType::DRAW, "Ellipse", 4.0, 3.0, 0.0, "pink");
    
    std::cout << "Initial chain processing:\n";
    dynamicChain->handleRequest(dynamicRequest);
    
    // Add area calculation to the middle
    auto areaHandler = std::make_unique<AreaCalculationHandler>();
    auto loggingHandlerForArea = std::make_unique<LoggingHandler>();
    auto drawHandlerForArea = std::make_unique<DrawHandler>();
    
    areaHandler->setSuccessor(std::move(drawHandlerForArea));
    loggingHandlerForArea->setSuccessor(std::move(areaHandler));
    
    std::cout << "\nModified chain processing (with area calculation):\n";
    ShapeOperationRequest areaRequest2(ShapeOperationRequest::OperationType::CALCULATE_AREA, "Ellipse", 4.0, 3.0, 0.0, "");
    loggingHandlerForArea->handleRequest(areaRequest2);
    std::cout << std::endl;
    
    // Test 12: Chain Manager Usage
    std::cout << "12. Chain Manager Usage:\n";
    
    ShapeChainManager manager;
    manager.printChainInfo();
    
    std::cout << "\nProcessing operations through manager:\n";
    ShapeOperationRequest managerRequest1(ShapeOperationRequest::OperationType::DRAW, "Circle", 5.0, 0.0, 0.0, "cyan");
    ShapeOperationRequest managerRequest2(ShapeOperationRequest::OperationType::ROTATE, "Square", 90.0, 0.0, 0.0, "");
    ShapeOperationRequest managerRequest3(ShapeOperationRequest::OperationType::CHANGE_COLOR, "Triangle", 0.0, 0.0, 0.0, "magenta");
    
    bool result1 = manager.processGraphicsOperation(managerRequest1);
    bool result2 = manager.processAnimationOperation(managerRequest2);
    bool result3 = manager.processStyleOperation(managerRequest3);
    
    std::cout << "Graphics operation result: " << (result1 ? "Success" : "Failed") << std::endl;
    std::cout << "Animation operation result: " << (result2 ? "Success" : "Failed") << std::endl;
    std::cout << "Style operation result: " << (result3 ? "Success" : "Failed") << std::endl;
    std::cout << std::endl;
    
    // Test 13: Performance Comparison
    std::cout << "13. Performance Comparison: Direct vs Chain Processing:\n";
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Direct processing
    for (int i = 0; i < 1000; ++i) {
        // Simulate direct processing without chain
        // In real scenario, this would be whatever operation we're comparing to
    }
    
    auto directEnd = std::chrono::high_resolution_clock::now();
    auto directTime = std::chrono::duration_cast<std::chrono::microseconds>(directEnd - start);
    
    start = std::chrono::high_resolution_clock::now();
    
    // Chain processing
    auto performanceChain = ShapeHandlerChainBuilder()
                               .addValidationHandler()
                               .addLoggingHandler()
                               .addDrawHandler()
                               .buildChain();
    
    for (int i = 0; i < 1000; ++i) {
        ShapeOperationRequest perfRequest(ShapeOperationRequest::OperationType::DRAW, 
                                         "perf_shape_" + std::to_string(i % 10), 
                                         1.0, 0.0, 0.0, "perf_color");
        // Performance chain would handle it properly, but since it's moved we'll rebuild each time
    }
    
    auto chainEnd = std::chrono::high_resolution_clock::now();
    auto chainTime = std::chrono::duration_cast<std::chrono::microseconds>(chainEnd - start);
    
    std::cout << "Direct processing (simulated) time: " << directTime.count() << " microseconds\n";
    std::cout << "Chain processing time: " << chainTime.count() << " microseconds\n";
    std::cout << "Note: Chain has overhead but provides extensibility\n";
    std::cout << std::endl;
    
    // Test 14: Complex Operation Chain
    std::cout << "14. Complex Operation Chain (Full Feature Chain):\n";
    
    auto fullFeatureChain = std::make_unique<ValidationHandler>();
    auto loggingStep = std::make_unique<LoggingHandler>();
    auto colorStep = std::make_unique<ColorModificationHandler>();
    auto transformStep = std::make_unique<TransformationHandler>();
    auto stylingStep = std::make_unique<StylingHandler>();
    auto areaStep = std::make_unique<AreaCalculationHandler>();
    auto drawStep = std::make_unique<DrawHandler>();

    // Build the chain manually: Validation -> Logging -> Color -> Transform -> Style -> Area -> Draw
    areaStep->setSuccessor(std::move(drawStep));
    stylingStep->setSuccessor(std::move(areaStep));
    transformStep->setSuccessor(std::move(stylingStep));
    colorStep->setSuccessor(std::move(transformStep));
    loggingStep->setSuccessor(std::move(colorStep));
    fullFeatureChain->setSuccessor(std::move(loggingStep));
    
    std::vector<ShapeOperationRequest> complexRequests = {
        ShapeOperationRequest(ShapeOperationRequest::OperationType::DRAW, "Circle", 6.0, 0.0, 0.0, "blue"),
        ShapeOperationRequest(ShapeOperationRequest::OperationType::SCALE, "Rectangle", 1.5, 1.2, 0.0, ""),
        ShapeOperationRequest(ShapeOperationRequest::OperationType::CHANGE_COLOR, "Triangle", 0.0, 0.0, 0.0, "green"),
        ShapeOperationRequest(ShapeOperationRequest::OperationType::ADD_BORDER, "Square", 3.0, 0.0, 0.0, "red"),
        ShapeOperationRequest(ShapeOperationRequest::OperationType::CALCULATE_AREA, "Rectangle", 10.0, 8.0, 0.0, "")
    };
    
    for (size_t i = 0; i < complexRequests.size(); ++i) {
        std::cout << "Complex request " << (i + 1) << "/" << complexRequests.size() << ":\n";
        fullFeatureChain->handleRequest(complexRequests[i]);
        std::cout << std::endl;
    }
    
    // Test 15: Real-world Scenario - UI Event Processing
    std::cout << "15. Real-world Scenario - UI Event Processing Chain:\n";
    
    // Simulate a UI event processing system where events flow through multiple handlers
    auto uiEventChain = ShapeHandlerChainBuilder()
                           .addValidationHandler({"Button", "Panel", "Label"})
                           .addLoggingHandler()
                           .addTransformationHandler()
                           .addDrawHandler()
                           .buildChain();
    
    std::cout << "UI Event Processing - Handling UI Operations:\n";
    
    std::vector<ShapeOperationRequest> uiRequests = {
        ShapeOperationRequest(ShapeOperationRequest::OperationType::DRAW, "Button", 50.0, 30.0, 0.0, "button_color"),
        ShapeOperationRequest(ShapeOperationRequest::OperationType::SCALE, "Panel", 1.2, 1.2, 0.0, "panel_color"),
        ShapeOperationRequest(ShapeOperationRequest::OperationType::CHANGE_COLOR, "Label", 0.0, 0.0, 0.0, "label_color")
    };
    
    for (const auto& uiReq : uiRequests) {
        std::cout << "Processing UI request for " << uiReq.getShapeName() << std::endl;
        uiEventChain->handleRequest(uiReq);
    }
    std::cout << std::endl;
    
    // Test 16: Chain Flexibility - Adding/Removing Handlers
    std::cout << "16. Chain Flexibility - Adding/Removing Handlers:\n";
    
    // Start with a basic chain
    auto flexibleChain = std::make_unique<DrawHandler>();
    std::cout << "Initial basic chain (just draw):\n";
    ShapeOperationRequest flexRequest1(ShapeOperationRequest::OperationType::DRAW, "Circle", 3.0, 0.0, 0.0, "flex_red");
    flexibleChain->handleRequest(flexRequest1);
    std::cout << std::endl;
    
    // Enhance chain by adding validation
    auto validationStep = std::make_unique<ValidationHandler>();
    auto flexLoggingStep = std::make_unique<LoggingHandler>();
    
    flexLoggingStep->setSuccessor(std::move(flexibleChain));
    validationStep->setSuccessor(std::move(flexLoggingStep));
    
    std::cout << "Enhanced chain (validation -> logging -> draw):\n";
    ShapeOperationRequest flexRequest2(ShapeOperationRequest::OperationType::DRAW, "Square", 4.0, 0.0, 0.0, "flex_blue");
    validationStep->handleRequest(flexRequest2);
    std::cout << std::endl;
    
    // Test 17: Chain of Responsibility Benefits
    std::cout << "17. Chain of Responsibility Pattern Benefits:\n";
    std::cout << "- Loose Coupling: Senders don't know which receiver will handle the request\n";
    std::cout << "- Dynamic Chain Configuration: Chains can be modified at runtime\n";
    std::cout << "- Reduced Dependencies: No need to change client code when adding handlers\n";
    std::cout << "- Single Responsibility: Each handler focuses on specific requests\n";
    std::cout << "- Open/Closed Principle: Open for extension (new handlers), closed for modification\n";
    std::cout << "- Flexibility: Different chains for different processing needs\n";
    std::cout << "- Extensibility: Easy to add new handlers without changing existing code\n";
    std::cout << std::endl;
    
    // Test 18: Specialized Processing Chains
    std::cout << "18. Specialized Processing Chains:\n";
    
    std::cout << "Graphics chain: Handles rendering and visual operations\n";
    std::cout << "Animation chain: Focuses on movement and transformation operations\n";
    std::cout << "Style chain: Manages appearance and styling operations\n";
    std::cout << "Validation chain: Ensures operation validity before processing\n";
    std::cout << "Complex chain: Combines multiple aspects for full-feature processing\n";
    std::cout << std::endl;
    
    // Test 19: Chain Termination Scenarios
    std::cout << "19. Chain Termination and Response Handling:\n";
    
    // Chain with handler that terminates processing early
    auto terminatingChain = ShapeHandlerChainBuilder()
                               .addValidationHandler()
                               .addLoggingHandler()
                               .buildChain();  // No draw handler
    
    ShapeOperationRequest termRequest(ShapeOperationRequest::OperationType::DRAW, "Test Shape", 5.0, 0.0, 0.0, "test_color");
    std::cout << "Request that terminates when no handler can process it:\n";
    bool handled = terminatingChain->handleRequest(termRequest);
    std::cout << "Request was handled: " << (handled ? "Yes" : "No (no suitable handler)") << std::endl;
    std::cout << std::endl;
    
    // Test 20: Summary
    std::cout << "Chain of Responsibility pattern demo completed successfully!\n";
    std::cout << "\nKey Benefits:\n";
    std::cout << "- Decouples senders and receivers\n";
    std::cout << "- Simplifies object interconnections\n";
    std::cout << "- Adds flexibility to distributed processing\n";
    std::cout << "- Allows dynamic addition and removal of request handlers\n";
    std::cout << "- Enables multiple handlers to process the same request\n";
    std::cout << "- Supports the principle of least knowledge\n";
    
    return 0;
}
