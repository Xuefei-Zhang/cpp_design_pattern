#include <iostream>
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

// Reuse the Shape hierarchy from previous examples
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

// Task execution states
enum class TaskState {
    PENDING,
    READY,
    RUNNING,
    COMPLETED,
    FAILED
};

// Forward declarations for circular dependencies
class TaskExecutor;
class TaskScheduler;

// Task class with dependencies and execution state
class Task {
public:
    std::string taskId;
    std::string taskName;
    std::chrono::steady_clock::time_point creationTime;
    TaskState state{TaskState::PENDING};
    std::vector<std::string> dependencies;  // IDs of tasks this task depends on
    std::vector<std::string> dependents;    // IDs of tasks that depend on this one
    std::function<void()> executionCallback;  // Callback for when task completes
    
    Task(const std::string& id, const std::string& name) 
        : taskId(id), taskName(name), creationTime(std::chrono::steady_clock::now()) {}
    
    virtual ~Task() = default;
    virtual void execute() = 0;
    virtual std::string getTaskType() const = 0;
    
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

// Concrete shape processing tasks - define these first
class GenerateShapeTask : public Task {
public:
    std::unique_ptr<Shape> shape;
    ShapeFactory::ShapeType shapeType;
    double param1, param2, posX, posY;

    GenerateShapeTask(const std::string& id, ShapeFactory::ShapeType type,
                      double p1 = 1.0, double p2 = 1.0, double x = 0.0, double y = 0.0)
        : Task(id, "Generate Shape"), shapeType(type), param1(p1), param2(p2), posX(x), posY(y) {}

    void execute() override {
        shape = ShapeFactory::createShape(shapeType, param1, param2, posX, posY, taskId);
        std::cout << "Generated shape: " << shape->getType()
                 << " [ID: " << taskId << "] with area: " << shape->getArea() << std::endl;

        // Simulate some processing time
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    std::string getTaskType() const override {
        return "GenerateShape";
    }
};

class ValidateShapeTask : public Task {
public:
    std::unique_ptr<Shape> shape;

    ValidateShapeTask(const std::string& id, std::unique_ptr<Shape> s)
        : Task(id, "Validate Shape"), shape(std::move(s)) {}

    void execute() override {
        if (shape) {
            double area = shape->getArea();
            bool isValid = area > 0.0; // Basic validation

            std::cout << "Validated shape: " << shape->getType()
                     << " [ID: " << taskId << "] - Valid: " << (isValid ? "YES" : "NO")
                     << " (Area: " << area << ")" << std::endl;

            if (!isValid) {
                std::cout << "Shape " << taskId << " failed validation!" << std::endl;
                throw std::runtime_error("Shape validation failed");
            }
        } else {
            std::cout << "Validation failed: No shape data for task " << taskId << std::endl;
            throw std::runtime_error("No shape data for validation");
        }

        // Simulate some validation time
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }

    std::string getTaskType() const override {
        return "ValidateShape";
    }
};

class TransformShapeTask : public Task {
public:
    std::unique_ptr<Shape> shape;
    double scaleX, scaleY;

    TransformShapeTask(const std::string& id, std::unique_ptr<Shape> s,
                       double sx = 1.0, double sy = 1.0)
        : Task(id, "Transform Shape"), shape(std::move(s)), scaleX(sx), scaleY(sy) {}

    void execute() override {
        if (shape) {
            // Apply transformation
            shape->resize(scaleX);

            std::cout << "Transformed shape: " << shape->getType()
                     << " [ID: " << taskId << "] - Scaled by: " << scaleX << std::endl;
        } else {
            std::cout << "Transformation failed: No shape data for task " << taskId << std::endl;
            throw std::runtime_error("No shape data for transformation");
        }

        // Simulate some transformation time
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }

    std::string getTaskType() const override {
        return "TransformShape";
    }
};

class RenderShapeTask : public Task {
public:
    std::unique_ptr<Shape> shape;

    RenderShapeTask(const std::string& id, std::unique_ptr<Shape> s)
        : Task(id, "Render Shape"), shape(std::move(s)) {}

    void execute() override {
        if (shape) {
            std::cout << "BEGIN RENDERING - Shape " << shape->getType()
                     << " [ID: " << taskId << "]" << std::endl;
            shape->draw();
            std::cout << "END RENDERING - Shape " << shape->getType()
                     << " [ID: " << taskId << "]" << std::endl;
        } else {
            std::cout << "Rendering failed: No shape data for task " << taskId << std::endl;
            throw std::runtime_error("No shape data for rendering");
        }

        // Simulate some rendering time
        std::this_thread::sleep_for(std::chrono::milliseconds(60));
    }

    std::string getTaskType() const override {
        return "RenderShape";
    }
};

// Task Executor class
class TaskExecutor {
private:
    std::string executorId;
    std::atomic<bool> shouldStop{false};
    std::function<void(const std::string&, TaskState)> onTaskStateChanged;

public:
    TaskExecutor(const std::string& id) : executorId(id) {}

    void setOnTaskStateChangedCallback(std::function<void(const std::string&, TaskState)> callback) {
        onTaskStateChanged = callback;
    }

    bool executeTask(std::shared_ptr<Task> task) {
        if (!task || task->getState() != TaskState::READY) {
            return false;
        }

        std::cout << "Executor " << executorId << " starting task: "
                 << task->taskName << " [ID: " << task->taskId << "]" << std::endl;

        // Mark as running
        task->updateState(TaskState::RUNNING);
        if (onTaskStateChanged) {
            onTaskStateChanged(task->taskId, TaskState::RUNNING);
        }

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
            if (onTaskStateChanged) {
                onTaskStateChanged(task->taskId, TaskState::COMPLETED);
            }

            // Call execution callback if set
            if (task->executionCallback) {
                task->executionCallback();
            }

            return true;
        } catch (const std::exception& e) {
            std::cout << "Executor " << executorId << " failed task: "
                     << task->taskName << " [ID: " << task->taskId
                     << "] Error: " << e.what() << std::endl;

            task->updateState(TaskState::FAILED);
            if (onTaskStateChanged) {
                onTaskStateChanged(task->taskId, TaskState::FAILED);
            }
            return false;
        }
    }

    const std::string& getId() const {
        return executorId;
    }
};

// Task Scheduler class
class TaskScheduler {
private:
    std::vector<std::unique_ptr<TaskExecutor>> executors;
    std::vector<std::shared_ptr<Task>> taskQueue;  // Tasks waiting to be scheduled
    std::vector<std::shared_ptr<Task>> readyQueue; // Tasks ready to execute
    std::vector<std::shared_ptr<Task>> runningTasks; // Currently executing tasks
    std::map<std::string, std::shared_ptr<Task>> taskMap;  // All tasks by ID
    
    mutable std::mutex schedulerMutex;
    std::condition_variable taskAvailable;
    std::atomic<bool> shouldStop{false};
    
    std::function<void()> onTaskCompletion;
    
public:
    TaskScheduler(size_t numExecutors = 2) {
        for (size_t i = 0; i < numExecutors; ++i) {
            auto executor = std::make_unique<TaskExecutor>("Executor_" + std::to_string(i));
            executor->setOnTaskStateChangedCallback([this](const std::string& taskId, TaskState newState) {
                onTaskStateChanged(taskId, newState);
            });
            executors.push_back(std::move(executor));
        }
    }
    
    void setOnTaskCompletionCallback(std::function<void()> callback) {
        onTaskCompletion = callback;
    }
    
    void addTask(std::shared_ptr<Task> task) {
        std::lock_guard<std::mutex> lock(schedulerMutex);
        taskMap[task->taskId] = task;
        taskQueue.push_back(task);
        
        // Check if any pending tasks become ready due to this new task
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
        // Start the scheduler loop in separate thread
        std::thread schedulerThread([this]() {
            schedulerLoop();
        });
        schedulerThread.detach();
        
        // Start executor threads
        for (size_t i = 0; i < executors.size(); ++i) {
            std::thread executorThread([this, i]() {
                executorLoop(i);
            });
            executorThread.detach();
        }
    }
    
    void schedulerLoop() {
        while (!shouldStop) {
            {
                std::unique_lock<std::mutex> lock(schedulerMutex);
                
                // Update ready tasks - check if any pending tasks are now ready
                updateReadyTasks();
                
                // Wait for new tasks or a reasonable time interval
                taskAvailable.wait_for(lock, std::chrono::milliseconds(100));
            }
        }
    }
    
    void executorLoop(size_t executorIndex) {
        while (!shouldStop) {
            std::shared_ptr<Task> task = getNextReadyTask();
            
            if (task) {
                executors[executorIndex]->executeTask(task);
            } else {
                // No ready tasks, wait a bit before checking again
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
        
        // Find tasks that are pending and have no dependencies
        for (auto& task : taskQueue) {
            if (task->isReady()) {
                task->updateState(TaskState::READY);
                newReadyTasks.push_back(task);
            }
        }
        
        // Remove ready tasks from the pending queue
        for (auto& readyTask : newReadyTasks) {
            taskQueue.erase(
                std::remove(taskQueue.begin(), taskQueue.end(), readyTask),
                taskQueue.end()
            );
            readyQueue.push_back(readyTask);
        }
        
        taskAvailable.notify_all();
    }
    
    void onTaskStateChanged(const std::string& taskId, TaskState newState) {
        std::lock_guard<std::mutex> lock(schedulerMutex);
        
        auto it = taskMap.find(taskId);
        if (it != taskMap.end()) {
            it->second->updateState(newState);
            
            if (newState == TaskState::COMPLETED || newState == TaskState::FAILED) {
                // Remove from running tasks
                runningTasks.erase(
                    std::remove(runningTasks.begin(), runningTasks.end(), it->second),
                    runningTasks.end()
                );
                
                // Update dependencies for all tasks
                updateDependencies(taskId);
                
                if (onTaskCompletion) {
                    onTaskCompletion();
                }
            }
        }
    }
    
    void updateDependencies(const std::string& completedTaskId) {
        // For each task that depends on the completed task, remove the dependency
        for (auto& pair : taskMap) {
            if (pair.second->getState() == TaskState::PENDING) {
                pair.second->removeDependency(completedTaskId);
                
                // Check if the task is now ready (no more dependencies)
                if (pair.second->isReady()) {
                    pair.second->updateState(TaskState::READY);
                    
                    // Move from taskQueue to readyQueue if needed
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
    
    size_t getPendingTaskCount() const {
        std::lock_guard<std::mutex> lock(schedulerMutex);
        return taskQueue.size();
    }
    
    size_t getReadyTaskCount() const {
        std::lock_guard<std::mutex> lock(schedulerMutex);
        return readyQueue.size();
    }
    
    size_t getRunningTaskCount() const {
        std::lock_guard<std::mutex> lock(schedulerMutex);
        return runningTasks.size();
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
    
    std::vector<std::shared_ptr<Task>> getAllTasks() const {
        std::lock_guard<std::mutex> lock(schedulerMutex);
        std::vector<std::shared_ptr<Task>> result;
        for (const auto& pair : taskMap) {
            result.push_back(pair.second);
        }
        return result;
    }
    
    void stop() {
        shouldStop = true;
    }
};

// Pipeline class that manages stages and tasks
class Pipeline {
private:
    std::string pipelineName;
    std::shared_ptr<TaskScheduler> scheduler;
    std::vector<std::string> stageNames;
    std::map<std::string, std::vector<std::string>> stageTasks;  // Stage name -> Task IDs
    std::atomic<int> taskCounter{0};
    std::function<void()> onCompleteCallback;

public:
    Pipeline(const std::string& name, size_t numExecutors = 2)
        : pipelineName(name), scheduler(std::make_shared<TaskScheduler>(numExecutors)) {

        scheduler->setOnTaskCompletionCallback([this]() {
            checkPipelineCompletion();
        });

        scheduler->start();
    }

    void setOnCompleteCallback(std::function<void()> callback) {
        onCompleteCallback = callback;
    }

    // Add a stage to the pipeline
    void addStage(const std::string& stageName) {
        stageNames.push_back(stageName);
        stageTasks[stageName] = std::vector<std::string>();
    }

    // Add a task to a specific stage
    void addTaskToStage(const std::string& stageName, std::shared_ptr<Task> task) {
        auto stageIt = stageTasks.find(stageName);
        if (stageIt != stageTasks.end()) {
            stageIt->second.push_back(task->taskId);
            scheduler->addTask(task);
        }
    }

    // Add a task dependency: taskA must complete before taskB can start
    void addTaskDependency(const std::string& taskId, const std::string& dependencyId) {
        scheduler->addDependency(taskId, dependencyId);
    }

    // Submit a shape processing task that goes through all stages
    std::string submitShapeProcessingTask(ShapeFactory::ShapeType shapeType,
                                          double param1, double param2,
                                          double x, double y) {
        // Create unique ID for this processing task
        int taskId = ++taskCounter;
        std::string baseId = "SHAPE_PROC_" + std::to_string(taskId);

        // Create tasks for each stage of processing
        auto generationTask = std::make_shared<GenerateShapeTask>(
            baseId + "_GENERATE", shapeType, param1, param2, x, y);

        auto validationTask = std::make_shared<ValidateShapeTask>(
            baseId + "_VALIDATE", nullptr);

        auto transformTask = std::make_shared<TransformShapeTask>(
            baseId + "_TRANSFORM", nullptr);

        auto renderTask = std::make_shared<RenderShapeTask>(
            baseId + "_RENDER", nullptr);

        // Add tasks to scheduler
        scheduler->addTask(generationTask);
        scheduler->addTask(validationTask);
        scheduler->addTask(transformTask);
        scheduler->addTask(renderTask);

        // Establish dependencies: generation -> validation -> transformation -> rendering
        scheduler->addDependency(validationTask->taskId, generationTask->taskId);
        scheduler->addDependency(transformTask->taskId, validationTask->taskId);
        scheduler->addDependency(renderTask->taskId, transformTask->taskId);

        // Set up callbacks to pass shape data between tasks
        generationTask->executionCallback = [generationTask, validationTask]() {
            // Pass the generated shape to the validation task
            std::shared_ptr<ValidateShapeTask> valTask =
                std::dynamic_pointer_cast<ValidateShapeTask>(validationTask);
            if (valTask) {
                valTask->shape = std::move(std::dynamic_pointer_cast<GenerateShapeTask>(generationTask)->shape);
            }
        };

        validationTask->executionCallback = [validationTask, transformTask]() {
            // Pass the validated shape to the transform task
            std::shared_ptr<TransformShapeTask> transTask =
                std::dynamic_pointer_cast<TransformShapeTask>(transformTask);
            if (transTask) {
                auto valTask = std::dynamic_pointer_cast<ValidateShapeTask>(validationTask);
                transTask->shape = std::move(valTask->shape);
            }
        };

        transformTask->executionCallback = [transformTask, renderTask]() {
            // Pass the transformed shape to the render task
            std::shared_ptr<RenderShapeTask> rendTask =
                std::dynamic_pointer_cast<RenderShapeTask>(renderTask);
            if (rendTask) {
                auto transTask = std::dynamic_pointer_cast<TransformShapeTask>(transformTask);
                rendTask->shape = std::move(transTask->shape);
            }
        };

        return baseId;
    }

    void checkPipelineCompletion() {
        auto tasks = scheduler->getAllTasks();
        bool allCompleted = true;

        for (const auto& task : tasks) {
            if (task->getState() != TaskState::COMPLETED && task->getState() != TaskState::FAILED) {
                allCompleted = false;
                break;
            }
        }

        if (allCompleted && onCompleteCallback) {
            onCompleteCallback();
        }
    }

    size_t getPendingTaskCount() const {
        return scheduler->getPendingTaskCount();
    }

    size_t getReadyTaskCount() const {
        return scheduler->getReadyTaskCount();
    }

    size_t getRunningTaskCount() const {
        return scheduler->getRunningTaskCount();
    }

    size_t getCompletedTaskCount() const {
        return scheduler->getCompletedTaskCount();
    }

    void printStatus() const {
        std::cout << "\n--- Pipeline '" << pipelineName << "' Status ---" << std::endl;
        std::cout << "Pending Tasks: " << getPendingTaskCount() << std::endl;
        std::cout << "Ready Tasks: " << getReadyTaskCount() << std::endl;
        std::cout << "Running Tasks: " << getRunningTaskCount() << std::endl;
        std::cout << "Completed Tasks: " << getCompletedTaskCount() << std::endl;
        std::cout << "------------------------\n" << std::endl;
    }

    std::shared_ptr<TaskScheduler> getScheduler() const {
        return scheduler;
    }
};

// Example usage
int main() {
    std::cout << "Pipeline Scheduler System with Shapes Example\n" << std::endl;

    // Create a pipeline for shape processing
    auto pipeline = std::make_shared<Pipeline>("Shape Processing Pipeline", 3); // 3 executors

    // Set up completion callback
    pipeline->getScheduler()->setOnTaskCompletionCallback([pipeline]() {
        static int completedTasks = 0;
        completedTasks++;
        std::cout << "Task completed. Total completed: " << completedTasks << std::endl;
    });

    std::cout << "Submitting shape processing tasks to pipeline...\n" << std::endl;

    // Submit multiple shape processing tasks with dependencies
    for (int i = 0; i < 6; ++i) {
        ShapeFactory::ShapeType type = static_cast<ShapeFactory::ShapeType>(i % 3);
        double p1 = 1.0 + (i * 0.5);
        double p2 = 1.0 + (i * 0.3);
        double x = i * 10.0;
        double y = i * 5.0;

        std::string taskId = pipeline->submitShapeProcessingTask(type, p1, p2, x, y);
        std::cout << "Submitted shape processing task: " << taskId << std::endl;
    }

    std::cout << "\nAll tasks submitted. Pipeline is processing...\n" << std::endl;

    // Monitor the pipeline status
    for (int i = 0; i < 15; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        pipeline->printStatus();
    }

    // Wait a bit more for tasks to complete
    std::cout << "Waiting for all tasks to complete..." << std::endl;
    for (int i = 0; i < 20; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        if (pipeline->getCompletedTaskCount() == 24) { // 6 tasks * 4 stages each
            std::cout << "All tasks completed!" << std::endl;
            break;
        }
    }

    std::cout << "\nFinal Pipeline Status:" << std::endl;
    pipeline->printStatus();

    std::cout << "\nPipeline Scheduler System demonstration completed!" << std::endl;

    return 0;
}