#ifndef CHAIN_OF_RESPONSIBILITY_PATTERNS_H
#define CHAIN_OF_RESPONSIBILITY_PATTERNS_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <sstream>

// Request class to represent operations on shapes
class ShapeOperationRequest {
public:
    enum class OperationType {
        DRAW,
        CALCULATE_AREA,
        APPLY_EFFECT,
        CHANGE_COLOR,
        ROTATE,
        SCALE,
        ADD_BORDER,
        ADD_SHADOW
    };

private:
    OperationType operationType_;
    std::string shapeName_;
    double param1_, param2_, param3_;  // General parameters for operations
    std::string stringValue_;         // String parameter for color, etc.
    bool processed_;                  // Has this request been handled?

public:
    ShapeOperationRequest(OperationType op, const std::string& shapeName,
                         double p1 = 0.0, double p2 = 0.0, double p3 = 0.0,
                         const std::string& strVal = "")
        : operationType_(op), shapeName_(shapeName), param1_(p1), param2_(p2),
          param3_(p3), stringValue_(strVal), processed_(false) {}

    OperationType getOperationType() const { return operationType_; }
    const std::string& getShapeName() const { return shapeName_; }
    double getParam1() const { return param1_; }
    double getParam2() const { return param2_; }
    double getParam3() const { return param3_; }
    const std::string& getStringValue() const { return stringValue_; }

    bool isProcessed() const { return processed_; }
    void markAsProcessed() { processed_ = true; }

    std::string getOperationAsString() const {
        switch (operationType_) {
            case OperationType::DRAW: return "DRAW";
            case OperationType::CALCULATE_AREA: return "CALCULATE_AREA";
            case OperationType::APPLY_EFFECT: return "APPLY_EFFECT";
            case OperationType::CHANGE_COLOR: return "CHANGE_COLOR";
            case OperationType::ROTATE: return "ROTATE";
            case OperationType::SCALE: return "SCALE";
            case OperationType::ADD_BORDER: return "ADD_BORDER";
            case OperationType::ADD_SHADOW: return "ADD_SHADOW";
            default: return "UNKNOWN";
        }
    }
};

// Abstract Handler - Base class for the chain of responsibility
class ShapeHandler {
protected:
    std::unique_ptr<ShapeHandler> successor_;
    std::string handlerName_;

public:
    explicit ShapeHandler(const std::string& name) : handlerName_(name) {}
    
    // Set successor in the chain
    void setSuccessor(std::unique_ptr<ShapeHandler> successor) {
        successor_ = std::move(successor);
    }
    
    // Handle request method - either handles it or passes to successor
    virtual bool handleRequest(const ShapeOperationRequest& request) {
        if (canHandle(request)) {
            processRequest(request);
            return true; // Request was handled
        } else if (successor_) {
            std::cout << "[" << handlerName_ << "] Cannot handle this request, passing to next handler..." << std::endl;
            return successor_->handleRequest(request);
        } else {
            std::cout << "[" << handlerName_ << "] Cannot handle and no successor available." << std::endl;
            return false; // Request couldn't be handled
        }
    }
    
    // Pure virtual methods to be implemented by concrete handlers
    virtual bool canHandle(const ShapeOperationRequest& request) = 0;
    virtual void processRequest(const ShapeOperationRequest& request) = 0;
    
    // Getters
    const std::string& getHandlerName() const { return handlerName_; }
    ShapeHandler* getSuccessor() const { return successor_.get(); }
    
    virtual ~ShapeHandler() = default;
};

// Concrete Handler 1: Draw Handler
class DrawHandler : public ShapeHandler {
public:
    DrawHandler() : ShapeHandler("DrawHandler") {}
    
    bool canHandle(const ShapeOperationRequest& request) override {
        return request.getOperationType() == ShapeOperationRequest::OperationType::DRAW ||
               request.getOperationType() == ShapeOperationRequest::OperationType::APPLY_EFFECT;
    }
    
    void processRequest(const ShapeOperationRequest& request) override {
        if (request.getOperationType() == ShapeOperationRequest::OperationType::DRAW) {
            std::cout << "[" << handlerName_ << "] Drawing " << request.getShapeName() 
                      << " at position (" << request.getParam1() << ", " << request.getParam2() << ")" << std::endl;
        } else if (request.getOperationType() == ShapeOperationRequest::OperationType::APPLY_EFFECT) {
            std::cout << "[" << handlerName_ << "] Applying visual effect to " << request.getShapeName() 
                      << ": " << request.getStringValue() << std::endl;
        }
        const_cast<ShapeOperationRequest&>(request).markAsProcessed();
    }
};

// Concrete Handler 2: Area Calculation Handler
class AreaCalculationHandler : public ShapeHandler {
public:
    AreaCalculationHandler() : ShapeHandler("AreaCalculationHandler") {}

    bool canHandle(const ShapeOperationRequest& request) override {
        return request.getOperationType() == ShapeOperationRequest::OperationType::CALCULATE_AREA;
    }

    void processRequest(const ShapeOperationRequest& request) override {
        std::cout << "[" << handlerName_ << "] Calculating area for " << request.getShapeName()
                  << " with parameters: " << request.getParam1() << ", " << request.getParam2() << std::endl;

        // Simulate area calculation based on parameters
        double area = request.getParam1() * request.getParam2(); // Simplified calculation
        std::cout << "Calculated area: " << area << std::endl;

        const_cast<ShapeOperationRequest&>(request).markAsProcessed();
    }
};

// Concrete Handler 3: Transformation Handler
class TransformationHandler : public ShapeHandler {
public:
    TransformationHandler() : ShapeHandler("TransformationHandler") {}
    
    bool canHandle(const ShapeOperationRequest& request) override {
        return request.getOperationType() == ShapeOperationRequest::OperationType::ROTATE ||
               request.getOperationType() == ShapeOperationRequest::OperationType::SCALE;
    }
    
    void processRequest(const ShapeOperationRequest& request) override {
        if (request.getOperationType() == ShapeOperationRequest::OperationType::ROTATE) {
            std::cout << "[" << handlerName_ << "] Rotating " << request.getShapeName() 
                      << " by " << request.getParam1() << " degrees around center" << std::endl;
        } else if (request.getOperationType() == ShapeOperationRequest::OperationType::SCALE) {
            std::cout << "[" << handlerName_ << "] Scaling " << request.getShapeName() 
                      << " by factors " << request.getParam1() << "x and " << request.getParam2() << "y" << std::endl;
        }
        const_cast<ShapeOperationRequest&>(request).markAsProcessed();
    }
};

// Concrete Handler 4: Color Modification Handler
class ColorModificationHandler : public ShapeHandler {
public:
    ColorModificationHandler() : ShapeHandler("ColorModificationHandler") {}
    
    bool canHandle(const ShapeOperationRequest& request) override {
        return request.getOperationType() == ShapeOperationRequest::OperationType::CHANGE_COLOR;
    }
    
    void processRequest(const ShapeOperationRequest& request) override {
        std::cout << "[" << handlerName_ << "] Changing color of " << request.getShapeName()
                  << " from current color to " << request.getStringValue() << std::endl;
        const_cast<ShapeOperationRequest&>(request).markAsProcessed();
    }
};

// Concrete Handler 5: Styling Handler
class StylingHandler : public ShapeHandler {
public:
    StylingHandler() : ShapeHandler("StylingHandler") {}
    
    bool canHandle(const ShapeOperationRequest& request) override {
        return request.getOperationType() == ShapeOperationRequest::OperationType::ADD_BORDER ||
               request.getOperationType() == ShapeOperationRequest::OperationType::ADD_SHADOW;
    }
    
    void processRequest(const ShapeOperationRequest& request) override {
        if (request.getOperationType() == ShapeOperationRequest::OperationType::ADD_BORDER) {
            std::cout << "[" << handlerName_ << "] Adding border to " << request.getShapeName() 
                      << " with width " << request.getParam1() 
                      << " and color " << request.getStringValue() << std::endl;
        } else if (request.getOperationType() == ShapeOperationRequest::OperationType::ADD_SHADOW) {
            std::cout << "[" << handlerName_ << "] Adding shadow to " << request.getShapeName() 
                      << " with distance " << request.getParam1() 
                      << " and color " << request.getStringValue() << std::endl;
        }
        const_cast<ShapeOperationRequest&>(request).markAsProcessed();
    }
};

// Advanced Handler: Validation Handler (checks if operations are valid)
class ValidationHandler : public ShapeHandler {
private:
    std::vector<std::string> allowedShapes_;

public:
    ValidationHandler(const std::vector<std::string>& allowedShapes = {}) 
        : ShapeHandler("ValidationHandler"), allowedShapes_(allowedShapes) {}
    
    bool canHandle(const ShapeOperationRequest& request) override {
        // This handler validates ALL requests
        return true;
    }
    
    void processRequest(const ShapeOperationRequest& request) override {
        bool isValid = true;
        
        // Validate shape name
        if (!allowedShapes_.empty()) {
            if (std::find(allowedShapes_.begin(), allowedShapes_.end(), 
                         request.getShapeName()) == allowedShapes_.end()) {
                std::cout << "[" << handlerName_ << "] ERROR: Shape '" << request.getShapeName() 
                          << "' is not in allowed list!" << std::endl;
                isValid = false;
            }
        }
        
        // Validate operation-specific parameters
        switch (request.getOperationType()) {
            case ShapeOperationRequest::OperationType::ROTATE:
                if (request.getParam1() < 0 || request.getParam1() > 360) {
                    std::cout << "[" << handlerName_ << "] ERROR: Invalid rotation angle: " 
                              << request.getParam1() << std::endl;
                    isValid = false;
                }
                break;
            case ShapeOperationRequest::OperationType::SCALE:
                if (request.getParam1() <= 0 || request.getParam2() <= 0) {
                    std::cout << "[" << handlerName_ << "] ERROR: Invalid scale factors: " 
                              << request.getParam1() << "x" << request.getParam2() << std::endl;
                    isValid = false;
                }
                break;
            default:
                break;
        }
        
        if (isValid) {
            std::cout << "[" << handlerName_ << "] Valid request for " << request.getShapeName() 
                      << " operation: " << request.getOperationAsString() << std::endl;
        } else {
            std::cout << "[" << handlerName_ << "] Request validation failed!" << std::endl;
        }
    }
    
    void setAllowedShapes(const std::vector<std::string>& shapes) {
        allowedShapes_ = shapes;
    }
    
    bool isShapeAllowed(const std::string& shapeName) const {
        return allowedShapes_.empty() || 
               std::find(allowedShapes_.begin(), allowedShapes_.end(), shapeName) != allowedShapes_.end();
    }
};

// Advanced Handler: Logging Handler (logs all operations)
class LoggingHandler : public ShapeHandler {
private:
    std::vector<std::string> logEntries_;

public:
    LoggingHandler() : ShapeHandler("LoggingHandler") {}
    
    bool canHandle(const ShapeOperationRequest& request) override {
        // This handler logs ALL requests
        return true;
    }
    
    void processRequest(const ShapeOperationRequest& request) override {
        std::ostringstream logStream;
        logStream << "[" << handlerName_ << "] LOGGED: " << request.getOperationAsString() 
                  << " on " << request.getShapeName();
        
        if (request.getParam1() != 0.0 || request.getParam2() != 0.0) {
            logStream << " with params(" << request.getParam1() << "," << request.getParam2() << ")";
        }
        
        if (!request.getStringValue().empty()) {
            logStream << " [value: " << request.getStringValue() << "]";
        }
        
        std::string logEntry = logStream.str();
        std::cout << logEntry << std::endl;
        logEntries_.push_back(logEntry);
    }
    
    const std::vector<std::string>& getLogEntries() const { return logEntries_; }
    size_t getLogCount() const { return logEntries_.size(); }
    void clearLogs() { logEntries_.clear(); }
};

// Chain Builder - Utility class for creating chains easily
class ShapeHandlerChainBuilder {
private:
    std::vector<std::unique_ptr<ShapeHandler>> handlers_;
    std::vector<std::string> handlerTypes_;

public:
    ShapeHandlerChainBuilder& addDrawHandler() {
        handlers_.push_back(std::make_unique<DrawHandler>());
        handlerTypes_.push_back("DrawHandler");
        return *this;
    }
    
    ShapeHandlerChainBuilder& addAreaCalculationHandler() {
        handlers_.push_back(std::make_unique<AreaCalculationHandler>());
        handlerTypes_.push_back("AreaCalculationHandler");
        return *this;
    }

    ShapeHandlerChainBuilder& addTransformationHandler() {
        handlers_.push_back(std::make_unique<TransformationHandler>());
        handlerTypes_.push_back("TransformationHandler");
        return *this;
    }

    ShapeHandlerChainBuilder& addColorModificationHandler() {
        handlers_.push_back(std::make_unique<ColorModificationHandler>());
        handlerTypes_.push_back("ColorModificationHandler");
        return *this;
    }

    ShapeHandlerChainBuilder& addStylingHandler() {
        handlers_.push_back(std::make_unique<StylingHandler>());
        handlerTypes_.push_back("StylingHandler");
        return *this;
    }

    ShapeHandlerChainBuilder& addValidationHandler(const std::vector<std::string>& allowedShapes = {}) {
        handlers_.push_back(std::make_unique<ValidationHandler>(allowedShapes));
        handlerTypes_.push_back("ValidationHandler");
        return *this;
    }

    ShapeHandlerChainBuilder& addLoggingHandler() {
        handlers_.push_back(std::make_unique<LoggingHandler>());
        handlerTypes_.push_back("LoggingHandler");
        return *this;
    }
    
    std::unique_ptr<ShapeHandler> buildChain() {
        if (handlers_.empty()) {
            return nullptr;
        }
        
        // Connect all handlers in sequence
        for (size_t i = 0; i < handlers_.size() - 1; ++i) {
            handlers_[i]->setSuccessor(std::move(handlers_[i + 1]));
        }
        
        // Return the first handler in the chain
        return std::move(handlers_[0]);
    }
    
    std::vector<std::string> getHandlerOrder() const {
        return handlerTypes_;
    }
    
    size_t getHandlerCount() const {
        return handlers_.size();
    }
};

// Specialized Chain: Graphics Processing Chain
class GraphicsProcessingChain {
private:
    std::unique_ptr<ShapeHandler> headHandler_;

public:
    GraphicsProcessingChain() {
        // Pre-configured chain for graphics operations
        headHandler_ = ShapeHandlerChainBuilder()
                          .addValidationHandler({"Circle", "Rectangle", "Triangle", "Line"})
                          .addLoggingHandler()
                          .addTransformationHandler()
                          .addDrawHandler()
                          .addStylingHandler()
                          .buildChain();
    }
    
    bool processOperation(const ShapeOperationRequest& request) {
        if (headHandler_) {
            std::cout << "[GRAPHICS CHAIN] Processing request through chain:" << std::endl;
            return headHandler_->handleRequest(request);
        }
        std::cout << "[GRAPHICS CHAIN] No handlers in chain!" << std::endl;
        return false;
    }
    
    void setChain(std::unique_ptr<ShapeHandler> chain) {
        headHandler_ = std::move(chain);
    }
    
    ShapeHandler* getChainHead() const { return headHandler_.get(); }
};

// Specialized Chain: Animation Processing Chain
class AnimationProcessingChain {
private:
    std::unique_ptr<ShapeHandler> headHandler_;

public:
    AnimationProcessingChain() {
        // Pre-configured chain for animation operations
        headHandler_ = ShapeHandlerChainBuilder()
                          .addValidationHandler()
                          .addLoggingHandler()
                          .addTransformationHandler()
                          .addDrawHandler()
                          .buildChain();
    }
    
    bool processAnimation(const ShapeOperationRequest& request) {
        if (headHandler_) {
            std::cout << "[ANIMATION CHAIN] Processing animation request:" << std::endl;
            return headHandler_->handleRequest(request);
        }
        std::cout << "[ANIMATION CHAIN] No handlers in chain!" << std::endl;
        return false;
    }
    
    ShapeHandler* getChainHead() const { return headHandler_.get(); }
};

// Specialized Chain: Style Processing Chain
class StyleProcessingChain {
private:
    std::unique_ptr<ShapeHandler> headHandler_;

public:
    StyleProcessingChain() {
        // Pre-configured chain for style operations
        headHandler_ = ShapeHandlerChainBuilder()
                          .addValidationHandler()
                          .addLoggingHandler()
                          .addColorModificationHandler()
                          .addStylingHandler()
                          .addDrawHandler()
                          .buildChain();
    }
    
    bool processStyle(const ShapeOperationRequest& request) {
        if (headHandler_) {
            std::cout << "[STYLE CHAIN] Processing style request:" << std::endl;
            return headHandler_->handleRequest(request);
        }
        std::cout << "[STYLE CHAIN] No handlers in chain!" << std::endl;
        return false;
    }
    
    ShapeHandler* getChainHead() const { return headHandler_.get(); }
};

// Chain Manager - Manages multiple chains for different purposes
class ShapeChainManager {
private:
    std::unique_ptr<GraphicsProcessingChain> graphicsChain_;
    std::unique_ptr<AnimationProcessingChain> animationChain_;
    std::unique_ptr<StyleProcessingChain> styleChain_;
    std::vector<std::unique_ptr<ShapeHandler>> customChains_;

public:
    ShapeChainManager() {
        graphicsChain_ = std::make_unique<GraphicsProcessingChain>();
        animationChain_ = std::make_unique<AnimationProcessingChain>();
        styleChain_ = std::make_unique<StyleProcessingChain>();
    }
    
    bool processGraphicsOperation(const ShapeOperationRequest& request) {
        return graphicsChain_->processOperation(request);
    }
    
    bool processAnimationOperation(const ShapeOperationRequest& request) {
        return animationChain_->processAnimation(request);
    }
    
    bool processStyleOperation(const ShapeOperationRequest& request) {
        return styleChain_->processStyle(request);
    }
    
    // Add a custom chain
    void addCustomChain(std::unique_ptr<ShapeHandler> chain) {
        customChains_.push_back(std::move(chain));
    }
    
    // Process request through a specific custom chain by index
    bool processThroughCustomChain(size_t chainIndex, const ShapeOperationRequest& request) {
        if (chainIndex < customChains_.size()) {
            std::cout << "[CUSTOM CHAIN " << chainIndex << "] Processing request:" << std::endl;
            return customChains_[chainIndex]->handleRequest(request);
        }
        return false;
    }
    
    size_t getCustomChainCount() const { return customChains_.size(); }
    
    void printChainInfo() const {
        std::cout << "[CHAIN MANAGER] Available chains:" << std::endl;
        std::cout << "  - Graphics Processing Chain (pre-configured)" << std::endl;
        std::cout << "  - Animation Processing Chain (pre-configured)" << std::endl;
        std::cout << "  - Style Processing Chain (pre-configured)" << std::endl;
        std::cout << "  - Custom Chains: " << customChains_.size() << std::endl;
    }
};

// Utility class to create complex request processing flows
class RequestProcessor {
public:
    static bool executeRequestWithChain(const ShapeOperationRequest& request,
                                       std::unique_ptr<ShapeHandler> chain) {
        if (chain) {
            std::cout << "[REQUEST PROCESSOR] Executing request through provided chain:" << std::endl;
            return chain->handleRequest(request);
        }
        return false;
    }

    // Removed batch method that was trying to copy the abstract chain
};

#endif // CHAIN_OF_RESPONSIBILITY_PATTERNS_H