#include "bridge_patterns.h"
#include <iostream>
#include <memory>
#include <vector>
#include <cmath>
#include <chrono>

int main() {
    std::cout << "=== Bridge Pattern Demo: Shape Rendering Abstraction ===\n\n";
    
    // Test 1: Basic Bridge Usage
    std::cout << "1. Basic Bridge Pattern Usage:\n";
    
    // Circle with vector renderer
    auto vectorRenderer1 = std::make_unique<VectorRenderer>();
    auto circle1 = std::make_unique<Circle>(5.0, "red", std::move(vectorRenderer1));
    circle1->draw();
    std::cout << "Circle area: " << circle1->getArea() << std::endl;
    
    // Circle with raster renderer
    auto rasterRenderer1 = std::make_unique<RasterRenderer>();
    auto circle2 = std::make_unique<Circle>(5.0, "blue", std::move(rasterRenderer1));
    circle2->draw();
    std::cout << "Circle area: " << circle2->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 2: Rectangle with different renderers
    std::cout << "2. Rectangle with Different Renderers:\n";
    
    auto svgRenderer1 = std::make_unique<SVGRenderer>();
    auto rect1 = std::make_unique<Rectangle>(8.0, 6.0, "green", std::move(svgRenderer1));
    rect1->draw();
    std::cout << "Rectangle area: " << rect1->getArea() << std::endl;
    
    auto vectorRenderer2 = std::make_unique<VectorRenderer>();
    auto rect2 = std::make_unique<Rectangle>(8.0, 6.0, "yellow", std::move(vectorRenderer2));
    rect2->draw();
    std::cout << "Rectangle area: " << rect2->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 3: Runtime Renderer Switching
    std::cout << "3. Runtime Renderer Switching:\n";
    
    auto circle3 = std::make_unique<Circle>(4.0, "purple", std::make_unique<VectorRenderer>());
    std::cout << "Original renderer: " << circle3->getRenderer()->getRendererType() << std::endl;
    circle3->draw();
    
    // Switch renderer at runtime
    circle3->setRenderer(std::make_unique<RasterRenderer>());
    std::cout << "After switching renderer: " << circle3->getRenderer()->getRendererType() << std::endl;
    circle3->draw();
    
    // Switch again
    circle3->setRenderer(std::make_unique<SVGRenderer>());
    std::cout << "After switching to SVG renderer: " << circle3->getRenderer()->getRendererType() << std::endl;
    circle3->draw();
    std::cout << std::endl;
    
    // Test 4: Themed Renderers
    std::cout << "4. Themed Renderers:\n";
    
    auto modernRenderer = std::make_unique<ModernRenderer>();
    auto circle4 = std::make_unique<Circle>(6.0, "cyan", std::move(modernRenderer));
    circle4->draw();
    
    auto vintageRenderer = std::make_unique<VintageRenderer>();
    auto rect3 = std::make_unique<Rectangle>(10.0, 7.0, "brown", std::move(vintageRenderer));
    rect3->draw();
    std::cout << std::endl;
    
    // Test 5: Complex Shapes with Bridge
    std::cout << "5. Complex Shapes with Bridge:\n";
    
    auto polygon1 = std::make_unique<Polygon>(6, 5.0, std::make_unique<VectorRenderer>());
    polygon1->draw();
    std::cout << "Polygon area: " << polygon1->getArea() << std::endl;
    std::cout << "Sides: " << polygon1->getSides() << std::endl;
    
    // Switch renderer for polygon
    polygon1->switchRendererTo("raster");
    polygon1->draw();
    std::cout << std::endl;
    
    // Test 6: Animation Bridge
    std::cout << "6. Animation Bridge Pattern:\n";
    
    auto animatedCircle = std::make_unique<AnimatedShape>(
        std::make_unique<Circle>(3.0, "orange", std::make_unique<PulsingAnimation>(1.5))
    );
    
    animatedCircle->animate();
    
    // Change animation type
    animatedCircle->changeAnimation(std::make_unique<SpinningAnimation>(2.0));
    animatedCircle->animate();
    std::cout << std::endl;
    
    // Test 7: BridgeManager Usage
    std::cout << "7. Bridge Manager Usage:\n";
    
    BridgeManager manager;
    
    // Add shapes with different renderers
    manager.addShape(std::make_unique<Circle>(4.0, "red", std::make_unique<VectorRenderer>()));
    manager.addShape(std::make_unique<Rectangle>(6.0, 4.0, "blue", std::make_unique<RasterRenderer>()));
    manager.addShape(std::make_unique<Triangle>(8.0, 6.0, "green", std::make_unique<SVGRenderer>()));
    manager.addShape(std::make_unique<Polygon>(5, 4.0, std::make_unique<ModernRenderer>()));
    
    std::cout << "Initial renderer configuration:\n";
    manager.printRendererInfo();
    
    std::cout << "\nRendering all shapes:\n";
    manager.renderAll();
    
    std::cout << "\nSwitching all renderers to VectorRenderer:\n";
    manager.switchAllRenderers(std::make_unique<VectorRenderer>());
    
    std::cout << "\nNew renderer configuration:\n";
    manager.printRendererInfo();
    
    std::cout << "\nRendering all shapes with new renderers:\n";
    manager.renderAll();
    std::cout << std::endl;
    
    // Test 8: Bridge Pattern Benefits Demonstration
    std::cout << "8. Bridge Pattern Benefits Demonstration:\n";
    
    std::cout << "- Decoupling: Shape abstraction is separate from rendering implementation\n";
    std::cout << "- Extensibility: Can add new renderers without modifying shapes\n";
    std::cout << "- Runtime flexibility: Can change implementations at runtime\n";
    std::cout << "- Parallel hierarchies: Shape and renderer hierarchies evolve independently\n";
    std::cout << std::endl;
    
    // Test 9: Comparison of Different Bridge Configurations
    std::cout << "9. Comparison of Different Bridge Configurations:\n";
    
    std::vector<std::unique_ptr<Shape>> comparisonShapes;
    
    // Same circle, different renderers
    comparisonShapes.push_back(std::make_unique<Circle>(3.0, "comparison_color", std::make_unique<VectorRenderer>()));
    comparisonShapes.push_back(std::make_unique<Circle>(3.0, "comparison_color", std::make_unique<RasterRenderer>()));
    comparisonShapes.push_back(std::make_unique<Circle>(3.0, "comparison_color", std::make_unique<SVGRenderer>()));
    
    for (size_t i = 0; i < comparisonShapes.size(); ++i) {
        std::cout << "Configuration " << i + 1 << ": ";
        comparisonShapes[i]->draw();
    }
    std::cout << std::endl;
    
    // Test 10: Performance Considerations
    std::cout << "10. Performance Considerations:\n";
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Create many shapes with different renderers
    std::vector<std::unique_ptr<Shape>> performanceShapes;
    for (int i = 0; i < 1000; ++i) {
        if (i % 3 == 0) {
            performanceShapes.push_back(std::make_unique<Circle>(1.0 + i % 5, "color", std::make_unique<VectorRenderer>()));
        } else if (i % 3 == 1) {
            performanceShapes.push_back(std::make_unique<Rectangle>(1.0 + i % 5, 1.0 + i % 3, "color", std::make_unique<RasterRenderer>()));
        } else {
            performanceShapes.push_back(std::make_unique<Triangle>(1.0 + i % 5, 1.0 + i % 4, "color", std::make_unique<SVGRenderer>()));
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Created 1000 shapes with different renderers in: " << duration.count() << " microseconds\n";
    
    // Test 11: Real-world Scenario - Graphics Library Integration
    std::cout << "\n11. Real-world Scenario - Graphics Library Integration:\n";
    
    // Simulate a graphics application that needs to support multiple rendering backends
    std::vector<std::unique_ptr<Shape>> graphicsLibraryShapes;
    
    // Add UI elements with different rendering requirements
    graphicsLibraryShapes.push_back(std::make_unique<Rectangle>(100.0, 30.0, "UI_Button", std::make_unique<VectorRenderer>()));
    graphicsLibraryShapes.push_back(std::make_unique<Circle>(50.0, "UI_Icon", std::make_unique<SVGRenderer>()));
    graphicsLibraryShapes.push_back(std::make_unique<Triangle>(60.0, 40.0, "UI_Indicator", std::make_unique<RasterRenderer>()));
    graphicsLibraryShapes.push_back(std::make_unique<Polygon>(8, 25.0, std::make_unique<ModernRenderer>()));
    
    std::cout << "Graphics library rendering elements:\n";
    for (const auto& shape : graphicsLibraryShapes) {
        std::cout << "  " << shape->getType() << " rendered with " 
                  << shape->getRenderer()->getRendererType() << std::endl;
    }
    std::cout << std::endl;
    
    // Test 12: Bridge Pattern Flexibility - Runtime Selection
    std::cout << "12. Runtime Rendering Selection:\n";
    
    // Simulate user selecting different rendering preferences
    std::string userChoice = "raster"; // Could come from user preferences
    
    auto flexibleCircle = std::make_unique<Circle>(7.0, "user_color", std::make_unique<VectorRenderer>());
    std::cout << "Initial: ";
    flexibleCircle->draw();
    
    // Change based on user preference
    if (userChoice == "vector") {
        flexibleCircle->setRenderer(std::make_unique<VectorRenderer>());
    } else if (userChoice == "raster") {
        flexibleCircle->setRenderer(std::make_unique<RasterRenderer>());
    } else if (userChoice == "svg") {
        flexibleCircle->setRenderer(std::make_unique<SVGRenderer>());
    }
    
    std::cout << "After user preference: ";
    flexibleCircle->draw();
    std::cout << std::endl;
    
    // Test 13: Multiple Abstraction-Implementation Combinations
    std::cout << "13. Multiple Abstraction-Implementation Combinations:\n";
    
    struct ShapeConfig {
        std::string shapeType;
        std::string rendererType;
    };
    
    std::vector<ShapeConfig> configs = {
        {"Circle", "VectorRenderer"},
        {"Rectangle", "RasterRenderer"},
        {"Triangle", "SVGRenderer"},
        {"Circle", "ModernRenderer"},
        {"Rectangle", "VintageRenderer"}
    };
    
    for (const auto& config : configs) {
        std::unique_ptr<Shape> shape;
        
        if (config.shapeType == "Circle") {
            if (config.rendererType == "VectorRenderer") {
                shape = std::make_unique<Circle>(3.0, "dynamic_color", std::make_unique<VectorRenderer>());
            } else if (config.rendererType == "RasterRenderer") {
                shape = std::make_unique<Circle>(3.0, "dynamic_color", std::make_unique<RasterRenderer>());
            } else if (config.rendererType == "SVGRenderer") {
                shape = std::make_unique<Circle>(3.0, "dynamic_color", std::make_unique<SVGRenderer>());
            } else if (config.rendererType == "ModernRenderer") {
                shape = std::make_unique<Circle>(3.0, "dynamic_color", std::make_unique<ModernRenderer>());
            } else if (config.rendererType == "VintageRenderer") {
                shape = std::make_unique<Circle>(3.0, "dynamic_color", std::make_unique<VintageRenderer>());
            }
        } else if (config.shapeType == "Rectangle") {
            if (config.rendererType == "VectorRenderer") {
                shape = std::make_unique<Rectangle>(5.0, 4.0, "dynamic_color", std::make_unique<VectorRenderer>());
            } else if (config.rendererType == "RasterRenderer") {
                shape = std::make_unique<Rectangle>(5.0, 4.0, "dynamic_color", std::make_unique<RasterRenderer>());
            } else if (config.rendererType == "SVGRenderer") {
                shape = std::make_unique<Rectangle>(5.0, 4.0, "dynamic_color", std::make_unique<SVGRenderer>());
            } else if (config.rendererType == "ModernRenderer") {
                shape = std::make_unique<Rectangle>(5.0, 4.0, "dynamic_color", std::make_unique<ModernRenderer>());
            } else if (config.rendererType == "VintageRenderer") {
                shape = std::make_unique<Rectangle>(5.0, 4.0, "dynamic_color", std::make_unique<VintageRenderer>());
            }
        } else if (config.shapeType == "Triangle") {
            if (config.rendererType == "VectorRenderer") {
                shape = std::make_unique<Triangle>(6.0, 4.0, "dynamic_color", std::make_unique<VectorRenderer>());
            }
            // Add other renderer types for triangle as well
        }
        
        if (shape) {
            std::cout << config.shapeType << " with " << config.rendererType << ": ";
            shape->draw();
        }
    }
    std::cout << std::endl;
    
    std::cout << "Bridge pattern demo completed successfully!\n";
    std::cout << "\nKey Benefits of Bridge Pattern:\n";
    std::cout << "- Decouples abstraction from implementation\n";
    std::cout << "- Supports multiple hierarchies of classes\n";
    std::cout << "- Enables runtime switching of implementations\n";
    std::cout << "- Improves extensibility and maintainability\n";
    std::cout << "- Allows independent variation of abstractions and implementations\n";
    
    return 0;
}