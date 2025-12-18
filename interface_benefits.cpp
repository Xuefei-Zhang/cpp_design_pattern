#include <iostream>
#include <vector>
#include <memory>
#include <string>

// Interface defining renderer contract
class IRenderer {
public:
    virtual ~IRenderer() = default;
    virtual void render(const std::string& obj) = 0;
    virtual void setup() = 0;
    virtual void cleanup() = 0;
};

// Interface defining drawable object contract  
class IDrawable {
public:
    virtual ~IDrawable() = default;
    // Pure virtual function - must be implemented by derived classes
    virtual void draw() const = 0;
    // Virtual function with default implementation
    virtual void rotate(double angle) { 
        std::cout << "Rotating object by " << angle << " degrees" << std::endl; 
    }
    virtual std::string getName() const = 0;
};

// Concrete implementations of drawable objects
class Circle : public IDrawable {
public:
    void draw() const override {
        std::cout << "Drawing a Circle" << std::endl;
    }
    
    std::string getName() const override {
        return "Circle";
    }
};

class Square : public IDrawable {
public:
    void draw() const override {
        std::cout << "Drawing a Square" << std::endl;
    }
    
    std::string getName() const override {
        return "Square";
    }
};

class Triangle : public IDrawable {
public:
    void draw() const override {
        std::cout << "Drawing a Triangle" << std::endl;
    }
    
    std::string getName() const override {
        return "Triangle";
    }
};

// Renderer implementations
class ConsoleRenderer : public IRenderer {
public:
    void render(const std::string& obj) override {
        std::cout << "[CONSOLE] Rendering: " << obj << std::endl;
    }
    
    void setup() override {
        std::cout << "[CONSOLE] Setting up renderer..." << std::endl;
    }
    
    void cleanup() override {
        std::cout << "[CONSOLE] Cleaning up renderer..." << std::endl;
    }
};

class OpenGLRenderer : public IRenderer {
public:
    void render(const std::string& obj) override {
        std::cout << "[OPENGL] Rendering: " << obj << std::endl;
    }
    
    void setup() override {
        std::cout << "[OPENGL] Initializing OpenGL context..." << std::endl;
    }
    
    void cleanup() override {
        std::cout << "[OPENGL] Destroying OpenGL context..." << std::endl;
    }
};

// Client code that works with the interfaces
class GraphicsEngine {
private:
    std::vector<std::unique_ptr<IDrawable>> objects;
    std::unique_ptr<IRenderer> renderer;

public:
    GraphicsEngine(std::unique_ptr<IRenderer> rend) : renderer(std::move(rend)) {
        renderer->setup();
    }
    
    ~GraphicsEngine() {
        renderer->cleanup();
    }
    
    void addObject(std::unique_ptr<IDrawable> obj) {
        objects.push_back(std::move(obj));
    }
    
    void renderAll() {
        for (const auto& obj : objects) {
            renderer->render(obj->getName());
            obj->draw();
        }
    }
};

int main() {
    std::cout << "=== Demo 1: Using Console Renderer ===" << std::endl;
    {
        auto engine = std::make_unique<GraphicsEngine>(std::make_unique<ConsoleRenderer>());
        
        engine->addObject(std::make_unique<Circle>());
        engine->addObject(std::make_unique<Square>());
        engine->addObject(std::make_unique<Triangle>());
        
        engine->renderAll();
    } // Engine destructor called here, cleaning up console renderer
    
    std::cout << "\n=== Demo 2: Swapping to OpenGL Renderer ===" << std::endl;
    {
        auto engine = std::make_unique<GraphicsEngine>(std::make_unique<OpenGLRenderer>());
        
        engine->addObject(std::make_unique<Circle>());
        engine->addObject(std::make_unique<Square>());
        
        engine->renderAll();
    } // Engine destructor called here, cleaning up OpenGL renderer
    
    std::cout << "\n=== Demo 3: Polymorphism with interfaces ===" << std::endl;
    std::vector<std::unique_ptr<IDrawable>> mixed_objects;
    mixed_objects.push_back(std::make_unique<Circle>());
    mixed_objects.push_back(std::make_unique<Square>());
    mixed_objects.push_back(std::make_unique<Triangle>());
    
    std::cout << "Iterating through mixed objects:" << std::endl;
    for (const auto& obj : mixed_objects) {
        // Works with any IDrawable regardless of actual type
        obj->rotate(45.0);
        obj->draw();
    }
    
    return 0;
}