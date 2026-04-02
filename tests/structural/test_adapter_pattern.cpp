#include "structural/adapter_patterns.h"
#include <iostream>
#include <memory>
#include <vector>

int main() {
    std::cout << "=== Adapter Pattern Demo: Shape Interface Compatibility ===\n\n";
    
    // Test 1: Object Adapter for Legacy Shapes
    std::cout << "1. Object Adapter for Legacy Shapes:\n";
    std::vector<std::unique_ptr<Shape>> shapes;
    
    // Add concrete shapes
    shapes.push_back(std::make_unique<Circle>(5.0, "red"));
    shapes.push_back(std::make_unique<Rectangle>(8.0, 6.0, "blue"));
    
    // Add adapted legacy shapes
    shapes.push_back(std::make_unique<LegacyShapeAdapter>(
        std::make_unique<LegacyCircle>(4.0)
    ));
    shapes.push_back(std::make_unique<LegacyShapeAdapter>(
        std::make_unique<LegacyRectangle>(10.0, 7.0)
    ));

    ShapeRenderer::renderAll(shapes);
    std::cout << "Total area of all shapes: " << ShapeRenderer::calculateTotalArea(shapes) << std::endl;
    std::cout << std::endl;

    // Test 2: Object Adapter for Third-Party Shapes
    std::cout << "2. Object Adapter for Third-Party Shapes:\n";
    std::vector<std::unique_ptr<Shape>> thirdPartyShapes;

    thirdPartyShapes.push_back(std::make_unique<ThirdPartyShapeAdapter>(
        std::make_unique<ThirdPartyEllipse>(6.0, 4.0)
    ));
    thirdPartyShapes.push_back(std::make_unique<ThirdPartyShapeAdapter>(
        std::make_unique<ThirdPartyBox>(12.0, 8.0)
    ));

    ShapeRenderer::renderAll(thirdPartyShapes);
    std::cout << "Total area of third-party shapes: " << ShapeRenderer::calculateTotalArea(thirdPartyShapes) << std::endl;
    std::cout << std::endl;

    // Test 3: Adapter for Modern Shape System
    std::cout << "3. Adapter for Modern Shape System:\n";
    ModernShapeData circleData;
    circleData.type = "circle";
    circleData.x = 100.0;
    circleData.y = 100.0;
    circleData.scale = 1.0;
    circleData.parameters = {5.0};

    ModernShapeData rectData;
    rectData.type = "rectangle";
    rectData.x = 200.0;
    rectData.y = 150.0;
    rectData.scale = 1.0;
    rectData.parameters = {10.0, 8.0};

    std::vector<std::unique_ptr<Shape>> modernShapes;
    modernShapes.push_back(std::make_unique<ModernShapeAdapter>(circleData));
    modernShapes.push_back(std::make_unique<ModernShapeAdapter>(rectData));

    ShapeRenderer::renderAll(modernShapes);
    std::cout << "Total area of modern shapes: " << ShapeRenderer::calculateTotalArea(modernShapes) << std::endl;
    std::cout << std::endl;
    
    // Test 4: Universal Adapter
    std::cout << "4. Universal Adapter Usage:\n";
    std::vector<std::unique_ptr<Shape>> universalShapes;
    
    std::cout << "Universal adapter was removed due to C++14 compatibility issues with lambda captures\n";
    std::cout << std::endl;
    
    // Test 5: Mixed Shapes from All Systems
    std::cout << "5. Mixed Shapes from All Systems:\n";
    std::vector<std::unique_ptr<Shape>> allShapes;
    
    // Add regular shapes
    allShapes.push_back(std::make_unique<Circle>(4.0, "green"));
    allShapes.push_back(std::make_unique<Rectangle>(6.0, 9.0, "yellow"));
    
    // Add legacy shapes via adapter
    allShapes.push_back(std::make_unique<LegacyShapeAdapter>(
        std::make_unique<LegacyCircle>(5.5)
    ));
    
    // Add third-party shapes via adapter
    allShapes.push_back(std::make_unique<ThirdPartyShapeAdapter>(
        std::make_unique<ThirdPartyBox>(15.0, 10.0)
    ));
    
    // Add modern shapes via adapter
    ModernShapeData modernCircle;
    modernCircle.type = "circle";
    modernCircle.x = 50.0;
    modernCircle.y = 50.0;
    modernCircle.scale = 1.0;
    modernCircle.parameters = {6.0};
    allShapes.push_back(std::make_unique<ModernShapeAdapter>(modernCircle));
    
    // Add universal adapter shapes - skipped due to C++14 compatibility issues
    // allShapes.push_back(UniversalShapeAdapter::createForLegacy(
    //     std::make_unique<LegacyRectangle>(8.0, 5.0)
    // ));
    
    ShapeRenderer::renderAll(allShapes);
    std::cout << "Total area of all mixed shapes: " << ShapeRenderer::calculateTotalArea(allShapes) << std::endl;
    std::cout << std::endl;
    
    // Test 6: Demonstration of Incompatible Interfaces
    std::cout << "6. Demonstration of Incompatible Interfaces (Before Adapters):\n";
    
    // These are incompatible interfaces that cannot be used directly
    auto legacyCircle = std::make_unique<LegacyCircle>(3.0);
    auto thirdPartyEllipse = std::make_unique<ThirdPartyEllipse>(4.0, 3.0);
    
    std::cout << "Legacy system call: ";
    legacyCircle->renderLegacy();
    std::cout << "Legacy area: " << legacyCircle->calculateLegacyArea() << std::endl;
    
    std::cout << "Third-party system call: ";
    thirdPartyEllipse->renderAsSVG();
    std::cout << "Third-party area: " << thirdPartyEllipse->getCalculatedArea() << std::endl;
    
    std::cout << "After adapter - can use with common interface:\n";
    
    // Now use them through adapters
    std::vector<std::unique_ptr<Shape>> adaptedShapes;
    adaptedShapes.push_back(std::make_unique<LegacyShapeAdapter>(std::move(legacyCircle)));
    adaptedShapes.push_back(std::make_unique<ThirdPartyShapeAdapter>(std::move(thirdPartyEllipse)));
    
    ShapeRenderer::renderAll(adaptedShapes);
    std::cout << std::endl;
    
    // Test 7: Adapter Pattern Benefits
    std::cout << "7. Adapter Pattern Benefits Demonstration:\n";
    
    std::cout << "- Interface Compatibility: ";
    std::cout << "Adapters allow incompatible interfaces to work together\n";
    
    std::cout << "- Client Code Independence: ";
    std::cout << "Client code can work with target interface regardless of adaptee\n";
    
    std::cout << "- Reusability: ";
    std::cout << "Existing incompatible classes can be reused without modification\n";
    
    std::cout << "- Flexibility: ";
    std::cout << "Can adapt multiple third-party systems to common interface\n";
    std::cout << std::endl;
    
    // Test 8: Performance Comparison - Direct vs Adapted
    std::cout << "8. Performance Test - Direct vs Adapted Access:\n";
    
    // Test direct access to legacy system
    auto legacyDirect = std::make_unique<LegacyCircle>(2.0);
    std::cout << "Direct legacy access: ";
    legacyDirect->renderLegacy();
    
    // Test access through adapter
    auto legacyAdapter = std::make_unique<LegacyShapeAdapter>(std::make_unique<LegacyCircle>(2.0));
    std::cout << "Through adapter: ";
    legacyAdapter->draw();
    
    std::cout << "Areas - Direct: " << legacyDirect->calculateLegacyArea() 
              << ", Adapter: " << legacyAdapter->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 9: Real-world Scenario - Graphics System Integration
    std::cout << "9. Real-world Scenario - Graphics System Integration:\n";
    
    // Simulate a graphics system that needs to work with different shape types
    std::vector<std::unique_ptr<Shape>> graphicsSystemShapes;
    
    // Add shapes from different systems using adapters
    graphicsSystemShapes.push_back(std::make_unique<Circle>(3.0, "canvas_color"));  // Native
    graphicsSystemShapes.push_back(std::make_unique<LegacyShapeAdapter>(  // Legacy system
        std::make_unique<LegacyCircle>(4.0)
    ));
    graphicsSystemShapes.push_back(std::make_unique<ThirdPartyShapeAdapter>(  // Third-party
        std::make_unique<ThirdPartyBox>(8.0, 6.0)
    ));
    ModernShapeData modernShape = {"triangle", 75.0, 75.0, 1.0, {7.0, 5.0}};
    graphicsSystemShapes.push_back(std::make_unique<ModernShapeAdapter>(modernShape));  // Modern
    // graphicsSystemShapes.push_back(UniversalShapeAdapter::createForLegacy(  // Universal
    //     std::make_unique<LegacyRectangle>(9.0, 4.0)
    // ));  // Skipped due to C++14 compatibility issues
    
    std::cout << "Graphics system processing shapes from multiple sources:\n";
    for (size_t i = 0; i < graphicsSystemShapes.size(); ++i) {
        std::cout << "Shape " << i + 1 << ": ";
        graphicsSystemShapes[i]->draw();
    }
    std::cout << "Total shapes processed: " << graphicsSystemShapes.size() << std::endl;
    std::cout << "Total area: " << ShapeRenderer::calculateTotalArea(graphicsSystemShapes) << std::endl;
    std::cout << std::endl;
    
    // Test 10: Adapter Chain - Multiple Adapters
    std::cout << "10. Advanced Pattern - Potential Adapter Chain (Conceptual):\n";
    
    // In practice, we might have adapters for adapters in complex systems
    // This example shows how adapters make integration possible
    std::cout << "Adapter allows integration of:\n";
    std::cout << "- Native shapes (Circle, Rectangle)\n";
    std::cout << "- Legacy system shapes (LegacyCircle, LegacyRectangle)\n";
    std::cout << "- Third-party library shapes (ThirdPartyEllipse, ThirdPartyBox)\n";
    std::cout << "- Modern data structure shapes (ModernShapeData)\n";
    std::cout << "All unified under the common Shape interface!\n";
    std::cout << "(Universal adapter skipped due to C++14 compatibility issues)\n";
    std::cout << std::endl;
    
    // Test 11: Comparison with Other Integration Approaches
    std::cout << "11. Comparison with Other Integration Approaches:\n";
    std::cout << "- Inheritance: Requires modifying original classes (not always possible)\n";
    std::cout << "- Composition: Adapter uses composition to achieve interface compatibility\n";
    std::cout << "- Direct Integration: Would require implementing all interfaces in each class\n";
    std::cout << "- Adapter Pattern: Provides clean separation between interfaces\n";
    std::cout << std::endl;
    
    std::cout << "Adapter pattern demo completed successfully!\n";
    std::cout << "\nKey Benefits of Adapter Pattern:\n";
    std::cout << "- Enables incompatible interfaces to work together\n";
    std::cout << "- Promotes reusability of existing code\n";
    std::cout << "- Maintains single responsibility principle\n";
    std::cout << "- Supports open/closed principle (open for extension, closed for modification)\n";
    std::cout << "- Facilitates integration of third-party libraries\n";
    
    return 0;
}