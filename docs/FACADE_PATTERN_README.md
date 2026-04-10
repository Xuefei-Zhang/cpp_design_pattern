# Facade Pattern in C++

This project demonstrates the Facade design pattern in C++ using a graphics/shape system context for consistency with other patterns.

## Overview

The Facade pattern is a structural design pattern that provides a simplified interface to a complex subsystem. It hides the complexities of the larger system and provides a unified, higher-level interface that makes the subsystem easier to use.

## Key Concepts

### When to use Facade pattern:
- When you need to provide a simple interface to a complex subsystem
- When there are many dependencies between clients and the implementation classes
- When you need to layer your subsystems (the facade makes the dependencies clearer)
- When you want to structure a system into layers
- When you need to provide a context-specific interface for different types of clients

### Components of Facade:
- **Facade**: Knows which subsystem classes are responsible for a request, delegates client requests to appropriate subsystem objects
- **Subsystems**: Implement functionality, handle work assigned by the Facade, have no knowledge of the facade
- **Client**: Communicates with the subsystem through the Facade only

## Implementation Variations

### 1. Basic Facade
- Single facade that simplifies access to multiple subsystems
- Provides high-level methods that coordinate multiple subsystems
- Hides internal complexity from clients

### 2. Specialized Facades
- Different facades for different contexts (GameDevelopmentFacade, UIDevelopmentFacade, etc.)
- Each facade tailored to specific use case requirements
- Promotes context-specific interfaces

### 3. Facade with State Management
- Facade manages subsystem state and initialization
- Coordinates subsystems that have dependencies
- Provides lifecycle management for complex systems

### 4. Subsystem Interface Facade
- Provides access to subsystems through the facade while maintaining their individual interfaces
- Allows clients to access lower-level functionality when needed
- Balances simplicity with flexibility

## Files

- `facade_patterns.h` - All Facade pattern implementations
- `test_facade_pattern.cpp` - Comprehensive test program
- `FACADE_PATTERN_README.md` - This documentation

## Key Examples

### 1. Basic Facade Implementation
```cpp
// Subsystem components
class RenderingEngine {
public:
    void initialize() { /* ... */ }
    void renderShape(const std::string& type, double size, const std::string& color) { /* ... */ }
    void swapBuffers() { /* ... */ }
    void shutdown() { /* ... */ }
};

class GeometryCalculator {
public:
    double calculateArea(const std::string& shapeType, double param) { /* ... */ }
    double calculatePerimeter(const std::string& shapeType, double param) { /* ... */ }
};

class SceneManager {
public:
    void createScene(const std::string& name) { /* ... */ }
    void addObject(const std::string& name, const std::string& type, double x, double y) { /* ... */ }
    void renderScene() { /* ... */ }
};

// Facade class
class GraphicsFacade {
private:
    RenderingEngine renderingEngine_;
    GeometryCalculator geometryCalculator_;
    SceneManager sceneManager_;

public:
    void initialize() {
        std::cout << "Initializing complex graphics system through facade...\n";
        renderingEngine_.initialize();
        sceneManager_.createScene("Main Scene");
    }
    
    void drawShape(const std::string& shapeType, double size, const std::string& color, 
                   double x = 0, double y = 0) {
        // Coordinate multiple subsystems
        double area = geometryCalculator_.calculateArea(shapeType, size);
        sceneManager_.addObject("Temp Object", shapeType, x, y);
        renderingEngine_.renderShape(shapeType, size, color);
        
        std::cout << "Shape drawn with area: " << area << std::endl;
    }
    
    void renderFrame() {
        renderingEngine_.clearScreen();
        sceneManager_.renderScene();
        renderingEngine_.swapBuffers();
    }
    
    void shutdown() {
        std::cout << "Shutting down graphics system...\n";
        sceneManager_.clearScene();
        renderingEngine_.shutdown();
    }
};
```

### 2. Using the Facade
```cpp
// Without facade - client needs to know about all subsystems
RenderingEngine engine;
GeometryCalculator calculator;
SceneManager scene;

engine.initialize();
scene.createScene("My Scene");
double area = calculator.calculateArea("circle", 5.0);
engine.renderShape("circle", 5.0, "red");
engine.swapBuffers();
engine.shutdown();

// With facade - simple interface
GraphicsFacade graphicsFacade;
graphicsFacade.initialize();
graphicsFacade.drawShape("circle", 5.0, "red", 100.0, 100.0);
graphicsFacade.renderFrame();
graphicsFacade.shutdown();
```

## Compile and Run

```bash
g++ -std=c++14 -o test_facade_pattern test_facade_pattern.cpp
./test_facade_pattern
```

## Educational Value

This implementation demonstrates:
- How to hide complexity behind a simple interface
- Coordination between multiple subsystems through a single entry point
- How to create context-specific interfaces (Game, UI, Animation facades)
- Proper encapsulation of subsystem dependencies
- State management and lifecycle coordination
- Performance considerations of facade vs direct access
- Real-world scenarios where facades are appropriate
- The difference between Facade and Adapter patterns

## Facade vs Other Patterns

- **Adapter**: Makes interfaces compatible, Facade provides a simplified interface to a subsystem
- **Mediator**: Defines how objects interact, Facade provides a unified interface to a subsystem
- **Flyweight**: Focuses on sharing objects to save memory, Facade focuses on simplifying access
- **Abstract Factory**: Creates families of objects, Facade provides an interface to a subsystem

## Important Notes

- Facade does not prevent direct access to subsystems
- Facade can serve multiple clients with different requirements
- Facade should be designed around client needs, not subsystem capabilities
- Can improve performance by coordinating subsystems optimally
- May become a god object if not carefully designed
- Can limit flexibility compared to direct subsystem access

## Benefits

- **Simplification**: Reduces complexity for the client
- **Decoupling**: Reduces dependencies between clients and subsystems
- **Ease of Use**: Provides a simple interface to complex systems
- **Centralization**: Coordinates multiple subsystems in one place
- **Maintenance**: Changes to subsystems don't affect clients
- **Testing**: Easier to unit test complex systems
- **Layering**: Provides a clear abstraction layer