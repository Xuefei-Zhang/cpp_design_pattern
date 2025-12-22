#include "flyweight_shapes.h"
#include <iostream>
#include <memory>
#include <vector>
#include <chrono>

int main() {
    std::cout << "=== Flyweight Pattern Demo: Shape System Optimization ===\n\n";
    
    // Test 1: Basic Flyweight Usage
    std::cout << "1. Basic Flyweight Pattern Usage:\n";
    
    // Get flyweights through the factory - identical shapes share the same flyweight object
    auto* circle1 = ShapeFlyweightFactory::createCircle(5.0, "red");
    auto* circle2 = ShapeFlyweightFactory::createCircle(5.0, "red");  // Same parameters, same flyweight
    auto* circle3 = ShapeFlyweightFactory::createCircle(3.0, "red");  // Different radius, different flyweight
    auto* circle4 = ShapeFlyweightFactory::createCircle(5.0, "blue"); // Different color, different flyweight
    
    std::cout << "circle1 and circle2 are " << (circle1 == circle2 ? "the same" : "different") << " object\n";
    std::cout << "circle1 and circle3 are " << (circle1 == circle3 ? "the same" : "different") << " object\n";
    std::cout << "circle1 and circle4 are " << (circle1 == circle4 ? "the same" : "different") << " object\n\n";
    
    // Use flyweight objects with different extrinsic states
    FlyweightShape shape1(circle1, 10.0, 10.0, 1.0);
    FlyweightShape shape2(circle2, 20.0, 20.0, 1.5);  // Same flyweight as shape1, different position/scale
    FlyweightShape shape3(circle3, 30.0, 30.0, 1.0);  // Different flyweight
    
    shape1.draw();
    shape2.draw();
    shape3.draw();
    std::cout << "Shape 1 area: " << shape1.getArea() << std::endl;
    std::cout << "Shape 2 area: " << shape2.getArea() << std::endl;  // Larger due to scale
    std::cout << "Shape 3 area: " << shape3.getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 2: Rectangle and Triangle Flyweights
    std::cout << "2. Rectangle and Triangle Flyweights:\n";
    
    auto* rect1 = ShapeFlyweightFactory::createRectangle(8.0, 6.0, "blue");
    auto* rect2 = ShapeFlyweightFactory::createRectangle(8.0, 6.0, "blue");  // Same as rect1
    auto* tri1 = ShapeFlyweightFactory::createTriangle(10.0, 8.0, "green");
    
    std::cout << "rect1 and rect2 are " << (rect1 == rect2 ? "the same" : "different") << " object\n\n";
    
    FlyweightShape rectShape(rect1, 50.0, 50.0, 1.0);
    FlyweightShape triShape(tri1, 100.0, 100.0, 0.8);
    
    rectShape.draw();
    triShape.draw();
    std::cout << std::endl;
    
    // Test 3: Flyweight Sharing Effectiveness
    std::cout << "3. Flyweight Sharing Effectiveness:\n";
    std::cout << "Current unique flyweight count: " << ShapeFlyweightFactory::getFlyweightCount() << std::endl;
    
    // Create many shapes that should reuse existing flyweights
    std::vector<FlyweightShape> manyCircles;
    for (int i = 0; i < 10; ++i) {
        // All use the same flyweight (since they have same parameters)
        auto* flyweight = ShapeFlyweightFactory::createCircle(2.0, "red");
        manyCircles.emplace_back(flyweight, i * 10.0, i * 10.0, 1.0 + i * 0.1);
    }
    
    std::cout << "After creating 10 similar circles, unique flyweight count: " 
              << ShapeFlyweightFactory::getFlyweightCount() << std::endl;
    std::cout << "All 10 circles share the same flyweight (they are the same object in memory)\n\n";
    
    // Test 4: Flyweight System Manager Usage
    std::cout << "4. Flyweight System Manager Usage:\n";
    
    FlyweightShapeSystem system;
    
    // Add many shapes with repeated characteristics to demonstrate sharing
    for (int i = 0; i < 5; ++i) {
        // These will all share the same circle flyweight
        system.addCircle(i * 20, i * 15, 3.0, "blue", 1.0);
    }
    
    for (int i = 0; i < 3; ++i) {
        // These will all share the same rectangle flyweight
        system.addRectangle(i * 30, i * 25, 4.0, 3.0, "red", 1.0);
    }
    
    system.renderAll();
    std::cout << "Total area: " << system.calculateTotalArea() << std::endl;
    std::cout << "Total shapes: " << system.getShapeCount() << std::endl;
    std::cout << "Unique flyweights used: " << system.getUniqueFlyweightCount() << std::endl;
    std::cout << "Memory efficiency: " << (system.getShapeCount() - system.getUniqueFlyweightCount()) 
              << " objects saved through sharing\n\n";
    
    // Test 5: Themed Flyweights
    std::cout << "5. Themed Flyweights:\n";
    
    auto baseCircle = std::make_unique<CircleFlyweight>(4.0, "gray");
    auto modernCircle = std::make_unique<ModernThemedFlyweight>(std::move(baseCircle));
    
    auto baseRect = std::make_unique<RectangleFlyweight>(6.0, 4.0, "brown");
    auto vintageRect = std::make_unique<VintageThemedFlyweight>(std::move(baseRect));
    
    FlyweightShape themedCircle(modernCircle.get(), 0, 0, 1.0);
    FlyweightShape themedRect(vintageRect.get(), 50, 50, 1.2);
    
    themedCircle.draw();
    themedRect.draw();
    std::cout << std::endl;
    
    // Test 6: Complex Scene with Multiple Flyweights
    std::cout << "6. Complex Scene with Multiple Flyweights:\n";
    
    FlyweightShapeSystem scene;
    
    // Create a complex scene with many repeated elements
    std::vector<std::tuple<std::string, double, double, std::string, double, double, double>> specs = {
        {"circle", 3.0, 0.0, "red", 0.0, 0.0, 1.0},   // type, param1, param2, color, x, y, scale
        {"rectangle", 5.0, 3.0, "blue", 10.0, 0.0, 1.0},
        {"circle", 3.0, 0.0, "red", 20.0, 0.0, 1.5},  // Same flyweight as first circle
        {"triangle", 6.0, 4.0, "green", 0.0, 10.0, 1.0},
        {"rectangle", 5.0, 3.0, "blue", 10.0, 10.0, 0.8},  // Same flyweight as second rectangle
        {"circle", 2.0, 0.0, "yellow", 20.0, 10.0, 1.0},   // Different flyweight (different radius)
    };
    
    for (const auto& spec : specs) {
        const std::string& type = std::get<0>(spec);
        double param1 = std::get<1>(spec);
        double param2 = std::get<2>(spec);
        const std::string& color = std::get<3>(spec);
        double x = std::get<4>(spec);
        double y = std::get<5>(spec);
        double scale = std::get<6>(spec);
        
        if (type == "circle") {
            scene.addCircle(x, y, param1, color, scale);
        } else if (type == "rectangle") {
            scene.addRectangle(x, y, param1, param2, color, scale);
        } else if (type == "triangle") {
            scene.addTriangle(x, y, param1, param2, color, scale);
        }
    }
    
    std::cout << "Complex scene created with " << scene.getShapeCount() << " shapes\n";
    std::cout << "Using " << scene.getUniqueFlyweightCount() << " unique flyweights\n";
    scene.renderAll();
    std::cout << "Scene total area: " << scene.calculateTotalArea() << std::endl;
    std::cout << std::endl;
    
    // Test 7: Performance Comparison - Direct Creation vs Flyweight Pattern
    std::cout << "7. Performance Comparison - Direct Creation vs Flyweight Pattern:\n";
    
    // Direct creation approach (memory intensive)
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::unique_ptr<ShapeFlyweight>> directShapes;
    
    for (int i = 0; i < 1000; ++i) {
        // Create many similar shapes (wastes memory)
        if (i % 3 == 0) {
            directShapes.push_back(std::make_unique<CircleFlyweight>(3.0, "red"));
        } else if (i % 3 == 1) {
            directShapes.push_back(std::make_unique<RectangleFlyweight>(4.0, 3.0, "blue"));
        } else {
            directShapes.push_back(std::make_unique<TriangleFlyweight>(5.0, 4.0, "green"));
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto directTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    ShapeFlyweightFactory::reset(); // Reset the factory for fair comparison
    
    // Flyweight approach (memory efficient)
    start = std::chrono::high_resolution_clock::now();
    std::vector<ShapeFlyweight*> flyweightPointers;
    
    for (int i = 0; i < 1000; ++i) {
        // Only create 3 unique flyweights total, reuse them
        if (i % 3 == 0) {
            flyweightPointers.push_back(ShapeFlyweightFactory::createCircle(3.0, "red"));
        } else if (i % 3 == 1) {
            flyweightPointers.push_back(ShapeFlyweightFactory::createRectangle(4.0, 3.0, "blue"));
        } else {
            flyweightPointers.push_back(ShapeFlyweightFactory::createTriangle(5.0, 4.0, "green"));
        }
    }
    end = std::chrono::high_resolution_clock::now();
    auto flyweightTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Direct creation (1000 objects): " << directTime.count() << " microseconds\n";
    std::cout << "Flyweight creation (1000 references to 3 shared objects): " << flyweightTime.count() << " microseconds\n";
    std::cout << "Flyweight pattern used: " << ShapeFlyweightFactory::getFlyweightCount() << " unique flyweights\n";
    std::cout << "Memory savings: " << (1000 - ShapeFlyweightFactory::getFlyweightCount()) 
              << " objects avoided through sharing\n\n";
    
    // Test 8: Real-world Scenario - UI Component System
    std::cout << "8. Real-world Scenario - UI Component System:\n";
    
    FlyweightShapeSystem uiSystem;
    
    // Create UI with many similar components (buttons, icons, etc.)
    std::vector<std::tuple<std::string, double, std::string, double, double, double>> uiComponents = {
        {"rectangle", 60.0, 25.0, "button_color", 10.0, 10.0, 1.0}, // Button
        {"rectangle", 60.0, 25.0, "button_color", 80.0, 10.0, 1.0}, // Same shape/color - same flyweight
        {"circle", 8.0, 0.0, "icon_color", 20.0, 20.0, 1.0},       // Icon
        {"circle", 8.0, 0.0, "icon_color", 90.0, 20.0, 1.0},       // Same shape/color - same flyweight
        {"rectangle", 200.0, 30.0, "panel_color", 10.0, 50.0, 1.0}, // Panel
        {"rectangle", 60.0, 25.0, "button_color", 220.0, 10.0, 1.0}, // Another button
    };
    
    for (const auto& comp : uiComponents) {
        const std::string& type = std::get<0>(comp);
        double param1 = std::get<1>(comp);
        double param2 = std::get<2>(comp);
        const std::string& color = std::get<3>(comp);
        double x = std::get<4>(comp);
        double y = std::get<5>(comp);
        double scale = std::get<6>(comp);
        
        if (type == "circle") {
            uiSystem.addCircle(x, y, param1, color, scale);
        } else if (type == "rectangle") {
            uiSystem.addRectangle(x, y, param1, param2, color, scale);
        }
    }
    
    std::cout << "UI system created with " << uiComponents.size() << " components\n";
    std::cout << "Using " << uiSystem.getUniqueFlyweightCount() << " unique flyweights\n";
    uiSystem.renderAll();
    std::cout << std::endl;
    
    // Test 9: Flyweight Factory Utilities
    std::cout << "9. Flyweight Factory Utilities:\n";
    std::cout << "Current flyweight count: " << ShapeFlyweightFactory::getFlyweightCount() << std::endl;
    
    std::cout << "Listing managed flyweights:\n";
    ShapeFlyweightFactory::listFlyweights();
    std::cout << std::endl;
    
    // Test 10: Factory Reset and Memory Management
    std::cout << "10. Factory Reset and Memory Management:\n";
    ShapeFlyweightFactory::reset();
    std::cout << "After reset, flyweight count: " << ShapeFlyweightFactory::getFlyweightCount() << std::endl;
    
    // Create a few more flyweights
    ShapeFlyweightFactory::createCircle(2.0, "orange");
    ShapeFlyweightFactory::createRectangle(3.0, 2.0, "purple");
    std::cout << "Created 2 new flyweights, count: " << ShapeFlyweightFactory::getFlyweightCount() << std::endl;
    std::cout << std::endl;
    
    // Test 11: Area Calculation with Scaled Flyweights
    std::cout << "11. Area Calculation with Scaled Flyweights:\n";
    
    auto* circleForArea = ShapeFlyweightFactory::createCircle(3.0, "area_test");
    
    FlyweightShape smallCircle(circleForArea, 0, 0, 0.5);   // Scale 0.5
    FlyweightShape normalCircle(circleForArea, 0, 0, 1.0); // Scale 1.0
    FlyweightShape largeCircle(circleForArea, 0, 0, 2.0);  // Scale 2.0
    
    std::cout << "Circle with scale 0.5: Area = " << smallCircle.getArea() << std::endl;
    std::cout << "Circle with scale 1.0: Area = " << normalCircle.getArea() << std::endl;
    std::cout << "Circle with scale 2.0: Area = " << largeCircle.getArea() << std::endl;
    std::cout << "Expected ratios: 1:4:16 (proportional to scale squared)\n\n";
    
    // Test 12: Large-Scale Flyweight Usage
    std::cout << "12. Large-Scale Flyweight Usage:\n";
    
    auto largeScaleStart = std::chrono::high_resolution_clock::now();
    
    // Create 5000 shapes with only 10 different flyweight types
    std::vector<FlyweightShape> bigScene;
    for (int i = 0; i < 5000; ++i) {
        int typeIndex = i % 10;
        double x = (i % 50) * 5.0;
        double y = (i / 50) * 5.0;
        
        switch (typeIndex) {
            case 0: // Red circles
                bigScene.emplace_back(ShapeFlyweightFactory::createCircle(1.0, "red"), x, y, 1.0);
                break;
            case 1: // Blue rectangles
                bigScene.emplace_back(ShapeFlyweightFactory::createRectangle(2.0, 1.5, "blue"), x, y, 1.0);
                break;
            case 2: // Green triangles
                bigScene.emplace_back(ShapeFlyweightFactory::createTriangle(2.0, 1.5, "green"), x, y, 1.0);
                break;
            case 3: // Yellow circles
                bigScene.emplace_back(ShapeFlyweightFactory::createCircle(1.0, "yellow"), x, y, 1.0);
                break;
            case 4: // Cyan rectangles
                bigScene.emplace_back(ShapeFlyweightFactory::createRectangle(2.0, 1.5, "cyan"), x, y, 1.0);
                break;
            case 5: // Magenta circles
                bigScene.emplace_back(ShapeFlyweightFactory::createCircle(0.8, "magenta"), x, y, 1.0);
                break;
            case 6: // Orange rectangles
                bigScene.emplace_back(ShapeFlyweightFactory::createRectangle(1.8, 1.2, "orange"), x, y, 1.0);
                break;
            case 7: // Pink triangles
                bigScene.emplace_back(ShapeFlyweightFactory::createTriangle(1.8, 1.2, "pink"), x, y, 1.0);
                break;
            case 8: // White circles
                bigScene.emplace_back(ShapeFlyweightFactory::createCircle(1.2, "white"), x, y, 1.0);
                break;
            case 9: // Black rectangles
                bigScene.emplace_back(ShapeFlyweightFactory::createRectangle(1.5, 1.0, "black"), x, y, 1.0);
                break;
        }
    }
    
    auto largeScaleEnd = std::chrono::high_resolution_clock::now();
    auto largeScaleDuration = std::chrono::duration_cast<std::chrono::milliseconds>(largeScaleEnd - largeScaleStart);
    
    std::cout << "Created 5000 shapes using only " << ShapeFlyweightFactory::getFlyweightCount() 
              << " unique flyweights\n";
    std::cout << "Time taken: " << largeScaleDuration.count() << " milliseconds\n";
    std::cout << "Memory savings: " << (5000 - ShapeFlyweightFactory::getFlyweightCount()) 
              << " objects avoided through sharing\n\n";
    
    // Test 13: Flyweight Pattern Benefits Demonstration
    std::cout << "13. Flyweight Pattern Benefits Demonstration:\n";
    std::cout << "- Memory Efficiency: " << (5000 - ShapeFlyweightFactory::getFlyweightCount()) 
              << " objects avoided through sharing\n";
    std::cout << "- Runtime Flexibility: Can change extrinsic state without affecting shared flyweights\n";
    std::cout << "- Performance: Fast creation of new shape instances\n";
    std::cout << "- Resource Management: Shared resources between objects\n";
    std::cout << "- System Complexity Hidden: Simple interface to complex subsystem\n\n";
    
    // Test 14: Intrinsic vs Extrinsic State Demonstration
    std::cout << "14. Intrinsic vs Extrinsic State Demonstration:\n";
    
    auto* sharedFlyweight = ShapeFlyweightFactory::createCircle(4.0, "demo_color");
    
    FlyweightShape instance1(sharedFlyweight, 10.0, 10.0, 1.0);  // Different extrinsic state
    FlyweightShape instance2(sharedFlyweight, 50.0, 30.0, 1.5); // Different extrinsic state
    FlyweightShape instance3(sharedFlyweight, 100.0, 75.0, 0.8); // Different extrinsic state
    
    std::cout << "All three instances share the same flyweight object\n";
    std::cout << "  Instance 1: Position (10,10), Scale 1.0\n";
    std::cout << "  Instance 2: Position (50,30), Scale 1.5\n";
    std::cout << "  Instance 3: Position (100,75), Scale 0.8\n";
    std::cout << "  Shared intrinsic properties: Circle with radius 4.0, color 'demo_color'\n\n";
    
    // Test 15: Dynamic Flyweight Creation
    std::cout << "15. Dynamic Flyweight Creation and Sharing:\n";
    
    struct ShapeSpec {
        std::string type;
        double param1, param2;
        std::string color;
    };
    
    std::vector<ShapeSpec> specsForDynamic = {
        {"circle", 2.0, 0.0, "dynamic_red"},
        {"rectangle", 3.0, 2.0, "dynamic_blue"},
        {"circle", 2.0, 0.0, "dynamic_red"},    // Same as first - should reuse
        {"triangle", 4.0, 3.0, "dynamic_green"},
        {"circle", 3.0, 0.0, "dynamic_red"},    // Different radius - new flyweight
        {"rectangle", 3.0, 2.0, "dynamic_blue"}, // Same as second - should reuse
    };
    
    for (size_t i = 0; i < specsForDynamic.size(); ++i) {
        const auto& spec = specsForDynamic[i];
        std::cout << "Creating " << spec.type << " #" << i+1 << ": ";
        
        ShapeFlyweight* flyweight = nullptr;
        if (spec.type == "circle") {
            flyweight = ShapeFlyweightFactory::createCircle(spec.param1, spec.color);
        } else if (spec.type == "rectangle") {
            flyweight = ShapeFlyweightFactory::createRectangle(spec.param1, spec.param2, spec.color);
        } else if (spec.type == "triangle") {
            flyweight = ShapeFlyweightFactory::createTriangle(spec.param1, spec.param2, spec.color);
        }
        
        FlyweightShape shape(flyweight, i*20.0, i*15.0, 1.0);
        shape.draw();
    }
    
    std::cout << "Total flyweights created: " << ShapeFlyweightFactory::getFlyweightCount() << std::endl;
    std::cout << "Expected: 4 unique flyweights (2 circles, 1 rectangle, 1 triangle)\n\n";
    
    std::cout << "Flyweight pattern demo completed successfully!\n";
    std::cout << "\nKey Benefits of Flyweight Pattern:\n";
    std::cout << "- Significantly reduces memory usage by sharing common state\n";
    std::cout << "- Improves performance when creating many similar objects\n";
    std::cout << "- Separates intrinsic (shared) from extrinsic (unique) state\n";
    std::cout << "- Supports large-scale applications with many similar objects\n";
    std::cout << "- Enables virtualization of objects when memory is limited\n";
    std::cout << "- Promotes system optimization through intelligent sharing\n";
    
    return 0;
}