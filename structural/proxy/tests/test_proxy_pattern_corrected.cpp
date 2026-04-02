#include "../include/proxy_shapes.h"
#include <iostream>
#include <memory>
#include <vector>
#include <chrono>

int main() {
    std::cout << "=== Proxy Pattern Demo: Shape Service Proxies ===\n\n";
    
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
    
    // Test 2: Logging Proxy
    std::cout << "2. Logging Proxy:\n";
    
    std::cout << "Creating logging proxy:\n";
    auto loggingCircleProxy = std::make_unique<LoggingShapeProxy>(
        std::make_unique<Circle>(4.0, "blue")
    );
    std::cout << "Logging proxy created\n";
    
    std::cout << "Accessing logging proxy:\n";
    loggingCircleProxy->draw();
    std::cout << "Logging circle area: " << loggingCircleProxy->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 3: Caching Proxy
    std::cout << "3. Caching Proxy:\n";
    
    auto cachingCircle = std::make_unique<CachingShapeProxy>(
        std::make_unique<Circle>(3.0, "purple")
    );
    std::cout << "First access to area (computes and caches): ";
    double area1 = cachingCircle->getArea();
    std::cout << area1 << std::endl;
    
    std::cout << "Second access to area (uses cache): ";
    double area2 = cachingCircle->getArea();
    std::cout << area2 << std::endl;
    
    cachingCircle->draw();
    std::cout << std::endl;
    
    // Test 4: Protection Proxy
    std::cout << "4. Protection Proxy:\n";
    
    auto authorizedCircle = std::make_unique<ProtectedShapeProxy>(
        std::make_unique<Circle>(3.0, "purple"), 
        "admin"
    );
    std::cout << "Authorized access (admin):\n";
    authorizedCircle->draw();
    std::cout << "Approved circle area: " << authorizedCircle->getArea() << std::endl;
    
    auto unauthorizedCircle = std::make_unique<ProtectedShapeProxy>(
        std::make_unique<Circle>(3.0, "purple"), 
        "guest"
    );
    std::cout << "\nUnauthorized access (guest):\n";
    unauthorizedCircle->draw();
    std::cout << "Restricted area access: " << unauthorizedCircle->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 5: Multiple Proxy Types
    std::cout << "5. Multiple Proxy Types Combined:\n";
    
    // Create a shape with logging and caching
    auto loggedAndCachedShape = std::make_unique<LoggingShapeProxy>(
        std::make_unique<CachingShapeProxy>(
            std::make_unique<Triangle>(6.0, 4.0, "orange")
        )
    );
    
    std::cout << "Accessing shape with both logging and caching:\n";
    loggedAndCachedShape->draw();
    std::cout << "Area (first access): " << loggedAndCachedShape->getArea() << std::endl;
    std::cout << "Area (second access - cached, still logged): " << loggedAndCachedShape->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 6: Performance Comparison
    std::cout << "6. Performance Comparison: Direct vs Cached Access:\n";
    
    // Direct access performance
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
        auto directShape = std::make_unique<Circle>(1.0 + (i % 3), "color" + std::to_string(i % 5));
        double area = directShape->getArea();
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto directTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // Cached access performance (first access calculates, subsequent use cache)
    start = std::chrono::high_resolution_clock::now();
    auto cachedShape = std::make_unique<CachingShapeProxy>(std::make_unique<Circle>(1.0, "test"));
    for (int i = 0; i < 1000; ++i) {
        double area = cachedShape->getArea();  // Only first call computes, others use cache
    }
    end = std::chrono::high_resolution_clock::now();
    auto cachedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Direct access (1000 calculations): " << directTime.count() << " microseconds\n";
    std::cout << "Cached access (1 calculation + 999 lookups): " << cachedTime.count() << " microseconds\n";
    std::cout << std::endl;
    
    // Test 7: Protection Level Testing
    std::cout << "7. Protection Level Testing:\n";
    
    std::vector<std::string> accessLevels = {"guest", "user", "admin", "superadmin"};
    for (const auto& level : accessLevels) {
        std::cout << "Access level '" << level << "':\n";
        auto protectedShape = std::make_unique<ProtectedShapeProxy>(
            std::make_unique<Rectangle>(5.0, 4.0, "test"), 
            level
        );
        protectedShape->draw();
        std::cout << "Area accessible: " << protectedShape->getArea() << std::endl;
        std::cout << std::endl;
    }
    
    // Test 8: Remote Operation Simulation
    std::cout << "8. Remote Operation Simulation:\n";
    
    auto remoteShape = std::make_unique<RemoteShapeProxy>(
        std::make_unique<Rectangle>(10.0, 8.0, "remote_target"), 
        "calculation_server"
    );
    
    std::cout << "Performing remote operation:\n";
    std::cout << "  Area: " << remoteShape->getArea() << std::endl;
    std::cout << "  Remote proxy simulated network operation\n";
    remoteShape->draw();
    std::cout << std::endl;
    
    // Test 9: Proxy Chaining
    std::cout << "9. Proxy Chaining (Multiple Proxies):\n";
    
    auto chainedProxy = std::make_unique<LoggingShapeProxy>(
        std::make_unique<CachingShapeProxy>(
            std::make_unique<ProtectedShapeProxy>(
                std::make_unique<Circle>(2.0, "chain_color"), 
                "admin"  // Authorized to see the actual shape
            )
        )
    );
    
    std::cout << "Accessing through chained proxies:\n";
    chainedProxy->draw();
    std::cout << "Chained proxy area: " << chainedProxy->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 10: Real-world Scenario - Graphics Pipeline
    std::cout << "10. Real-world Scenario - Graphics Pipeline:\n";
    
    // Simulate a graphics pipeline with multiple proxy types
    std::vector<std::unique_ptr<Shape>> graphicsPipeline;
    graphicsPipeline.push_back(std::make_unique<RemoteShapeProxy>(std::make_unique<Circle>(3.0, "pipeline_circle"), "graphics_server"));
    graphicsPipeline.push_back(std::make_unique<LoggingShapeProxy>(std::make_unique<Rectangle>(4.0, 3.0, "pipeline_rectangle")));
    graphicsPipeline.push_back(std::make_unique<CachingShapeProxy>(std::make_unique<Triangle>(6.0, 4.0, "pipeline_triangle")));
    graphicsPipeline.push_back(std::make_unique<ProtectedShapeProxy>(std::make_unique<Circle>(2.0, "pipeline_protected"), "user"));
    
    std::cout << "Graphics pipeline with " << graphicsPipeline.size() << " shapes using different proxies:\n";
    for (size_t i = 0; i < graphicsPipeline.size(); ++i) {
        std::cout << "  Shape " << (i + 1) << ": ";
        graphicsPipeline[i]->draw();
    }
    std::cout << std::endl;
    
    // Test 11: Logging Details
    std::cout << "11. Logging Details Test:\n";
    
    auto detailedLoggingShape = std::make_unique<LoggingShapeProxy>(
        std::make_unique<Circle>(1.5, "detailed_log")
    );
    
    std::cout << "First operation:\n";
    detailedLoggingShape->draw();
    std::cout << "Second operation:\n";
    detailedLoggingShape->getArea();
    std::cout << std::endl;
    
    // Test 12: Memory Efficiency with Proxy Pattern
    std::cout << "12. Memory Efficiency Test:\n";
    
    // Create many shapes with caching proxy (demonstrates that the underlying object stays the same)
    std::vector<std::unique_ptr<CachingShapeProxy>> shapeCaches;
    for (int i = 0; i < 10; ++i) {
        shapeCaches.push_back(std::make_unique<CachingShapeProxy>(
            std::make_unique<Rectangle>(2.0 + (i % 3), 1.0 + (i % 2), "efficiency_test")
        ));
    }
    
    std::cout << "Created " << shapeCaches.size() << " caching proxies\n";
    std::cout << "Each proxy caches area calculations independently\n";
    for (size_t i = 0; i < shapeCaches.size() && i < 3; ++i) {  // Just show first 3
        std::cout << "  Proxy " << (i+1) << " area: " << shapeCaches[i]->getArea() << std::endl;
    }
    std::cout << std::endl;
    
    // Test 13: Proxy Pattern Benefits Demonstration
    std::cout << "13. Proxy Pattern Benefits Demonstration:\n";
    std::cout << "- Remote Proxy: Simulates access to remote objects transparently\n";
    std::cout << "- Logging Proxy: Adds transparent logging without modifying original objects\n";
    std::cout << "- Caching Proxy: Improves performance by caching expensive operations\n";
    std::cout << "- Protection Proxy: Controls access to objects based on permissions\n";
    std::cout << "- Proxy Chaining: Multiple behaviors can be layered together\n";
    std::cout << "- Transparency: Client code works the same with or without proxies\n";
    std::cout << "- Single Responsibility: Each proxy handles one aspect of functionality\n";
    std::cout << "- Open/Closed Principle: New proxy types can be added without modifying existing code\n";
    std::cout << std::endl;
    
    // Test 14: Proxy vs Direct Access
    std::cout << "14. Proxy vs Direct Access Comparison:\n";
    
    std::cout << "Direct access: ";
    auto directRectangle = std::make_unique<Rectangle>(5.0, 4.0, "direct_color");
    directRectangle->draw();
    std::cout << "Area: " << directRectangle->getArea() << std::endl;
    
    std::cout << "With logging proxy: ";
    auto loggingRectangle = std::make_unique<LoggingShapeProxy>(
        std::make_unique<Rectangle>(5.0, 4.0, "logged_color")
    );
    loggingRectangle->draw();
    std::cout << "Area: " << loggingRectangle->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 15: Complex Proxy Interaction
    std::cout << "15. Complex Proxy Interaction:\n";
    
    // Combination that shows multiple proxy effects
    auto complexProxy = std::make_unique<LoggingShapeProxy>(
        std::make_unique<RemoteShapeProxy>(
            std::make_unique<CachingShapeProxy>(
                std::make_unique<ProtectedShapeProxy>(
                    std::make_unique<Rectangle>(8.0, 6.0, "complex_color"), 
                    "admin"
                )
            ),
            "complex_server"
        )
    );
    
    std::cout << "Complex proxy interaction (logging + remote + caching + protection):\n";
    complexProxy->draw();
    std::cout << "Area: " << complexProxy->getArea() << std::endl;
    std::cout << "Notice: Multiple proxy effects are combined!\n";
    std::cout << std::endl;
    
    std::cout << "Proxy pattern demo completed successfully!\n";
    std::cout << "\nKey Benefits of Proxy Pattern:\n";
    std::cout << "- Provides a surrogate for another object to control access to it\n";
    std::cout << "- Adds functionality without modifying the original object\n";
    std::cout << "- Enables lazy initialization of expensive objects (Virtual Proxy)\n";
    std::cout << "- Provides remote access to objects in different address spaces (Remote Proxy)\n";
    std::cout << "- Controls object access based on permissions (Protection Proxy)\n";
    std::cout << "- Improves performance through caching (Caching Proxy)\n";
    std::cout << "- Adds transparent logging or auditing capabilities\n";
    std::cout << "- Follows the Single Responsibility principle\n";
    
    return 0;
}