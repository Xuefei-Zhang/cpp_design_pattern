#ifndef PROXY_PATTERNS_H
#define PROXY_PATTERNS_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <chrono>
#include <thread>
#include <functional>

// Subject interface - defines the common interface for RealSubject and Proxy
class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw() const = 0;
    virtual std::string getType() const = 0;
    virtual double getArea() const = 0;
    virtual void printInfo() const = 0;
};

// RealSubject - the actual object that the proxy represents
class Circle : public Shape {
private:
    double radius_;
    std::string color_;
    std::string name_;

public:
    explicit Circle(double radius, const std::string& color = "red", const std::string& name = "Circle") 
        : radius_(radius), color_(color), name_(name) {
        std::cout << "[REAL SUBJECT] Circle object created: " << name_ << " (radius=" << radius_ << ", color=" << color_ << ")\n";
    }
    
    void draw() const override {
        std::cout << "[REAL CIRCLE] Drawing " << name_ << ": radius=" << radius_ << ", color=" << color_ << std::endl;
    }
    
    std::string getType() const override {
        return name_;
    }
    
    double getArea() const override {
        return 3.14159 * radius_ * radius_;
    }
    
    void printInfo() const override {
        std::cout << "Circle - Name: " << name_ << ", Radius: " << radius_ << ", Color: " << color_ << std::endl;
    }
    
    double getRadius() const { return radius_; }
    const std::string& getColor() const { return color_; }
    const std::string& getName() const { return name_; }
};

class Rectangle : public Shape {
private:
    double width_, height_;
    std::string color_;
    std::string name_;

public:
    Rectangle(double width, double height, const std::string& color = "blue", 
              const std::string& name = "Rectangle") 
        : width_(width), height_(height), color_(color), name_(name) {
        std::cout << "[REAL SUBJECT] Rectangle object created: " << name_ << " (" << width_ << "x" << height_ << ", color=" << color_ << ")\n";
    }
    
    void draw() const override {
        std::cout << "[REAL RECTANGLE] Drawing " << name_ << ": " << width_ << "x" << height_ 
                  << ", color=" << color_ << std::endl;
    }
    
    std::string getType() const override {
        return name_;
    }
    
    double getArea() const override {
        return width_ * height_;
    }
    
    void printInfo() const override {
        std::cout << "Rectangle - Name: " << name_ << ", Width: " << width_ 
                  << ", Height: " << height_ << ", Color: " << color_ << std::endl;
    }
    
    double getWidth() const { return width_; }
    double getHeight() const { return height_; }
    const std::string& getColor() const { return color_; }
    const std::string& getName() const { return name_; }
};

class Triangle : public Shape {
private:
    double base_, height_;
    std::string color_;
    std::string name_;

public:
    Triangle(double base, double height, const std::string& color = "green", 
             const std::string& name = "Triangle") 
        : base_(base), height_(height), color_(color), name_(name) {
        std::cout << "[REAL SUBJECT] Triangle object created: " << name_ << " (base=" << base_ << ", height=" << height_ << ", color=" << color_ << ")\n";
    }
    
    void draw() const override {
        std::cout << "[REAL TRIANGLE] Drawing " << name_ << ": base=" << base_ << ", height=" << height_ 
                  << ", color=" << color_ << std::endl;
    }
    
    std::string getType() const override {
        return name_;
    }
    
    double getArea() const override {
        return 0.5 * base_ * height_;
    }
    
    void printInfo() const override {
        std::cout << "Triangle - Name: " << name_ << ", Base: " << base_ 
                  << ", Height: " << height_ << ", Color: " << color_ << std::endl;
    }
    
    double getBase() const { return base_; }
    double getHeight() const { return height_; }
    const std::string& getColor() const { return color_; }
    const std::string& getName() const { return name_; }
};

// Proxy interface - base class for all proxy types
class ShapeProxy : public Shape {
protected:
    std::string proxyName_;

public:
    explicit ShapeProxy(const std::string& name = "GenericProxy") : proxyName_(name) {}
    
    virtual ~ShapeProxy() = default;
    
    virtual void validateAccess() const {
        std::cout << "[" << proxyName_ << "] Access validated\n";
    }
    
    virtual void beforeOperation() const {
        std::cout << "[" << proxyName_ << "] Pre-operation hook\n";
    }
    
    virtual void afterOperation() const {
        std::cout << "[" << proxyName_ << "] Post-operation hook\n";
    }
    
    // Methods from Shape interface will be implemented by concrete proxies
    virtual void draw() const override = 0;
    virtual std::string getType() const override = 0;
    virtual double getArea() const override = 0;
    virtual void printInfo() const override = 0;
};

// 1. Remote Proxy - simulates access to a remote object
class RemoteShapeProxy : public ShapeProxy {
private:
    std::unique_ptr<Shape> realShape_;
    std::string serverAddress_;
    double networkLatency_;

public:
    RemoteShapeProxy(std::unique_ptr<Shape> shape, const std::string& server, 
                     double latency = 0.1) 
        : ShapeProxy("RemoteProxy"), realShape_(std::move(shape)), 
          serverAddress_(server), networkLatency_(latency) {}
    
    void draw() const override {
        std::cout << "[" << proxyName_ << "] Connecting to server " << serverAddress_ << "...\n";
        simulateNetworkDelay();
        std::cout << "[" << proxyName_ << "] Rendering " << realShape_->getType() 
                  << " remotely with " << serverAddress_ << std::endl;
        realShape_->draw();
        std::cout << "[" << proxyName_ << "] Render completed on remote server " << serverAddress_ << std::endl;
    }
    
    std::string getType() const override {
        std::cout << "[" << proxyName_ << "] Fetching type from remote server " << serverAddress_ << "...\n";
        simulateNetworkDelay();
        return realShape_->getType();
    }
    
    double getArea() const override {
        std::cout << "[" << proxyName_ << "] Requesting area calculation from server " << serverAddress_ << "...\n";
        simulateNetworkDelay();
        double area = realShape_->getArea();
        std::cout << "[" << proxyName_ << "] Received area from server: " << area << std::endl;
        return area;
    }
    
    void printInfo() const override {
        std::cout << "[" << proxyName_ << "] Fetching info from " << serverAddress_ << "...\n";
        simulateNetworkDelay();
        realShape_->printInfo();
    }
    
private:
    void simulateNetworkDelay() const {
        // Simulate network communication delay
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(networkLatency_ * 1000)));
    }
};

// 2. Virtual Proxy - delays creation of expensive objects until needed
class VirtualShapeProxy : public ShapeProxy {
private:
    std::function<std::unique_ptr<Shape>()> shapeFactory_;
    mutable std::unique_ptr<Shape> realShape_;
    mutable bool isShapeCreated_;

public:
    explicit VirtualShapeProxy(std::function<std::unique_ptr<Shape>()> factory) 
        : ShapeProxy("VirtualProxy"), shapeFactory_(std::move(factory)), isShapeCreated_(false) {}
    
    void draw() const override {
        createRealShapeIfNeeded();
        std::cout << "[" << proxyName_ << "] Drawing virtual shape:\n";
        realShape_->draw();
    }
    
    std::string getType() const override {
        createRealShapeIfNeeded();
        return realShape_->getType();
    }
    
    double getArea() const override {
        createRealShapeIfNeeded();
        return realShape_->getArea();
    }
    
    void printInfo() const override {
        createRealShapeIfNeeded();
        realShape_->printInfo();
    }
    
private:
    void createRealShapeIfNeeded() const {
        if (!isShapeCreated_) {
            std::cout << "[" << proxyName_ << "] Creating real shape on demand...\n";
            realShape_ = shapeFactory_();
            isShapeCreated_ = true;
        }
    }
};

// 3. Protection Proxy - controls access to the real object
class ProtectionProxy : public ShapeProxy {
private:
    std::unique_ptr<Shape> realShape_;
    std::string requiredPermission_;
    std::string currentUserRole_;

public:
    ProtectionProxy(std::unique_ptr<Shape> shape, const std::string& requiredPermission,
                    const std::string& currentUserRole = "guest") 
        : ShapeProxy("ProtectionProxy"), realShape_(std::move(shape)), 
          requiredPermission_(requiredPermission), currentUserRole_(currentUserRole) {}
    
    void validateAccess() const override {
        bool hasAccess = (currentUserRole_ == "admin" || currentUserRole_ == "user" || 
                         requiredPermission_ == "public");
        
        if (!hasAccess) {
            std::cout << "[" << proxyName_ << "] ACCESS DENIED - User role '" << currentUserRole_ 
                      << "' lacks required permission '" << requiredPermission_ << "'" << std::endl;
            throw std::runtime_error("Access denied to protected resource");
        } else {
            std::cout << "[" << proxyName_ << "] ACCESS GRANTED - User role '" << currentUserRole_ 
                      << "' has required permission '" << requiredPermission_ << "'" << std::endl;
        }
    }
    
    void draw() const override {
        try {
            validateAccess();
            std::cout << "[" << proxyName_ << "] Drawing with protection:\n";
            realShape_->draw();
        } catch (const std::exception& e) {
            std::cout << "[" << proxyName_ << "] Draw operation failed: " << e.what() << std::endl;
        }
    }
    
    std::string getType() const override {
        try {
            validateAccess();
            return realShape_->getType();
        } catch (const std::exception& e) {
            std::cout << "[" << proxyName_ << "] GetType operation failed: " << e.what() << std::endl;
            return "PROTECTED_SHAPE_ACCESS_DENIED";
        }
    }
    
    double getArea() const override {
        try {
            validateAccess();
            std::cout << "[" << proxyName_ << "] Calculating area with protection:\n";
            return realShape_->getArea();
        } catch (const std::exception& e) {
            std::cout << "[" << proxyName_ << "] GetArea operation failed: " << e.what() << std::endl;
            return -1.0; // Error indicator
        }
    }
    
    void printInfo() const override {
        try {
            validateAccess();
            realShape_->printInfo();
        } catch (const std::exception& e) {
            std::cout << "[" << proxyName_ << "] PrintInfo operation failed: " << e.what() << std::endl;
        }
    }
    
    void setCurrentUserRole(const std::string& role) {
        currentUserRole_ = role;
    }
    
    std::string getCurrentUserRole() const {
        return currentUserRole_;
    }
};

// 4. Caching Proxy - caches expensive operations
class CachingProxy : public ShapeProxy {
private:
    std::unique_ptr<Shape> realShape_;
    mutable double cachedArea_;
    mutable bool areaCached_;
    mutable std::string cachedType_;
    mutable bool typeCached_;

public:
    explicit CachingProxy(std::unique_ptr<Shape> shape) 
        : ShapeProxy("CachingProxy"), realShape_(std::move(shape)), 
          cachedArea_(0.0), areaCached_(false), typeCached_(false) {}
    
    void draw() const override {
        std::cout << "[" << proxyName_ << "] Drawing with caching proxy:\n";
        realShape_->draw(); // Draw doesn't change often, so no caching needed
    }
    
    std::string getType() const override {
        if (!typeCached_) {
            std::cout << "[" << proxyName_ << "] Fetching and caching type...\n";
            cachedType_ = realShape_->getType();
            typeCached_ = true;
        } else {
            std::cout << "[" << proxyName_ << "] Returning cached type...\n";
        }
        return cachedType_;
    }
    
    double getArea() const override {
        if (!areaCached_) {
            std::cout << "[" << proxyName_ << "] Calculating and caching area...\n";
            cachedArea_ = realShape_->getArea();
            areaCached_ = true;
        } else {
            std::cout << "[" << proxyName_ << "] Returning cached area...\n";
        }
        return cachedArea_;
    }
    
    void printInfo() const override {
        realShape_->printInfo(); // Info doesn't change, so no caching needed
    }
    
    // Methods to manage cache
    void clearAreaCache() {
        areaCached_ = false;
        cachedArea_ = 0.0;
    }
    
    void clearTypeCache() {
        typeCached_ = false;
        cachedType_ = "";
    }
    
    void clearAllCaches() {
        clearAreaCache();
        clearTypeCache();
    }
    
    bool isAreaCached() const { return areaCached_; }
    bool isTypeCached() const { return typeCached_; }
};

// 5. Logging Proxy - logs access to the real object
class LoggingProxy : public ShapeProxy {
private:
    std::unique_ptr<Shape> realShape_;
    std::vector<std::string> logEntries_;

public:
    explicit LoggingProxy(std::unique_ptr<Shape> shape) 
        : ShapeProxy("LoggingProxy"), realShape_(std::move(shape)) {}
    
    void draw() const override {
        std::string logEntry = "CALL draw() on " + realShape_->getType();
        logEntries_.push_back(logEntry);
        
        std::cout << "[" << proxyName_ << "] LOG: " << logEntry << std::endl;
        realShape_->draw();
    }
    
    std::string getType() const override {
        std::string logEntry = "CALL getType()";
        logEntries_.push_back(logEntry);
        
        std::cout << "[" << proxyName_ << "] LOG: " << logEntry << std::endl;
        return realShape_->getType();
    }
    
    double getArea() const override {
        std::string logEntry = "CALL getArea() on " + realShape_->getType();
        logEntries_.push_back(logEntry);
        
        std::cout << "[" << proxyName_ << "] LOG: " << logEntry << std::endl;
        return realShape_->getArea();
    }
    
    void printInfo() const override {
        std::string logEntry = "CALL printInfo() on " + realShape_->getType();
        logEntries_.push_back(logEntry);
        
        std::cout << "[" << proxyName_ << "] LOG: " << logEntry << std::endl;
        realShape_->printInfo();
    }
    
    // Access to log entries
    const std::vector<std::string>& getLogEntries() const {
        return logEntries_;
    }
    
    void printLog() const {
        std::cout << "[" << proxyName_ << "] Operation Log (" << logEntries_.size() << " entries):\n";
        for (size_t i = 0; i < logEntries_.size(); ++i) {
            std::cout << "  " << (i + 1) << ". " << logEntries_[i] << std::endl;
        }
    }
};

// 6. Smart Proxy - adds reference counting and other smart features
class SmartShapeProxy : public ShapeProxy {
private:
    std::unique_ptr<Shape> realShape_;
    mutable int referenceCount_;
    std::string ownerId_;

public:
    SmartShapeProxy(std::unique_ptr<Shape> shape, const std::string& ownerId = "System") 
        : ShapeProxy("SmartProxy"), realShape_(std::move(shape)), 
          referenceCount_(1), ownerId_(ownerId) {}
    
    // Copy constructor for reference counting (in a real implementation)
    SmartShapeProxy(const SmartShapeProxy& other) 
        : ShapeProxy(other.proxyName_), realShape_(nullptr), 
          referenceCount_(other.referenceCount_), ownerId_(other.ownerId_) {
        // In a real implementation, we'd handle shared ownership differently
        std::cout << "[" << proxyName_ << "] Copy constructor called\n";
    }
    
    void draw() const override {
        beforeOperation();
        std::cout << "[" << proxyName_ << "] RefCount: " << referenceCount_ 
                  << ", Owner: " << ownerId_ << std::endl;
        if (realShape_) {
            realShape_->draw();
        } else {
            std::cout << "[SMART PROXY] WARNING: Attempting to draw null shape\n";
        }
        afterOperation();
    }
    
    std::string getType() const override {
        beforeOperation();
        if (realShape_) {
            return realShape_->getType();
        } else {
            return "Null Shape";
        }
        afterOperation();
    }
    
    double getArea() const override {
        beforeOperation();
        if (realShape_) {
            return realShape_->getArea();
        } else {
            return 0.0;
        }
        afterOperation();
    }
    
    void printInfo() const override {
        beforeOperation();
        if (realShape_) {
            realShape_->printInfo();
        } else {
            std::cout << "Null Shape Info" << std::endl;
        }
        afterOperation();
    }
    
    int getReferenceCount() const { return referenceCount_; }
    
    void incrementRefCount() { referenceCount_++; }
    void decrementRefCount() { if(referenceCount_ > 0) referenceCount_--; }
    
    bool isNullShape() const { return !realShape_; }
};

// 7. Advanced Proxy: Proxy with additional functionality
class EnhancedProxy : public ShapeProxy {
private:
    std::unique_ptr<Shape> realShape_;
    std::string style_;
    double scale_;
    bool visible_;

public:
    EnhancedProxy(std::unique_ptr<Shape> shape, const std::string& style = "default", 
                  double scale = 1.0, bool visible = true) 
        : ShapeProxy("EnhancedProxy"), realShape_(std::move(shape)), 
          style_(style), scale_(scale), visible_(visible) {}
    
    void draw() const override {
        if (!visible_) {
            std::cout << "[" << proxyName_ << "] Shape is not visible, skipping draw\n";
            return;
        }
        
        std::cout << "[" << proxyName_ << "] Enhanced drawing with style: " << style_ 
                  << " and scale: " << scale_ << std::endl;
        beforeOperation();
        
        // Apply scale transformation
        // In a real implementation, we'd adjust the drawing parameters
        realShape_->draw();
        
        afterOperation();
    }
    
    std::string getType() const override {
        return realShape_ ? realShape_->getType() : "Null Shape";
    }
    
    std::string getStyle() const { return style_; }
    double getScale() const { return scale_; }
    bool isVisible() const { return visible_; }
    
    void setStyle(const std::string& style) { style_ = style; }
    void setScale(double scale) { scale_ = scale; }
    void setVisible(bool visible) { visible_ = visible; }
    
    double getArea() const override {
        if (realShape_) {
            // Scale affects the area calculation
            return realShape_->getArea() * scale_ * scale_;
        }
        return 0.0;
    }
    
    void printInfo() const override {
        std::cout << "EnhancedProxy - Style: " << style_ 
                  << ", Scale: " << scale_ 
                  << ", Visible: " << (visible_ ? "Yes" : "No") << std::endl;
        if (realShape_) {
            realShape_->printInfo();
        }
    }
};

// 8. Factory for creating proxy objects
class ProxyFactory {
public:
    static std::unique_ptr<Shape> createRemoteProxy(std::unique_ptr<Shape> shape, 
                                                   const std::string& server) {
        return std::make_unique<RemoteShapeProxy>(std::move(shape), server);
    }
    
    static std::unique_ptr<Shape> createVirtualProxy(std::function<std::unique_ptr<Shape>()> factory) {
        return std::make_unique<VirtualShapeProxy>(std::move(factory));
    }
    
    static std::unique_ptr<Shape> createProtectionProxy(std::unique_ptr<Shape> shape, 
                                                        const std::string& perm, 
                                                        const std::string& user = "guest") {
        return std::make_unique<ProtectionProxy>(std::move(shape), perm, user);
    }
    
    static std::unique_ptr<Shape> createCachingProxy(std::unique_ptr<Shape> shape) {
        return std::make_unique<CachingProxy>(std::move(shape));
    }
    
    static std::unique_ptr<Shape> createLoggingProxy(std::unique_ptr<Shape> shape) {
        return std::make_unique<LoggingProxy>(std::move(shape));
    }
    
    static std::unique_ptr<Shape> createSmartProxy(std::unique_ptr<Shape> shape, 
                                                  const std::string& owner = "System") {
        return std::make_unique<SmartShapeProxy>(std::move(shape), owner);
    }
    
    static std::unique_ptr<Shape> createEnhancedProxy(std::unique_ptr<Shape> shape, 
                                                     const std::string& style = "default", 
                                                     double scale = 1.0, 
                                                     bool visible = true) {
        return std::make_unique<EnhancedProxy>(std::move(shape), style, scale, visible);
    }
};

// 9. Proxy Manager for complex proxy operations
class ProxyManager {
private:
    std::vector<std::unique_ptr<Shape>> proxiedShapes_;

public:
    void addProxiedShape(std::unique_ptr<Shape> shape) {
        proxiedShapes_.push_back(std::move(shape));
    }
    
    void renderAll() const {
        std::cout << "[PROXY MANAGER] Rendering all " << proxiedShapes_.size() << " shapes:\n";
        for (size_t i = 0; i < proxiedShapes_.size(); ++i) {
            std::cout << "Shape " << (i + 1) << ": ";
            proxiedShapes_[i]->draw();
        }
    }
    
    double calculateTotalArea() const {
        double total = 0.0;
        for (const auto& shape : proxiedShapes_) {
            total += shape->getArea();
        }
        return total;
    }
    
    void applyGlobalScale(double scale) {
        std::cout << "[PROXY MANAGER] Applying global scale of " << scale << std::endl;
        // In a real implementation, this would modify scale-dependent proxies
    }
    
    size_t getShapeCount() const { return proxiedShapes_.size(); }
    
    void clear() { proxiedShapes_.clear(); }
};

#endif // PROXY_PATTERNS_H