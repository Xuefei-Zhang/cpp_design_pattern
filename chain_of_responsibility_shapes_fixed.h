#ifndef CHAIN_OF_RESPONSIBILITY_PATTERNS_H
#define CHAIN_OF_RESPONSIBILITY_PATTERNS_H

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <chrono>

// Request class to represent operations on shapes
class ShapeOperationRequest {
public:
    enum class OperationType {
        DRAW,
        CALCULATE_AREA,
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
    
    // Main method to handle requests (either handles or forwards to successor)
    virtual bool handleRequest(const ShapeOperationRequest& request) = 0;

    // Getters
    const std::string& getHandlerName() const { return handlerName_; }
    ShapeHandler* getSuccessor() const { return successor_.get(); }

    // Pure virtual methods for polymorphic behavior
    virtual std::string getHandlerType() const = 0;

    virtual ~ShapeHandler() = default;
};

// Concrete Handler 1: Validation Handler
class ValidationHandler : public ShapeHandler {
private:
    std::vector<std::string> allowedShapes_;

public:
    ValidationHandler(const std::vector<std::string>& allowedShapes = {})
        : ShapeHandler("ValidationHandler"), allowedShapes_(allowedShapes) {}

    std::string getHandlerType() const override {
        return "ValidationHandler";
    }
    
    bool handleRequest(const ShapeOperationRequest& request) override {
        // Validate shape name
        bool isAllowed = allowedShapes_.empty() || 
                        std::find(allowedShapes_.begin(), allowedShapes_.end(), 
                                 request.getShapeName()) != allowedShapes_.end();
        
        if (isAllowed) {
            std::cout << "[" << handlerName_ << "] Valid request for " << request.getShapeName() 
                      << " operation: " << request.getOperationAsString() << std::endl;
            
            // If we have a successor, pass the request along
            if (successor_) {
                return successor_->handleRequest(request);
            }
            return true; // Request handled by validation only
        } else {
            std::cout << "[" << handlerName_ << "] Invalid request - Shape '" 
                      << request.getShapeName() << "' is not allowed!" << std::endl;
            return false; // Request not valid
        }
    }
    
    void setAllowedShapes(const std::vector<std::string>& shapes) {
        allowedShapes_ = shapes;
    }
};

// Concrete Handler 2: Logging Handler
class LoggingHandler : public ShapeHandler {
public:
    LoggingHandler() : ShapeHandler("LoggingHandler") {}

    std::string getHandlerType() const override {
        return "LoggingHandler";
    }

    bool handleRequest(const ShapeOperationRequest& request) override {
        std::cout << "[" << handlerName_ << "] Logging operation: "
                  << request.getOperationAsString()
                  << " on " << request.getShapeName() << std::endl;

        // Pass to successor if available
        if (successor_) {
            return successor_->handleRequest(request);
        }
        return true; // Log handled, no further processing needed
    }
};

// Concrete Handler 3: Drawing Handler
class DrawHandler : public ShapeHandler {
public:
    DrawHandler() : ShapeHandler("DrawHandler") {}

    std::string getHandlerType() const override {
        return "DrawHandler";
    }

    bool handleRequest(const ShapeOperationRequest& request) override {
        if (request.getOperationType() == ShapeOperationRequest::OperationType::DRAW) {
            std::cout << "[" << handlerName_ << "] Drawing " << request.getShapeName()
                      << " with size (" << request.getParam1() << " x " << request.getParam2()
                      << "), color=" << request.getStringValue() << std::endl;
            return true; // Successfully handled
        } else if (successor_) {
            std::cout << "[" << handlerName_ << "] Cannot handle "
                      << request.getOperationAsString() << ", passing to next handler..." << std::endl;
            return successor_->handleRequest(request);
        } else {
            std::cout << "[" << handlerName_ << "] Cannot handle "
                      << request.getOperationAsString() << " and no successor available." << std::endl;
            return false; // Could not handle
        }
    }
};

// Concrete Handler 4: Area Calculation Handler
class AreaCalculationHandler : public ShapeHandler {
public:
    AreaCalculationHandler() : ShapeHandler("AreaCalculationHandler") {}

    std::string getHandlerType() const override {
        return "AreaCalculationHandler";
    }

    bool handleRequest(const ShapeOperationRequest& request) override {
        if (request.getOperationType() == ShapeOperationRequest::OperationType::CALCULATE_AREA) {
            double area = request.getParam1() * request.getParam2(); // Simplified calculation
            std::cout << "[" << handlerName_ << "] Calculated area for " << request.getShapeName()
                      << ": " << area << std::endl;
            return true; // Successfully handled
        } else if (successor_) {
            std::cout << "[" << handlerName_ << "] Cannot handle "
                      << request.getOperationAsString() << ", passing to next handler..." << std::endl;
            return successor_->handleRequest(request);
        } else {
            std::cout << "[" << handlerName_ << "] Cannot handle "
                      << request.getOperationAsString() << " and no successor available." << std::endl;
            return false; // Could not handle
        }
    }
};

// Concrete Handler 5: Transformation Handler
class TransformationHandler : public ShapeHandler {
public:
    TransformationHandler() : ShapeHandler("TransformationHandler") {}

    std::string getHandlerType() const override {
        return "TransformationHandler";
    }

    bool handleRequest(const ShapeOperationRequest& request) override {
        if (request.getOperationType() == ShapeOperationRequest::OperationType::ROTATE ||
            request.getOperationType() == ShapeOperationRequest::OperationType::SCALE) {

            if (request.getOperationType() == ShapeOperationRequest::OperationType::ROTATE) {
                std::cout << "[" << handlerName_ << "] Rotating " << request.getShapeName()
                          << " by " << request.getParam1() << " degrees" << std::endl;
            } else if (request.getOperationType() == ShapeOperationRequest::OperationType::SCALE) {
                std::cout << "[" << handlerName_ << "] Scaling " << request.getShapeName()
                          << " by factors " << request.getParam1() << " x " << request.getParam2() << std::endl;
            }
            return true; // Successfully handled
        } else if (successor_) {
            std::cout << "[" << handlerName_ << "] Cannot handle "
                      << request.getOperationAsString() << ", passing to next handler..." << std::endl;
            return successor_->handleRequest(request);
        } else {
            std::cout << "[" << handlerName_ << "] Cannot handle "
                      << request.getOperationAsString() << " and no successor available." << std::endl;
            return false; // Could not handle
        }
    }
};

// Concrete Handler 6: Color Modification Handler
class ColorModificationHandler : public ShapeHandler {
public:
    ColorModificationHandler() : ShapeHandler("ColorModificationHandler") {}

    std::string getHandlerType() const override {
        return "ColorModificationHandler";
    }

    bool handleRequest(const ShapeOperationRequest& request) override {
        if (request.getOperationType() == ShapeOperationRequest::OperationType::CHANGE_COLOR) {
            std::cout << "[" << handlerName_ << "] Changing color of " << request.getShapeName()
                      << " to " << request.getStringValue() << std::endl;
            return true; // Successfully handled
        } else if (successor_) {
            std::cout << "[" << handlerName_ << "] Cannot handle "
                      << request.getOperationAsString() << ", passing to next handler..." << std::endl;
            return successor_->handleRequest(request);
        } else {
            std::cout << "[" << handlerName_ << "] Cannot handle "
                      << request.getOperationAsString() << " and no successor available." << std::endl;
            return false; // Could not handle
        }
    }
};

// Concrete Handler 7: Styling Handler
class StylingHandler : public ShapeHandler {
public:
    StylingHandler() : ShapeHandler("StylingHandler") {}

    std::string getHandlerType() const override {
        return "StylingHandler";
    }

    bool handleRequest(const ShapeOperationRequest& request) override {
        if (request.getOperationType() == ShapeOperationRequest::OperationType::ADD_BORDER ||
            request.getOperationType() == ShapeOperationRequest::OperationType::ADD_SHADOW) {

            if (request.getOperationType() == ShapeOperationRequest::OperationType::ADD_BORDER) {
                std::cout << "[" << handlerName_ << "] Adding border to " << request.getShapeName()
                          << " with width " << request.getParam1() << std::endl;
            } else if (request.getOperationType() == ShapeOperationRequest::OperationType::ADD_SHADOW) {
                std::cout << "[" << handlerName_ << "] Adding shadow to " << request.getShapeName()
                          << " with distance " << request.getParam1() << std::endl;
            }
            return true; // Successfully handled
        } else if (successor_) {
            std::cout << "[" << handlerName_ << "] Cannot handle "
                      << request.getOperationAsString() << ", passing to next handler..." << std::endl;
            return successor_->handleRequest(request);
        } else {
            std::cout << "[" << handlerName_ << "] Cannot handle "
                      << request.getOperationAsString() << " and no successor available." << std::endl;
            return false; // Could not handle
        }
    }
};

// Chain Builder for easy chain composition
class ShapeHandlerChainBuilder {
private:
    std::vector<std::unique_ptr<ShapeHandler>> handlers_;
    std::vector<std::string> handlerTypes_;

public:
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

// Graphics Processing Chain (specialized chain)
class GraphicsProcessingChain {
private:
    std::unique_ptr<ShapeHandler> chain_;

public:
    GraphicsProcessingChain(const std::vector<std::string>& allowedShapes = {}) {
        chain_ = ShapeHandlerChainBuilder()
                    .addValidationHandler(allowedShapes)
                    .addLoggingHandler()
                    .addDrawHandler()
                    .buildChain();
    }
    
    bool processOperation(const ShapeOperationRequest& request) {
        if (chain_) {
            std::cout << "[GRAPHICS CHAIN] Processing operation: " 
                      << request.getOperationAsString() << std::endl;
            return chain_->handleRequest(request);
        }
        return false; // No chain to process
    }
    
    void setChain(std::unique_ptr<ShapeHandler> newChain) {
        chain_ = std::move(newChain);
    }
    
    ShapeHandler* getChainHead() const { return chain_.get(); }
};

// Animation Processing Chain (specialized chain)
class AnimationProcessingChain {
private:
    std::unique_ptr<ShapeHandler> chain_;

public:
    AnimationProcessingChain() {
        chain_ = ShapeHandlerChainBuilder()
                    .addValidationHandler()
                    .addLoggingHandler()
                    .addTransformationHandler()
                    .buildChain();
    }
    
    bool processAnimation(const ShapeOperationRequest& request) {
        if (chain_) {
            std::cout << "[ANIMATION CHAIN] Processing animation: " 
                      << request.getOperationAsString() << std::endl;
            return chain_->handleRequest(request);
        }
        return false; // No chain to process
    }
    
    ShapeHandler* getChainHead() const { return chain_.get(); }
};

// Style Processing Chain (specialized chain)
class StyleProcessingChain {
private:
    std::unique_ptr<ShapeHandler> chain_;

public:
    StyleProcessingChain() {
        chain_ = ShapeHandlerChainBuilder()
                    .addValidationHandler()
                    .addLoggingHandler()
                    .addColorModificationHandler()
                    .addStylingHandler()
                    .buildChain();
    }
    
    bool processStyle(const ShapeOperationRequest& request) {
        if (chain_) {
            std::cout << "[STYLE CHAIN] Processing style: " 
                      << request.getOperationAsString() << std::endl;
            return chain_->handleRequest(request);
        }
        return false; // No chain to process
    }
    
    ShapeHandler* getChainHead() const { return chain_.get(); }
};

// Chain Manager to manage multiple specialized chains
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
    
    void addCustomChain(std::unique_ptr<ShapeHandler> chain) {
        customChains_.push_back(std::move(chain));
    }
    
    size_t getCustomChainCount() const { return customChains_.size(); }
    
    void printChainInfo() const {
        std::cout << "[SHAPE CHAIN MANAGER] Available specialized chains:" << std::endl;
        std::cout << "  - Graphics Processing Chain" << std::endl;
        std::cout << "  - Animation Processing Chain" << std::endl;
        std::cout << "  - Style Processing Chain" << std::endl;
        std::cout << "  - Custom Chains: " << customChains_.size() << std::endl;
    }
};

#endif // CHAIN_OF_RESPONSIBILITY_PATTERNS_H