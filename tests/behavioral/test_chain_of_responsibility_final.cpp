#include "behavioral/chain_of_responsibility_shapes_fixed.h"
#include <iostream>
#include <memory>
#include <vector>
#include <chrono>

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
        std::cout << "Complex chain created\n";
        
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
    auto partialValidation = std::make_unique<ValidationHandler>();
    auto partialLogging = std::make_unique<LoggingHandler>();
    partialValidation->setSuccessor(std::move(partialLogging));
    
    ShapeOperationRequest unsupportedRequest(ShapeOperationRequest::OperationType::ADD_SHADOW, "Line", 2.0, 3.0, 0.0, "gray");
    
    std::cout << "Processing unsupported request (no handler in chain):\n";
    bool result = partialValidation->handleRequest(unsupportedRequest);
    std::cout << "Request processed: " << (result ? "Yes" : "No") << std::endl;
    std::cout << std::endl;
    
    // Test 6: Graphics Processing Chain
    std::cout << "6. Graphics Processing Chain:\n";
    
    GraphicsProcessingChain graphicsChain({"Circle", "Rectangle", "Square"});
    
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
    
    auto restrictedValidation = std::make_unique<ValidationHandler>();
    restrictedValidation->setAllowedShapes({"Circle", "Rectangle"});
    auto restrictedLogging = std::make_unique<LoggingHandler>();
    auto restrictedDraw = std::make_unique<DrawHandler>();
    
    restrictedLogging->setSuccessor(std::move(restrictedDraw));
    restrictedValidation->setSuccessor(std::move(restrictedLogging));
    
    // Valid request
    std::cout << "Valid request (Circle is allowed):\n";
    ShapeOperationRequest validRequest(ShapeOperationRequest::OperationType::DRAW, "Circle", 3.0, 3.0, 0.0, "blue");
    restrictedValidation->handleRequest(validRequest);

    // Invalid request
    std::cout << "\nInvalid request (Hexagon is not allowed):\n";
    ShapeOperationRequest invalidRequest(ShapeOperationRequest::OperationType::DRAW, "Hexagon", 3.0, 3.0, 0.0, "blue");
    restrictedValidation->handleRequest(invalidRequest);
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
        // Create a new chain for each request as the original is consumed
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
    std::cout << "13. Chain of Responsibility Pattern Benefits:\n";
    
    std::cout << "- Loose coupling between sender and receiver\n";
    std::cout << "- Dynamic configuration of processing pipelines\n";
    std::cout << "- Multiple objects can handle the same request\n";
    std::cout << "- Requests can be handled without specifying the exact handler\n";
    std::cout << "- Easy to add new handlers without changing existing code\n";
    std::cout << "- Promotes single responsibility principle\n";
    std::cout << "- Supports open/closed principle\n";
    std::cout << std::endl;
    
    // Test 14: Complex Operation Chain
    std::cout << "14. Complex Operation Chain (Full Feature Chain):\n";
    
    auto fullFeatureChain = ShapeHandlerChainBuilder()
                               .addValidationHandler({"Circle", "Rectangle", "Triangle", "Square"})
                               .addLoggingHandler()
                               .addColorModificationHandler()
                               .addTransformationHandler()
                               .addStylingHandler()
                               .addAreaCalculationHandler()
                               .addDrawHandler()
                               .buildChain();
    
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
    
    // Test 16: Chain Flexibility - Adding Handlers
    std::cout << "16. Chain Flexibility - Adding/Removing Handlers:\n";
    
    // Start with a basic chain
    auto basicDrawChain = std::make_unique<DrawHandler>();
    std::cout << "Initial basic chain (just draw):\n";
    ShapeOperationRequest flexRequest1(ShapeOperationRequest::OperationType::DRAW, "Circle", 3.0, 0.0, 0.0, "flex_red");
    basicDrawChain->handleRequest(flexRequest1);
    std::cout << std::endl;
    
    // Enhance chain by adding validation and logging
    auto validationStep = std::make_unique<ValidationHandler>();
    auto loggingStep = std::make_unique<LoggingHandler>();
    
    loggingStep->setSuccessor(std::move(basicDrawChain));
    validationStep->setSuccessor(std::move(loggingStep));
    
    std::cout << "Enhanced chain (validation -> logging -> draw):\n";
    ShapeOperationRequest flexRequest2(ShapeOperationRequest::OperationType::DRAW, "Square", 4.0, 0.0, 0.0, "flex_blue");
    validationStep->handleRequest(flexRequest2);
    std::cout << std::endl;
    
    // Test 17: Advanced Chain Usage - Custom Chain Creation
    std::cout << "17. Advanced Chain Usage - Custom Chain Creation:\n";
    
    // Create a chain for processing animation effects
    auto animationProcessingChain = ShapeHandlerChainBuilder()
                                       .addValidationHandler()
                                       .addLoggingHandler()
                                       .addTransformationHandler()
                                       .addDrawHandler()
                                       .buildChain();
    
    ShapeOperationRequest animationEffect(ShapeOperationRequest::OperationType::ROTATE, "AnimatedShape", 45.0, 0.0, 0.0, "animation_color");
    std::cout << "Processing animation effect:\n";
    animationProcessingChain->handleRequest(animationEffect);
    std::cout << std::endl;
    
    // Test 18: Chain Termination Scenarios
    std::cout << "18. Chain Termination Scenarios:\n";
    
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
    
    // Test 19: Specialized Processing Chains
    std::cout << "19. Specialized Processing Chains:\n";
    
    std::cout << "Graphics chain: Handles rendering and visual operations\n";
    std::cout << "Animation chain: Focuses on movement and transformation operations\n";
    std::cout << "Style chain: Manages appearance and styling operations\n";
    std::cout << std::endl;
    
    // Test 20: Performance Testing
    std::cout << "20. Performance Test - Creating and Processing Many Operations:\n";
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Create many operations and process them through a chain
    for (int i = 0; i < 1000; ++i) {
        auto testChain = ShapeHandlerChainBuilder()
                            .addValidationHandler()
                            .addLoggingHandler()
                            .addDrawHandler()
                            .buildChain();
        
        ShapeOperationRequest testReq(ShapeOperationRequest::OperationType::DRAW, 
                                     "PerformanceTestShape" + std::to_string(i % 5), 
                                     1.0 + (i % 3), 0.0, 0.0, "perftest_color");
        testChain->handleRequest(testReq);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Processed 1000 operations through chains in: " << duration.count() << " microseconds\n";
    std::cout << "Average time per operation: " << (duration.count() / 1000.0) << " microseconds\n";
    std::cout << std::endl;
    
    // Test 21: Chain Pattern Benefits Summary
    std::cout << "21. Chain Pattern Benefits Summary:\n";
    std::cout << "- Decouples request senders from receivers\n";
    std::cout << "- Supports multiple receivers for the same request\n";
    std::cout << "- Enables dynamic request handling\n";
    std::cout << "- Provides flexibility in assigning responsibilities\n";
    std::cout << "- Allows for complex processing pipelines\n";
    std::cout << std::endl;
    
    std::cout << "Chain of Responsibility pattern demo completed successfully!\n";
    std::cout << "\nKey Benefits of Chain of Responsibility Pattern:\n";
    std::cout << "- Decouples senders and receivers\n";
    std::cout << "- Simplifies object interconnections\n";
    std::cout << "- Adds flexibility to distributed processing\n";
    std::cout << "- Enables dynamic addition and removal of request handlers\n";
    std::cout << "- Allows multiple handlers to process the same request\n";
    std::cout << "- Supports the principle of least knowledge\n";
    std::cout << "- Promotes single responsibility: each handler has one reason to change\n";
    std::cout << "- Enables flexible processing workflows\n";
    
    return 0;
}