#include "../include/abstract_factory_shapes.h"
#include <iostream>
#include <memory>
#include <vector>
#include <chrono>

int main() {
    std::cout << "=== Abstract Factory Pattern Demo: Themed Shape Systems ===\n\n";
    
    // Test 1: Basic Abstract Factory Usage
    std::cout << "1. Basic Abstract Factory Usage:\n";
    
    auto basicFactory = std::make_unique<BasicThemeFactory>();
    auto advancedFactory = std::make_unique<AdvancedThemeFactory>();
    
    // Create shapes using basic factory
    auto basicCircle = basicFactory->createCircle(5.0, "blue");
    auto basicRect = basicFactory->createRectangle(10.0, 8.0, "red");
    
    std::cout << "Basic Factory Shapes:\n";
    basicCircle->draw();
    basicCircle->printInfo();
    basicRect->draw();
    basicRect->printInfo();
    
    // Create shapes using advanced factory
    auto advancedCircle = advancedFactory->createCircle(5.0, "blue");
    auto advancedRect = advancedFactory->createRectangle(10.0, 8.0, "red");
    
    std::cout << "\nAdvanced Factory Shapes:\n";
    advancedCircle->draw();
    advancedCircle->printInfo();
    advancedRect->draw();
    advancedRect->printInfo();
    std::cout << std::endl;
    
    // Test 2: Creating Complete Systems with Different Factories
    std::cout << "2. Complete Shape Systems with Different Themes:\n";
    
    std::vector<std::unique_ptr<ShapeAbstractFactory>> factories;
    factories.push_back(std::make_unique<BasicThemeFactory>());
    factories.push_back(std::make_unique<AdvancedThemeFactory>());
    factories.push_back(std::make_unique<ModernThemeFactory>());
    factories.push_back(std::make_unique<VintageThemeFactory>());

    std::vector<std::string> factoryNames = {"Basic", "Advanced", "Modern", "Vintage"};

    for (size_t i = 0; i < factories.size(); ++i) {
        std::cout << factoryNames[i] << " Factory System:\n";

        auto circle = factories[i]->createCircle(4.0, "custom_color");
        auto renderer = factories[i]->createRenderer("high");
        auto animator = factories[i]->createAnimator(1.5);

        renderer->render(*circle);
        animator->animate(*circle);
        std::cout << "Area: " << circle->getArea() << std::endl;
        std::cout << std::endl;
    }
    
    // Test 3: Factory Selector/Manager Usage
    std::cout << "3. Factory Selector Usage:\n";
    
    for (const auto& theme : AbstractFactorySelector::getAvailableThemes()) {
        auto factory = AbstractFactorySelector::getFactory(theme);
        std::unique_ptr<Shape> shape;
        std::unique_ptr<ShapeRenderer> renderer;
        std::unique_ptr<ShapeAnimator> animator;
        factory->createShapeSystem(6.0, "circle", shape, renderer, animator, "themed_color");

        std::cout << theme << " theme system:\n";
        renderer->render(*shape);
        animator->animate(*shape);
        std::cout << "Area: " << shape->getArea() << std::endl;
        std::cout << std::endl;
    }
    
    // Test 4: Client Usage with Different Factories
    std::cout << "4. Client Usage with Different Factories:\n";
    
    ShapeSystemClient client(std::make_unique<BasicThemeFactory>());
    
    std::cout << "Using Basic Theme:\n";
    client.createAndDisplayShape("circle", 7.0, "cyan");
    client.createAndDisplayShape("rectangle", 9.0, "magenta");
    
    // Change factory at runtime
    client.setFactory(std::make_unique<ModernThemeFactory>());
    std::cout << "Switched to Modern Theme:\n";
    client.createAndDisplayShape("circle", 7.0, "silver");
    client.createAndDisplayShape("triangle", 9.0, "gold");
    
    // Switch again
    client.setFactory(std::make_unique<VintageThemeFactory>());
    std::cout << "Switched to Vintage Theme:\n";
    client.createAndDisplayShape("circle", 7.0, "brown");
    client.createAndDisplayShape("rectangle", 9.0, "beige");
    std::cout << std::endl;
    
    // Test 5: Complete System Creation
    std::cout << "5. Complete System Creation:\n";
    
    std::vector<std::unique_ptr<ShapeSystemClient>> clients;
    clients.push_back(std::make_unique<ShapeSystemClient>(std::make_unique<BasicThemeFactory>()));
    clients.push_back(std::make_unique<ShapeSystemClient>(std::make_unique<AdvancedThemeFactory>()));
    clients.push_back(std::make_unique<ShapeSystemClient>(std::make_unique<ModernThemeFactory>()));
    clients.push_back(std::make_unique<ShapeSystemClient>(std::make_unique<VintageThemeFactory>()));
    
    std::vector<std::string> shapes = {"circle", "rectangle", "triangle"};
    
    for (size_t i = 0; i < clients.size(); ++i) {
        for (const auto& shapeType : shapes) {
            std::cout << clients[i]->getFactoryType() << " factory creating " << shapeType << ":\n";
            clients[i]->createCompleteShapeSystem(5.0, shapeType);
        }
    }
    
    // Test 6: Product Family Consistency Test
    std::cout << "6. Product Family Consistency Test:\n";
    
    auto testFactoryConsistency = [](const ShapeAbstractFactory& factory, const std::string& factoryName) {
        std::cout << factoryName << " Factory Product Family:\n";
        
        auto circle = factory.createCircle(3.0);
        auto rect = factory.createRectangle(6.0, 4.0);
        auto renderer = factory.createRenderer();
        auto animator = factory.createAnimator();
        
        std::cout << "  Circle: " << circle->getType() << std::endl;
        std::cout << "  Rectangle: " << rect->getType() << std::endl;
        std::cout << "  Renderer: " << renderer->getRendererType() << std::endl;
        std::cout << "  Animator: " << animator->getAnimationType() << std::endl;
        std::cout << std::endl;
    };
    
    testFactoryConsistency(*std::make_unique<BasicThemeFactory>(), "Basic");
    testFactoryConsistency(*std::make_unique<AdvancedThemeFactory>(), "Advanced");
    testFactoryConsistency(*std::make_unique<ModernThemeFactory>(), "Modern");
    testFactoryConsistency(*std::make_unique<VintageThemeFactory>(), "Vintage");
    
    // Test 7: Performance and Memory Efficiency
    std::cout << "7. Performance Test - Creating Multiple Systems:\n";
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 100; ++i) {
        auto factory = AbstractFactorySelector::getFactory("advanced");
        std::unique_ptr<Shape> shape;
        std::unique_ptr<ShapeRenderer> renderer;
        std::unique_ptr<ShapeAnimator> animator;
        factory->createShapeSystem(2.0 + (i % 3), "circle", shape, renderer, animator);
        // Shapes are automatically cleaned up by unique_ptr
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Created 100 advanced theme systems in: " << duration.count() << " microseconds" << std::endl;
    std::cout << std::endl;
    
    // Test 8: Real-world Scenario - UI Theme Factory
    std::cout << "8. Real-world Scenario - UI Theme Factory:\n";
    
    struct UIComponent {
        std::string name;
        std::string type;
        double size;
    };
    
    std::vector<UIComponent> components = {
        {"Button", "circle", 20.0},
        {"Panel", "rectangle", 100.0},
        {"Icon", "triangle", 15.0}
    };
    
    for (const auto& theme : {"modern", "vintage"}) {
        std::cout << "\n" << theme << " UI Theme:\n";
        auto factory = AbstractFactorySelector::getFactory(theme);
        
        for (const auto& comp : components) {
            std::unique_ptr<Shape> shape;
            std::unique_ptr<ShapeRenderer> renderer;
            std::unique_ptr<ShapeAnimator> animator;
            factory->createShapeSystem(comp.size, comp.type, shape, renderer, animator);
            std::cout << "  " << comp.name << " as " << comp.type << ": ";
            renderer->render(*shape);
            std::cout << "  Area: " << shape->getArea() << std::endl;
        }
    }
    std::cout << std::endl;
    
    // Test 9: Factory Method vs Abstract Factory Comparison
    std::cout << "9. Factory Method vs Abstract Factory Comparison:\n";
    
    std::cout << "\nFactory Method (creates single type):\n";
    auto basicCircleFactory = std::make_unique<BasicThemeFactory>();
    auto singleCircle = basicCircleFactory->createCircle(5.0, "comparison_color");
    singleCircle->draw();
    std::cout << "Area: " << singleCircle->getArea() << std::endl;
    
    std::cout << "\nAbstract Factory (creates family of related types):\n";
    std::unique_ptr<Shape> shape;
    std::unique_ptr<ShapeRenderer> renderer;
    std::unique_ptr<ShapeAnimator> animator;
    basicCircleFactory->createShapeSystem(5.0, "circle", shape, renderer, animator, "comparison_color");
    renderer->render(*shape);
    animator->animate(*shape);
    std::cout << "Area: " << shape->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 10: Extensibility Test - Adding New Theme
    std::cout << "10. Extensibility Test - Demonstrating New Theme Addition:\n";
    std::cout << "New themes can be added by implementing ShapeAbstractFactory\n";
    std::cout << "without changing existing client code\n";
    std::cout << "Available themes: ";
    for (const auto& theme : AbstractFactorySelector::getAvailableThemes()) {
        std::cout << theme << " ";
    }
    std::cout << std::endl << std::endl;
    
    std::cout << "Abstract Factory pattern demo completed successfully!\n";
    std::cout << "\nKey Benefits of Abstract Factory Pattern:\n";
    std::cout << "- Creates families of related objects without specifying their concrete classes\n";
    std::cout << "- Ensures compatibility between products in a family\n";
    std::cout << "- Promotes consistency across product families\n";
    std::cout << "- Follows the Dependency Inversion Principle\n";
    std::cout << "- Supports Open/Closed Principle (open for extension, closed for modification)\n";
    std::cout << "- Allows switching between product families at runtime\n";
    
    return 0;
}