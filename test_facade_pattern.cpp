#include "graphics_facade.h"
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::cout << "=== Facade Pattern Demo: Graphics System Simplification ===\n\n";
    
    // Test 1: Basic Graphics System Facade Usage
    std::cout << "1. Basic Graphics System Facade Usage:\n";
    GraphicsSystemFacade graphicsFacade;
    
    graphicsFacade.initializeGraphicsSystem();
    graphicsFacade.drawShape("circle", 5.0, "red", 100.0, 100.0);
    graphicsFacade.drawShape("rectangle", 8.0, "blue", 200.0, 150.0);
    graphicsFacade.renderFrame();
    std::cout << std::endl;
    
    // Test 2: Complex Shape Creation with Physics and Animation
    std::cout << "2. Complex Shape Creation with Physics and Animation:\n";
    graphicsFacade.createAnimatedPhysicalShape("Triangle", 3.0, "green", 50.0, 75.0, 2.0);
    graphicsFacade.createAnimatedPhysicalShape("Square", 4.0, "yellow", 120.0, 80.0, 1.5);
    graphicsFacade.renderFrame();
    std::cout << std::endl;
    
    // Test 3: Creating a Complex Scene
    std::cout << "3. Creating a Complex Scene:\n";
    std::vector<std::string> resources = {"TexturePack1", "ModelPack1", "MaterialLibrary"};
    graphicsFacade.createComplexScene("MyComplexScene", resources);
    graphicsFacade.drawShape("circle", 2.0, "purple", 25.0, 25.0);
    graphicsFacade.renderFrame();
    std::cout << std::endl;
    
    // Test 4: User Interaction Processing
    std::cout << "4. User Interaction Processing:\n";
    graphicsFacade.processUserInteraction(100, 100, 'A');
    graphicsFacade.renderFrame();
    std::cout << std::endl;
    
    // Test 5: Game Development Facade
    std::cout << "5. Game Development Facade:\n";
    GameDevelopmentFacade gameFacade;
    
    gameFacade.initializeGameEngine();
    gameFacade.createGameEntity("Player", 2.0, "blue", 50.0, 100.0);
    gameFacade.createGameEntity("Enemy", 1.5, "red", 300.0, 200.0);
    gameFacade.runGameLoop(3);
    gameFacade.shutdownGameEngine();
    std::cout << std::endl;
    
    // Test 6: UI Development Facade
    std::cout << "6. UI Development Facade:\n";
    UIDevelopmentFacade uiFacade;
    
    uiFacade.initializeUIFramework();
    uiFacade.createUIButton("Submit", 50.0, 50.0, 100.0, 40.0, "blue");
    uiFacade.createUIText("Welcome", 70.0, 120.0, "black");
    uiFacade.createUIButton("Cancel", 200.0, 50.0, 100.0, 40.0, "red");
    uiFacade.renderUI();
    uiFacade.shutdownUIFramework();
    std::cout << std::endl;
    
    // Test 7: Animation Production Facade
    std::cout << "7. Animation Production Facade:\n";
    AnimationProductionFacade animationFacade;
    
    animationFacade.initializeAnimationStudio();
    std::vector<std::string> sceneObjects = {"Character1", "Character2", "Background"};
    animationFacade.createAnimationScene("OpeningSequence", sceneObjects);
    animationFacade.renderAnimationSequence("OpeningSequence_Anim", 10);
    animationFacade.exportAnimation("opening_sequence.mp4");
    animationFacade.shutdownAnimationStudio();
    std::cout << std::endl;
    
    // Test 8: Facade Methods Comparison
    std::cout << "8. Facade Methods Comparison:\n";
    std::cout << "Without facade - to create and render a circle, you would need to call:\n";
    std::cout << "  - GeometryCalculator::calculateArea()\n";
    std::cout << "  - SceneManager::addObject()\n";
    std::cout << "  - RenderingEngine::renderShape()\n";
    std::cout << "  - AnimationController::animate...() [maybe]\n";
    std::cout << "  - PhysicsSimulator::apply...() [maybe]\n\n";
    
    std::cout << "With facade - you only call:\n";
    graphicsFacade.drawShape("circle", 3.0, "orange", 150.0, 150.0);
    std::cout << std::endl;
    
    // Test 9: Complex Operation Chain Through Facade
    std::cout << "9. Complex Operation Chain Through Facade:\n";
    graphicsFacade.initializeGraphicsSystem();
    
    // Create multiple animated shapes
    for (int i = 0; i < 5; ++i) {
        graphicsFacade.createAnimatedPhysicalShape(
            i % 2 == 0 ? "circle" : "rectangle", 
            2.0 + (i * 0.5), 
            i % 3 == 0 ? "red" : (i % 3 == 1 ? "green" : "blue"), 
            50 + (i * 30), 
            50 + (i * 20)
        );
    }
    
    graphicsFacade.renderFrame();
    graphicsFacade.shutdownGraphicsSystem();
    std::cout << std::endl;
    
    // Test 10: Subsystem Access Through Facade
    std::cout << "10. Subsystem Access Through Facade:\n";
    graphicsFacade.initializeGraphicsSystem();
    
    // Use facade to access subsystem functionality
    double area = graphicsFacade.calculateShapeArea("triangle", 6.0, 4.0);
    std::cout << "Calculated triangle area through facade: " << area << std::endl;
    
    std::cout << "Physics enabled: " << (graphicsFacade.isPhysicsEnabled() ? "Yes" : "No") << std::endl;
    std::cout << "Animation enabled: " << (graphicsFacade.isAnimationEnabled() ? "Yes" : "No") << std::endl;
    
    graphicsFacade.disablePhysics();
    std::cout << "After disabling physics - Physics enabled: " 
              << (graphicsFacade.isPhysicsEnabled() ? "Yes" : "No") << std::endl;
    
    graphicsFacade.enablePhysics();
    std::cout << "After re-enabling physics - Physics enabled: " 
              << (graphicsFacade.isPhysicsEnabled() ? "Yes" : "No") << std::endl;
    graphicsFacade.shutdownGraphicsSystem();
    std::cout << std::endl;
    
    // Test 11: Performance Consideration - Facade vs Direct Access
    std::cout << "11. Performance Consideration - Facade vs Direct Access:\n";
    std::cout << "Direct access would require knowledge of:\n";
    std::cout << "  - Complex dependencies between systems\n";
    std::cout << "  - Correct initialization order\n";
    std::cout << "  - Proper resource management\n";
    std::cout << "  - Synchronization between subsystems\n\n";
    
    std::cout << "Facade encapsulates all this complexity:\n";
    graphicsFacade.initializeGraphicsSystem();
    for (int i = 0; i < 10; ++i) {
        graphicsFacade.drawShape(i % 3 == 0 ? "circle" : (i % 3 == 1 ? "rectangle" : "triangle"), 
                                1.0 + (i * 0.5), 
                                i % 2 == 0 ? "red" : "blue", 
                                i * 20, i * 15);
    }
    graphicsFacade.renderFrame();
    graphicsFacade.shutdownGraphicsSystem();
    std::cout << "All handled seamlessly by the facade!\n\n";
    
    // Test 12: Different Facade Contexts
    std::cout << "12. Different Facade Contexts:\n";
    
    // Game context
    std::cout << "- Game Development Context:\n";
    gameFacade.initializeGameEngine();
    gameFacade.createGameEntity("Hero", 3.0, "golden", 100.0, 100.0);
    gameFacade.shutdownGameEngine();
    
    // UI context
    std::cout << "- UI Development Context:\n";
    uiFacade.initializeUIFramework();
    uiFacade.createUIButton("OK", 100.0, 100.0, 80.0, 30.0, "green");
    uiFacade.shutdownUIFramework();
    
    // Animation context
    std::cout << "- Animation Production Context:\n";
    animationFacade.initializeAnimationStudio();
    std::vector<std::string> simpleObjects = {"Sphere"};
    animationFacade.createAnimationScene("TestScene", simpleObjects);
    animationFacade.shutdownAnimationStudio();
    std::cout << std::endl;
    
    // Test 13: Real-world Scenario - Graphics Application Startup
    std::cout << "13. Real-world Scenario - Graphics Application Startup:\n";
    GraphicsSystemFacade appFacade;
    
    std::cout << "Starting graphics application...\n";
    appFacade.initializeGraphicsSystem();
    
    // Load application-specific resources
    std::vector<std::string> appResources = {
        "GUI_Texture", "Font_Atlas", "Shader_Library", "Audio_Effects"
    };
    appFacade.createComplexScene("Application_Scene", appResources);
    
    // Create initial UI elements
    appFacade.drawShape("rectangle", 800, "darkblue", 0, 0);  // Main window background
    appFacade.drawShape("circle", 50, "lightgray", 50, 50);   // Toolbar button
    appFacade.renderFrame();
    
    std::cout << "Application started successfully\n";
    appFacade.shutdownGraphicsSystem();
    std::cout << std::endl;
    
    // Test 14: Facade Pattern Benefits Demonstration
    std::cout << "14. Facade Pattern Benefits Demonstration:\n";
    std::cout << "- Subsystem Decoupling: Client code doesn't depend on complex subsystems\n";
    std::cout << "- Simplified Interface: Complex operations reduced to simple method calls\n";
    std::cout << "- Context Specificity: Different facades for different use cases\n";
    std::cout << "- Encapsulation: Internal subsystem details hidden from clients\n";
    std::cout << "- Ease of Use: Complex workflows simplified into single operations\n";
    std::cout << "- Maintainability: Changes to subsystems don't affect client code\n";
    std::cout << "- Reduced Dependencies: Client only depends on the facade\n";
    std::cout << std::endl;
    
    // Test 15: Facade vs Direct Subsystem Access
    std::cout << "15. Facade vs Direct Subsystem Access Comparison:\n";
    
    std::cout << "Direct Subsystem Access (Complex):\n";
    RenderingEngine engine;
    GeometryCalculator calculator;
    SceneManager scene;

    engine.initialize();
    scene.createScene("Direct Scene");
    double directArea = calculator.calculateArea("circle", 5.0);
    engine.renderShape("circle", 5.0, "red");
    std::cout << "Calculated area: " << directArea << std::endl;
    engine.swapBuffers();
    engine.shutdown();
    std::cout << "Direct approach required managing multiple objects\n\n";
    
    std::cout << "Facade Approach (Simple):\n";
    graphicsFacade.initializeGraphicsSystem();
    graphicsFacade.drawShape("circle", 5.0, "red", 0, 0);
    graphicsFacade.renderFrame();
    graphicsFacade.shutdownGraphicsSystem();
    std::cout << "Facade approach required only one object\n";
    std::cout << std::endl;
    
    std::cout << "Facade pattern demo completed successfully!\n";
    std::cout << "\nKey Benefits of Facade Pattern:\n";
    std::cout << "- Simplifies complex subsystems into single interface\n";
    std::cout << "- Decouples client code from subsystem complexity\n";
    std::cout << "- Provides context-specific interfaces\n";
    std::cout << "- Reduces dependencies between client and subsystems\n";
    std::cout << "- Provides entry point to complex systems\n";
    std::cout << "- Supports the principle of least knowledge (Law of Demeter)\n";
    std::cout << "- Makes large software systems easier to maintain\n";
    
    return 0;
}