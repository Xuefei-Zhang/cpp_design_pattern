#include "structural/corrected_proxy_patterns.h"
#include <iostream>
#include <memory>
#include <chrono>

int main() {
    std::cout << "=== Proxy Pattern Demo: Shape Management System ===\n\n";
    
    // Test 1: Basic Proxy Usage
    std::cout << "1. Basic Proxy Pattern Usage:\n";
    
    auto circle = std::make_unique<Circle>(5.0, "red", "BasicCircle");
    circle->draw();
    std::cout << "Circle area: " << circle->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 2: Remote Proxy
    std::cout << "2. Remote Proxy - Simulating Remote Access:\n";
    
    auto remoteCircle = std::make_unique<RemoteShapeProxy>(
        std::make_unique<Circle>(4.0, "blue", "RemoteCircle"), 
        "graphics.remote.server.com", 0.1
    );
    
    std::cout << "Accessing remote shape:\n";
    remoteCircle->draw();
    std::cout << "Remote shape area: " << remoteCircle->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 3: Virtual Proxy (Lazy Initialization)
    std::cout << "3. Virtual Proxy - Lazy Initialization:\n";
    
    std::cout << "Creating virtual proxy (shape not created yet):\n";
    auto virtualTriangle = std::make_unique<VirtualShapeProxy>(
        []() { return std::make_unique<Triangle>(8.0, 6.0, "green", "VirtualTriangle"); }
    );
    
    std::cout << "Virtual proxy created, but underlying triangle not initialized\n";
    std::cout << "Accessing virtual proxy (now creates the shape):\n";
    virtualTriangle->draw();
    std::cout << "Virtual triangle area: " << virtualTriangle->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 4: Protection Proxy
    std::cout << "4. Protection Proxy - Access Control:\n";
    
    auto protectedCircle = std::make_unique<ProtectionProxy>(
        std::make_unique<Circle>(6.0, "purple", "ProtectedCircle"), 
        "admin", "admin"  // Required permission: admin, User role: admin (granted)
    );
    
    std::cout << "Admin access granted:\n";
    protectedCircle->draw();
    std::cout << "Protected circle area: " << protectedCircle->getArea() << std::endl;
    
    std::cout << "\nTrying guest access (should be denied):\n";
    auto guestProtectedCircle = std::make_unique<ProtectionProxy>(
        std::make_unique<Circle>(6.0, "purple", "ProtectedCircle"), 
        "admin", "guest"  // Required permission: admin, User role: guest (denied)
    );
    guestProtectedCircle->draw();
    double area = guestProtectedCircle->getArea(); 
    std::cout << "Guest access area result: " << area << " (error indicator)\n";
    std::cout << std::endl;
    
    // Test 5: Caching Proxy
    std::cout << "5. Caching Proxy - Performance Optimization:\n";
    
    auto cachingRectangle = std::make_unique<CachingProxy>(
        std::make_unique<Rectangle>(7.0, 5.0, "cyan", "CachingRectangle")
    );
    
    std::cout << "First area access (calculates and caches):\n";
    double area1 = cachingRectangle->getArea();
    std::cout << "Area: " << area1 << std::endl;
    
    std::cout << "\nSecond area access (uses cache):\n";
    double area2 = cachingRectangle->getArea();
    std::cout << "Area: " << area2 << std::endl;
    
    std::cout << "\nFirst type access (fetches and caches):\n";
    std::string type1 = cachingRectangle->getType();
    std::cout << "Type: " << type1 << std::endl;
    
    std::cout << "\nSecond type access (uses cache):\n";
    std::string type2 = cachingRectangle->getType();
    std::cout << "Type: " << type2 << std::endl;
    
    cachingRectangle->draw();
    std::cout << std::endl;
    
    // Test 6: Logging Proxy
    std::cout << "6. Logging Proxy - Operation Tracking:\n";
    
    auto loggingTriangle = std::make_unique<LoggingProxy>(
        std::make_unique<Triangle>(10.0, 8.0, "orange", "LoggingTriangle")
    );
    
    std::cout << "Operations with logging proxy:\n";
    loggingTriangle->draw();
    double logArea = loggingTriangle->getArea();
    std::cout << "Triangle area: " << logArea << std::endl;
    std::cout << "Triangle type: " << loggingTriangle->getType() << std::endl;
    loggingTriangle->printInfo();
    
    std::cout << "\nOperation log contents:\n";
    static_cast<LoggingProxy*>(loggingTriangle.get())->printLog();
    std::cout << std::endl;
    
    // Test 7: Enhanced Proxy
    std::cout << "7. Enhanced Proxy - Styling and Scaling:\n";
    
    auto enhancedCircle = std::make_unique<EnhancedProxy>(
        std::make_unique<Circle>(4.0, "enhanced_color", "EnhancedCircle"),
        "fancy_style", 1.5, true
    );
    
    enhancedCircle->draw();
    std::cout << "Enhanced circle area (scaled): " << enhancedCircle->getArea() << std::endl;
    enhancedCircle->printInfo();
    
    // Change properties
    std::cout << "\nChanging scale to 2.0:\n";
    enhancedCircle->setScale(2.0);
    std::cout << "Area with new scale: " << enhancedCircle->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 8: Proxy Factory Usage
    std::cout << "8. Proxy Factory - Easy Proxy Creation:\n";
    
    // Create different types of proxies using the factory
    auto factoryCircle = ShapeProxyFactory::createRemoteProxy(
        std::make_unique<Circle>(2.0, "factory_color", "FactoryCircle"),
        "remote.factory.server.com"
    );
    factoryCircle->draw();
    
    auto factoryProtectedRect = ShapeProxyFactory::createProtectionProxy(
        std::make_unique<Rectangle>(5.0, 4.0, "factory_color", "FactoryProtectedRect"),
        "user", "user"
    );
    factoryProtectedRect->draw();
    std::cout << std::endl;
    
    // Test 9: Multiple Proxy Layers (Proxy Chaining)
    std::cout << "9. Multiple Proxy Layers (Proxy Chaining):\n";
    
    // Chain: Logging -> Caching -> Protection -> Circle
    auto chainedProxy = std::make_unique<LoggingProxy>(
        std::make_unique<CachingProxy>(
            std::make_unique<ProtectionProxy>(
                std::make_unique<Circle>(3.0, "chain_color", "ChainedCircleProxy"),
                "user", "user"
            )
        )
    );
    
    std::cout << "Accessing shape through chained proxies:\n";
    chainedProxy->draw();
    std::cout << "Area: " << chainedProxy->getArea() << std::endl;
    std::cout << "Type: " << chainedProxy->getType() << std::endl;
    std::cout << std::endl;
    
    // Test 10: Multi-Feature Proxy (Combining multiple features)
    std::cout << "10. Multi-Feature Proxy - Combining Multiple Capabilities:\n";
    
    auto multiFeatureShape = std::make_unique<MultiFeatureProxy>(
        std::make_unique<Rectangle>(8.0, 6.0, "multi_color", "MultiFeatureRect"),
        true,    // Enable logging
        true,    // Enable caching 
        true,    // Enable protection
        "user",  // Required permission
        "user"   // Current user role (granted access)
    );
    
    std::cout << "Accessing multi-feature proxy:\n";
    multiFeatureShape->draw();
    std::cout << "Area: " << multiFeatureShape->getArea() << std::endl;
    std::cout << "Type: " << multiFeatureShape->getType() << std::endl;
    std::cout << std::endl;
    
    // Test 11: Smart Proxy (Reference Counting)
    std::cout << "11. Smart Proxy - Reference Counting Simulation:\n";
    
    auto smartCircle = std::make_unique<SmartShapeProxy>(
        std::make_unique<Circle>(2.5, "smart_color", "SmartCircle"), 
        "ShapeManager"
    );
    
    std::cout << "Smart proxy properties:\n";
    smartCircle->draw();
    std::cout << "Area: " << smartCircle->getArea() << std::endl;
    std::cout << "Reference count: " << smartCircle->getReferenceCount() << std::endl;
    std::cout << std::endl;
    
    // Test 12: Proxy Manager Usage
    std::cout << "12. Proxy Manager - Managing Multiple Proxied Shapes:\n";
    
    ProxyManager manager("GraphicsShapeManager");
    
    // Add various shapes with different proxies to the manager
    manager.addShape(std::make_unique<RemoteShapeProxy>(
        std::make_unique<Circle>(3.0, "mgr_circle_color", "ManagedRemoteCircle"), 
        "mgr_server.com"
    ));
    
    manager.addShape(std::make_unique<LoggingProxy>(
        std::make_unique<Rectangle>(6.0, 4.0, "mgr_rect_color", "ManagedLoggingRect")
    ));
    
    manager.addShape(std::make_unique<ProtectionProxy>(
        std::make_unique<Triangle>(7.0, 5.0, "mgr_tri_color", "ManagedProtectedTriangle"),
        "user", "user"
    ));
    
    manager.addShape(std::make_unique<CachingProxy>(
        std::make_unique<Circle>(2.5, "mgr_cache_color", "ManagedCachingCircle")
    ));
    
    std::cout << "Manager contains " << manager.getShapeCount() << " shapes with different proxies\n";
    std::cout << "Rendering all managed shapes:\n";
    manager.renderAll();
    
    std::cout << "Total area of managed shapes: " << manager.calculateTotalArea() << std::endl;
    std::cout << std::endl;
    
    // Test 13: Performance Comparison
    std::cout << "13. Performance Comparison - Direct vs Proxy Access:\n";
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Direct access performance
    for (int i = 0; i < 1000; ++i) {
        auto directShape = std::make_unique<Circle>(1.0 + (i % 3), "direct", "DirectShape" + std::to_string(i % 100));
        double area = directShape->getArea();
    }
    
    auto directEnd = std::chrono::high_resolution_clock::now();
    auto directDuration = std::chrono::duration_cast<std::chrono::microseconds>(directEnd - start);
    
    // Proxy access performance
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
        auto proxiedShape = std::make_unique<LoggingProxy>(
            std::make_unique<Circle>(1.0 + (i % 3), "logged", "LoggedShape" + std::to_string(i % 100))
        );
        double area = proxiedShape->getArea();
    }
    
    auto proxyEnd = std::chrono::high_resolution_clock::now();
    auto proxyDuration = std::chrono::duration_cast<std::chrono::microseconds>(proxyEnd - start);
    
    std::cout << "Direct creation and area calc (1000 iterations): " 
              << directDuration.count() << " microseconds\n";
    std::cout << "Proxy creation and area calc (1000 iterations): " 
              << proxyDuration.count() << " microseconds\n";
    std::cout << "Proxy overhead: " << (proxyDuration.count() - directDuration.count()) << " microseconds\n";
    std::cout << std::endl;
    
    // Test 14: Different Shape Types with Proxies
    std::cout << "14. Different Shape Types with Proxies:\n";
    
    std::vector<std::unique_ptr<Shape>> shapesWithProxies;
    
    // Add different shapes with different proxy configurations
    shapesWithProxies.push_back(ShapeProxyFactory::createCachingProxy(
        std::make_unique<Circle>(3.0, "red", "RedCircleProxy")
    ));
    
    shapesWithProxies.push_back(ShapeProxyFactory::createLoggingProxy(
        std::make_unique<Rectangle>(8.0, 6.0, "blue", "BlueRectangleProxy")
    ));
    
    shapesWithProxies.push_back(ShapeProxyFactory::createRemoteProxy(
        std::make_unique<Triangle>(10.0, 8.0, "green", "GreenTriangleProxy"),
        "remote.shapeserver.com"
    ));
    
    shapesWithProxies.push_back(std::make_unique<EnhancedProxy>(
        std::make_unique<Circle>(2.5, "enhanced", "EnhancedCircleProxy"),
        "glow_style", 1.2, true
    ));
    
    for (size_t i = 0; i < shapesWithProxies.size(); ++i) {
        std::cout << "Shape " << (i + 1) << ": " << shapesWithProxies[i]->getType() << std::endl;
        shapesWithProxies[i]->draw();
        std::cout << "  Area: " << shapesWithProxies[i]->getArea() << std::endl << std::endl;
    }
    
    // Test 15: Real-world Scenario - Graphics Framework
    std::cout << "15. Real-world Scenario - Integrated Graphics Framework:\n";
    
    // Simulate a complex graphics application with different needs
    std::vector<std::tuple<std::string, std::string, std::unique_ptr<Shape>>> graphicsComponents = {
        {"UI Element", "Basic", std::make_unique<ProtectionProxy>(std::make_unique<Rectangle>(100.0, 30.0, "lightgray", "UIButton"), "public", "guest")},
        {"Game Object", "Complex", ShapeProxyFactory::createEnhancedProxy(std::make_unique<Circle>(10.0, "object_color", "GameObject"), "glowing", 1.0, true)},
        {"Background", "Remote", ShapeProxyFactory::createRemoteProxy(std::make_unique<Rectangle>(800.0, 600.0, "bg_color", "Background"), "remote.bgserver.com")},
        {"Effect", "Caching", ShapeProxyFactory::createCachingProxy(std::make_unique<Circle>(5.0, "effect_color", "ParticleEffect"))},
        {"Icon", "Logging", ShapeProxyFactory::createLoggingProxy(std::make_unique<Triangle>(8.0, 8.0, "icon_color", "AppIcon"))}
    };
    
    for (auto& [componentType, proxyType, shape] : graphicsComponents) {
        std::cout << componentType << " with " << proxyType << " proxy: ";
        shape->draw();
        std::cout << "  Area: " << shape->getArea() << std::endl;
    }
    std::cout << std::endl;
    
    // Test 16: Proxy Switching and Dynamic Configuration
    std::cout << "16. Dynamic Proxy Configuration:\n";
    
    auto dynamicShape = std::make_unique<ProtectionProxy>(
        std::make_unique<Circle>(4.0, "dynamic_color", "DynamicCircle"), 
        "user", "guest"  // Initially denied access
    );
    
    std::cout << "Initial access level (guest - denied):\n";
    dynamicShape->draw();
    
    // Change access level to user (granted access)
    dynamicShape->setCurrentUserRole("user");
    std::cout << "\nAfter changing to user role (granted access):\n";
    dynamicShape->draw();
    std::cout << "Area with user access: " << dynamicShape->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 17: Proxy Pattern Benefits Demonstration
    std::cout << "17. Proxy Pattern Benefits Demonstration:\n";
    std::cout << "- Remote Proxy: Transparent network communication simulation\n";
    std::cout << "- Virtual Proxy: Lazy initialization of expensive objects\n";
    std::cout << "- Protection Proxy: Access control without modifying original objects\n";
    std::cout << "- Caching Proxy: Performance improvement through result caching\n";
    std::cout << "- Logging Proxy: Transparent operation logging and monitoring\n";
    std::cout << "- Enhanced Proxy: Added functionality without changing core classes\n";
    std::cout << "- Factory Integration: Easy proxy creation and management\n";
    std::cout << "- Proxy Chaining: Multiple behaviors combined\n";
    std::cout << "- Transparency: Same interface for all shapes regardless of proxy wrapping\n";
    std::cout << std::endl;
    
    // Test 18: Memory Efficiency with Virtual Proxy
    std::cout << "18. Virtual Proxy Memory Efficiency Test:\n";
    
    std::cout << "Creating 1000 virtual proxies (memory efficient - no objects created yet):\n";
    std::vector<std::unique_ptr<VirtualShapeProxy>> virtualProxies;
    for (int i = 0; i < 1000; ++i) {
        virtualProxies.push_back(std::make_unique<VirtualShapeProxy>(
            [i]() { 
                return std::make_unique<Circle>(
                    1.0 + (i % 3), 
                    "virtual_color_" + std::to_string(i % 5), 
                    "VirtualCircle_" + std::to_string(i)
                ); 
            }
        ));
    }
    std::cout << "Created 1000 virtual proxies, but underlying objects remain uninitialized\n";
    
    std::cout << "Accessing only 10 virtual proxies (now instantiate the objects):\n";
    for (int i = 0; i < 10; ++i) {
        double area = virtualProxies[i]->getArea(); // This triggers object creation
        if (i < 3) { // Print first 3 for demonstration
            std::cout << "  Virtual proxy " << i << " area: " << area << std::endl;
        }
    }
    std::cout << "Only " << 10 << " of " << 1000 << " objects were actually created\n";
    std::cout << std::endl;
    
    // Test 19: Complex Proxy Chain Example
    std::cout << "19. Complex Proxy Chain - Multiple Layered Proxies:\n";
    
    // Create very complex proxy chain: Logging -> Caching -> MultiFeature -> Protection -> Remote -> Shape
    auto complexChain = std::make_unique<LoggingProxy>(
        std::make_unique<CachingProxy>(
            std::make_unique<MultiFeatureProxy>(
                std::make_unique<ProtectionProxy>(
                    std::make_unique<RemoteShapeProxy>(
                        std::make_unique<Rectangle>(12.0, 8.0, "complex", "ComplexChainRect"),
                        "complex.server.com", 0.05
                    ),
                    "user", "user"  // Granted access
                ),
                true,  // logging
                true,  // caching
                true   // protection
            )
        )
    );
    
    std::cout << "Accessing through complex proxy chain:\n";
    complexChain->draw();
    std::cout << "Area: " << complexChain->getArea() << std::endl;
    std::cout << "Type: " << complexChain->getType() << std::endl;
    std::cout << std::endl;
    
    // Test 20: Factory for Complex Proxy Creation
    std::cout << "20. Factory for Complex Proxy Creation:\n";
    
    auto complexShape = ShapeProxyFactory::createFullFeaturedShape(
        std::make_unique<Circle>(3.5, "factory_complex", "FactoryComplexCircle"),
        "complex.server.com",  // For remote access
        "user",                // Permission required
        "user",                // User role
        "glowing",             // Style
        1.3                    // Scale
    );
    
    std::cout << "Creating complex shape with factory (includes logging, protection, enhancement):\n";
    complexShape->draw();
    std::cout << "Complex shape area: " << complexShape->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 21: Extensibility - Adding New Proxy Types
    std::cout << "21. Proxy Pattern Extensibility:\n";
    std::cout << "- New proxy types can be created by implementing the Shape interface\n";
    std::cout << "- Existing code doesn't need modification when adding new proxies\n";
    std::cout << "- Proxy factory can be extended to create new proxy types\n";
    std::cout << "- Same interface maintained for all proxy types\n";
    std::cout << std::endl;
    
    // Test 22: Comparison with Direct Usage
    std::cout << "22. Proxy vs Direct Usage Comparison:\n";
    
    std::cout << "\nDirect usage (no proxy):\n";
    Circle directCircle(4.0, "direct_color", "DirectCircle");
    directCircle.draw();
    std::cout << "Area: " << directCircle.getArea() << std::endl;
    
    std::cout << "\nWith proxy (adds functionality transparently):\n";
    auto proxiedCircle = std::make_unique<EnhancedProxy>(
        std::make_unique<LoggingProxy>(
            std::make_unique<CachingProxy>(
                std::make_unique<Circle>(4.0, "proxied_color", "ProxiedCircle")
            )
        ),
        "enhanced_style", 1.2, true
    );
    proxiedCircle->draw();
    std::cout << "Enhanced area: " << proxiedCircle->getArea() << std::endl;
    
    std::cout << "\nBenefits of Proxy Pattern:\n";
    std::cout << "- Transforms a simple object into a more powerful object\n";
    std::cout << "- Provides control over how the object is accessed\n";
    std::cout << "- Allows for cross-cutting concerns (logging, security, caching)\n";
    std::cout << "- Maintains the same interface while adding functionality\n";
    std::cout << "- Enables lazy initialization and remote access transparently\n";
    std::cout << std::endl;
    
    // Test 23: Clean Shutdown
    std::cout << "23. Clean System Shutdown:\n";
    manager.clear();
    std::cout << "Proxy manager cleared, all resources released\n";
    
    std::cout << "\nProxy pattern demo completed successfully!\n";
    std::cout << "\nKey Benefits of Proxy Pattern:\n";
    std::cout << "- Provides a surrogate or placeholder for another object to control access\n";
    std::cout << "- Enables additional functionality without modifying original object\n";
    std::cout << "- Supports multiple types of proxy behavior (remote, virtual, protection, etc.)\n";
    std::cout << "- Maintains the same interface as the original object\n";
    std::cout << "- Supports the Single Responsibility principle\n";
    std::cout << "- Enables transparent remote access\n";
    std::cout << "- Supports lazy initialization and performance optimization\n";
    std::cout << "- Facilitates access control and security\n";
    
    return 0;
}