#ifndef PROXY_SHAPES_H
#define PROXY_SHAPES_H

#include "shape_proxy.h"
#include <map>
#include <string>
#include <memory>
#include <chrono>
#include <thread>

// Logging Proxy - logs all operations on the shape
class LoggingShapeProxy : public Shape {
private:
    std::unique_ptr<Shape> realShape_;
    
public:
    explicit LoggingShapeProxy(std::unique_ptr<Shape> shape) 
        : realShape_(std::move(shape)) {}
    
    void draw() const override {
        std::cout << "[LOG] Calling draw() on " << realShape_->getType() 
                  << " at " << getCurrentTime() << std::endl;
        realShape_->draw();
        std::cout << "[LOG] Completed draw() on " << realShape_->getType() << std::endl;
    }
    
    std::string getType() const override {
        std::cout << "[LOG] Calling getType() at " << getCurrentTime() << std::endl;
        return realShape_->getType();
    }
    
    double getArea() const override {
        std::cout << "[LOG] Calling getArea() at " << getCurrentTime() << std::endl;
        auto area = realShape_->getArea();
        std::cout << "[LOG] Area of " << realShape_->getType() << " is " << area << std::endl;
        return area;
    }
    
private:
    std::string getCurrentTime() const {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        return std::to_string(time_t);
    }
};

// Caching Proxy - caches expensive operations
class CachingShapeProxy : public Shape {
private:
    std::unique_ptr<Shape> realShape_;
    mutable double cachedArea_;
    mutable bool areaCached_;
    
public:
    explicit CachingShapeProxy(std::unique_ptr<Shape> shape) 
        : realShape_(std::move(shape)), cachedArea_(0.0), areaCached_(false) {}
    
    void draw() const override {
        std::cout << "[CACHE] Drawing with caching proxy: ";
        realShape_->draw();
    }
    
    std::string getType() const override {
        return realShape_->getType();
    }
    
    double getArea() const override {
        if (!areaCached_) {
            std::cout << "[CACHE] Computing area for first time..." << std::endl;
            cachedArea_ = realShape_->getArea();
            areaCached_ = true;
        } else {
            std::cout << "[CACHE] Returning cached area..." << std::endl;
        }
        return cachedArea_;
    }
    
    // Method to clear the cache if needed
    void clearCache() {
        areaCached_ = false;
        cachedArea_ = 0.0;
    }
};

// Protection Proxy - controls access to the shape
class ProtectedShapeProxy : public Shape {
private:
    std::unique_ptr<Shape> realShape_;
    std::string accessLevel_;
    bool authorized_;
    
public:
    ProtectedShapeProxy(std::unique_ptr<Shape> shape, const std::string& accessLevel) 
        : realShape_(std::move(shape)), accessLevel_(accessLevel), authorized_(false) {
        // In a real system, this would check against user credentials
        // For demo, we'll just authorize based on access level
        authorized_ = (accessLevel_ == "admin" || accessLevel_ == "user");
    }
    
    void draw() const override {
        if (authorized_) {
            std::cout << "[PROTECTED] Access granted for " << accessLevel_ << " - ";
            realShape_->draw();
        } else {
            std::cout << "[PROTECTED] Access denied for " << accessLevel_ 
                      << " - Cannot draw " << realShape_->getType() << std::endl;
        }
    }
    
    std::string getType() const override {
        if (authorized_) {
            return realShape_->getType();
        } else {
            return "Protected Shape (Access Denied)";
        }
    }
    
    double getArea() const override {
        if (authorized_) {
            std::cout << "[PROTECTED] Access granted for " << accessLevel_ << " - ";
            return realShape_->getArea();
        } else {
            std::cout << "[PROTECTED] Access denied for " << accessLevel_ 
                      << " - Cannot calculate area" << std::endl;
            return -1.0; // Error value
        }
    }
    
    bool isAuthorized() const {
        return authorized_;
    }
};

// Remote Proxy Simulation - simulates network delay
class RemoteShapeProxy : public Shape {
private:
    std::unique_ptr<Shape> realShape_;
    std::string serverAddress_;
    
public:
    RemoteShapeProxy(std::unique_ptr<Shape> shape, const std::string& serverAddress) 
        : realShape_(std::move(shape)), serverAddress_(serverAddress) {}
    
    void draw() const override {
        std::cout << "[REMOTE] Connecting to server " << serverAddress_ << "..." << std::endl;
        simulateNetworkDelay();
        std::cout << "[REMOTE] Rendering shape on remote server..." << std::endl;
        realShape_->draw();
        std::cout << "[REMOTE] Operation completed on server " << serverAddress_ << std::endl;
    }
    
    std::string getType() const override {
        std::cout << "[REMOTE] Fetching type from server " << serverAddress_ << "..." << std::endl;
        simulateNetworkDelay();
        return realShape_->getType();
    }
    
    double getArea() const override {
        std::cout << "[REMOTE] Requesting area calculation from server " << serverAddress_ << "..." << std::endl;
        simulateNetworkDelay();
        auto result = realShape_->getArea();
        std::cout << "[REMOTE] Received area from server: " << result << std::endl;
        return result;
    }
    
private:
    void simulateNetworkDelay() const {
        // Simulate network delay
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
};

#endif // PROXY_SHAPES_H