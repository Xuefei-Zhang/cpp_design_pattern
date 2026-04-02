#include "../include/factory_method_shapes.h"
#include <iostream>
#include <memory>
#include <vector>

int main() {
    std::cout << "=== Factory Method Pattern Demo: Shape Creation ===\n\n";
    
    // Test 1: Basic Factory Method usage
    std::cout << "1. Basic Factory Method Usage:\n";
    auto circleFactory = std::make_unique<CircleFactory>();
    auto rectFactory = std::make_unique<RectangleFactory>();
    auto triangleFactory = std::make_unique<TriangleFactory>();
    auto squareFactory = std::make_unique<SquareFactory>();
    
    auto circle = circleFactory->makeShape(5.0, 0.0, "red", "solid");
    circle->draw();
    circle->printInfo();
    
    auto rectangle = rectFactory->makeShape(10.0, 8.0, "blue", "dashed");
    rectangle->draw();
    rectangle->printInfo();
    
    auto triangle = triangleFactory->makeShape(6.0, 4.0, "green", "dotted");
    triangle->draw();
    triangle->printInfo();
    
    auto square = squareFactory->makeShape(7.0, 0.0, "yellow", "solid");
    square->draw();
    square->printInfo();
    std::cout << std::endl;
    
    // Test 2: Factory Manager usage
    std::cout << "2. Factory Manager Usage:\n";
    ShapeFactoryManager manager(std::make_unique<CircleFactory>());
    
    std::cout << "Current factory: " << manager.getFactoryType() << std::endl;
    auto shape1 = manager.createShape(3.0, 0.0, "purple", "dashed");
    shape1->draw();
    
    // Change factory at runtime
    manager.setFactory(std::make_unique<RectangleFactory>());
    std::cout << "Changed to factory: " << manager.getFactoryType() << std::endl;
    auto shape2 = manager.createShape(12.0, 9.0, "orange", "dotted");
    shape2->draw();
    
    // Change again
    manager.setFactory(std::make_unique<TriangleFactory>());
    std::cout << "Changed to factory: " << manager.getFactoryType() << std::endl;
    auto shape3 = manager.createShape(8.0, 6.0, "pink", "solid");
    shape3->draw();
    std::cout << std::endl;
    
    // Test 3: Themed Factories
    std::cout << "3. Themed Factory Usage:\n";
    auto modernFactory = std::make_unique<ModernShapeFactory>();
    auto vintageFactory = std::make_unique<VintageShapeFactory>();
    
    std::cout << "Modern factory defaults - Color: " << modernFactory->getDefaultColor() 
              << ", Style: " << modernFactory->getDefaultStyle() << std::endl;
    
    auto modernCircle = modernFactory->makeShape(4.0, 0.0, "default", "solid");
    modernCircle->draw();
    modernCircle->printInfo();
    
    std::cout << "Vintage factory defaults - Color: " << vintageFactory->getDefaultColor() 
              << ", Style: " << vintageFactory->getDefaultStyle() << std::endl;
    
    auto vintageRect = vintageFactory->makeShape(15.0, 10.0, "default", "solid");
    vintageRect->draw();
    vintageRect->printInfo();
    std::cout << std::endl;
    
    // Test 4: Polymorphic Factory Usage
    std::cout << "4. Polymorphic Factory Usage:\n";
    std::vector<std::unique_ptr<ShapeFactory>> factories;
    factories.push_back(std::make_unique<CircleFactory>());
    factories.push_back(std::make_unique<RectangleFactory>());
    factories.push_back(std::make_unique<TriangleFactory>());
    
    for (size_t i = 0; i < factories.size(); ++i) {
        std::cout << "Factory " << i + 1 << ": ";
        auto shape = factories[i]->makeShape(5.0 + i, 3.0 + i, 
                                             i == 0 ? "red" : (i == 1 ? "blue" : "green"),
                                             i == 0 ? "solid" : (i == 1 ? "dashed" : "dotted"));
        shape->printInfo();
    }
    std::cout << std::endl;
    
    // Test 5: Factory with Suite Creation
    std::cout << "5. Factory Suite Creation:\n";
    auto colorfulSuiteFactory = std::make_unique<ColorfulSuiteFactory>();
    
    std::cout << "Creating basic suite:\n";
    auto basicSuite = colorfulSuiteFactory->createBasicSuite();
    for (auto& shape : basicSuite) {
        shape->draw();
        std::cout << "Area: " << shape->getArea() << std::endl;
    }
    
    std::cout << "\nCreating complex suite:\n";
    auto complexSuite = colorfulSuiteFactory->createComplexSuite();
    for (auto& shape : complexSuite) {
        shape->draw();
        std::cout << "Area: " << shape->getArea() << std::endl;
    }
    std::cout << std::endl;
    
    // Test 6: Factory Method vs Direct Construction Comparison
    std::cout << "6. Factory Method vs Direct Construction:\n";
    
    // Using Factory Method
    std::cout << "Using Factory Method:\n";
    auto factoryCircle = circleFactory->makeShape(6.0, 0.0, "cyan", "solid");
    factoryCircle->draw();
    
    // Direct Construction
    std::cout << "Direct Construction:\n";
    auto directCircle = std::make_unique<Circle>(6.0, "cyan", "solid");
    directCircle->draw();
    
    std::cout << "\nBenefits of Factory Method:\n";
    std::cout << "- Encapsulates object creation logic\n";
    std::cout << "- Allows for polymorphic creation\n";
    std::cout << "- Enables runtime selection of concrete types\n";
    std::cout << "- Supports Open/Closed Principle (new factories without changing client code)\n";
    std::cout << std::endl;
    
    // Test 7: Factory Configuration Example
    std::cout << "7. Factory Configuration Example:\n";
    
    // Different configuration for different scenarios
    struct ShapeConfig {
        std::string type;
        double param1, param2;
        std::string color;
        std::string style;
    };
    
    std::vector<ShapeConfig> configs = {
        {"circle", 5.0, 0.0, "red", "solid"},
        {"rectangle", 8.0, 6.0, "blue", "dashed"},
        {"triangle", 10.0, 7.0, "green", "dotted"}
    };
    
    for (const auto& config : configs) {
        std::unique_ptr<ShapeFactory> factory;
        
        if (config.type == "circle") {
            factory = std::make_unique<CircleFactory>();
        } else if (config.type == "rectangle") {
            factory = std::make_unique<RectangleFactory>();
        } else if (config.type == "triangle") {
            factory = std::make_unique<TriangleFactory>();
        } else {
            factory = std::make_unique<CircleFactory>(); // default
        }
        
        auto shape = factory->makeShape(config.param1, config.param2, config.color, config.style);
        std::cout << "Created " << config.type << ": ";
        shape->printInfo();
    }
    std::cout << std::endl;
    
    // Test 8: Template-based Factory Method (advanced example)
    std::cout << "8. Template-Based Factory Method:\n";
    
    // Using the manager with different factories
    std::vector<std::unique_ptr<ShapeFactoryManager>> managers;
    managers.push_back(std::make_unique<ShapeFactoryManager>(std::make_unique<CircleFactory>()));
    managers.push_back(std::make_unique<ShapeFactoryManager>(std::make_unique<SquareFactory>()));
    
    for (size_t i = 0; i < managers.size(); ++i) {
        std::cout << "Manager " << i << " with " << managers[i]->getFactoryType() << ": ";
        auto shape = managers[i]->createShape(4.0 + i, 0.0, "magenta", "solid");
        shape->printInfo();
        std::cout << "Area: " << shape->getArea() << std::endl;
    }
    std::cout << std::endl;
    
    // Test 9: Factory Inheritance Example
    std::cout << "9. Factory Inheritance Example:\n";
    
    // Modern factory inheriting themed properties
    auto modernCircle2 = std::make_unique<ModernShapeFactory>()->makeShape(7.0, 0.0);
    std::cout << "Modern factory with default settings: ";
    modernCircle2->draw();
    modernCircle2->printInfo();
    
    // Custom color overrides the theme
    auto modernCircle3 = std::make_unique<ModernShapeFactory>()->makeShape(7.0, 0.0, "custom_red");
    std::cout << "Modern factory with custom color: ";
    modernCircle3->draw();
    modernCircle3->printInfo();
    std::cout << std::endl;
    
    // Test 10: Real-world Scenario - UI Component Factory
    std::cout << "10. Real-world Scenario - UI Component Factory:\n";
    
    // Simulate different UI themes using factory methods
    struct UIConfig {
        std::string theme;
        std::string componentType;
        double size;
    };
    
    std::vector<UIConfig> uiConfigs = {
        {"Modern", "button", 20.0},
        {"Vintage", "icon", 30.0}
    };
    
    for (const auto& ui : uiConfigs) {
        std::unique_ptr<ShapeFactory> uiFactory;
        
        if (ui.theme == "Modern") {
            uiFactory = std::make_unique<ModernShapeFactory>();
        } else if (ui.theme == "Vintage") {
            uiFactory = std::make_unique<VintageShapeFactory>();
        }
        
        auto component = uiFactory->makeShape(ui.size, ui.size * 0.5);
        std::cout << ui.theme << " " << ui.componentType << ": ";
        component->draw();
    }
    std::cout << std::endl;
    
    std::cout << "Factory Method pattern demo completed successfully!\n";
    std::cout << "\nKey Benefits of Factory Method Pattern:\n";
    std::cout << "- Defines an interface for creating objects, but lets subclasses decide which class to instantiate\n";
    std::cout << "- Promotes loose coupling by eliminating the need to bind application-specific classes into the code\n";
    std::cout << "- Follows the Open/Closed Principle - open for extension, closed for modification\n";
    std::cout << "- Enhances code maintainability and extensibility\n";
    std::cout << "- Supports creation of complex objects with different implementations\n";
    
    return 0;
}