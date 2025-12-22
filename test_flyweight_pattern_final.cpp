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
    
    // Test 4: Complex Shape System Usage
    std::cout << "4. Complex Shape System with Multiple Flyweights:\n";
    
    std::vector<std::unique_ptr<FlyweightShape>> complexSystem;
    
    // Create a scene with repeated shapes to demonstrate sharing
    for (int i = 0; i < 5; ++i) {
        complexSystem.push_back(std::make_unique<FlyweightShape>(
            ShapeFlyweightFactory::createCircle(3.0, "blue"), i * 20.0, 0.0, 1.0));
    }
    
    for (int i = 0; i < 3; ++i) {
        complexSystem.push_back(std::make_unique<FlyweightShape>(
            ShapeFlyweightFactory::createRectangle(5.0, 4.0, "red"), i * 30.0, 50.0, 1.0));
    }
    
    for (int i = 0; i < 4; ++i) {
        complexSystem.push_back(std::make_unique<FlyweightShape>(
            ShapeFlyweightFactory::createTriangle(6.0, 5.0, "green"), i * 25.0, 100.0, 1.0));
    }
    
    std::cout << "Created complex system with " << complexSystem.size() << " shapes\n";
    std::cout << "Using only " << ShapeFlyweightFactory::getFlyweightCount() << " unique flyweights\n";
    std::cout << "Sharing efficiency: " << (complexSystem.size() - ShapeFlyweightFactory::getFlyweightCount()) 
              << " objects saved through sharing\n";
    
    // Render the system
    for (const auto& shape : complexSystem) {
        shape->draw();
    }
    std::cout << std::endl;
    
    // Test 5: Themed Flyweights
    std::cout << "5. Themed Flyweights:\n";

    // Create base shapes through factory first
    auto* factoryCircle = ShapeFlyweightFactory::createCircle(4.0, "gray");
    auto* factoryRect = ShapeFlyweightFactory::createRectangle(6.0, 4.0, "brown");

    // Create themed versions manually (these are concrete examples)
    auto themedCircle = std::make_unique<ModernThemedFlyweight>(std::make_unique<CircleFlyweight>(4.0, "gray"));
    auto themedRect = std::make_unique<VintageThemedFlyweight>(std::make_unique<RectangleFlyweight>(6.0, 4.0, "brown"));

    FlyweightShape themedCircleShape(themedCircle.get(), 0, 0, 1.0);
    FlyweightShape themedRectShape(themedRect.get(), 50, 50, 1.2);

    themedCircleShape.draw();
    themedRectShape.draw();
    std::cout << std::endl;
    
    // Test 6: Performance Test - Direct Creation vs Flyweight Pattern
    std::cout << "6. Performance Test - Direct Creation vs Flyweight Pattern:\n";
    
    // Direct creation approach (memory intensive)
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::unique_ptr<ShapeFlyweight>> directShapes;
    
    for (int i = 0; i < 1000; ++i) {
        // Create many similar objects (wastes memory)
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
    
    // Reset factory for fair comparison
    ShapeFlyweightFactory::reset();
    
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
    
    // Test 7: Real-world Scenario - UI Component System
    std::cout << "7. Real-world Scenario - UI Component System:\n";
    
    // Create a UI with many similar components to demonstrate sharing
    std::vector<std::unique_ptr<FlyweightShape>> uiComponents;
    
    // Create many buttons (same shape and style, different positions)
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 4; ++col) {
            uiComponents.push_back(std::make_unique<FlyweightShape>(
                ShapeFlyweightFactory::createRectangle(30.0, 20.0, "lightblue"), 
                col * 40.0, row * 30.0, 1.0));
        }
    }
    
    // Add some other UI elements
    uiComponents.push_back(std::make_unique<FlyweightShape>(
        ShapeFlyweightFactory::createCircle(5.0, "blue"), 10.0, 150.0, 1.0));
    uiComponents.push_back(std::make_unique<FlyweightShape>(
        ShapeFlyweightFactory::createTriangle(8.0, 6.0, "green"), 60.0, 150.0, 1.0));
    
    std::cout << "UI created with " << uiComponents.size() << " components\n";
    std::cout << "Using " << ShapeFlyweightFactory::getFlyweightCount() << " unique flyweights\n";
    std::cout << "Sharing efficiency: " << (uiComponents.size() - ShapeFlyweightFactory::getFlyweightCount()) 
              << " objects saved through sharing\n";
    
    for (size_t i = 0; i < std::min(static_cast<size_t>(5), uiComponents.size()); ++i) {
        std::cout << "UI Component " << i + 1 << ": ";
        uiComponents[i]->draw();
    }
    std::cout << std::endl;
    
    // Test 8: Area Calculation with Scaling
    std::cout << "8. Area Calculation with Scaling:\n";

    auto* areaTestCircle = ShapeFlyweightFactory::createCircle(3.0, "area_test");
    
    std::vector<std::pair<double, double>> scalesAndExpectedAreas = {
        {0.5, 3.14159 * 3.0 * 3.0 * 0.5 * 0.5},  // Expected: ~7.0686
        {1.0, 3.14159 * 3.0 * 3.0 * 1.0 * 1.0},  // Expected: ~28.2743
        {1.5, 3.14159 * 3.0 * 3.0 * 1.5 * 1.5},  // Expected: ~63.6172
        {2.0, 3.14159 * 3.0 * 3.0 * 2.0 * 2.0}   // Expected: ~113.097
    };
    
    for (const auto& scaleAreaPair : scalesAndExpectedAreas) {
        double scale = scaleAreaPair.first;
        double expected = scaleAreaPair.second;
        
        FlyweightShape scaledCircle(areaTestCircle, 0, 0, scale);
        double actual = scaledCircle.getArea();
        
        std::cout << "Circle with scale " << scale << ": Actual area = " << actual 
                  << ", Expected = " << expected 
                  << ", Diff = " << abs(actual - expected) << std::endl;
    }
    std::cout << std::endl;
    
    // Test 9: Factory Utilities and Management
    std::cout << "9. Factory Utilities and Management:\n";
    std::cout << "Current flyweight count: " << ShapeFlyweightFactory::getFlyweightCount() << std::endl;
    
    std::cout << "Listing managed flyweights:\n";
    ShapeFlyweightFactory::listFlyweights();
    
    // Reset factory to clean state
    ShapeFlyweightFactory::reset();
    std::cout << "After reset, flyweight count: " << ShapeFlyweightFactory::getFlyweightCount() << std::endl;
    
    // Create a few new flyweights
    ShapeFlyweightFactory::createCircle(2.0, "orange");
    ShapeFlyweightFactory::createRectangle(3.0, 2.0, "purple");
    std::cout << "Created 2 new flyweights, count: " << ShapeFlyweightFactory::getFlyweightCount() << std::endl;
    std::cout << std::endl;
    
    // Test 10: Intrinsic vs Extrinsic State Demonstration
    std::cout << "10. Intrinsic vs Extrinsic State Demonstration:\n";
    
    auto* sharedFlyweight = ShapeFlyweightFactory::createCircle(4.0, "demo_color");
    
    // Multiple instances sharing the same flyweight but with different extrinsic state
    std::vector<FlyweightShape> instances;
    instances.emplace_back(sharedFlyweight, 10.0, 10.0, 1.0);   // Different position/scale
    instances.emplace_back(sharedFlyweight, 50.0, 30.0, 1.5);   // Different position/scale
    instances.emplace_back(sharedFlyweight, 100.0, 75.0, 0.8);  // Different position/scale
    
    std::cout << "All three instances share the same flyweight object (intrinsic state)\n";
    std::cout << "  Shared properties: Circle, radius=4.0, color='demo_color'\n";
    std::cout << "  Unique properties for each instance:\n";
    
    for (size_t i = 0; i < instances.size(); ++i) {
        std::cout << "    Instance " << (i + 1) << ": Position=(" 
                  << instances[i].getX() << "," << instances[i].getY() 
                  << "), Scale=" << instances[i].getScale() << std::endl;
    }
    std::cout << std::endl;
    
    // Test 11: Dynamic Flyweight Creation and Sharing
    std::cout << "11. Dynamic Flyweight Creation and Sharing:\n";
    
    struct ShapeSpec {
        std::string type;
        double param1, param2;
        std::string color;
    };
    
    std::vector<ShapeSpec> specs = {
        {"circle", 2.0, 0.0, "dynamic_red"},
        {"rectangle", 3.0, 2.0, "dynamic_blue"},
        {"circle", 2.0, 0.0, "dynamic_red"},    // Same as first - should reuse
        {"triangle", 4.0, 3.0, "dynamic_green"},
        {"circle", 3.0, 0.0, "dynamic_red"},    // Different radius - new flyweight
        {"rectangle", 3.0, 2.0, "dynamic_blue"}, // Same as second - should reuse
    };
    
    std::vector<std::unique_ptr<FlyweightShape>> dynamicShapes;
    
    for (size_t i = 0; i < specs.size(); ++i) {
        const auto& spec = specs[i];
        std::cout << "Creating " << spec.type << " #" << (i+1) << " (color=" << spec.color << "): ";
        
        ShapeFlyweight* flyweight = nullptr;
        if (spec.type == "circle") {
            flyweight = ShapeFlyweightFactory::createCircle(spec.param1, spec.color);
        } else if (spec.type == "rectangle") {
            flyweight = ShapeFlyweightFactory::createRectangle(spec.param1, spec.param2, spec.color);
        } else if (spec.type == "triangle") {
            flyweight = ShapeFlyweightFactory::createTriangle(spec.param1, spec.param2, spec.color);
        }
        
        if (flyweight) {
            dynamicShapes.push_back(std::make_unique<FlyweightShape>(flyweight, i*20.0, i*15.0, 1.0));
            std::cout << "Created, flyweight pointer: " << flyweight << std::endl;
        }
    }
    
    std::cout << "\nTotal unique flyweights created: " << ShapeFlyweightFactory::getFlyweightCount() << std::endl;
    std::cout << "Expected: 4 unique flyweights (2 circles, 1 rectangle, 1 triangle)\n";
    std::cout << std::endl;
    
    // Test 12: Large-Scale Flyweight Usage
    std::cout << "12. Large-Scale Flyweight Usage Test:\n";
    
    ShapeFlyweightFactory::reset();  // Start fresh
    
    auto startLarge = std::chrono::high_resolution_clock::now();
    
    // Create 5000 shapes with only 12 different flyweight types
    std::vector<std::unique_ptr<FlyweightShape>> largeScaleShapes;
    for (int i = 0; i < 5000; ++i) {
        int typeIndex = i % 12;
        double x = (i % 50) * 5.0;
        double y = (i / 50) * 5.0;
        
        ShapeFlyweight* flyweight = nullptr;
        switch (typeIndex) {
            case 0:  // Red circles
                flyweight = ShapeFlyweightFactory::createCircle(1.0, "red");
                break;
            case 1:  // Blue rectangles
                flyweight = ShapeFlyweightFactory::createRectangle(2.0, 1.5, "blue");
                break;
            case 2:  // Green triangles
                flyweight = ShapeFlyweightFactory::createTriangle(2.0, 1.5, "green");
                break;
            case 3:  // Yellow circles
                flyweight = ShapeFlyweightFactory::createCircle(1.0, "yellow");
                break;
            case 4:  // Cyan rectangles
                flyweight = ShapeFlyweightFactory::createRectangle(2.0, 1.5, "cyan");
                break;
            case 5:  // Magenta circles
                flyweight = ShapeFlyweightFactory::createCircle(0.8, "magenta");
                break;
            case 6:  // Orange rectangles
                flyweight = ShapeFlyweightFactory::createRectangle(1.8, 1.2, "orange");
                break;
            case 7:  // Pink triangles
                flyweight = ShapeFlyweightFactory::createTriangle(1.8, 1.2, "pink");
                break;
            case 8:  // White circles
                flyweight = ShapeFlyweightFactory::createCircle(1.2, "white");
                break;
            case 9:  // Black rectangles
                flyweight = ShapeFlyweightFactory::createRectangle(1.5, 1.0, "black");
                break;
            case 10: // Gray triangles
                flyweight = ShapeFlyweightFactory::createTriangle(1.5, 1.0, "gray");
                break;
            case 11: // Purple circles
                flyweight = ShapeFlyweightFactory::createCircle(1.1, "purple");
                break;
        }
        
        largeScaleShapes.push_back(std::make_unique<FlyweightShape>(flyweight, x, y, 1.0));
    }
    
    auto endLarge = std::chrono::high_resolution_clock::now();
    auto largeScaleDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endLarge - startLarge);

    std::cout << "Created 5000 shapes using only " << ShapeFlyweightFactory::getFlyweightCount()
              << " unique flyweights\n";
    std::cout << "Time taken: " << largeScaleDuration.count() << " milliseconds\n";
    std::cout << "Memory savings: " << (5000 - ShapeFlyweightFactory::getFlyweightCount())
              << " objects avoided through sharing\n";
    std::cout << "Sharing efficiency: " <<
              static_cast<double>(5000 - ShapeFlyweightFactory::getFlyweightCount()) / 5000.0 * 100.0
              << "%\n\n";
    
    // Test 13: Comparison with No Pattern Implementation
    std::cout << "13. Comparison: Flyweight Pattern vs No Pattern:\n";
    
    std::cout << "Without Flyweight pattern (direct instantiation):\n";
    std::cout << "  - Each shape object stores its own geometry and rendering properties\n";
    std::cout << "  - Memory usage is proportional to number of objects\n";
    std::cout << "  - Creating many similar objects is memory intensive\n";
    std::cout << "  - Changes to rendering properties require updating multiple objects\n\n";
    
    std::cout << "With Flyweight pattern (shared intrinsic state):\n";
    std::cout << "  - Common properties are shared between similar objects\n";
    std::cout << "  - Memory usage is proportional to number of unique property combinations\n";
    std::cout << "  - Creating many similar objects is memory efficient\n";
    std::cout << "  - Changes to shared properties affect all similar objects\n\n";
    
    // Test 14: Benefits Summary
    std::cout << "14. Flyweight Pattern Benefits Summary:\n";
    std::cout << "- Memory Efficiency: Significant reduction in memory usage through sharing\n";
    std::cout << "- Performance: Faster object creation (reuse vs construction)\n";
    std::cout << "- Resource Management: Consolidated resource handling\n";
    std::cout << "- Scalability: Handles large numbers of similar objects efficiently\n";
    std::cout << "- Flexibility: Easy to add new flyweight types\n";
    std::cout << "- Separation of Concerns: Clear distinction between shared and unique state\n";
    std::cout << std::endl;
    
    // Test 15: Proper Cleanup
    std::cout << "15. Cleanup and Shutdown:\n";
    std::cout << "Resetting factory and clearing all flyweights...\n";
    ShapeFlyweightFactory::reset();
    std::cout << "Factory state cleared. Current flyweight count: " 
              << ShapeFlyweightFactory::getFlyweightCount() << std::endl;
    
    std::cout << "\nFlyweight pattern demo completed successfully!\n";
    std::cout << "\nKey Principles Demonstrated:\n";
    std::cout << "- Separation of intrinsic (shared) and extrinsic (unique) state\n";
    std::cout << "- Efficient memory usage through object sharing\n";
    std::cout << "- Transparent client interface that hides complexity\n";
    std::cout << "- Runtime flexibility to dynamically create and share objects\n";
    std::cout << "- Scalability for systems with many similar objects\n";
    
    return 0;
}