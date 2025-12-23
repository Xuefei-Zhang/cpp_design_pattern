#ifndef PROXY_PATTERNS_H
#define PROXY_PATTERNS_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <chrono>
#include <thread>
#include <functional>
#include <map>

// Subject interface - defines the common interface for RealSubject and Proxy
class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw() const = 0;
    virtual std::string getType() const = 0;
    virtual double getArea() const = 0;
    virtual void printInfo() const = 0;
};

// RealSubject classes
class Circle : public Shape {
private:
    double radius_;
    std::string color_;
    std::string name_;

public:
    Circle(double radius, const std::string& color = "red", const std::string& name = "Circle") 
        : radius_(radius), color_(color), name_(name) {
        std::cout << "[REAL SUBJECT] Circle created: " << name_ << " (radius=" << radius_ << ", color=" << color_ << ")\n";
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
    
    // Accessors
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
        std::cout << "[REAL SUBJECT] Rectangle created: " << name_ << " (dimensions=" << width_ << "x" << height_ << ", color=" << color_ << ")\n";
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
    
    // Accessors
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
        std::cout << "[REAL SUBJECT] Triangle created: " << name_ << " (base=" << base_ << ", height=" << height_ << ", color=" << color_ << ")\n";
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
    
    // Accessors
    double getBase() const { return base_; }
    double getHeight() const { return height_; }
    const std::string& getColor() const { return color_; }
    const std::string& getName() const { return name_; }
};

// Base Proxy class
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
    
    virtual void trackOperation() const {
        std::cout << "[" << proxyName_ << "] Operation tracked\n";
    }
    
    // These methods will be implemented by concrete proxies
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
                  << " remotely via " << serverAddress_ << std::endl;
        realShape_->draw();
        std::cout << "[" << proxyName_ << "] Render completed on remote server " << serverAddress_ << std::endl;
    }
    
    std::string getType() const override {
        std::cout << "[" << proxyName_ << "] Fetching type info from server " << serverAddress_ << "...\n";
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
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(networkLatency_ * 100)));
    }
};

// 2. Virtual Proxy - delays the creation of expensive objects until needed
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
            std::cout << "[" << proxyName_ << "] Creating expensive shape on demand...\n";
            realShape_ = shapeFactory_();
            isShapeCreated_ = true;
        }
    }
};

// 3. Protection Proxy - controls access based on permissions
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
        bool hasAccess = (currentUserRole_ == "admin" || 
                         (requiredPermission_ == "public") ||
                         (requiredPermission_ == "user" && currentUserRole_ == "user"));
        
        if (!hasAccess) {
            std::cout << "[" << proxyName_ << "] ACCESS DENIED - Role '" << currentUserRole_ 
                      << "' lacks permission '" << requiredPermission_ << "'" << std::endl;
            return;
        } else {
            std::cout << "[" << proxyName_ << "] ACCESS GRANTED - Role '" << currentUserRole_ 
                      << "' has permission '" << requiredPermission_ << "'" << std::endl;
        }
    }
    
    void draw() const override {
        validateAccess();
        // Only draw if access is granted
        if (hasRequiredAccess()) {
            std::cout << "[" << proxyName_ << "] Drawing with protection:\n";
            realShape_->draw();
        } else {
            std::cout << "[" << proxyName_ << "] Drawing blocked due to insufficient privileges\n";
        }
    }
    
    std::string getType() const override {
        validateAccess();
        if (hasRequiredAccess()) {
            return realShape_->getType();
        }
        return "PROTECTED_SHAPE_ACCESS_DENIED";
    }
    
    double getArea() const override {
        validateAccess();
        if (hasRequiredAccess()) {
            std::cout << "[" << proxyName_ << "] Calculating area with protection:\n";
            return realShape_->getArea();
        }
        return -1.0; // Error value for denied access
    }
    
    void printInfo() const override {
        validateAccess();
        if (hasRequiredAccess()) {
            realShape_->printInfo();
        } else {
            std::cout << "Shape information protected - access denied" << std::endl;
        }
    }
    
    void setCurrentUserRole(const std::string& role) {
        currentUserRole_ = role;
    }
    
    std::string getCurrentUserRole() const {
        return currentUserRole_;
    }
    
private:
    bool hasRequiredAccess() const {
        return (currentUserRole_ == "admin" || 
                (requiredPermission_ == "public") ||
                (requiredPermission_ == "user" && currentUserRole_ == "user"));
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
        realShape_->draw();
    }
    
    std::string getType() const override {
        if (!typeCached_) {
            std::cout << "[" << proxyName_ << "] Fetching and caching type...\n";
            cachedType_ = realShape_->getType();
            typeCached_ = true;
            return cachedType_;
        } else {
            std::cout << "[" << proxyName_ << "] Returning cached type...\n";
            return cachedType_;
        }
    }
    
    double getArea() const override {
        if (!areaCached_) {
            std::cout << "[" << proxyName_ << "] Calculating and caching area...\n";
            cachedArea_ = realShape_->getArea();
            areaCached_ = true;
            return cachedArea_;
        } else {
            std::cout << "[" << proxyName_ << "] Returning cached area...\n";
            return cachedArea_;
        }
    }
    
    void printInfo() const override {
        realShape_->printInfo();
    }
    
    void clearCache() {
        areaCached_ = false;
        typeCached_ = false;
        std::cout << "[" << proxyName_ << "] Cache cleared\n";
    }
    
    bool isAreaCached() const { return areaCached_; }
    bool isTypeCached() const { return typeCached_; }
};

// 5. Logging Proxy - logs all operations
class LoggingProxy : public ShapeProxy {
private:
    std::unique_ptr<Shape> realShape_;
    std::vector<std::string> operationLog_;

public:
    explicit LoggingProxy(std::unique_ptr<Shape> shape) 
        : ShapeProxy("LoggingProxy"), realShape_(std::move(shape)) {}
    
    void draw() const override {
        std::string logEntry = "draw() on " + realShape_->getType();
        operationLog_.push_back(logEntry);
        std::cout << "[" << proxyName_ << "] LOG: " << logEntry << std::endl;
        realShape_->draw();
    }
    
    std::string getType() const override {
        std::string logEntry = "getType()";
        operationLog_.push_back(logEntry);
        std::cout << "[" << proxyName_ << "] LOG: " << logEntry << std::endl;
        return realShape_->getType();
    }
    
    double getArea() const override {
        std::string logEntry = "getArea() on " + realShape_->getType();
        operationLog_.push_back(logEntry);
        std::cout << "[" << proxyName_ << "] LOG: " << logEntry << std::endl;
        return realShape_->getArea();
    }
    
    void printInfo() const override {
        std::string logEntry = "printInfo() on " + realShape_->getType();
        operationLog_.push_back(logEntry);
        std::cout << "[" << proxyName_ << "] LOG: " << logEntry << std::endl;
        realShape_->printInfo();
    }
    
    void printLog() const {
        std::cout << "[" << proxyName_ << "] Operation log (" << operationLog_.size() << " operations):\n";
        for (size_t i = 0; i < operationLog_.size(); ++i) {
            std::cout << "  " << (i + 1) << ". " << operationLog_[i] << std::endl;
        }
    }
    
    size_t getLogSize() const { return operationLog_.size(); }
};

// 6. Smart Reference Proxy - adds reference counting
class SmartShapeProxy : public ShapeProxy {
private:
    std::unique_ptr<Shape> realShape_;
    mutable int referenceCount_;
    std::string ownerId_;

public:
    SmartShapeProxy(std::unique_ptr<Shape> shape, const std::string& ownerId = "System") 
        : ShapeProxy("SmartProxy"), realShape_(std::move(shape)), 
          referenceCount_(1), ownerId_(ownerId) {}
    
    void draw() const override {
        beforeOperation();
        std::cout << "[" << proxyName_ << "] RefCount: " << referenceCount_ 
                  << ", Owner: " << ownerId_ << std::endl;
        if (realShape_) {
            realShape_->draw();
        } else {
            std::cout << "[" << proxyName_ << "] Shape is null, cannot draw\n";
        }
        afterOperation();
    }
    
    std::string getType() const override {
        if (realShape_) {
            return realShape_->getType();
        } else {
            return "Null Shape";
        }
    }
    
    double getArea() const override {
        if (realShape_) {
            return realShape_->getArea();
        } else {
            return 0.0;
        }
    }
    
    void printInfo() const override {
        std::cout << "SmartShapeProxy - Owner: " << ownerId_ 
                  << ", References: " << referenceCount_ << std::endl;
        if (realShape_) {
            realShape_->printInfo();
        }
    }
    
    int getReferenceCount() const { return referenceCount_; }
    void incrementRefCount() { referenceCount_++; }
    void decrementRefCount() { if (referenceCount_ > 0) referenceCount_--; }
};

// 7. Enhanced Proxy - adds multiple enhancements
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
        realShape_->draw();
        afterOperation();
    }
    
    std::string getType() const override {
        return style_ + " " + realShape_->getType();
    }
    
    double getArea() const override {
        return realShape_->getArea() * scale_ * scale_;
    }
    
    void printInfo() const override {
        std::cout << "EnhancedProxy - Style: " << style_ 
                  << ", Scale: " << scale_ 
                  << ", Visible: " << (visible_ ? "Yes" : "No") << std::endl;
        realShape_->printInfo();
    }
    
    void setStyle(const std::string& style) { style_ = style; }
    void setScale(double scale) { scale_ = scale; }
    void setVisible(bool visible) { visible_ = visible; }
    
    std::string getStyle() const { return style_; }
    double getScale() const { return scale_; }
    bool isVisible() const { return visible_; }
};

// 8. Proxy Factory - creates different types of proxies
class ProxyFactory {
public:
    static std::unique_ptr<Shape> createRemoteProxy(std::unique_ptr<Shape> shape,
                                                   const std::string& server,
                                                   double latency = 0.1) {
        return std::make_unique<RemoteShapeProxy>(std::move(shape), server, latency);
    }
    
    static std::unique_ptr<Shape> createVirtualProxy(std::function<std::unique_ptr<Shape>()> factory) {
        return std::make_unique<VirtualShapeProxy>(std::move(factory));
    }
    
    static std::unique_ptr<Shape> createProtectionProxy(std::unique_ptr<Shape> shape, 
                                                        const std::string& permission, 
                                                        const std::string& userRole = "guest") {
        return std::make_unique<ProtectionProxy>(std::move(shape), permission, userRole);
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
                                                     double scale = 1.0, bool visible = true) {
        return std::make_unique<EnhancedProxy>(std::move(shape), style, scale, visible);
    }
};

// 9. Proxy Manager - manages collections of proxied shapes
class ProxyManager {
private:
    std::vector<std::unique_ptr<Shape>> proxiedShapes_;

public:
    void addShape(std::unique_ptr<Shape> shape) {
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
    
    size_t getShapeCount() const { return proxiedShapes_.size(); }
    
    void clear() { proxiedShapes_.clear(); }
    
    void printAllInfo() const {
        std::cout << "[PROXY MANAGER] Information for all shapes:\n";
        for (size_t i = 0; i < proxiedShapes_.size(); ++i) {
            std::cout << "Shape " << (i + 1) << ": ";
            proxiedShapes_[i]->printInfo();
        }
    }
    
    Shape* getShape(size_t index) {
        if (index < proxiedShapes_.size()) {
            return proxiedShapes_[index].get();
        }
        return nullptr;
    }
};

// 10. Composite Proxy - combines multiple proxy behaviors
class CompositeProxy : public ShapeProxy {
private:
    std::unique_ptr<Shape> realShape_;
    bool enableLogging_, enableCaching_, enableProtection_;
    std::string accessLevel_;
    std::vector<std::string> logEntries_;
    mutable double cachedArea_;
    mutable bool areaCached_;

public:
    CompositeProxy(std::unique_ptr<Shape> shape, bool logging = true, bool caching = true, 
                   bool protection = false, const std::string& requiredAccess = "user")
        : ShapeProxy("CompositeProxy"), realShape_(std::move(shape)), 
          enableLogging_(logging), enableCaching_(caching), enableProtection_(protection),
          accessLevel_(requiredAccess), areaCached_(false) {}
    
    void draw() const override {
        logOperation("draw()");
        if (enableProtection_ && !checkAccess()) {
            std::cout << "[" << proxyName_ << "] ACCESS DENIED - Cannot draw\n";
            return;
        }
        realShape_->draw();
    }
    
    std::string getType() const override {
        logOperation("getType()");
        if (enableProtection_ && !checkAccess()) {
            return "PROTECTED_SHAPE_ACCESS_DENIED";
        }
        return realShape_->getType();
    }
    
    double getArea() const override {
        logOperation("getArea()");
        if (enableProtection_ && !checkAccess()) {
            return -1.0;
        }
        
        if (enableCaching_ && !areaCached_) {
            std::cout << "[" << proxyName_ << "] Calculating and caching area...\n";
            cachedArea_ = realShape_->getArea();
            areaCached_ = true;
            return cachedArea_;
        } else if (enableCaching_ && areaCached_) {
            std::cout << "[" << proxyName_ << "] Returning cached area...\n";
            return cachedArea_;
        } else {
            return realShape_->getArea();
        }
    }
    
    void printInfo() const override {
        logOperation("printInfo()");
        if (enableProtection_ && !checkAccess()) {
            std::cout << "Shape information protected - access denied\n";
            return;
        }
        realShape_->printInfo();
    }
    
private:
    void logOperation(const std::string& operation) {
        if (enableLogging_) {
            logEntries_.push_back(operation);
            std::cout << "[" << proxyName_ << "] LOG: " << operation << std::endl;
        }
    }
    
    bool checkAccess() const {
        return accessLevel_ == "admin" || accessLevel_ == "user"; // Simplified check
    }
    
public:
    size_t getLogSize() const { return logEntries_.size(); }
    void clearCache() { areaCached_ = false; cachedArea_ = 0.0; }
};

#endif // PROXY_PATTERNS_H