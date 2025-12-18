#include <iostream>
#include <memory>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <chrono>
#include <random>

// Using the Shape hierarchy from the enhanced_shape_factory example
class IDrawable {
public:
    virtual ~IDrawable() = default;
    virtual void draw() const = 0;
    virtual void resize(double factor) = 0;
    virtual double getArea() const = 0;
};

class Shape : public IDrawable {
protected:
    double x = 0.0, y = 0.0;  // Position coordinates
    double scale = 1.0;       // Scale factor

public:
    Shape(double pos_x = 0.0, double pos_y = 0.0) : x(pos_x), y(pos_y) {}

    // Common functionality for all shapes
    void setPosition(double pos_x, double pos_y) {
        x = pos_x;
        y = pos_y;
    }

    void getPosition(double& pos_x, double& pos_y) const {
        pos_x = x;
        pos_y = y;
    }

    virtual std::string getType() const = 0;

    // Default implementation for resize
    void resize(double factor) override {
        scale *= factor;
    }

    // Pure virtual functions that derived classes must implement
    virtual void draw() const = 0;
    virtual double getArea() const = 0;
};

class Circle : public Shape {
private:
    double radius;

public:
    Circle(double r = 1.0, double pos_x = 0.0, double pos_y = 0.0)
        : Shape(pos_x, pos_y), radius(r) {}

    void draw() const override {
        std::cout << "Drawing a " << getType() << " with radius " << radius
                  << " at position (" << x << ", " << y << ")" << std::endl;
    }

    std::string getType() const override {
        return "Circle";
    }

    double getArea() const override {
        return 3.14159 * radius * radius * scale * scale;
    }
};

class Rectangle : public Shape {
private:
    double width, height;

public:
    Rectangle(double w = 1.0, double h = 1.0, double pos_x = 0.0, double pos_y = 0.0)
        : Shape(pos_x, pos_y), width(w), height(h) {}

    void draw() const override {
        std::cout << "Drawing a " << getType() << " with dimensions "
                  << width << "x" << height << " at position (" << x << ", " << y << ")" << std::endl;
    }

    std::string getType() const override {
        return "Rectangle";
    }

    double getArea() const override {
        return width * height * scale * scale;
    }
};

class Triangle : public Shape {
private:
    double base, height;

public:
    Triangle(double b = 1.0, double h = 1.0, double pos_x = 0.0, double pos_y = 0.0)
        : Shape(pos_x, pos_y), base(b), height(h) {}

    void draw() const override {
        std::cout << "Drawing a " << getType() << " with base " << base
                  << " and height " << height << " at position (" << x << ", " << y << ")" << std::endl;
    }

    std::string getType() const override {
        return "Triangle";
    }

    double getArea() const override {
        return 0.5 * base * height * scale * scale;
    }
};

class ShapeFactory {
public:
    enum class ShapeType {
        CIRCLE,
        RECTANGLE,
        TRIANGLE
    };

    static std::unique_ptr<Shape> createShape(ShapeType type,
                                              double param1 = 1.0,
                                              double param2 = 1.0,
                                              double pos_x = 0.0,
                                              double pos_y = 0.0) {
        switch (type) {
            case ShapeType::CIRCLE:
                return std::make_unique<Circle>(param1, pos_x, pos_y);
            case ShapeType::RECTANGLE:
                return std::make_unique<Rectangle>(param1, param2, pos_x, pos_y);
            case ShapeType::TRIANGLE:
                return std::make_unique<Triangle>(param1, param2, pos_x, pos_y);
            default:
                return nullptr;
        }
    }

    static std::unique_ptr<Shape> createShape(const std::string& shapeType,
                                              double param1 = 1.0,
                                              double param2 = 1.0,
                                              double pos_x = 0.0,
                                              double pos_y = 0.0) {
        if (shapeType == "circle") {
            return std::make_unique<Circle>(param1, pos_x, pos_y);
        } else if (shapeType == "rectangle") {
            return std::make_unique<Rectangle>(param1, param2, pos_x, pos_y);
        } else if (shapeType == "triangle") {
            return std::make_unique<Triangle>(param1, param2, pos_x, pos_y);
        }

        return nullptr;
    }
};

// Thread-safe queue for shapes
template<typename T>
class ThreadSafeQueue {
private:
    mutable std::mutex mtx;
    std::queue<std::unique_ptr<T>> q;
    std::condition_variable cond;

public:
    ThreadSafeQueue() = default;

    void push(std::unique_ptr<T> item) {
        std::lock_guard<std::mutex> lock(mtx);
        q.push(std::move(item));
        cond.notify_one();  // Notify one waiting consumer
    }

    std::unique_ptr<T> wait_and_pop() {
        std::unique_lock<std::mutex> lock(mtx);
        // Wait until queue is not empty
        while(q.empty()) {
            cond.wait(lock);
        }
        std::unique_ptr<T> item = std::move(q.front());
        q.pop();
        return item;
    }

    bool try_pop(std::unique_ptr<T>& item) {
        std::lock_guard<std::mutex> lock(mtx);
        if(q.empty()) {
            return false;
        }
        item = std::move(q.front());
        q.pop();
        return true;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx);
        return q.empty();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mtx);
        return q.size();
    }
};

// Shape Factory Producer class
class ShapeFactoryProducer {
private:
    ThreadSafeQueue<Shape>& shapeBuffer;
    std::mt19937 gen;
    std::uniform_int_distribution<> dis_shape;
    std::uniform_real_distribution<> dis_param;
    int maxShapes;
    
public:
    ShapeFactoryProducer(ThreadSafeQueue<Shape>& buffer, int totalShapes = 10) 
        : shapeBuffer(buffer), 
          gen(std::random_device{}()), 
          dis_shape(0, 2), 
          dis_param(0.5, 5.0),
          maxShapes(totalShapes) {}

    void produce() {
        for(int i = 0; i < maxShapes; ++i) {
            // Randomly select shape type
            int shapeType = dis_shape(gen);
            std::unique_ptr<Shape> shape;
            
            switch(shapeType) {
                case 0: // Circle
                    shape = ShapeFactory::createShape(ShapeFactory::ShapeType::CIRCLE, 
                                                     dis_param(gen), 0.0, 
                                                     dis_param(gen) * 10, dis_param(gen) * 10);
                    break;
                case 1: // Rectangle
                    shape = ShapeFactory::createShape(ShapeFactory::ShapeType::RECTANGLE, 
                                                     dis_param(gen), dis_param(gen),
                                                     dis_param(gen) * 10, dis_param(gen) * 10);
                    break;
                case 2: // Triangle
                    shape = ShapeFactory::createShape(ShapeFactory::ShapeType::TRIANGLE, 
                                                     dis_param(gen), dis_param(gen),
                                                     dis_param(gen) * 10, dis_param(gen) * 10);
                    break;
            }
            
            if(shape) {
                std::cout << "Producer: Creating " << shape->getType() 
                         << " (Area: " << shape->getArea() << ")" << std::endl;
                shapeBuffer.push(std::move(shape));
                
                // Simulate production time
                std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(200 + (dis_param(gen) * 300))));
            }
        }
        
        std::cout << "Producer: Finished creating " << maxShapes << " shapes." << std::endl;
    }
};

// Shape Consumer class
class ShapeConsumer {
private:
    ThreadSafeQueue<Shape>& shapeBuffer;
    std::string consumerName;
    bool& shouldStop;
    
public:
    ShapeConsumer(ThreadSafeQueue<Shape>& buffer, const std::string& name, bool& stopFlag) 
        : shapeBuffer(buffer), consumerName(name), shouldStop(stopFlag) {}
    
    void consume() {
        while(!shouldStop || !shapeBuffer.empty()) {
            std::unique_ptr<Shape> shape = shapeBuffer.wait_and_pop();
            
            if(shape) {
                std::cout << "Consumer " << consumerName << ": Rendering " 
                         << shape->getType() << " (Area: " << shape->getArea() << ")" << std::endl;
                shape->draw();
                
                // Simulate rendering time
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        }
        
        std::cout << "Consumer " << consumerName << ": Finished consuming shapes." << std::endl;
    }
};

int main() {
    std::cout << "Producer-Consumer Pattern with Shapes Example\n" << std::endl;
    
    // Create the shared buffer (queue) for shapes
    ThreadSafeQueue<Shape> shapeBuffer;
    
    // Flag to control when consumers should stop
    bool shouldStop = false;
    
    // Create producer and consumer threads
    ShapeFactoryProducer producer(shapeBuffer, 15);
    ShapeConsumer consumer1(shapeBuffer, "1", shouldStop);
    ShapeConsumer consumer2(shapeBuffer, "2", shouldStop);
    
    // Start threads
    std::thread producerThread(&ShapeFactoryProducer::produce, &producer);
    std::thread consumer1Thread(&ShapeConsumer::consume, &consumer1);
    std::thread consumer2Thread(&ShapeConsumer::consume, &consumer2);
    
    // Wait for producer to finish
    producerThread.join();
    
    // After producer finishes, allow consumers to finish processing remaining items
    shouldStop = true;
    
    // Wait for consumers to finish
    consumer1Thread.join();
    consumer2Thread.join();
    
    std::cout << "\nProducer-Consumer demonstration completed!" << std::endl;
    
    return 0;
}