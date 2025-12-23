#include "proxy_patterns.h"
#include <iostream>
#include <memory>
#include <chrono>
#include <vector>

int main() {
    std::cout << "=== Proxy Pattern Demo: Shape Access Control and Enhancement ===\n\n";
    
    // Test 1: Basic Proxy Usage
    std::cout << "1. Basic Proxy Pattern Usage:\n";
    
    auto circle = std::make_unique<Circle>(5.0, "red", "MainCircle");
    circle->draw();
    std::cout << "Circle area: " << circle->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 2: Remote Proxy
    std::cout << "2. Remote Proxy Simulation:\n";
    
    auto remoteCircle = std::make_unique<RemoteShapeProxy>(
        std::make_unique<Circle>(3.0, "blue", "RemoteCircle"), 
        "graphics.remote.server.com", 0.2  // 200ms latency
    );
    
    std::cout << "Accessing remote shape:\n";
    remoteCircle->draw();
    std::cout << "Remote shape area: " << remoteCircle->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 3: Virtual Proxy (Lazy Initialization)
    std::cout << "3. Virtual Proxy (Lazy Initialization):\n";
    
    std::cout << "Creating virtual proxy (underlying shape not created yet):\n";
    auto virtualCircle = std::make_unique<VirtualShapeProxy>(
        []() { return std::make_unique<Circle>(4.0, "green", "VirtualCircle"); }
    );
    
    std::cout << "Virtual proxy created, underlying shape is not yet instantiated\n";
    std::cout << "Accessing virtual proxy (now creates the real shape):\n";
    virtualCircle->draw();
    std::cout << "Virtual circle area: " << virtualCircle->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 4: Protection Proxy
    std::cout << "4. Protection Proxy:\n";
    
    auto protectedCircle = std::make_unique<ProtectionProxy>(
        std::make_unique<Circle>(6.0, "purple", "ProtectedCircle"), 
        "admin_only", "admin"  // Requires admin permission, user has admin role
    );
    
    std::cout << "Admin access granted:\n";
    protectedCircle->draw();
    std::cout << "Area: " << protectedCircle->getArea() << std::endl;
    
    std::cout << "\nTrying guest access (should be denied):\n";
    auto guestProtectedCircle = std::make_unique<ProtectionProxy>(
        std::make_unique<Circle>(6.0, "purple", "ProtectedCircle"), 
        "admin_only", "guest"  // Requires admin permission, user has guest role
    );
    guestProtectedCircle->draw();
    std::cout << "Area: " << guestProtectedCircle->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 5: Caching Proxy
    std::cout << "5. Caching Proxy:\n";
    
    auto cachingCircle = std::make_unique<CachingProxy>(
        std::make_unique<Circle>(2.0, "orange", "CachingCircle")
    );
    
    std::cout << "First area access (calculates and caches):\n";
    double area1 = cachingCircle->getArea();
    std::cout << "Area: " << area1 << std::endl;
    
    std::cout << "\nSecond area access (uses cached value):\n";
    double area2 = cachingCircle->getArea();
    std::cout << "Area: " << area2 << std::endl;
    
    std::cout << "\nDrawing the shape:\n";
    cachingCircle->draw();
    std::cout << std::endl;
    
    // Test 6: Logging Proxy
    std::cout << "6. Logging Proxy:\n";
    
    auto loggingRect = std::make_unique<LoggingProxy>(
        std::make_unique<Rectangle>(8.0, 6.0, "cyan", "LoggingRect")
    );
    
    std::cout << "Operations with logging proxy:\n";
    loggingRect->draw();
    double area = loggingRect->getArea();
    std::cout << "Rectangle area: " << area << std::endl;
    loggingRect->printInfo();
    
    std::cout << "\nLog entries:\n";
    loggingRect->printLog();
    std::cout << std::endl;
    
    // Test 7: Enhanced Proxy with Multiple Features
    std::cout << "7. Enhanced Proxy with Multiple Features:\n";
    
    auto enhancedTri = std::make_unique<EnhancedProxy>(
        std::make_unique<Triangle>(10.0, 8.0, "yellow", "EnhancedTriangle"),
        "fancy_style", 1.5, true
    );
    
    enhancedTri->draw();
    std::cout << "Enhanced triangle area: " << enhancedTri->getArea() << std::endl;
    enhancedTri->printInfo();
    
    // Change properties and see the effect
    enhancedTri->setScale(2.0);
    std::cout << "\nAfter scaling to 2.0, area: " << enhancedTri->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 8: Proxy Factory Usage
    std::cout << "8. Proxy Factory Usage:\n";
    
    auto originalShape = std::make_unique<Circle>(3.5, "factory_color", "FactoryOriginal");
    
    // Create different proxies using the factory
    auto remoteShape = ProxyFactory::createRemoteProxy(
        std::make_unique<Circle>(3.5, "factory_color", "FactoryOriginal"), 
        "remote.server.com"
    );
    
    auto cachedShape = ProxyFactory::createCachingProxy(
        std::make_unique<Rectangle>(7.0, 5.0, "cached_color", "CachedFactoryRect")
    );
    
    auto loggedShape = ProxyFactory::createLoggingProxy(
        std::make_unique<Triangle>(8.0, 6.0, "logged_color", "LoggedFactoryTriangle")
    );
    
    std::cout << "Remote shape from factory:\n";
    remoteShape->draw();
    
    std::cout << "\nCached shape from factory - first access:\n";
    double cachedArea = cachedShape->getArea();
    std::cout << "Area: " << cachedArea << std::endl;
    
    std::cout << "\nCached shape - second access (should use cache):\n";
    double cachedArea2 = cachedShape->getArea();
    std::cout << "Area: " << cachedArea2 << std::endl;
    
    std::cout << "\nLogged shape from factory:\n";
    loggedShape->draw();
    std::cout << std::endl;
    
    // Test 9: Proxy Chaining (Multiple Proxies on One Object)
    std::cout << "9. Proxy Chaining (Multiple Proxies on One Object):\n";
    
    auto chainedProxy = std::make_unique<LoggingProxy>(
        std::make_unique<CachingProxy>(
            std::make_unique<ProtectionProxy>(
                std::make_unique<Circle>(4.0, "chain_color", "ChainedCircle"), 
                "user", "user"  // Permission granted
            )
        )
    );
    
    std::cout << "Executing operation through chained proxies (Logging -> Caching -> Protection -> Real Object):\n";
    chainedProxy->draw();
    std::cout << "Area through chain: " << chainedProxy->getArea() << std::endl;
    std::cout << "Type through chain: " << chainedProxy->getType() << std::endl;
    
    std::cout << "\nLog of chained proxy operations:\n";
    // Note: We can only see logs from the outermost proxy in this simple implementation
    static_cast<LoggingProxy*>(chainedProxy.get())->printLog();
    std::cout << std::endl;
    
    // Test 10: Performance Comparison
    std::cout << "10. Performance Comparison - Direct vs Proxy Access:\n";
    
    // Direct access performance
    auto startTime = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
        auto directCircle = std::make_unique<Circle>(1.0, "perf_direct", "DirectCircle" + std::to_string(i % 10));
        double area = directCircle->getArea();
    }
    auto directTime = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - startTime
    );
    
    // Proxy access performance
    startTime = std::chrono::high_resolution_clock::now();
    std::vector<std::unique_ptr<Shape>> proxyShapes;
    for (int i = 0; i < 1000; ++i) {
        proxyShapes.push_back(std::make_unique<LoggingProxy>(
            std::make_unique<Circle>(1.0, "perf_logged", "LoggedCircle" + std::to_string(i % 10))
        ));
        double area = proxyShapes.back()->getArea();
    }
    auto proxyTime = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - startTime
    );
    
    std::cout << "Direct creation and calculation (1000 iterations): " 
              << directTime.count() << " microseconds\n";
    std::cout << "Proxy creation and calculation (1000 iterations): " 
              << proxyTime.count() << " microseconds\n";
    std::cout << "Proxy overhead: " << (proxyTime.count() - directTime.count()) << " microseconds\n";
    std::cout << std::endl;
    
    // Test 11: Different Shape Types with Proxies
    std::cout << "11. Different Shape Types with Proxies:\n";
    
    std::vector<std::unique_ptr<Shape>> shapesWithProxies;
    
    // Add shapes with different proxy types
    shapesWithProxies.push_back(std::make_unique<LoggingProxy>(
        std::make_unique<Circle>(3.0, "proxy_red", "ProxyCircle")
    ));
    
    shapesWithProxies.push_back(std::make_unique<CachingProxy>(
        std::make_unique<Rectangle>(6.0, 4.0, "proxy_blue", "ProxyRectangle")
    ));
    
    shapesWithProxies.push_back(std::make_unique<ProtectionProxy>(
        std::make_unique<Triangle>(7.0, 5.0, "proxy_green", "ProxyTriangle"),
        "user", "user"
    ));
    
    shapesWithProxies.push_back(std::make_unique<EnhancedProxy>(
        std::make_unique<Circle>(2.5, "proxy_yellow", "ProxySmallCircle"),
        "bold", 1.2, true
    ));
    
    std::cout << "Processing " << shapesWithProxies.size() << " shapes with different proxies:\n";
    for (size_t i = 0; i < shapesWithProxies.size(); ++i) {
        std::cout << "Shape " << (i + 1) << ": ";
        shapesWithProxies[i]->draw();
        std::cout << "  Area: " << shapesWithProxies[i]->getArea() << std::endl;
        std::cout << "  Type: " << shapesWithProxies[i]->getType() << std::endl;
    }
    std::cout << std::endl;
    
    // Test 12: Proxy Manager Usage
    std::cout << "12. Proxy Manager Usage:\n";
    
    ProxyManager manager;

    // Add various shapes with proxies to the manager
    manager.addProxiedShape(std::make_unique<RemoteShapeProxy>(
        std::make_unique<Circle>(5.0, "manager_red", "RemoteCircle"),
        "server1.example.com"
    ));

    manager.addProxiedShape(std::make_unique<LoggingProxy>(
        std::make_unique<Rectangle>(10.0, 8.0, "manager_blue", "LoggedRect")
    ));

    manager.addProxiedShape(std::make_unique<ProtectionProxy>(
        std::make_unique<Triangle>(12.0, 9.0, "manager_green", "ProtectedTriangle"),
        "admin", "admin"
    ));

    std::cout << "Manager contains " << manager.getShapeCount() << " shapes with proxies\n";
    std::cout << "Rendering all managed shapes:\n";
    manager.renderAll();

    std::cout << "Total area of all managed shapes: " << manager.calculateTotalArea() << std::endl;
    std::cout << std::endl;
    
    // Test 13: Real-world Scenario - Graphics Rendering Pipeline
    std::cout << "13. Real-world Scenario - Graphics Rendering Pipeline:\n";
    
    struct GraphicComponent {
        std::string type;
        double param1, param2;
        std::string color;
        std::string style;
    };
    
    std::vector<GraphicComponent> components = {
        {"circle", 6.0, 0.0, "canvas_bg", "caching"},
        {"rectangle", 15.0, 10.0, "button_primary", "logging"},
        {"triangle", 8.0, 6.0, "icon_special", "protection"},
        {"circle", 3.0, 0.0, "indicator_active", "enhanced"}
    };
    
    std::vector<std::unique_ptr<Shape>> graphicsPipeline;
    for (const auto& comp : components) {
        std::unique_ptr<Shape> shape;
        
        if (comp.type == "circle") {
            shape = std::make_unique<Circle>(comp.param1, comp.color, "GP_" + comp.type);
        } else if (comp.type == "rectangle") {
            shape = std::make_unique<Rectangle>(comp.param1, comp.param2, comp.color, "GP_" + comp.type);
        } else if (comp.type == "triangle") {
            shape = std::make_unique<Triangle>(comp.param1, comp.param2, comp.color, "GP_" + comp.type);
        }
        
        // Apply styling based on component style
        if (comp.style == "caching") {
            shape = std::make_unique<CachingProxy>(std::move(shape));
        } else if (comp.style == "logging") {
            shape = std::make_unique<LoggingProxy>(std::move(shape));
        } else if (comp.style == "protection") {
            shape = std::make_unique<ProtectionProxy>(std::move(shape), "user", "user");
        } else if (comp.style == "enhanced") {
            shape = std::make_unique<EnhancedProxy>(std::move(shape), "fancy", 1.0, true);
        }
        
        graphicsPipeline.push_back(std::move(shape));
    }
    
    std::cout << "Graphics pipeline with " << graphicsPipeline.size() << " stylized components:\n";
    for (auto& component : graphicsPipeline) {
        component->draw();
        std::cout << "  Area: " << component->getArea() << std::endl;
    }
    std::cout << std::endl;
    
    // Test 14: Proxy Pattern Benefits Demonstration
    std::cout << "14. Proxy Pattern Benefits Demonstration:\n";
    std::cout << "- Remote Proxy: Transparent access to remote objects\n";
    std::cout << "- Virtual Proxy: Lazy initialization, deferring expensive object creation\n";
    std::cout << "- Protection Proxy: Access control without modifying original objects\n";
    std::cout << "- Caching Proxy: Performance improvement through result caching\n";
    std::cout << "- Logging Proxy: Transparent logging of operations\n";
    std::cout << "- Enhanced Proxy: Additional features without changing originals\n";
    std::cout << "- Proxy Chaining: Multiple behaviors combined\n";
    std::cout << "- Transparency: Same interface for real objects and proxies\n";
    std::cout << std::endl;
    
    // Test 15: Factory Pattern Integration
    std::cout << "15. Factory Pattern Integration:\n";
    
    // Demonstrates how proxy factories can be integrated with other patterns
    auto factoryCircle = std::make_unique<Circle>(4.0, "factory_integration", "FactoryCircle");
    auto factoryProtectedCircle = ProxyFactory::createProtectionProxy(
        std::make_unique<Circle>(4.0, "factory_integration", "FactoryCircle"), 
        "user", "user"
    );
    
    std::cout << "Shape created through proxy factory:\n";
    factoryProtectedCircle->draw();
    std::cout << std::endl;
    
    // Test 16: Proxy vs Direct Access Comparison
    std::cout << "16. Proxy vs Direct Access Comparison:\n";
    
    std::cout << "\nDirect Access:\n";
    Circle directCircle(3.0, "direct_color", "DirectCircle");
    directCircle.draw();
    std::cout << "Direct area: " << directCircle.getArea() << std::endl;
    
    std::cout << "\nWith Proxy (Remote Simulation):\n";
    auto remoteDirect = std::make_unique<RemoteShapeProxy>(
        std::make_unique<Circle>(3.0, "proxy_color", "ProxyCircle"), 
        "simulated_server"
    );
    remoteDirect->draw();
    std::cout << "Proxied area: " << remoteDirect->getArea() << std::endl;
    
    std::cout << "\nBenefits of using Proxy:\n";
    std::cout << "- Transparent access to remote/local objects\n";
    std::cout << "- Additional functionality without modifying original classes\n";
    std::cout << "- Access control management\n";
    std::cout << "- Performance optimizations through caching\n";
    std::cout << "- Operation logging and monitoring\n";
    std::cout << std::endl;
    
    // Test 17: Memory and Resource Management
    std::cout << "17. Memory and Resource Management:\n";
    
    // Show that proxy destruction is handled properly
    {
        auto tempProxy = std::make_unique<LoggingProxy>(
            std::make_unique<Rectangle>(5.0, 4.0, "temp_color", "TempRect")
        );
        tempProxy->draw();
        std::cout << "Temporary proxy created and used in scope\n";
    } // Proxy and underlying shape are automatically destroyed
    
    std::cout << "Proxy and underlying object properly destroyed when out of scope\n";
    std::cout << std::endl;
    
    // Test 18: Multiple Proxy Types on Same Object
    std::cout << "18. Multiple Proxy Types on Same Object:\n";
    
    auto baseShape = std::make_unique<Circle>(2.0, "multi_proxy", "MultiProxyCircle");
    
    // Apply multiple proxies step by step
    std::unique_ptr<Shape> multiProxiedShape = std::make_unique<RemoteShapeProxy>(std::move(baseShape), "multi_server");
    multiProxiedShape = std::make_unique<LoggingProxy>(std::move(multiProxiedShape));
    multiProxiedShape = std::make_unique<CachingProxy>(std::move(multiProxiedShape));
    
    std::cout << "Accessing shape with multiple proxy layers:\n";
    multiProxiedShape->draw();
    std::cout << "First area calculation:\n";
    double firstArea = multiProxiedShape->getArea();
    std::cout << "Area: " << firstArea << std::endl;
    std::cout << "Second area calculation (should use cache):\n";
    double secondArea = multiProxiedShape->getArea();
    std::cout << "Area: " << secondArea << std::endl;
    std::cout << std::endl;
    
    // Test 19: Performance Under Load
    std::cout << "19. Performance Under Load - Creating Many Proxy Objects:\n";
    
    auto loadStart = std::chrono::high_resolution_clock::now();
    
    std::vector<std::unique_ptr<Shape>> proxyLoad;
    for (int i = 0; i < 5000; ++i) {
        if (i % 3 == 0) {
            proxyLoad.push_back(std::make_unique<LoggingProxy>(
                std::make_unique<Circle>(1.0 + (i % 4), "load_test", "LoadCircle" + std::to_string(i))
            ));
        } else if (i % 3 == 1) {
            proxyLoad.push_back(std::make_unique<CachingProxy>(
                std::make_unique<Rectangle>(2.0 + (i % 3), 1.5 + (i % 2), "load_test", "LoadRect" + std::to_string(i))
            ));
        } else {
            proxyLoad.push_back(std::make_unique<ProtectionProxy>(
                std::make_unique<Triangle>(3.0 + (i % 2), 2.0 + (i % 3), "load_test", "LoadTriangle" + std::to_string(i)),
                "user", "user"
            ));
        }
    }
    
    auto loadEnd = std::chrono::high_resolution_clock::now();
    auto loadTime = std::chrono::duration_cast<std::chrono::microseconds>(loadEnd - loadStart);
    
    std::cout << "Created " << proxyLoad.size() << " proxied objects in: " 
              << loadTime.count() << " microseconds\n";
    std::cout << std::endl;
    
    // Test 20: Proxy Pattern Best Practices
    std::cout << "20. Proxy Pattern Best Practices:\n";
    std::cout << "- Maintain the same interface as the real object\n";
    std::cout << "- Keep proxy overhead minimal\n";
    std::cout << "- Ensure proper resource cleanup\n";
    std::cout << "- Use appropriate proxy types for specific needs\n";
    std::cout << "- Consider memory implications with nested proxies\n";
    std::cout << "- Maintain transparency to the client\n";
    std::cout << "- Properly implement all interface methods\n";
    std::cout << std::endl;
    
    std::cout << "Proxy pattern demo completed successfully!\n";
    std::cout << "\nKey Benefits of Proxy Pattern:\n";
    std::cout << "- Provides a surrogate for another object to control access to it\n";
    std::cout << "- Adds functionality without modifying the original object\n";
    std::cout << "- Enables lazy initialization of expensive objects (Virtual Proxy)\n";
    std::cout << "- Provides remote access to objects in different address spaces (Remote Proxy)\n";
    std::cout << "- Controls object access based on permissions (Protection Proxy)\n";
    std::cout << "- Improves performance through caching (Caching Proxy)\n";
    std::cout << "- Adds transparent logging or auditing capabilities (Logging Proxy)\n";
    std::cout << "- Supports the Single Responsibility principle\n";
    
    return 0;
}