#include "proxy_shapes.h"
#include <iostream>
#include <memory>
#include <vector>

int main() {
    std::cout << "=== Proxy Pattern Demo: Shape Rendering ===\n\n";
    
    // Test 1: Basic shapes
    std::cout << "1. Basic Shape Operations:\n";
    auto circle = std::make_unique<Circle>(5.0, "red");
    auto rectangle = std::make_unique<Rectangle>(10.0, 5.0, "blue");
    
    std::cout << "Circle area: " << circle->getArea() << std::endl;
    circle->draw();
    std::cout << "Rectangle area: " << rectangle->getArea() << std::endl;
    rectangle->draw();
    std::cout << std::endl;
    
    // Test 2: Logging Proxy
    std::cout << "2. Logging Proxy Demo:\n";
    auto loggingCircle = std::make_unique<LoggingShapeProxy>(
        std::make_unique<Circle>(3.0, "green")
    );
    loggingCircle->draw();
    std::cout << "Circle type: " << loggingCircle->getType() << std::endl;
    std::cout << "Circle area: " << loggingCircle->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 3: Caching Proxy
    std::cout << "3. Caching Proxy Demo:\n";
    auto cachingTriangle = std::make_unique<CachingShapeProxy>(
        std::make_unique<Triangle>(8.0, 6.0, "yellow")
    );
    std::cout << "First access to area: ";
    double area1 = cachingTriangle->getArea();
    std::cout << "Second access to area: ";
    double area2 = cachingTriangle->getArea();
    std::cout << "Third access to area: ";
    double area3 = cachingTriangle->getArea();
    std::cout << "Triangle area calculated: " << area1 << std::endl;
    std::cout << "All accesses returned the same value: " 
              << (area1 == area2 && area2 == area3 ? "Yes" : "No") << std::endl;
    std::cout << std::endl;
    
    // Test 4: Protection Proxy
    std::cout << "4. Protection Proxy Demo:\n";
    auto protectedCircle = std::make_unique<ProtectedShapeProxy>(
        std::make_unique<Circle>(4.0, "purple"), "admin"
    );
    auto protectedRectangle = std::make_unique<ProtectedShapeProxy>(
        std::make_unique<Rectangle>(7.0, 3.0, "orange"), "guest"
    );
    
    std::cout << "Admin access to circle:\n";
    protectedCircle->draw();
    std::cout << "Circle type: " << protectedCircle->getType() << std::endl;
    std::cout << "Circle area: " << protectedCircle->getArea() << std::endl;
    
    std::cout << "\nGuest access to rectangle:\n";
    protectedRectangle->draw();
    std::cout << "Rectangle type: " << protectedRectangle->getType() << std::endl;
    std::cout << "Rectangle area: " << protectedRectangle->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 5: Remote Proxy Simulation
    std::cout << "5. Remote Proxy Simulation:\n";
    auto remoteCircle = std::make_unique<RemoteShapeProxy>(
        std::make_unique<Circle>(6.0, "cyan"), "192.168.1.100"
    );
    std::cout << "Accessing remote circle:\n";
    std::cout << "Type: " << remoteCircle->getType() << std::endl;
    std::cout << "Area: " << remoteCircle->getArea() << std::endl;
    remoteCircle->draw();
    std::cout << std::endl;
    
    // Test 6: Lazy Shape Proxy (Virtual Proxy)
    std::cout << "6. Lazy Shape Proxy (Virtual Proxy) Demo:\n";
    std::cout << "Creating lazy proxy without creating actual shape...\n";
    auto lazyCircle = std::make_unique<LazyShapeProxy>([]() {
        return std::make_unique<Circle>(7.0, "magenta");
    });
    std::cout << "Lazy proxy created. Type: " << lazyCircle->getType() << std::endl;
    std::cout << "Accessing area (this will create the real shape):\n";
    std::cout << "Area: " << lazyCircle->getArea() << std::endl;
    std::cout << "Now drawing (real shape already created):\n";
    lazyCircle->draw();
    std::cout << std::endl;
    
    // Test 7: Chain of Proxies
    std::cout << "7. Chain of Proxies Demo:\n";
    // Create a shape with multiple proxies: Lazy -> Caching -> Logging
    auto chainedProxy = std::make_unique<LoggingShapeProxy>(
        std::make_unique<CachingShapeProxy>(
            std::make_unique<LazyShapeProxy>([]() { return std::make_unique<Rectangle>(12.0, 8.0, "indigo"); })
        )
    );
    
    std::cout << "Accessing chained proxy (Lazy -> Caching -> Logging):\n";
    double area = chainedProxy->getArea();  // This creates the shape, caches it, and logs
    std::cout << "Area: " << area << std::endl;
    
    std::cout << "\nAccessing area again (should use cache, still log):\n";
    double area_repeated = chainedProxy->getArea();  // This should use cache
    std::cout << "Area: " << area_repeated << std::endl;
    
    std::cout << "\nDrawing the shape:\n";
    chainedProxy->draw();
    
    // Test 8: Performance comparison
    std::cout << "\n8. Performance Comparison:\n";
    auto basicCircle = std::make_unique<Circle>(10.0, "black");
    auto cachingCircle = std::make_unique<CachingShapeProxy>(
        std::make_unique<Circle>(10.0, "white")
    );
    
    std::cout << "Accessing basic circle area 5 times:\n";
    for (int i = 0; i < 5; ++i) {
        std::cout << "  Access " << (i+1) << ": " << basicCircle->getArea() << std::endl;
    }
    
    std::cout << "\nAccessing caching circle area 5 times:\n";
    for (int i = 0; i < 5; ++i) {
        std::cout << "  Access " << (i+1) << ": " << cachingCircle->getArea() << std::endl;
    }
    
    std::cout << "\nProxy pattern demo completed successfully!\n";
    return 0;
}