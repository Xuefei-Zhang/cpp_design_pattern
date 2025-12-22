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
#include <functional>
#include <atomic>
#include <map>
#include <set>
#include <algorithm>
#include <sstream>
#include <regex>

// Shape hierarchy (same as before)
class IDrawable {
public:
    virtual ~IDrawable() = default;
    virtual void draw() const = 0;
    virtual void resize(double factor) = 0;
    virtual double getArea() const = 0;
};

class Shape : public IDrawable {
protected:
    double x = 0.0, y = 0.0;
    double scale = 1.0;
    std::string id;

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

// Shape factory
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

// Task execution state
enum class TaskState {
    PENDING,
    READY,
    RUNNING,
    COMPLETED,
    FAILED
};

// Forward declarations
class TaskExecutor;
class TaskScheduler;

// Task base class
class Task {
public:
    std::string taskId;
    std::string taskName;
    std::string taskType;
    std::chrono::steady_clock::time_point creationTime;
    TaskState state{TaskState::PENDING};
    std::vector<std::string> dependencies;
    std::vector<std::string> dependents;
    std::function<void()> executionCallback;
    
    Task(const std::string& id, const std::string& name, const std::string& type) 
        : taskId(id), taskName(name), taskType(type), creationTime(std::chrono::steady_clock::now()) {}
    
    virtual ~Task() = default;
    virtual void execute() = 0;
    
    void addDependency(const std::string& depId) {
        dependencies.push_back(depId);
    }
    
    void addDependent(const std::string& depId) {
        dependents.push_back(depId);
    }
    
    bool isReady() const {
        return state == TaskState::PENDING && dependencies.empty();
    }
    
    void markAsReady() {
        if (state == TaskState::PENDING && dependencies.empty()) {
            state = TaskState::READY;
        }
    }
    
    void removeDependency(const std::string& depId) {
        dependencies.erase(
            std::remove(dependencies.begin(), dependencies.end(), depId),
            dependencies.end()
        );
    }
    
    void updateState(TaskState newState) {
        state = newState;
    }
    
    TaskState getState() const {
        return state;
    }
};

// Trace Logger for Chrome Tracing format
class TraceLogger {
private:
    std::ofstream traceFile;
    std::chrono::steady_clock::time_point startTime;
    mutable std::mutex logMutex;

public:
    TraceLogger(const std::string& filename = "config_pipeline_trace.json") 
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
        logEvent(taskName, "execution", "B", "ConfigPipeline", workerId, ts, args);
    }
    
    void logTaskEnd(const std::string& taskName, const std::string& stageName, 
                    const std::string& taskId, const std::string& workerId) {
        auto now = std::chrono::steady_clock::now();
        uint64_t ts = std::chrono::duration_cast<std::chrono::microseconds>(
            now - startTime).count();
        std::map<std::string, std::string> args = {{"stage", stageName}, {"taskId", taskId}};
        logEvent(taskName, "execution", "E", "ConfigPipeline", workerId, ts, args);
    }
    
    void finalize() {
        if (traceFile.is_open()) {
            traceFile.seekp(-1, std::ios_base::end); // Remove the last comma
            if (traceFile.tellp() > 0) {
                traceFile << "\n],\"displayTimeUnit\":\"ms\"}" << std::endl;
            } else {
                traceFile << "],\"displayTimeUnit\":\"ms\"}" << std::endl;
            }
            traceFile.close();
        }
    }
    
    ~TraceLogger() {
        finalize();
    }
};

static TraceLogger configTraceLogger;

// Concrete task types based on configuration
class GenerateShapeTask : public Task {
public:
    std::unique_ptr<Shape> shape;
    ShapeFactory::ShapeType shapeType;
    double param1, param2, posX, posY;

    GenerateShapeTask(const std::string& id, const std::string& name, 
                      ShapeFactory::ShapeType type, double p1, double p2, double x, double y)
        : Task(id, name, "GenerateShape"), shapeType(type), param1(p1), param2(p2), posX(x), posY(y) {}

    void execute() override {
        shape = ShapeFactory::createShape(shapeType, param1, param2, posX, posY, taskId);
        std::cout << "Generated shape: " << shape->getType() 
                 << " [ID: " << taskId << "] with area: " << shape->getArea() << std::endl;
    }
};

class ValidateShapeTask : public Task {
public:
    std::unique_ptr<Shape> shape;

    ValidateShapeTask(const std::string& id, const std::string& name, std::unique_ptr<Shape> s)
        : Task(id, name, "ValidateShape"), shape(std::move(s)) {}

    void execute() override {
        if (shape) {
            double area = shape->getArea();
            bool isValid = area > 0.0;
            std::cout << "Validated shape: " << shape->getType() 
                     << " [ID: " << taskId << "] - Valid: " << (isValid ? "YES" : "NO") 
                     << " (Area: " << area << ")" << std::endl;
        } else {
            std::cout << "Validation failed: No shape data for task " << taskId << std::endl;
        }
    }
};

class TransformShapeTask : public Task {
public:
    std::unique_ptr<Shape> shape;
    double scaleX, scaleY;

    TransformShapeTask(const std::string& id, const std::string& name, 
                       std::unique_ptr<Shape> s, double sx, double sy)
        : Task(id, name, "TransformShape"), shape(std::move(s)), scaleX(sx), scaleY(sy) {}

    void execute() override {
        if (shape) {
            shape->resize(scaleX);
            std::cout << "Transformed shape: " << shape->getType() 
                     << " [ID: " << taskId << "] - Scaled by: " << scaleX << std::endl;
        } else {
            std::cout << "Transformation failed: No shape data for task " << taskId << std::endl;
        }
    }
};

class RenderShapeTask : public Task {
public:
    std::unique_ptr<Shape> shape;

    RenderShapeTask(const std::string& id, const std::string& name, std::unique_ptr<Shape> s)
        : Task(id, name, "RenderShape"), shape(std::move(s)) {}

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
};

// Simple JSON-like parser for configuration (we'll implement a basic version)
class ConfigParser {
public:
    struct StageConfig {
        std::string name;
        size_t workers;
        std::vector<std::map<std::string, std::string>> tasks;
    };
    
    struct PipelineConfig {
        std::string name;
        std::vector<StageConfig> stages;
        std::vector<std::pair<std::string, std::string>> dependencies; // task1 -> task2
    };
    
    static PipelineConfig parseJsonConfig(const std::string& jsonContent) {
        PipelineConfig config;
        // Simple parsing - in a real implementation you'd use a proper JSON library
        // For now, we'll implement a basic parser that looks for specific patterns
        std::string content = jsonContent;
        
        // Find pipeline name
        size_t namePos = content.find("\"name\"");
        if (namePos != std::string::npos) {
            size_t start = content.find("\"", namePos + 6);
            size_t end = content.find("\"", start + 1);
            if (start != std::string::npos && end != std::string::npos) {
                config.name = content.substr(start + 1, end - start - 1);
            }
        }
        
        return config;
    }
};

// Task Executor class
class TaskExecutor {
private:
    std::string executorId;
    std::atomic<bool> shouldStop{false};
    
public:
    TaskExecutor(const std::string& id) : executorId(id) {}
    
    bool executeTask(std::shared_ptr<Task> task) {
        if (!task || task->getState() != TaskState::READY) {
            return false;
        }
        
        std::string stageName = executorId; // Simplified: in real impl, this would come from context
        std::cout << "Executor " << executorId << " starting task: " 
                 << task->taskName << " [ID: " << task->taskId << "]" << std::endl;
        
        // Log task start
        configTraceLogger.logTaskBegin(task->taskName, stageName, task->taskId, executorId);
        
        // Mark as running
        task->updateState(TaskState::RUNNING);
        
        // Execute the task
        auto startTime = std::chrono::steady_clock::now();
        try {
            task->execute();
            auto endTime = std::chrono::steady_clock::now();
            
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
            std::cout << "Executor " << executorId << " completed task: " 
                     << task->taskName << " [ID: " << task->taskId << "] in " 
                     << duration.count() << "ms" << std::endl;
            
            task->updateState(TaskState::COMPLETED);
            return true;
        } catch (const std::exception& e) {
            std::cout << "Executor " << executorId << " failed task: " 
                     << task->taskName << " [ID: " << task->taskId 
                     << "] Error: " << e.what() << std::endl;
            
            task->updateState(TaskState::FAILED);
            return false;
        }

        // Log task end
        configTraceLogger.logTaskEnd(task->taskName, stageName, task->taskId, executorId);
        return true;
    }

    const std::string& getId() const {
        return executorId;
    }
};

// Task Scheduler class
class TaskScheduler {
private:
    std::vector<std::unique_ptr<TaskExecutor>> executors;
    std::vector<std::shared_ptr<Task>> taskQueue;
    std::vector<std::shared_ptr<Task>> readyQueue;
    std::vector<std::shared_ptr<Task>> runningTasks;
    std::map<std::string, std::shared_ptr<Task>> taskMap;
    
    mutable std::mutex schedulerMutex;
    std::condition_variable taskAvailable;
    std::atomic<bool> shouldStop{false};
    
public:
    TaskScheduler(size_t numExecutors = 2) {
        for (size_t i = 0; i < numExecutors; ++i) {
            executors.push_back(std::make_unique<TaskExecutor>("Worker_" + std::to_string(i)));
        }
    }
    
    void addTask(std::shared_ptr<Task> task) {
        std::lock_guard<std::mutex> lock(schedulerMutex);
        taskMap[task->taskId] = task;
        taskQueue.push_back(task);
        updateReadyTasks();
        taskAvailable.notify_all();
    }
    
    void addDependency(const std::string& taskId, const std::string& dependencyId) {
        std::lock_guard<std::mutex> lock(schedulerMutex);
        
        auto it = taskMap.find(taskId);
        if (it != taskMap.end()) {
            it->second->addDependency(dependencyId);
            
            // Also add the reverse dependency (dependent)
            auto depIt = taskMap.find(dependencyId);
            if (depIt != taskMap.end()) {
                depIt->second->addDependent(taskId);
            }
        }
    }
    
    void start() {
        // Start executor threads
        for (size_t i = 0; i < executors.size(); ++i) {
            std::thread executorThread([this, i]() {
                executorLoop(i);
            });
            executorThread.detach();
        }
    }
    
    void executorLoop(size_t executorIndex) {
        while (!shouldStop) {
            std::shared_ptr<Task> task = getNextReadyTask();
            
            if (task) {
                executors[executorIndex]->executeTask(task);
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }
    }
    
    std::shared_ptr<Task> getNextReadyTask() {
        std::lock_guard<std::mutex> lock(schedulerMutex);
        
        if (readyQueue.empty()) {
            return nullptr;
        }
        
        auto task = readyQueue.back();
        readyQueue.pop_back();
        runningTasks.push_back(task);
        
        return task;
    }
    
    void updateReadyTasks() {
        std::vector<std::shared_ptr<Task>> newReadyTasks;
        
        for (auto& task : taskQueue) {
            if (task->isReady()) {
                task->updateState(TaskState::READY);
                newReadyTasks.push_back(task);
            }
        }
        
        for (auto& readyTask : newReadyTasks) {
            taskQueue.erase(
                std::remove(taskQueue.begin(), taskQueue.end(), readyTask),
                taskQueue.end()
            );
            readyQueue.push_back(readyTask);
        }
        
        taskAvailable.notify_all();
    }
    
    void updateDependencies(const std::string& completedTaskId) {
        for (auto& pair : taskMap) {
            if (pair.second->getState() == TaskState::PENDING) {
                pair.second->removeDependency(completedTaskId);
                
                if (pair.second->isReady()) {
                    pair.second->updateState(TaskState::READY);
                    
                    auto it = std::find(taskQueue.begin(), taskQueue.end(), pair.second);
                    if (it != taskQueue.end()) {
                        taskQueue.erase(it);
                        readyQueue.push_back(pair.second);
                    }
                }
            }
        }
        
        taskAvailable.notify_all();
    }
    
    size_t getCompletedTaskCount() const {
        std::lock_guard<std::mutex> lock(schedulerMutex);
        size_t count = 0;
        for (const auto& pair : taskMap) {
            if (pair.second->getState() == TaskState::COMPLETED) {
                ++count;
            }
        }
        return count;
    }
    
    void waitForCompletion() {
        while (getCompletedTaskCount() < taskMap.size()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    
    void stop() {
        shouldStop = true;
    }
};

// Configurable Pipeline class
class ConfigurablePipeline {
private:
    std::string pipelineName;
    std::shared_ptr<TaskScheduler> scheduler;
    std::atomic<int> taskCounter{0};
    
public:
    ConfigurablePipeline(const std::string& name, size_t numExecutors = 2) 
        : pipelineName(name), scheduler(std::make_shared<TaskScheduler>(numExecutors)) {
        scheduler->start();
    }
    
    // Create pipeline from configuration
    static std::shared_ptr<ConfigurablePipeline> createFromConfig(const std::string& configFile) {
        std::ifstream file(configFile);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open config file: " + configFile);
        }
        
        std::string content((std::istreambufers_iterator<char>(file)),
                           std::istreambufers_iterator<char>());
        file.close();
        
        auto config = ConfigParser::parseJsonConfig(content);
        
        auto pipeline = std::make_shared<ConfigurablePipeline>(config.name, 4); // Default 4 executors
        
        // Create tasks based on configuration
        for (const auto& stage : config.stages) {
            for (const auto& taskInfo : stage.tasks) {
                std::string taskId = taskInfo.at("id");
                std::string taskType = taskInfo.at("type");
                
                // Create appropriate task based on type
                std::shared_ptr<Task> task = createTaskFromConfig(taskId, taskType, taskInfo);
                if (task) {
                    pipeline->scheduler->addTask(task);
                }
            }
        }
        
        // Add dependencies based on configuration
        for (const auto& dep : config.dependencies) {
            pipeline->scheduler->addDependency(dep.first, dep.second);
        }
        
        return pipeline;
    }
    
    static std::shared_ptr<Task> createTaskFromConfig(const std::string& taskId, 
                                                     const std::string& taskType,
                                                     const std::map<std::string, std::string>& config) {
        // This is a simplified example - in a real implementation, you'd have more complex parameter parsing
        if (taskType == "GenerateShape") {
            ShapeFactory::ShapeType shapeType = ShapeFactory::ShapeType::CIRCLE;
            double param1 = 1.0, param2 = 1.0, posX = 0.0, posY = 0.0;
            
            // Parse parameters from config if available
            auto it = config.find("shapeType");
            if (it != config.end()) {
                if (it->second == "rectangle") shapeType = ShapeFactory::ShapeType::RECTANGLE;
                else if (it->second == "triangle") shapeType = ShapeFactory::ShapeType::TRIANGLE;
            }
            
            return std::make_shared<GenerateShapeTask>(taskId, taskId, shapeType, param1, param2, posX, posY);
        }
        else if (taskType == "ValidateShape") {
            return std::make_shared<ValidateShapeTask>(taskId, taskId, nullptr);
        }
        else if (taskType == "TransformShape") {
            double scaleX = 1.0, scaleY = 1.0;
            return std::make_shared<TransformShapeTask>(taskId, taskId, nullptr, scaleX, scaleY);
        }
        else if (taskType == "RenderShape") {
            return std::make_shared<RenderShapeTask>(taskId, taskId, nullptr);
        }
        
        return nullptr;
    }
    
    void submitTask(std::shared_ptr<Task> task) {
        scheduler->addTask(task);
    }
    
    void waitForCompletion() {
        scheduler->waitForCompletion();
    }
    
    size_t getCompletedTaskCount() const {
        return scheduler->getCompletedTaskCount();
    }
};

// Example usage
int main() {
    std::cout << "Configurable Pipeline System with Shapes Example\n" << std::endl;
    
    // Create a sample configuration file content
    std::string configContent = R"({
        "name": "Shape Processing Pipeline",
        "stages": [
            {
                "name": "Generation",
                "workers": 2,
                "tasks": [
                    {
                        "id": "gen_circle_1",
                        "type": "GenerateShape",
                        "shapeType": "circle",
                        "param1": 2.0,
                        "posX": 0.0,
                        "posY": 0.0
                    },
                    {
                        "id": "gen_rect_1", 
                        "type": "GenerateShape",
                        "shapeType": "rectangle",
                        "param1": 3.0,
                        "param2": 4.0,
                        "posX": 5.0,
                        "posY": 5.0
                    }
                ]
            },
            {
                "name": "Validation", 
                "workers": 2,
                "tasks": [
                    {
                        "id": "validate_circle_1",
                        "type": "ValidateShape"
                    },
                    {
                        "id": "validate_rect_1",
                        "type": "ValidateShape" 
                    }
                ]
            }
        ],
        "dependencies": [
            ["validate_circle_1", "gen_circle_1"],
            ["validate_rect_1", "gen_rect_1"]
        ]
    })";
    
    // Write config to file
    std::ofstream configFile("shape_pipeline_config.json");
    configFile << configContent;
    configFile.close();
    
    try {
        // Create pipeline from configuration file
        auto pipeline = ConfigurablePipeline::createFromConfig("shape_pipeline_config.json");
        
        std::cout << "Pipeline created from configuration. Starting execution...\n" << std::endl;
        
        // Wait for tasks to complete
        pipeline->waitForCompletion();
        
        std::cout << "\nFinal Status:" << std::endl;
        std::cout << "Completed Tasks: " << pipeline->getCompletedTaskCount() << std::endl;
        std::cout << "\nConfigurable Pipeline System demonstration completed!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}