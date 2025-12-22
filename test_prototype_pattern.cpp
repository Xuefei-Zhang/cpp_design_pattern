#include "prototype_shapes.h"
#include <iostream>
#include <memory>
#include <vector>
#include <chrono>

int main() {
    std::cout << "=== Prototype Pattern Demo: Shape Cloning and Registry ===\n\n";
    
    // Test 1: Basic Prototype Usage and Cloning
    std::cout << "1. Basic Prototype Usage and Cloning:\n";
    
    auto circleProto = std::make_unique<CirclePrototype>(5.0, "red", "solid", 10.0, 10.0);
    std::cout << "Original Circle: ";
    circleProto->draw();
    circleProto->printInfo();
    
    auto clonedCircle = circleProto->clone();
    std::cout << "Cloned Circle: ";
    clonedCircle->draw();
    clonedCircle->printInfo();
    
    // Modify the clone to show they're independent
    dynamic_cast<CirclePrototype*>(clonedCircle.get())->setRadius(7.0);
    dynamic_cast<CirclePrototype*>(clonedCircle.get())->setColor("blue");
    std::cout << "After modifying clone - Original: ";
    circleProto->draw();
    std::cout << "After modifying clone - Clone: ";
    clonedCircle->draw();
    std::cout << std::endl;
    
    // Test 2: Rectangle Prototype
    std::cout << "2. Rectangle Prototype Usage:\n";
    auto rectProto = std::make_unique<RectanglePrototype>(8.0, 6.0, "blue", "dashed", 20.0, 15.0);
    std::cout << "Original Rectangle: ";
    rectProto->draw();
    
    auto clonedRect = rectProto->clone();
    std::cout << "Cloned Rectangle: ";
    clonedRect->draw();
    
    // Modify clone to verify independence
    dynamic_cast<RectanglePrototype*>(clonedRect.get())->setWidth(12.0);
    dynamic_cast<RectanglePrototype*>(clonedRect.get())->setColor("yellow");
    std::cout << "After modifying clone - Original: ";
    rectProto->draw();
    std::cout << "After modifying clone - Clone: ";
    clonedRect->draw();
    std::cout << std::endl;
    
    // Test 3: Triangle Prototype
    std::cout << "3. Triangle Prototype Usage:\n";
    auto triProto = std::make_unique<TrianglePrototype>(10.0, 8.0, "green", "dotted", 5.0, 5.0);
    std::cout << "Original Triangle: ";
    triProto->draw();
    
    auto clonedTri = triProto->clone();
    std::cout << "Cloned Triangle: ";
    clonedTri->draw();
    
    // Verify independence
    std::cout << "Independence test - Original area: " << triProto->getArea()
              << ", Clone area: " << clonedTri->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 4: Registry System Usage
    std::cout << "4. Registry System Usage:\n";
    PrototypeRegistry registry;
    
    // Register prototype instances
    registry.registerPrototype("basic_circle", std::make_unique<CirclePrototype>(3.0, "purple", "solid", 0.0, 0.0));
    registry.registerPrototype("basic_rect", std::make_unique<RectanglePrototype>(5.0, 4.0, "orange", "dashed", 0.0, 0.0));
    registry.registerPrototype("basic_tri", std::make_unique<TrianglePrototype>(6.0, 4.0, "cyan", "solid", 0.0, 0.0));
    
    std::cout << "Registered prototypes: ";
    auto ids = registry.getAvailableIds();
    for (const auto& id : ids) {
        std::cout << id << " ";
    }
    std::cout << std::endl;
    
    // Create shapes from the registry
    auto circleFromRegistry = registry.create("basic_circle");
    auto rectFromRegistry = registry.create("basic_rect");
    auto triFromRegistry = registry.create("basic_tri");
    
    std::cout << "Created from registry:\n";
    circleFromRegistry->draw();
    rectFromRegistry->draw();
    triFromRegistry->draw();
    
    // Modify one of the created instances to show it's independent from prototype
    dynamic_cast<CirclePrototype*>(circleFromRegistry.get())->setRadius(10.0);
    dynamic_cast<CirclePrototype*>(circleFromRegistry.get())->setColor("magenta");
    std::cout << "After modification - Registry basic circle: ";
    circleFromRegistry->draw();
    
    // Get the original prototype (not a clone) to show it's unchanged
    std::cout << "Prototype itself unchanged: ";
    registry.getPrototype("basic_circle")->draw();
    std::cout << std::endl;
    
    // Test 5: Factory Usage
    std::cout << "5. Factory Usage:\n";
    
    // Register prototypes in the factory
    ShapeFactory::registerPrototype("factory_circle", std::make_unique<CirclePrototype>(4.0, "lime", "solid", 1.0, 1.0));
    ShapeFactory::registerPrototype("factory_rect", std::make_unique<RectanglePrototype>(7.0, 3.0, "pink", "dotted", 2.0, 2.0));
    
    std::cout << "Available shapes in factory: ";
    for (const auto& type : ShapeFactory::getAvailableShapeTypes()) {
        std::cout << type << " ";
    }
    std::cout << std::endl;
    
    // Create shapes using factory
    auto factoryCircle = ShapeFactory::createShape("factory_circle");
    auto factoryRect = ShapeFactory::createShape("factory_rect");
    
    std::cout << "Shapes created via factory:\n";
    factoryCircle->draw();
    factoryRect->draw();
    std::cout << std::endl;
    
    // Test 6: Complex Shape Prototype (Composite Pattern with Prototypes)
    std::cout << "6. Complex Shape Prototype (Composite):\n";
    
    auto complexShape = std::make_unique<ComplexShapePrototype>("Robot", 100.0, 100.0);
    complexShape->addComponent(std::make_unique<CirclePrototype>(5.0, "gray", "solid", 0.0, -20.0)); // Head
    complexShape->addComponent(std::make_unique<RectanglePrototype>(10.0, 15.0, "gray", "solid", 0.0, 0.0)); // Body
    complexShape->addComponent(std::make_unique<RectanglePrototype>(3.0, 8.0, "gray", "solid", -7.0, 0.0)); // Left Arm
    complexShape->addComponent(std::make_unique<RectanglePrototype>(3.0, 8.0, "gray", "solid", 7.0, 0.0)); // Right Arm
    
    std::cout << "Original Complex Shape: ";
    complexShape->draw();
    
    auto clonedComplex = complexShape->clone();
    std::cout << "Cloned Complex Shape: ";
    clonedComplex->draw();
    
    // Modify clone to show deep copying
    dynamic_cast<ComplexShapePrototype*>(clonedComplex.get())->setName("Cloned Robot");
    std::cout << "After renaming clone: ";
    clonedComplex->draw();
    std::cout << "Original remains: ";
    complexShape->draw();
    std::cout << std::endl;
    
    // Test 7: Configurable Shape Prototype
    std::cout << "7. Configurable Shape Prototype:\n";
    
    auto baseCircle = std::make_unique<CirclePrototype>(6.0, "red", "solid", 0.0, 0.0);
    auto configurableCircle = std::make_unique<ConfigurableShapePrototype>(std::move(baseCircle), "Special Circle");
    configurableCircle->setProperty("layer", "foreground");
    configurableCircle->setProperty("animation", "rotating");
    configurableCircle->setProperty("id", "circle_001");
    
    std::cout << "Original Configurable Shape: ";
    configurableCircle->draw();
    
    auto clonedConfigurable = configurableCircle->clone();
    std::cout << "Cloned Configurable Shape: ";
    clonedConfigurable->draw();
    
    // Modify the clone's properties
    dynamic_cast<ConfigurableShapePrototype*>(clonedConfigurable.get())->setProperty("id", "circle_002");
    dynamic_cast<ConfigurableShapePrototype*>(clonedConfigurable.get())->setProperty("animation", "pulsing");
    
    std::cout << "After modifying clone properties: ";
    std::cout << "Original: ";
    configurableCircle->draw();
    std::cout << "Clone: ";
    clonedConfigurable->draw();
    std::cout << std::endl;
    
    // Test 8: Performance Comparison - Direct Creation vs Prototype
    std::cout << "8. Performance Test - Direct Creation vs Prototype:\n";
    
    // Direct creation
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::unique_ptr<ShapePrototype>> directShapes;
    for (int i = 0; i < 1000; ++i) {
        directShapes.push_back(std::make_unique<CirclePrototype>(2.0 + (i % 5), 
                                                                 i % 2 ? "red" : "blue"));
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto directTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // Prototype-based creation
    start = std::chrono::high_resolution_clock::now();
    auto templateCircle = std::make_unique<CirclePrototype>(2.0, "template_color");
    std::vector<std::unique_ptr<ShapePrototype>> prototypeShapes;
    for (int i = 0; i < 1000; ++i) {
        auto shape = templateCircle->clone();
        dynamic_cast<CirclePrototype*>(shape.get())->setRadius(2.0 + (i % 5));
        dynamic_cast<CirclePrototype*>(shape.get())->setColor(i % 2 ? "red" : "blue");
        prototypeShapes.push_back(std::move(shape));
    }
    end = std::chrono::high_resolution_clock::now();
    auto prototypeTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Direct creation (1000 circles): " << directTime.count() << " microseconds\n";
    std::cout << "Prototype creation (1000 circles): " << prototypeTime.count() << " microseconds\n";
    std::cout << std::endl;
    
    // Test 9: Registry with Complex Shapes
    std::cout << "9. Registry with Complex Shapes:\n";
    
    auto houseProto = std::make_unique<ComplexShapePrototype>("House", 0.0, 0.0);
    houseProto->addComponent(std::make_unique<RectanglePrototype>(20.0, 15.0, "brown", "solid", 0.0, 0.0)); // Base
    houseProto->addComponent(std::make_unique<TrianglePrototype>(20.0, 10.0, "red", "solid", 0.0, -10.0)); // Roof
    houseProto->addComponent(std::make_unique<RectanglePrototype>(5.0, 8.0, "blue", "solid", -5.0, 5.0)); // Door
    houseProto->addComponent(std::make_unique<CirclePrototype>(1.0, "yellow", "solid", 5.0, -3.0)); // Window
    
    PrototypeRegistry complexRegistry;
    complexRegistry.registerPrototype("house", std::move(houseProto));
    
    auto house1 = complexRegistry.create("house");
    auto house2 = complexRegistry.create("house");
    
    std::cout << "Created two houses from prototype:\n";
    std::cout << "House 1: ";
    house1->draw();
    std::cout << "House 2: ";
    house2->draw();
    
    // Modify one to show independence
    dynamic_cast<ComplexShapePrototype*>(house1.get())->setName("House Alpha");
    dynamic_cast<ComplexShapePrototype*>(house2.get())->setName("House Beta");
    std::cout << "After renaming - House 1: " << house1->getType() 
              << ", House 2: " << house2->getType() << std::endl;
    std::cout << std::endl;
    
    // Test 10: Prototype Pattern Benefits Demonstration
    std::cout << "10. Prototype Pattern Benefits Demonstration:\n";
    
    std::cout << "- Avoiding expensive initialization: ";
    std::cout << "Prototype avoids reinitialization of complex objects\n";
    
    std::cout << "- Dynamic loading: ";
    std::cout << "Objects can be added to registry at runtime\n";
    
    std::cout << "- Reduced subclassing: ";
    std::cout << "One cloning mechanism works for all prototype types\n";
    
    std::cout << "- Configurability: ";
    std::cout << "Easily create variations of existing objects\n";
    
    std::cout << "- Performance: ";
    std::cout << "Often faster than constructing from scratch for complex objects\n";
    std::cout << std::endl;
    
    // Test 11: Real-world Scenario - UI Component Factory
    std::cout << "11. Real-world Scenario - UI Component Factory:\n";
    
    // Register different UI component templates
    ShapeFactory::registerPrototype("button_default", 
        std::make_unique<RectanglePrototype>(50.0, 25.0, "lightgray", "solid", 0.0, 0.0));
    ShapeFactory::registerPrototype("icon_home", 
        std::make_unique<TrianglePrototype>(15.0, 15.0, "blue", "solid", 0.0, 0.0));
    ShapeFactory::registerPrototype("window_frame", 
        std::make_unique<RectanglePrototype>(200.0, 150.0, "white", "solid", 0.0, 0.0));
    
    // Create multiple UI components efficiently
    auto button1 = ShapeFactory::createShape("button_default");
    auto button2 = ShapeFactory::createShape("button_default");
    auto icon = ShapeFactory::createShape("icon_home");
    auto window = ShapeFactory::createShape("window_frame");
    
    // Customize each instance
    dynamic_cast<RectanglePrototype*>(button1.get())->setX(10.0);
    dynamic_cast<RectanglePrototype*>(button1.get())->setY(10.0);
    dynamic_cast<RectanglePrototype*>(button1.get())->setColor("green");
    
    dynamic_cast<RectanglePrototype*>(button2.get())->setX(70.0);
    dynamic_cast<RectanglePrototype*>(button2.get())->setY(10.0);
    dynamic_cast<RectanglePrototype*>(button2.get())->setColor("red");
    
    dynamic_cast<TrianglePrototype*>(icon.get())->setX(100.0);
    dynamic_cast<TrianglePrototype*>(icon.get())->setY(100.0);
    
    dynamic_cast<RectanglePrototype*>(window.get())->setX(50.0);
    dynamic_cast<RectanglePrototype*>(window.get())->setY(50.0);
    
    std::cout << "Customized UI components created from prototypes:\n";
    button1->draw();
    button2->draw();
    icon->draw();
    window->draw();
    std::cout << std::endl;
    
    std::cout << "Prototype pattern demo completed successfully!\n";
    std::cout << "\nKey Benefits of Prototype Pattern:\n";
    std::cout << "- Avoids expensive initialization of complex objects\n";
    std::cout << "- Allows runtime addition of new classes\n";
    std::cout << "- Reduces the need for subclassing\n";
    std::cout << "- Provides an alternative to Factory pattern when creation is complex\n";
    std::cout << "- Supports dynamic configuration of applications\n";
    
    return 0;
}