#include "proxy_pattern_implementation.h"
#include <iostream>
#include <memory>
#include <chrono>

int main() {
    std::cout << "=== Proxy Pattern Demo: Shape Service Abstraction ===\n\n";
    
    // Test 1: Basic Proxy Pattern Usage
    std::cout << "1. Basic Proxy Pattern Usage:\n";
    
    auto circle = std::make_unique<Circle>(5.0, "red", "BasicCircle");
    circle->draw();
    std::cout << "Circle area: " << circle->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 2: Remote Proxy Usage
    std::cout << "2. Remote Proxy Usage:\n";
    
    auto remoteCircle = std::make_unique<RemoteShapeProxy>(
        std::make_unique<Circle>(4.0, "blue", "RemoteCircle"), 
        "graphics.example.com", 0.2  // 200ms latency
    );
    
    std::cout << "Accessing remote shape:\n";
    remoteCircle->draw();
    std::cout << "Remote shape area: " << remoteCircle->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 3: Virtual Proxy (Lazy Initialization)
    std::cout << "3. Virtual Proxy (Lazy Initialization):\n";
    
    std::cout << "Creating virtual proxy (expensive shape not created yet):\n";
    auto virtualTriangle = std::make_unique<VirtualShapeProxy>(
        []() { return std::make_unique<Triangle>(8.0, 6.0, "green", "VirtualTriangle"); }
    );
    std::cout << "Virtual proxy created, but underlying triangle not initialized yet\n";
    
    std::cout << "Accessing virtual shape (now creates the expensive object):\n";
    virtualTriangle->draw();
    std::cout << "Virtual triangle area: " << virtualTriangle->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 4: Protection Proxy
    std::cout << "4. Protection Proxy:\n";
    
    auto protectedCircle = std::make_unique<ProtectionProxy>(
        std::make_unique<Circle>(6.0, "purple", "ProtectedCircle"), 
        "admin", "admin"  // Requires admin permission, user has admin role
    );
    
    std::cout << "Admin access granted:\n";
    protectedCircle->draw();
    std::cout << "Protected circle area: " << protectedCircle->getArea() << std::endl;
    
    std::cout << "\nTrying guest access (should be denied):\n";
    auto guestProtectedCircle = std::make_unique<ProtectionProxy>(
        std::make_unique<Circle>(6.0, "purple", "ProtectedCircle"), 
        "admin", "guest"  // Requires admin permission, user has guest role
    );
    guestProtectedCircle->draw();
    std::cout << "Guest access area: " << guestProtectedCircle->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 5: Caching Proxy
    std::cout << "5. Caching Proxy:\n";
    
    auto cachingRectangle = std::make_unique<CachingProxy>(
        std::make_unique<Rectangle>(7.0, 5.0, "cyan", "CachingRectangle")
    );
    
    std::cout << "First area access (calculates and caches):\n";
    double area1 = cachingRectangle->getArea();
    std::cout << "Area: " << area1 << std::endl;
    
    std::cout << "\nSecond area access (uses cache):\n";
    double area2 = cachingRectangle->getArea();
    std::cout << "Area: " << area2 << std::endl;
    
    std::cout << "\nGetting type (first time, caches):\n";
    std::string type1 = cachingRectangle->getType();
    std::cout << "Type: " << type1 << std::endl;
    
    std::cout << "\nGetting type again (uses cache):\n";
    std::string type2 = cachingRectangle->getType();
    std::cout << "Type: " << type2 << std::endl;
    
    cachingRectangle->draw();
    std::cout << std::endl;
    
    // Test 6: Logging Proxy
    std::cout << "6. Logging Proxy:\n";
    
    auto loggingTriangle = std::make_unique<LoggingProxy>(
        std::make_unique<Triangle>(10.0, 8.0, "orange", "LoggingTriangle")
    );
    
    std::cout << "Operations with logging proxy:\n";
    loggingTriangle->draw();
    double area = loggingTriangle->getArea();
    std::cout << "Triangle area: " << area << std::endl;
    loggingTriangle->getType();
    loggingTriangle->printInfo();
    
    std::cout << std::endl;
    
    // Test 7: Multiple Proxy Types Applied
    std::cout << "7. Multiple Proxy Types Applied:\n";
    
    // Apply multiple proxies layer by layer: Logging -> Caching -> Protected -> Real Shape
    auto multiProxyShape = std::make_unique<LoggingProxy>(
        std::make_unique<CachingProxy>(
            std::make_unique<ProtectionProxy>(
                std::make_unique<Rectangle>(9.0, 7.0, "multicolor", "MultiProxyRect"),
                "user", "user"
            )
        )
    );
    
    std::cout << "Accessing shape through multiple proxy layers (Logging -> Caching -> Protection -> Real):\n";
    multiProxyShape->draw();
    std::cout << "Area: " << multiProxyShape->getArea() << std::endl;
    std::cout << "Type: " << multiProxyShape->getType() << std::endl;
    std::cout << std::endl;
    
    // Test 8: Smart Proxy (Reference Counting Simulation)
    std::cout << "8. Smart Proxy (Reference Counting Simulation):\n";
    
    auto smartCircle = std::make_unique<SmartShapeProxy>(
        std::make_unique<Circle>(3.5, "smart_red", "SmartCircle"), 
        "User A"
    );
    
    std::cout << "Smart proxy properties:\n";
    smartCircle->draw();
    std::cout << "Reference count: " << smartCircle->getReferenceCount() << std::endl;
    std::cout << std::endl;
    
    // Test 9: Enhanced Proxy with Styling
    std::cout << "9. Enhanced Proxy with Styling:\n";
    
    auto enhancedCircle = std::make_unique<EnhancedProxy>(
        std::make_unique<Circle>(4.0, "enhanced_color", "EnhancedCircle"),
        "fancy_style", 1.5, true
    );
    
    enhancedCircle->draw();
    std::cout << "Enhanced circle area (scaled): " << enhancedCircle->getArea() << std::endl;
    enhancedCircle->printInfo();
    
    // Change properties
    enhancedCircle->setScale(2.0);
    std::cout << "\nAfter scaling to 2.0, area: " << enhancedCircle->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 10: Proxy Factory Usage
    std::cout << "10. Proxy Factory Usage:\n";
    
    auto factoryShape = ProxyFactory::createRemoteProxy(
        std::make_unique<Circle>(2.0, "factory_color", "FactoryCircle"),
        "remote.graphics.server.com"
    );
    factoryShape->draw();
    
    auto cachingShape = ProxyFactory::createCachingProxy(
        std::make_unique<Rectangle>(6.0, 4.0, "factory_caching", "FactoryCachingRect")
    );
    std::cout << "\nFirst access: ";
    double factoryArea1 = cachingShape->getArea();
    std::cout << "Area: " << factoryArea1 << std::endl;
    
    std::cout << "Second access (cached): ";
    double factoryArea2 = cachingShape->getArea();
    std::cout << "Area: " << factoryArea2 << std::endl;
    std::cout << std::endl;
    
    // Test 11: Composite Proxy (Combining Multiple Behaviors)
    std::cout << "11. Composite Proxy (Multiple Behaviors Combined):\n";
    
    auto compositeShape = std::make_unique<CompositeProxy>(
        std::make_unique<Triangle>(12.0, 9.0, "composite_color", "CompositeTriangle"),
        true,  // logging enabled
        true,  // caching enabled
        false, // protection disabled  - to allow access
        "user"
    );
    
    std::cout << "Accessing composite proxy with logging and caching:\n";
    compositeShape->draw();
    std::cout << "Area: " << compositeShape->getArea() << std::endl;
    std::cout << "Log operations performed: " << compositeShape->getLogSize() << std::endl;
    std::cout << std::endl;
    
    // Test 12: Proxy Manager Usage
    std::cout << "12. Proxy Manager Usage:\n";
    
    ProxyManager manager;
    
    // Add various proxied shapes to the manager
    manager.addShape(std::make_unique<RemoteShapeProxy>(
        std::make_unique<Circle>(3.0, "manager_remote", "ManagedRemoteCircle"), 
        "server1.example.com"
    ));
    
    manager.addShape(std::make_unique<LoggingProxy>(
        std::make_unique<Rectangle>(5.0, 4.0, "manager_logging", "ManagedLoggingRect")
    ));
    
    manager.addShape(std::make_unique<ProtectionProxy>(
        std::make_unique<Triangle>(7.0, 5.0, "manager_protected", "ManagedProtectedTriangle"),
        "user", "user"  // Permission granted
    ));
    
    manager.addShape(std::make_unique<CachingProxy>(
        std::make_unique<Circle>(2.5, "manager_caching", "ManagedCachingCircle")
    ));
    
    std::cout << "Manager contains " << manager.getShapeCount() << " shapes with different proxies\n";
    std::cout << "Rendering all managed shapes:\n";
    manager.renderAll();
    
    std::cout << "Total area of all managed shapes: " << manager.calculateTotalArea() << std::endl;
    std::cout << std::endl;
    
    // Test 13: Performance Comparison - Direct vs Proxy Access 
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
    
    std::cout << "Direct creation and calculation (1000 iterations): " 
              << directDuration.count() << " microseconds\n";
    std::cout << "Proxy creation and calculation (1000 iterations): " 
              << proxyDuration.count() << " microseconds\n";
    std::cout << "Proxy overhead: " << (proxyDuration.count() - directDuration.count()) << " microseconds\n";
    std::cout << std::endl;
    
    // Test 14: Real-world Scenario - Game Asset Manager
    std::cout << "14. Real-world Scenario - Game Asset Management:\n";
    
    // Simulate different game asset types with different proxy requirements
    std::vector<std::tuple<std::string, std::unique_ptr<Shape>>> gameAssets = {
        {"Player", std::make_unique<RemoteShapeProxy>(std::make_unique<Circle>(5.0, "player_color", "Player"), "asset_server")},
        {"UI_Element", std::make_unique<CachingProxy>(std::make_unique<Rectangle>(100.0, 30.0, "ui_color", "UI_Element"))},
        {"Environment", std::make_unique<ProtectionProxy>(std::make_unique<Triangle>(20.0, 15.0, "env_color", "Environment"), "admin", "admin")},
        {"Effect", std::make_unique<LoggingProxy>(std::make_unique<Circle>(2.0, "effect_color", "Effect"))}
    };
    
    for (auto& [assetType, assetShape] : gameAssets) {
        std::cout << "Game " << assetType << ": ";
        assetShape->draw();
        std::cout << "  Area: " << assetShape->getArea() << std::endl;
    }
    std::cout << std::endl;
    
    // Test 15: Proxy Pattern Benefits Demonstration
    std::cout << "15. Proxy Pattern Benefits Demonstration:\n";
    std::cout << "- Remote Proxy: Transparent access to remote objects\n";
    std::cout << "- Virtual Proxy: Lazy initialization of expensive objects\n";
    std::cout << "- Protection Proxy: Access control management\n";
    std::cout << "- Caching Proxy: Performance optimization through caching\n";
    std::cout << "- Logging Proxy: Transparent operation logging\n";
    std::cout << "- Enhanced Proxy: Add functionality without modifying original classes\n";
    std::cout << "- Proxy Factory: Easy creation of different proxy types\n";
    std::cout << "- Transparency: Client code works the same with or without proxies\n";
    std::cout << std::endl;
    
    // Test 16: Different Shape Types with Proxies
    std::cout << "16. Different Shape Types with Proxies:\n";
    
    std::vector<std::unique_ptr<Shape>> shapesWithProxies;
    
    // Add different shape types with different proxy configurations
    shapesWithProxies.push_back(std::make_unique<LoggingProxy>(
        std::make_unique<Circle>(3.0, "red_proxy", "RedCircleProxy")
    ));
    
    shapesWithProxies.push_back(std::make_unique<CachingProxy>(
        std::make_unique<Rectangle>(8.0, 6.0, "blue_proxy", "BlueRectangleProxy")
    ));
    
    shapesWithProxies.push_back(std::make_unique<RemoteShapeProxy>(
        std::make_unique<Triangle>(10.0, 8.0, "green_proxy", "GreenTriangleProxy"), 
        "remote_calculation_server"
    ));
    
    shapesWithProxies.push_back(std::make_unique<EnhancedProxy>(
        std::make_unique<Circle>(2.5, "enhanced_proxy", "EnhancedCircleProxy"),
        "rounded", 1.3, true
    ));
    
    std::cout << "Processing " << shapesWithProxies.size() << " shapes with different proxy types:\n";
    for (size_t i = 0; i < shapesWithProxies.size(); ++i) {
        std::cout << "Shape " << (i + 1) << " (" << shapesWithProxies[i]->getType() << "): ";
        shapesWithProxies[i]->draw();
        std::cout << "  Area: " << shapesWithProxies[i]->getArea() << std::endl;
    }
    std::cout << std::endl;
    
    // Test 17: Proxy Method Chaining and Composition
    std::cout << "17. Proxy Method Chaining and Complex Composition:\n";
    
    // Create a complex proxy chain
    auto complexChain = std::make_unique<EnhancedProxy>(
        std::make_unique<CompositeProxy>(
            std::make_unique<LoggingProxy>(
                std::make_unique<CachingProxy>(
                    std::make_unique<ProtectionProxy>(
                        std::make_unique<Rectangle>(15.0, 10.0, "complex", "ComplexChainRect"),
                        "user", "user"
                    )
                )
            ),
            true,  // logging enabled in composite
            true,  // caching enabled in composite
            false, // protection disabled in composite (we've already added protection)
            "user" // access level in composite
        ),
        "complex_style", 1.2, true
    );
    
    std::cout << "Accessing shape through complex proxy chain:\n";
    complexChain->draw();
    std::cout << "Area: " << complexChain->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 18: Virtual Proxy Efficiency Test
    std::cout << "18. Virtual Proxy Efficiency Test:\n";
    
    std::cout << "Creating multiple virtual proxies (no real shapes created yet):\n";
    std::vector<std::unique_ptr<VirtualShapeProxy>> virtualProxies;
    for (int i = 0; i < 100; ++i) {
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
    std::cout << "Created 100 virtual proxies, but real shapes not created yet\n";
    
    std::cout << "Accessing only first 10 virtual proxies (now creates real shapes):\n";
    for (int i = 0; i < 10; ++i) {
        std::cout << "Virtual proxy " << i << " area: " << virtualProxies[i]->getArea() << std::endl;
    }
    std::cout << "Only 10 of 100 real shapes were created\n";
    std::cout << std::endl;
    
    // Test 19: Proxy Factory Flexibility
    std::cout << "19. Proxy Factory Flexibility:\n";
    
    std::vector<std::string> proxyTypes = {"remote", "caching", "logging", "protection", "enhanced"};
    
    for (const auto& proxyType : proxyTypes) {
        std::cout << "Creating " << proxyType << " proxy for a circle:\n";
        std::unique_ptr<Shape> proxyShape;
        
        if (proxyType == "remote") {
            proxyShape = ProxyFactory::createRemoteProxy(
                std::make_unique<Circle>(3.0, proxyType + "_color", proxyType + "_circle"), 
                "test_server_" + proxyType
            );
        } else if (proxyType == "caching") {
            proxyShape = ProxyFactory::createCachingProxy(
                std::make_unique<Circle>(3.0, proxyType + "_color", proxyType + "_circle")
            );
        } else if (proxyType == "logging") {
            proxyShape = ProxyFactory::createLoggingProxy(
                std::make_unique<Circle>(3.0, proxyType + "_color", proxyType + "_circle")
            );
        } else if (proxyType == "protection") {
            proxyShape = ProxyFactory::createProtectionProxy(
                std::make_unique<Circle>(3.0, proxyType + "_color", proxyType + "_circle"),
                "user", "user"
            );
        } else if (proxyType == "enhanced") {
            proxyShape = ProxyFactory::createEnhancedProxy(
                std::make_unique<Circle>(3.0, proxyType + "_color", proxyType + "_circle"),
                "fancy_style"
            );
        }
        
        if (proxyShape) {
            proxyShape->draw();
            std::cout << "  Area: " << proxyShape->getArea() << std::endl;
        }
        std::cout << std::endl;
    }
    
    // Test 20: Proxy Pattern vs Direct Access Comparison
    std::cout << "20. Proxy Pattern vs Direct Access Comparison:\n";
    
    std::cout << "\nDirect Access:\n";
    auto directCircle = std::make_unique<Circle>(4.0, "direct_color", "DirectCircle");
    directCircle->draw();
    std::cout << "Direct area: " << directCircle->getArea() << std::endl;
    
    std::cout << "\nWith Proxy (Remote Simulation):\n";
    auto proxyCircle = std::make_unique<RemoteShapeProxy>(
        std::make_unique<Circle>(4.0, "proxy_color", "ProxyCircle"), 
        "simulation_server"
    );
    proxyCircle->draw();
    std::cout << "Proxy area: " << proxyCircle->getArea() << std::endl;
    
    std::cout << "\nBenefits of Proxy Pattern:\n";
    std::cout << "- Transparent access to complex operations\n";
    std::cout << "- Additional functionality without modifying original classes\n";
    std::cout << "- Access control and security layers\n";
    std::cout << "- Performance optimization (caching, lazy initialization)\n";
    std::cout << "- Logging and monitoring capabilities\n";
    std::cout << "- Abstraction of complex subsystems\n";
    std::cout << std::endl;
    
    // Test 21: Advanced Proxy - Composite Proxy with Protection Enabled
    std::cout << "21. Advanced Proxy - Composite Proxy with Protection:\n";
    
    auto protectedComposite = std::make_unique<CompositeProxy>(
        std::make_unique<Rectangle>(6.0, 4.0, "protected_composite", "ProtectedCompositeRect"),
        true,   // logging enabled
        true,   // caching enabled
        true,   // protection enabled
        "admin" // required access level
    );
    
    // Try with admin access (should work)
    std::cout << "With admin access:\n";
    protectedComposite->draw();
    std::cout << "Area: " << protectedComposite->getArea() << std::endl;
    
    std::cout << std::endl;
    
    // Test 22: Proxy Pattern Best Practices
    std::cout << "22. Proxy Pattern Best Practices:\n";
    std::cout << "- Maintain the same interface as the real object\n";
    std::cout << "- Keep proxy overhead minimal\n";
    std::cout << "- Ensure proper resource management\n";
    std::cout << "- Provide clear separation between proxy and real object responsibilities\n";
    std::cout << "- Use appropriate proxy type for the specific use case\n";
    std::cout << "- Consider performance implications with deep proxy chains\n";
    std::cout << "- Maintain transparency to client code\n";
    std::cout << std::endl;
    
    std::cout << "Proxy pattern demo completed successfully!\n";
    std::cout << "\nKey Benefits of Proxy Pattern:\n";
    std::cout << "- Controls access to an object without changing the object itself\n";
    std::cout << "- Provides a surrogate or placeholder for another object\n";
    std::cout << "- Adds functionality (security, caching, etc.) without modifying existing code\n";
    std::cout << "- Enables lazy initialization of expensive objects (Virtual Proxy)\n";
    std::cout << "- Supports remote object access (Remote Proxy)\n";
    std::cout << "- Facilitates access control (Protection Proxy)\n";
    std::cout << "- Promotes the Single Responsibility Principle\n";
    std::cout << "- Supports the Open/Closed Principle (open for extension, closed for modification)\n";
    
    return 0;
}