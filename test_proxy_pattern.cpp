#include "proxy_shapes.h"
#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
#include <thread>

int main() {
    std::cout << "=== Proxy Pattern Demo: Shape Rendering Proxies ===\n\n";
    
    // Test 1: Remote Proxy Simulation
    std::cout << "1. Remote Proxy Simulation:\n";
    
    auto remoteCircleProxy = std::make_unique<RemoteShapeProxy>(
        std::make_unique<Circle>(5.0, "red"), 
        "server1.example.com"
    );
    remoteCircleProxy->draw();
    std::cout << "Remote circle area: " << remoteCircleProxy->getArea() << std::endl;
    std::cout << std::endl;
    
    auto remoteRectProxy = std::make_unique<RemoteShapeProxy>(
        std::make_unique<Rectangle>(8.0, 6.0, "blue"), 
        "server2.example.com"
    );
    remoteRectProxy->draw();
    std::cout << "Remote rectangle area: " << remoteRectProxy->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 2: Virtual Proxy (lazy initialization)
    std::cout << "2. Virtual Proxy (Lazy Initialization):\n";
    
    std::cout << "Creating virtual proxy (expensive object not created yet):\n";
    auto virtualTriangleProxy = std::make_unique<VirtualShapeProxy>(
        []() { return std::make_unique<Triangle>(10.0, 8.0, "green"); }
    );
    std::cout << "Virtual proxy created, but underlying triangle not initialized yet\n";
    
    std::cout << "Accessing virtual proxy (now creates the expensive object):\n";
    virtualTriangleProxy->draw();
    std::cout << "Virtual triangle area: " << virtualTriangleProxy->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 3: Protection Proxy
    std::cout << "3. Protection Proxy:\n";
    
    auto protectedCircle = std::make_unique<ProtectedShapeProxy>(
        std::make_unique<Circle>(3.0, "purple"), 
        "admin"
    );
    protectedCircle->draw();
    std::cout << "Protected circle area: " << protectedCircle->getArea() << std::endl;
    
    auto unauthorizedCircle = std::make_unique<ProtectedShapeProxy>(
        std::make_unique<Circle>(3.0, "purple"), 
        "guest"
    );
    std::cout << "Attempting to access unauthorized (guest access):\n";
    unauthorizedCircle->draw();
    std::cout << "Unauthorized area access: " << unauthorizedCircle->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 4: Smart Proxy with Reference Counting
    std::cout << "4. Smart Proxy with Reference Counting:\n";
    
    auto smartCircle = std::make_unique<SmartShapeProxy>(std::make_unique<Circle>(4.0, "orange"));
    std::cout << "Reference count after creation: " << smartCircle->getReferenceCount() << std::endl;
    
    // Simulate sharing the proxy
    auto anotherRef = smartCircle.get();  // Not actually sharing since it's unique_ptr, but shows the counter concept
    std::cout << "Simulated shared access - reference count remains: " << smartCircle->getReferenceCount() << std::endl;
    
    smartCircle->draw();
    std::cout << "Smart circle area: " << smartCircle->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 5: Caching Proxy
    std::cout << "5. Caching Proxy:\n";
    
    auto cachingRect = std::make_unique<CachingShapeProxy>(std::make_unique<Rectangle>(7.0, 5.0, "cyan"));
    std::cout << "First access to area (computes and caches): ";
    double area1 = cachingRect->getArea();
    std::cout << area1 << std::endl;
    
    std::cout << "Second access to area (uses cache): ";
    double area2 = cachingRect->getArea();
    std::cout << area2 << std::endl;
    
    std::cout << "Third access to area (still uses cache): ";
    double area3 = cachingRect->getArea();
    std::cout << area3 << std::endl;
    
    cachingRect->draw();
    std::cout << std::endl;
    
    // Test 6: Logging Proxy
    std::cout << "6. Logging Proxy:\n";
    
    auto loggingCircle = std::make_unique<LoggingShapeProxy>(std::make_unique<Circle>(2.5, "pink"));
    loggingCircle->draw();
    std::cout << "Logged circle area: " << loggingCircle->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 7: Multiple Proxy Layers
    std::cout << "7. Multiple Proxy Layers (Proxy Chaining):\n";
    
    auto chainedProxy = std::make_unique<LoggingShapeProxy>(
        std::make_unique<CachingShapeProxy>(
            std::make_unique<VirtualShapeProxy>(
                []() { return std::make_unique<Circle>(6.0, "rainbow"); }
            )
        )
    );
    
    std::cout << "Accessing through chained proxies:\n";
    chainedProxy->draw();
    std::cout << "Chained proxy area: " << chainedProxy->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 8: Performance Comparison
    std::cout << "8. Performance Comparison:\n";
    
    // Direct access performance
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
        auto directShape = std::make_unique<Circle>(1.0 + (i % 3), "color" + std::to_string(i % 5));
        double area = directShape->getArea();
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto directTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // Proxy access performance (with virtual proxy overhead)
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
        auto virtualShape = std::make_unique<VirtualShapeProxy>(
            [i]() { return std::make_unique<Circle>(1.0 + (i % 3), "color" + std::to_string(i % 5)); }
        );
        double area = virtualShape->getArea();
    }
    end = std::chrono::high_resolution_clock::now();
    auto proxyTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Direct creation and calculation (1000 iterations): " << directTime.count() << " microseconds\n";
    std::cout << "Virtual proxy creation and calculation (1000 iterations): " << proxyTime.count() << " microseconds\n";
    std::cout << std::endl;
    
    // Test 9: Authorization Levels with Protection Proxy
    std::cout << "9. Authorization Levels with Protection Proxy:\n";
    
    std::vector<std::string> accessLevels = {"guest", "user", "admin", "superadmin"};
    for (const auto& level : accessLevels) {
        std::cout << "Access level '" << level << "':\n";
        auto protectedShape = std::make_unique<ProtectedShapeProxy>(
            std::make_unique<Triangle>(5.0, 4.0, "multicolor"), 
            level
        );
        protectedShape->draw();
        std::cout << "Area accessible: " << protectedShape->getArea() << std::endl;
        std::cout << std::endl;
    }
    
    // Test 10: Real-world Scenario - Graphics Pipeline
    std::cout << "10. Real-world Scenario - Graphics Pipeline:\n";
    
    // Simulate a graphics pipeline with multiple proxy types
    std::vector<std::unique_ptr<Shape>> graphicsPipeline;
    
    // Add shapes with different proxy types to simulate a graphics system
    graphicsPipeline.push_back(std::make_unique<RemoteShapeProxy>(
        std::make_unique<Circle>(3.0, "pipeline_circle"), 
        "graphics_server"
    ));
    
    graphicsPipeline.push_back(std::make_unique<VirtualShapeProxy>(
        []() { return std::make_unique<Rectangle>(4.0, 3.0, "pipeline_rectangle"); }
    ));
    
    graphicsPipeline.push_back(std::make_unique<LoggingShapeProxy>(
        std::make_unique<Triangle>(6.0, 4.0, "pipeline_triangle")
    ));
    
    graphicsPipeline.push_back(std::make_unique<CachingShapeProxy>(
        std::make_unique<Circle>(2.0, "pipeline_cached_circle")
    ));
    
    std::cout << "Graphics pipeline with " << graphicsPipeline.size() << " shapes:\n";
    for (size_t i = 0; i < graphicsPipeline.size(); ++i) {
        std::cout << "  Shape " << (i + 1) << ": ";
        graphicsPipeline[i]->draw();
    }
    std::cout << std::endl;
    
    // Test 11: Proxy Factory Usage
    std::cout << "11. Proxy Factory Usage:\n";
    
    auto factoryProxy = std::make_unique<ShapeProxyFactory>();
    
    // Create different proxied shapes using factory
    auto factoryCircle = factoryProxy->createProtectedShape("circle", 4.0, "factory_red", "admin");
    auto factoryRect = factoryProxy->createLoggingShape("rectangle", 5.0, 3.0, "factory_blue");
    auto factoryTriangle = factoryProxy->createCachingShape("triangle", 6.0, 4.0, "factory_green");
    
    std::cout << "Factory-created protected shape: ";
    factoryCircle->draw();
    std::cout << "Factory-created logging shape: ";
    factoryRect->draw();
    std::cout << "Factory-created caching shape: ";
    factoryTriangle->draw();
    std::cout << std::endl;
    
    // Test 12: Remote Operations Simulation
    std::cout << "12. Remote Operations Simulation:\n";
    
    auto remoteOperations = std::make_unique<RemoteShapeProxy>(
        std::make_unique<Rectangle>(10.0, 8.0, "operation_target"), 
        "calculation_server"
    );
    
    std::cout << "Performing remote calculations:\n";
    std::cout << "  Area: " << remoteOperations->getArea() << std::endl;
    std::cout << "  Remote proxy performed network operation simulation\n";
    remoteOperations->draw();
    std::cout << std::endl;
    
    // Test 13: Complex Shape with Multiple Proxies
    std::cout << "13. Complex Shape with Multiple Proxy Types:\n";
    
    // Create a complex shape composition with multiple proxy layers
    auto complexShape = std::make_unique<LoggingShapeProxy>(
        std::make_unique<CachingShapeProxy>(
            std::make_unique<ProtectedShapeProxy>(
                std::make_unique<VirtualShapeProxy>(
                    []() { return std::make_unique<CompositeShape>( /* complex shape data */ ); }
                ),
                "admin"
            )
        )
    );
    
    std::cout << "Complex shape with logging, caching, protection, and virtual proxies:\n";
    complexShape->draw();
    std::cout << "Complex shape area: " << complexShape->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 14: Proxy Pattern Benefits Demonstration
    std::cout << "14. Proxy Pattern Benefits Demonstration:\n";
    std::cout << "- Remote Proxy: Simulates access to remote objects transparently\n";
    std::cout << "- Virtual Proxy: Defers expensive object creation until needed\n";
    std::cout << "- Protection Proxy: Controls access based on permissions\n";
    std::cout << "- Caching Proxy: Improves performance by caching expensive computations\n";
    std::cout << "- Logging Proxy: Adds transparent logging capabilities\n";
    std::cout << "- Smart Proxy: Adds reference counting and management\n";
    std::cout << "- Proxy Chaining: Multiple behaviors can be layered\n";
    std::cout << "- Transparency: Client code works the same with or without proxies\n";
    std::cout << std::endl;
    
    // Test 15: Memory-Efficient Proxy Usage
    std::cout << "15. Memory-Efficient Proxy Usage:\n";
    
    std::cout << "Creating 1000 virtual proxies (objects not created until accessed):\n";
    std::vector<std::unique_ptr<VirtualShapeProxy>> virtualProxies;
    for (int i = 0; i < 1000; ++i) {
        virtualProxies.push_back(std::make_unique<VirtualShapeProxy>(
            [i]() { return std::make_unique<Circle>(1.0 + (i % 5), "virtual_" + std::to_string(i % 3)); }
        ));
    }
    std::cout << "Created 1000 virtual proxies, but underlying objects not created yet\n";
    
    // Only create a few underlying objects
    for (int i = 0; i < 10; ++i) {
        virtualProxies[i]->getArea(); // This triggers actual object creation
    }
    std::cout << "Only 10 underlying objects created when actually needed\n";
    std::cout << std::endl;
    
    // Test 16: Proxy Pattern Limitations Awareness
    std::cout << "16. Proxy Pattern Considerations:\n";
    std::cout << "- Added complexity to the system\n";
    std::cout << "- Potential performance overhead from proxy layers\n";
    std::cout << "- Increased number of classes in the design\n";
    std::cout << "- Possible security issues if proxies aren't properly designed\n";
    std::cout << "- Debugging can be more challenging through proxy layers\n";
    std::cout << std::endl;
    
    std::cout << "Proxy pattern demo completed successfully!\n";
    std::cout << "\nKey Benefits of Proxy Pattern:\n";
    std::cout << "- Controls access to the original object\n";
    std::cout << "- Provides a surrogate or placeholder for another object\n";
    std::cout << "- Adds functionality like caching, logging, or security without changing client code\n";
    std::cout << "- Enables lazy initialization of expensive objects\n";
    std::cout << "- Supports transparent remote object access\n";
    std::cout << "- Facilitates conditional access to objects based on permissions\n";
    
    return 0;
}