#ifndef GRAPHICS_FACADE_H
#define GRAPHICS_FACADE_H

#include "facade_subsystems.h"

// Facade: Graphics System Facade - simplifies complex graphics operations
class GraphicsSystemFacade {
private:
    // Subsystem components
    RenderingEngine renderingEngine_;
    PhysicsSimulator physicsSimulator_;
    AnimationController animationController_;
    SceneManager sceneManager_;
    ResourceManager resourceManager_;
    GeometryCalculator geometryCalculator_;
    InputHandler inputHandler_;

public:
    // Simple initialization method
    void initializeGraphicsSystem() {
        std::cout << "[GRAPHICS FACADE] Initializing complex graphics system...\n";
        renderingEngine_.initialize();
        sceneManager_.createScene("Main Scene");
        inputHandler_.setupMouseInput();
        inputHandler_.setupKeyboardInput();
        std::cout << "[GRAPHICS FACADE] Graphics system initialized successfully!\n\n";
    }
    
    // High-level method to draw a shape with all associated calculations and rendering
    void drawShape(const std::string& shapeType, double size, const std::string& color, 
                   double x = 0, double y = 0) {
        std::cout << "[GRAPHICS FACADE] Drawing shape: " << shapeType << " with size " 
                  << size << " and color " << color << std::endl;
        
        // Calculate geometric properties
        double area = geometryCalculator_.calculateArea(shapeType, size);
        auto bounds = geometryCalculator_.calculateBoundingBox(shapeType, size);
        
        // Manage scene
        sceneManager_.addObject("Temp Object", shapeType, x, y);
        
        // Render the shape
        renderingEngine_.renderShape(shapeType, size, color);
        
        std::cout << "[GRAPHICS FACADE] Area: " << area << ", Bounds: " 
                  << bounds.first << "x" << bounds.second << std::endl;
    }
    
    // High-level method to create an animated physical shape
    void createAnimatedPhysicalShape(const std::string& shapeType, double size, 
                                   const std::string& color, double x, double y, 
                                   double duration = 1.0) {
        std::cout << "[GRAPHICS FACADE] Creating animated physical " << shapeType << std::endl;
        
        // Calculate area and bounds
        double area = geometryCalculator_.calculateArea(shapeType, size);
        auto bounds = geometryCalculator_.calculateBoundingBox(shapeType, size);
        
        // Add to scene
        sceneManager_.addObject("Animated " + shapeType, shapeType, x, y);
        
        // Apply physics simulation
        double velX = 0, velY = 0;
        physicsSimulator_.applyGravity(shapeType, x, y);
        physicsSimulator_.calculateVelocity(1.0, 9.8, velY);
        
        // Apply animations
        animationController_.animatePosition(shapeType, x, y, x + 10, y + 10, duration);
        animationController_.animateScale(shapeType, 1.0, 1.5, duration);
        animationController_.animateColor(shapeType, color, "light" + color, duration);
        
        // Perform the actual rendering
        renderingEngine_.renderShape(shapeType, size, color);
        
        std::cout << "[GRAPHICS FACADE] Physical animation complete for " << shapeType 
                  << " with area: " << area << std::endl;
    }
    
    // High-level method to set up a complete scene with lighting, physics, and animation
    void createComplexScene(const std::string& sceneName, 
                           const std::vector<std::string>& resourceList) {
        std::cout << "[GRAPHICS FACADE] Creating complex scene: " << sceneName << std::endl;
        
        // Load necessary resources
        resourceManager_.preloadResources(resourceList);
        
        // Set up scene
        sceneManager_.createScene(sceneName);
        
        // Configure rendering
        renderingEngine_.setViewport(0, 0, 800, 600);
        renderingEngine_.clearScreen();
        
        // Enable physics and animations
        physicsSimulator_.enablePhysics();
        animationController_.enableAnimations();
        
        std::cout << "[GRAPHICS FACADE] Complex scene created with " 
                  << resourceList.size() << " resources\n";
    }
    
    // High-level method to handle user input and update graphics
    void processUserInteraction(int mouseX, int mouseY, char keyPressed) {
        std::cout << "[GRAPHICS FACADE] Processing user interaction...\n";
        
        // Handle input
        inputHandler_.handleMouseClick(mouseX, mouseY);
        inputHandler_.handleKeyPress(keyPressed);
        
        // Update scene based on input
        // In a real system, we might add a shape at the clicked position
        renderingEngine_.renderShape("circle", 1.0, "highlight");
        
        // Process input events
        inputHandler_.processInputEvents();
    }
    
    // High-level rendering method
    void renderFrame() {
        std::cout << "[GRAPHICS FACADE] Rendering complete frame...\n";
        
        // Clear screen
        renderingEngine_.clearScreen();
        
        // Render scene
        sceneManager_.renderScene();
        
        // Swap buffers for display
        renderingEngine_.swapBuffers();
        
        std::cout << "[GRAPHICS FACADE] Frame rendering complete\n";
    }
    
    // High-level cleanup method
    void shutdownGraphicsSystem() {
        std::cout << "[GRAPHICS FACADE] Shutting down graphics system...\n";
        
        // Clear scene
        sceneManager_.clearScene();
        
        // Release resources
        resourceManager_.releaseUnusedResources();
        
        // Shutdown rendering engine
        renderingEngine_.shutdown();
        
        std::cout << "[GRAPHICS FACADE] Graphics system shutdown complete\n";
    }
    
    // Utility methods to access subsystem functionality through the facade
    double calculateShapeArea(const std::string& shapeType, double param1, double param2 = 0.0) {
        return geometryCalculator_.calculateArea(shapeType, param1, param2);
    }
    
    void enablePhysics() { physicsSimulator_.enablePhysics(); }
    void disablePhysics() { physicsSimulator_.disablePhysics(); }
    bool isPhysicsEnabled() const { return physicsSimulator_.isPhysicsEnabled(); }
    
    void enableAnimations() { animationController_.enableAnimations(); }
    void disableAnimations() { animationController_.disableAnimations(); }
    bool isAnimationEnabled() const { return animationController_.isAnimationEnabled(); }
    
    int getSceneObjectCount() const { return sceneManager_.getObjectCount(); }
    void clearScene() { sceneManager_.clearScene(); }
};

// Specialized Facades for specific purposes

// Game Development Facade
class GameDevelopmentFacade {
private:
    GraphicsSystemFacade graphicsFacade_;
    
public:
    void initializeGameEngine() {
        std::cout << "[GAME DEVELOPMENT FACADE] Initializing game engine...\n";
        graphicsFacade_.initializeGraphicsSystem();
        std::cout << "[GAME DEVELOPMENT FACADE] Game engine initialized\n\n";
    }
    
    void createGameEntity(const std::string& entityType, double size, 
                         const std::string& color, double x, double y) {
        std::cout << "[GAME DEVELOPMENT FACADE] Creating game entity: " << entityType << std::endl;
        
        // Use the graphics facade to create a physical animated shape
        graphicsFacade_.createAnimatedPhysicalShape(entityType, size, color, x, y);
        
        std::cout << "[GAME DEVELOPMENT FACADE] Game entity created\n";
    }
    
    void runGameLoop(int frames) {
        std::cout << "[GAME DEVELOPMENT FACADE] Running game loop for " << frames << " frames\n";
        
        for (int i = 0; i < frames; ++i) {
            std::cout << "--- Frame " << (i + 1) << " ---" << std::endl;
            
            // Process input (could be called elsewhere in real implementation)
            // graphicsFacade_.processUserInteraction(100, 100, 'A');
            
            // Render frame
            graphicsFacade_.renderFrame();
            
            std::cout << "-------------------\n";
        }
    }
    
    void shutdownGameEngine() {
        std::cout << "[GAME DEVELOPMENT FACADE] Shutting down game engine...\n";
        graphicsFacade_.shutdownGraphicsSystem();
        std::cout << "[GAME DEVELOPMENT FACADE] Game engine shutdown complete\n";
    }
};

// UI Development Facade
class UIDevelopmentFacade {
private:
    GraphicsSystemFacade graphicsFacade_;
    
public:
    void initializeUIFramework() {
        std::cout << "[UI DEVELOPMENT FACADE] Initializing UI framework...\n";
        graphicsFacade_.initializeGraphicsSystem();
        graphicsFacade_.disablePhysics();  // UI usually doesn't need physics
        std::cout << "[UI DEVELOPMENT FACADE] UI framework initialized\n\n";
    }
    
    void createUIButton(const std::string& text, double x, double y, 
                       double width, double height, const std::string& color = "blue") {
        std::cout << "[UI DEVELOPMENT FACADE] Creating button: " << text << std::endl;
        
        // Create a rectangle for the button
        graphicsFacade_.drawShape("rectangle", width, color, x, y);
        
        std::cout << "[UI DEVELOPMENT FACADE] Button created at (" << x << "," << y << ")\n";
    }
    
    void createUIText(const std::string& text, double x, double y, 
                     const std::string& color = "black") {
        std::cout << "[UI DEVELOPMENT FACADE] Creating text: " << text << std::endl;
        
        // In a real system, this would render text
        // For demonstration, we'll just render a small circle to represent text
        graphicsFacade_.drawShape("circle", 0.5, color, x, y);
        
        std::cout << "[UI DEVELOPMENT FACADE] Text rendered at (" << x << "," << y << ")\n";
    }
    
    void renderUI() {
        std::cout << "[UI DEVELOPMENT FACADE] Rendering UI...\n";
        graphicsFacade_.renderFrame();
        std::cout << "[UI DEVELOPMENT FACADE] UI rendering complete\n";
    }
    
    void shutdownUIFramework() {
        std::cout << "[UI DEVELOPMENT FACADE] Shutting down UI framework...\n";
        graphicsFacade_.shutdownGraphicsSystem();
        std::cout << "[UI DEVELOPMENT FACADE] UI framework shutdown complete\n";
    }
};

// Animation Production Facade
class AnimationProductionFacade {
private:
    GraphicsSystemFacade graphicsFacade_;
    
public:
    void initializeAnimationStudio() {
        std::cout << "[ANIMATION PRODUCTION FACADE] Initializing animation studio...\n";
        graphicsFacade_.initializeGraphicsSystem();
        graphicsFacade_.enableAnimations();
        graphicsFacade_.enablePhysics();
        std::cout << "[ANIMATION PRODUCTION FACADE] Animation studio initialized\n\n";
    }
    
    void createAnimationScene(const std::string& sceneName, 
                             const std::vector<std::string>& objects) {
        std::cout << "[ANIMATION PRODUCTION FACADE] Creating animation scene: " << sceneName << std::endl;
        
        // Create the complex scene with resources
        std::vector<std::string> resources = {"textures", "models", "materials"};
        graphicsFacade_.createComplexScene(sceneName, resources);
        
        // Add objects to the scene
        double yPos = 50.0;
        for (const auto& obj : objects) {
            graphicsFacade_.createAnimatedPhysicalShape(obj, 3.0, "multicolor", 100.0, yPos);
            yPos += 20.0;  // space out objects
        }
        
        std::cout << "[ANIMATION PRODUCTION FACADE] Animation scene created with " 
                  << objects.size() << " objects\n";
    }
    
    void renderAnimationSequence(const std::string& sequenceName, int frameCount) {
        std::cout << "[ANIMATION PRODUCTION FACADE] Rendering animation sequence: " 
                  << sequenceName << " with " << frameCount << " frames\n";
        
        for (int i = 0; i < frameCount; ++i) {
            std::cout << "Rendering animation frame " << (i + 1) << "/" << frameCount << std::endl;
            graphicsFacade_.renderFrame();
        }
        
        std::cout << "[ANIMATION PRODUCTION FACADE] Animation sequence rendering complete\n";
    }
    
    void exportAnimation(const std::string& fileName) {
        std::cout << "[ANIMATION PRODUCTION FACADE] Exporting animation to: " << fileName << std::endl;
        std::cout << "[ANIMATION PRODUCTION FACADE] Export complete\n";
    }
    
    void shutdownAnimationStudio() {
        std::cout << "[ANIMATION PRODUCTION FACADE] Shutting down animation studio...\n";
        graphicsFacade_.shutdownGraphicsSystem();
        std::cout << "[ANIMATION PRODUCTION FACADE] Animation studio shutdown complete\n";
    }
};

#endif // GRAPHICS_FACADE_H