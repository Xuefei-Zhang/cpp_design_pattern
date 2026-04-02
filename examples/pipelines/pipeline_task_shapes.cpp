#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>
#include <chrono>
#include <random>
#include <functional>
#include <atomic>
#include <map>

// Reuse the Shape hierarchy from the previous example
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
    std::string id;           // Unique identifier for tracking

public:
    Shape(double pos_x = 0.0, double pos_y = 0.0, const std::string& shape_id = "") 
        : x(pos_x), y(pos_y), id(shape_id) {}

    void setPosition(double pos_x, double pos_y) {
        x = pos_x;
        y = pos_y;
    }

    void getPosition(double& pos_x, double& pos_y) const {
        pos_x = x;
        pos_y = y;
    }

    virtual std::string getType() const = 0;

    void resize(double factor) override {
        scale *= factor;
    }

    virtual void draw() const = 0;
    virtual double getArea() const = 0;
    
    const std::string& getId() const { return id; }
    void setId(const std::string& newId) { id = newId; }
};

class Circle : public Shape {
private:
    double radius;

public:
    Circle(double r = 1.0, double pos_x = 0.0, double pos_y = 0.0, const std::string& shape_id = "")
        : Shape(pos_x, pos_y, shape_id), radius(r) {}

    void draw() const override {
        std::cout << "Drawing a " << getType() << " with radius " << radius
                  << " at position (" << x << ", " << y << ") [ID: " << id << "]" << std::endl;
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
    Rectangle(double w = 1.0, double h = 1.0, double pos_x = 0.0, double pos_y = 0.0, const std::string& shape_id = "")
        : Shape(pos_x, pos_y, shape_id), width(w), height(h) {}

    void draw() const override {
        std::cout << "Drawing a " << getType() << " with dimensions "
                  << width << "x" << height << " at position (" << x << ", " << y << ") [ID: " << id << "]" << std::endl;
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
    Triangle(double b = 1.0, double h = 1.0, double pos_x = 0.0, double pos_y = 0.0, const std::string& shape_id = "")
        : Shape(pos_x, pos_y, shape_id), base(b), height(h) {}

    void draw() const override {
        std::cout << "Drawing a " << getType() << " with base " << base
                  << " and height " << height << " at position (" << x << ", " << y << ") [ID: " << id << "]" << std::endl;
    }

    std::string getType() const override {
        return "Triangle";
    }

    double getArea() const override {
        return 0.5 * base * height * scale * scale;
    }
};

// Trace Logger for Chrome Tracing format
class TraceLogger {
private:
    std::ofstream traceFile;
    std::chrono::steady_clock::time_point startTime;
    mutable std::mutex logMutex;

public:
    TraceLogger(const std::string& filename = "pipeline_trace.json")
        : traceFile(filename), startTime(std::chrono::steady_clock::now()) {
        traceFile << "{\"traceEvents\": [" << std::endl;
    }

    void logEvent(const std::string& name, const std::string& cat,
                  const std::string& ph, const std::string& pid,
                  const std::string& tid, uint64_t ts,
                  const std::map<std::string, std::string>& args = {}) {
        std::lock_guard<std::mutex> lock(logMutex);
        traceFile << "{"
                  << "\"name\":\"" << name << "\","
                  << "\"cat\":\"" << cat << "\","
                  << "\"ph\":\"" << ph << "\","
                  << "\"pid\":\"" << pid << "\","
                  << "\"tid\":\"" << tid << "\","
                  << "\"ts\":" << ts;

        if (!args.empty()) {
            traceFile << ",\"args\":{";
            bool first = true;
            for (const auto& arg : args) {
                if (!first) traceFile << ",";
                traceFile << "\"" << arg.first << "\":\"" << arg.second << "\"";
                first = false;
            }
            traceFile << "}";
        }

        traceFile << "},";
    }

    void logTaskBegin(const std::string& taskName, const std::string& stageName,
                      const std::string& taskId, const std::string& workerId) {
        auto now = std::chrono::steady_clock::now();
        uint64_t ts = std::chrono::duration_cast<std::chrono::microseconds>(
            now - startTime).count();
        std::map<std::string, std::string> args = {{"stage", stageName}, {"taskId", taskId}};
        logEvent(taskName, "execution", "B", "PipelineSystem", workerId, ts, args);
    }

    void logTaskEnd(const std::string& taskName, const std::string& stageName,
                    const std::string& taskId, const std::string& workerId) {
        auto now = std::chrono::steady_clock::now();
        uint64_t ts = std::chrono::duration_cast<std::chrono::microseconds>(
            now - startTime).count();
        std::map<std::string, std::string> args = {{"stage", stageName}, {"taskId", taskId}};
        logEvent(taskName, "execution", "E", "PipelineSystem", workerId, ts, args);
    }

    ~TraceLogger() {
        finalize();
    }

    void finalize() {
        if (traceFile.is_open()) {
            traceFile.seekp(-1, std::ios_base::end); // Remove the last comma
            if (traceFile.tellp() > 0) {  // Make sure we have content to modify
                traceFile << "\n],\"displayTimeUnit\":\"ms\"}" << std::endl;
            } else {
                traceFile << "],\"displayTimeUnit\":\"ms\"}" << std::endl;
            }
            traceFile.close();
        }
    }
};

static TraceLogger traceLogger;

// Factory class for creating shapes
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
                                              double pos_y = 0.0,
                                              const std::string& id = "") {
        switch (type) {
            case ShapeType::CIRCLE:
                return std::make_unique<Circle>(param1, pos_x, pos_y, id);
            case ShapeType::RECTANGLE:
                return std::make_unique<Rectangle>(param1, param2, pos_x, pos_y, id);
            case ShapeType::TRIANGLE:
                return std::make_unique<Triangle>(param1, param2, pos_x, pos_y, id);
            default:
                return nullptr;
        }
    }
};

// Task base class
class Task {
public:
    std::string taskId;
    std::chrono::steady_clock::time_point creationTime;
    
    Task(const std::string& id) : taskId(id), creationTime(std::chrono::steady_clock::now()) {}
    virtual ~Task() = default;
    virtual void execute() = 0;
    virtual std::string getName() const = 0;
};

// Task Queue - thread-safe queue for tasks
template<typename T = Task>
class TaskQueue {
private:
    mutable std::mutex mtx;
    std::queue<std::unique_ptr<T>> q;
    std::condition_variable cond;
    size_t maxSize;

public:
    TaskQueue(size_t max_size = 100) : maxSize(max_size) {}

    bool push(std::unique_ptr<T> task) {
        std::lock_guard<std::mutex> lock(mtx);
        if (q.size() >= maxSize) {
            return false; // Queue is full
        }
        q.push(std::move(task));
        cond.notify_one();
        return true;
    }

    std::unique_ptr<T> waitAndPop() {
        std::unique_lock<std::mutex> lock(mtx);
        while(q.empty()) {
            cond.wait(lock);
        }
        std::unique_ptr<T> task = std::move(q.front());
        q.pop();
        return task;
    }

    bool tryPop(std::unique_ptr<T>& task) {
        std::lock_guard<std::mutex> lock(mtx);
        if(q.empty()) {
            return false;
        }
        task = std::move(q.front());
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

    size_t getMaxSize() const {
        return maxSize;
    }
};

// Pipeline Stage class
class PipelineStage {
private:
    std::string stageName;
    std::vector<std::thread> workers;
    std::unique_ptr<TaskQueue<Task>> inputQueue;
    std::unique_ptr<TaskQueue<Task>> outputQueue;
    std::atomic<bool> shouldStop{false};
    std::function<void()> onTaskCompleted;  // Callback for monitoring
    
public:
    PipelineStage(const std::string& name, size_t numWorkers = 1, size_t queueSize = 50)
        : stageName(name), inputQueue(std::make_unique<TaskQueue<Task>>(queueSize)) {
        
        // If there are more stages after this one, create an output queue
        outputQueue = std::make_unique<TaskQueue<Task>>(queueSize);
        
        // Create worker threads
        for(size_t i = 0; i < numWorkers; ++i) {
            workers.emplace_back([this, i]() {
                workerFunction(i);
            });
        }
    }
    
    // Allow connecting to next stage
    void setOutputQueue(TaskQueue<Task>* nextQueue) {
        outputQueue.reset(nextQueue);  // Share the queue with the next stage
    }
    
    TaskQueue<Task>* getInputQueue() {
        return inputQueue.get();
    }
    
    TaskQueue<Task>* getOutputQueue() {
        return outputQueue.get();
    }
    
    void setOnTaskCompletedCallback(std::function<void()> callback) {
        onTaskCompleted = callback;
    }

    void addTask(std::unique_ptr<Task> task) {
        inputQueue->push(std::move(task));
    }

    void workerFunction(size_t workerId) {
        while (!shouldStop) {
            std::unique_ptr<Task> task = inputQueue->waitAndPop();

            if (task) {
                std::string workerStr = stageName + "_Worker_" + std::to_string(workerId);

                // Use a more descriptive name that includes the stage for tracing purposes
                std::string traceTaskName = stageName + "_" + task->getName();

                std::cout << "Stage '" << stageName << "' Worker " << workerId
                         << " processing task: " << task->getName()
                         << " [ID: " << task->taskId << "]" << std::endl;

                // Log the start of the task
                traceLogger.logTaskBegin(traceTaskName, stageName, task->taskId, workerStr);

                // Execute the task
                auto startTime = std::chrono::steady_clock::now();
                task->execute();
                auto endTime = std::chrono::steady_clock::now();

                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
                std::cout << "Stage '" << stageName << "' Worker " << workerId
                         << " completed task: " << task->getName()
                         << " in " << duration.count() << "ms" << std::endl;

                // Log the end of the task
                traceLogger.logTaskEnd(traceTaskName, stageName, task->taskId, workerStr);

                // Pass to next stage if there is one and if the task should continue
                if (outputQueue && shouldPassToNextStage(task.get())) {
                    if (!outputQueue->push(std::move(task))) {
                        std::cout << "Warning: Output queue for stage '" << stageName << "' is full!" << std::endl;
                    }
                }

                // Call the completion callback if set
                if (onTaskCompleted) {
                    onTaskCompleted();
                }
            }
        }
    }
    
    // Virtual method to determine if task should pass to next stage
    virtual bool shouldPassToNextStage(Task* task) {
        return true; // By default, pass all tasks
    }

    void stop() {
        shouldStop = true;
        // Notify all waiting threads
        // Note: In a real implementation, you'd need to ensure queues wake up waiting threads
        
        for (auto& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }
    
    ~PipelineStage() {
        stop();
    }
    
    size_t getNumWorkers() const {
        return workers.size();
    }
    
    const std::string& getName() const {
        return stageName;
    }
    
    size_t getInputQueueSize() const {
        return inputQueue->size();
    }
    
    size_t getOutputQueueSize() const {
        return outputQueue ? outputQueue->size() : 0;
    }
};

// Concrete task types for shape processing pipeline
class GenerateShapeTask : public Task {
public:
    std::unique_ptr<Shape> shape;
    ShapeFactory::ShapeType shapeType;
    double param1, param2, posX, posY;
    
    GenerateShapeTask(const std::string& id, ShapeFactory::ShapeType type, 
                      double p1 = 1.0, double p2 = 1.0, double x = 0.0, double y = 0.0)
        : Task(id), shapeType(type), param1(p1), param2(p2), posX(x), posY(y) {}

    void execute() override {
        shape = ShapeFactory::createShape(shapeType, param1, param2, posX, posY, taskId);
        std::cout << "Generated shape: " << shape->getType() 
                 << " [ID: " << taskId << "] with area: " << shape->getArea() << std::endl;
    }
    
    std::string getName() const override {
        return "GenerateShapeTask";
    }
};

class ValidateShapeTask : public Task {
public:
    std::unique_ptr<Shape> shape;
    
    ValidateShapeTask(const std::string& id, std::unique_ptr<Shape> s)
        : Task(id), shape(std::move(s)) {}

    void execute() override {
        if (shape) {
            double area = shape->getArea();
            bool isValid = area > 0.0; // Basic validation
            
            std::cout << "Validated shape: " << shape->getType() 
                     << " [ID: " << taskId << "] - Valid: " << (isValid ? "YES" : "NO") 
                     << " (Area: " << area << ")" << std::endl;
            
            if (!isValid) {
                std::cout << "Shape " << taskId << " failed validation!" << std::endl;
            }
        } else {
            std::cout << "Validation failed: No shape data for task " << taskId << std::endl;
        }
    }
    
    std::string getName() const override {
        return "ValidateShapeTask";
    }
};

class TransformShapeTask : public Task {
public:
    std::unique_ptr<Shape> shape;
    double scaleX, scaleY, rotationAngle;
    
    TransformShapeTask(const std::string& id, std::unique_ptr<Shape> s, 
                       double sx = 1.0, double sy = 1.0, double rot = 0.0)
        : Task(id), shape(std::move(s)), scaleX(sx), scaleY(sy), rotationAngle(rot) {}

    void execute() override {
        if (shape) {
            // In this simplified example, we just resize the shape
            shape->resize(scaleX);
            
            std::cout << "Transformed shape: " << shape->getType() 
                     << " [ID: " << taskId << "] - Scaled by: " << scaleX << std::endl;
        } else {
            std::cout << "Transformation failed: No shape data for task " << taskId << std::endl;
        }
    }
    
    std::string getName() const override {
        return "TransformShapeTask";
    }
};

class RenderShapeTask : public Task {
public:
    std::unique_ptr<Shape> shape;
    
    RenderShapeTask(const std::string& id, std::unique_ptr<Shape> s)
        : Task(id), shape(std::move(s)) {}

    void execute() override {
        if (shape) {
            std::cout << "BEGIN RENDERING - Shape " << shape->getType() 
                     << " [ID: " << taskId << "]" << std::endl;
            shape->draw();
            std::cout << "END RENDERING - Shape " << shape->getType() 
                     << " [ID: " << taskId << "]" << std::endl;
        } else {
            std::cout << "Rendering failed: No shape data for task " << taskId << std::endl;
        }
    }
    
    std::string getName() const override {
        return "RenderShapeTask";
    }
};

// Specialized pipeline stage that filters out invalid shapes
class ValidationStage : public PipelineStage {
public:
    ValidationStage(const std::string& name, size_t numWorkers = 1, size_t queueSize = 50)
        : PipelineStage(name, numWorkers, queueSize) {}
    
    bool shouldPassToNextStage(Task* task) override {
        // For validate tasks, only pass if validation succeeded
        ValidateShapeTask* validateTask = dynamic_cast<ValidateShapeTask*>(task);
        if (validateTask && validateTask->shape) {
            // Simplified: we'll say all shapes pass validation in this example
            // In reality, you might check the validation result
            return validateTask->shape->getArea() > 0;
        }
        return true; // Pass all other tasks
    }
};

// Main pipeline controller
class ShapeProcessingPipeline {
private:
    std::vector<std::unique_ptr<PipelineStage>> stages;
    std::atomic<size_t> completedTasks{0};
    std::mutex statsMutex;
    std::map<std::string, size_t> taskStats;
    
public:
    ShapeProcessingPipeline() {
        setupPipeline();
    }
    
    void setupPipeline() {
        // Stage 1: Generation - Creates shapes
        auto generationStage = std::make_unique<PipelineStage>("Generation", 2 /*workers*/, 30);
        generationStage->setOnTaskCompletedCallback([this]() {
            std::lock_guard<std::mutex> lock(statsMutex);
            completedTasks++;
            taskStats["generated"]++;
        });
        stages.push_back(std::move(generationStage));
        
        // Stage 2: Validation - Validates shapes
        auto validationStage = std::make_unique<ValidationStage>("Validation", 2 /*workers*/, 30);
        validationStage->setOnTaskCompletedCallback([this]() {
            std::lock_guard<std::mutex> lock(statsMutex);
            completedTasks++;
            taskStats["validated"]++;
        });
        stages.push_back(std::move(validationStage));
        
        // Stage 3: Transformation - Transforms shapes
        auto transformStage = std::make_unique<PipelineStage>("Transformation", 2 /*workers*/, 30);
        transformStage->setOnTaskCompletedCallback([this]() {
            std::lock_guard<std::mutex> lock(statsMutex);
            completedTasks++;
            taskStats["transformed"]++;
        });
        stages.push_back(std::move(transformStage));
        
        // Stage 4: Rendering - Renders final shapes
        auto renderStage = std::make_unique<PipelineStage>("Rendering", 2 /*workers*/, 30);
        renderStage->setOnTaskCompletedCallback([this]() {
            std::lock_guard<std::mutex> lock(statsMutex);
            completedTasks++;
            taskStats["rendered"]++;
        });
        stages.push_back(std::move(renderStage));
        
        // Connect stages: each stage's output feeds the next stage's input
        for (size_t i = 0; i < stages.size() - 1; ++i) {
            stages[i]->setOutputQueue(stages[i + 1]->getInputQueue());
        }
    }
    
    void submitShapeGenerationTask(ShapeFactory::ShapeType type, 
                                   double p1 = 1.0, double p2 = 1.0, 
                                   double x = 0.0, double y = 0.0) {
        static std::atomic<int> taskCounter{0};
        int taskId = ++taskCounter;
        std::string id = "SHAPE_TASK_" + std::to_string(taskId);
        
        auto task = std::make_unique<GenerateShapeTask>(id, type, p1, p2, x, y);
        stages[0]->addTask(std::move(task));
    }
    
    size_t getTotalCompletedTasks() const {
        return completedTasks.load();
    }
    
    std::map<std::string, size_t> getTaskStatistics() const {
        std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(statsMutex));
        return taskStats;
    }
    
    void printStatus() {
        std::cout << "\n--- Pipeline Status ---" << std::endl;
        for (const auto& stage : stages) {
            std::cout << "Stage '" << stage->getName() << "': Input Queue Size: "
                     << stage->getInputQueueSize()
                     << ", Workers: " << stage->getNumWorkers() << std::endl;
        }
        std::cout << "Total Completed Tasks: " << getTotalCompletedTasks() << std::endl;
        auto stats = getTaskStatistics();
        for (const auto& stat : stats) {
            std::cout << "  " << stat.first << ": " << stat.second << std::endl;
        }
        std::cout << "--- End Status ---\n" << std::endl;
    }
    
    void waitForCompletion() {
        // Keep checking until all queues are empty and no tasks are in progress
        while (true) {
            bool allEmpty = true;
            for (const auto& stage : stages) {
                if (stage->getInputQueueSize() > 0) {
                    allEmpty = false;
                    break;
                }
            }
            
            // If all queues are empty, break after a short delay
            if (allEmpty) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                break;
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
    
    ~ShapeProcessingPipeline() {
        for (auto& stage : stages) {
            stage->stop();
        }
    }
};

int main() {
    std::cout << "Pipeline + Task System with Shapes Example\n" << std::endl;

    // Create the pipeline
    ShapeProcessingPipeline pipeline;

    // Submit multiple shape generation tasks to start the pipeline
    std::cout << "Submitting tasks to pipeline...\n" << std::endl;

    // Generate various shapes with different properties
    for (int i = 0; i < 8; ++i) {
        ShapeFactory::ShapeType type = static_cast<ShapeFactory::ShapeType>(i % 3);
        double p1 = 1.0 + (i * 0.5);
        double p2 = 1.0 + (i * 0.3);
        double x = i * 10.0;
        double y = i * 5.0;

        pipeline.submitShapeGenerationTask(type, p1, p2, x, y);

        // Small delay between submissions to simulate realistic usage
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    std::cout << "\nAll tasks submitted. Pipeline is processing...\n" << std::endl;

    // Print periodic status updates
    for (int i = 0; i < 10; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        pipeline.printStatus();
    }

    // Wait for all tasks to complete
    std::cout << "Waiting for pipeline to finish processing..." << std::endl;
    pipeline.waitForCompletion();

    std::cout << "\nFinal Status:" << std::endl;
    pipeline.printStatus();

    std::cout << "\nPipeline + Task System demonstration completed!" << std::endl;

    // Explicitly finalize the trace logger to ensure proper file format
    traceLogger.finalize();

    return 0;
}