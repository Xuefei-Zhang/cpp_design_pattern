## TaskScheduler 类详细解析

### 1. 类成员变量

```cpp
private:
    std::vector<std::unique_ptr<TaskExecutor>> executors;     // 线程池，执行任务
    std::vector<std::shared_ptr<Task>> taskQueue;            // 等待处理的任务队列（有依赖的）
    std::vector<std::shared_ptr<Task>> readyQueue;           // 准备执行的任务队列（无依赖的）
    std::vector<std::shared_ptr<Task>> runningTasks;         // 正在执行的任务队列
    std::map<std::string, std::shared_ptr<Task>> taskMap;    // 所有任务的映射表
    
    mutable std::mutex schedulerMutex;                       // 保护共享资源的互斥锁
    std::condition_variable taskAvailable;                   // 任务可用通知条件变量
    std::atomic<bool> shouldStop{false};                     // 停止标志
```

### 2. 三大队列的作用

TaskScheduler 使用三个队列来管理任务的不同状态：

- **taskQueue**: 存放刚添加的任务（有依赖关系，暂时无法执行）
- **readyQueue**: 存放已满足依赖条件的任务（可以立即执行）
- **runningTasks**: 存放正在被 Executor 执行的任务（正在运行）

### 3. 核心方法解析

#### `addTask()` - 添加任务
```cpp
void addTask(std::shared_ptr<Task> task) {
    std::lock_guard<std::mutex> lock(schedulerMutex);
    taskMap[task->taskId] = task;    // 将任务加入映射表
    taskQueue.push_back(task);       // 将任务加入等待队列
    updateReadyTasks();              // 检查是否有任务变为可执行
    taskAvailable.notify_all();      // 通知等待线程有新任务
}
```

#### `updateReadyTasks()` - 更新就绪任务
```cpp
void updateReadyTasks() {
    std::vector<std::shared_ptr<Task>> newReadyTasks;
    
    // 找出没有依赖的任务
    for (auto& task : taskQueue) {
        if (task->isReady()) {    // 依赖列表为空
            task->updateState(TaskState::READY);
            newReadyTasks.push_back(task);
        }
    }
    
    // 从等待队列移除，加入就绪队列
    for (auto& readyTask : newReadyTasks) {
        taskQueue.erase(
            std::remove(taskQueue.begin(), taskQueue.end(), readyTask),
            taskQueue.end()
        );
        readyQueue.push_back(readyTask);
    }
    
    taskAvailable.notify_all();
}
```

#### `getNextReadyTask()` - 获取可执行任务
```cpp
std::shared_ptr<Task> getNextReadyTask() {
    std::lock_guard<std::mutex> lock(schedulerMutex);
    
    if (readyQueue.empty()) {
        return nullptr;
    }
    
    auto task = readyQueue.back();      // 从队列末尾取出任务
    readyQueue.pop_back();
    runningTasks.push_back(task);       // 将任务移到运行队列
    
    return task;
}
```

#### `onTaskStateChanged()` - 任务状态变化处理
```cpp
void onTaskStateChanged(const std::string& taskId, TaskState newState) {
    std::lock_guard<std::mutex> lock(schedulerMutex);

    auto it = taskMap.find(taskId);
    if (it != taskMap.end()) {
        it->second->updateState(newState);

        // 如果任务完成或失败
        if (newState == TaskState::COMPLETED || newState == TaskState::FAILED) {
            // 从运行队列移除
            runningTasks.erase(
                std::remove(runningTasks.begin(), runningTasks.end(), it->second),
                runningTasks.end()
            );

            // 更新依赖关系
            updateDependencies(taskId);  // 重要：更新其他任务的依赖

            if (onTaskCompletion) {
                onTaskCompletion();
            }
        }
    }
}
```

#### `updateDependencies()` - 依赖关系更新
```cpp
void updateDependencies(const std::string& completedTaskId) {
    // 对每个等待的任务，移除已完成任务的依赖
    for (auto& pair : taskMap) {
        if (pair.second->getState() == TaskState::PENDING) {
            pair.second->removeDependency(completedTaskId);  // 移除依赖

            if (pair.second->isReady()) {  // 检查是否变为可执行
                pair.second->updateState(TaskState::READY);
                
                // 从等待队列移到就绪队列
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
```

### 4. 多线程协调机制

#### `schedulerLoop()` - 调度器主循环
```cpp
void schedulerLoop() {
    while (!shouldStop) {
        {
            std::unique_lock<std::mutex> lock(schedulerMutex);
            
            // 检查是否有任务变为就绪
            updateReadyTasks();
            
            // 等待新任务或定时检查
            taskAvailable.wait_for(lock, std::chrono::milliseconds(100));
        }
    }
}
```

#### `executorLoop()` - 执行器循环
```cpp
void executorLoop(size_t executorIndex) {
    while (!shouldStop) {
        std::shared_ptr<Task> task = getNextReadyTask();  // 获取可执行任务
        
        if (task) {
            executors[executorIndex]->executeTask(task);  // 执行任务
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}
```

### 5. 关键流程总结

1. **任务添加**: 任务被添加到 `taskQueue`，然后 `updateReadyTasks()` 检查依赖
2. **依赖检查**: 如果任务没有依赖，移到 `readyQueue`
3. **任务分发**: Executor 从 `readyQueue` 获取任务执行
4. **状态反馈**: 任务完成后回调 `onTaskStateChanged()`
5. **依赖更新**: 完成的任务会触发其他等待它的任务更新依赖状态
6. **队列流转**: 其他任务可能从 `taskQueue` 流向 `readyQueue`，形成新的执行机会

这个设计的核心是**依赖驱动**的调度机制：任务完成时会通知调度器更新其他任务的依赖状态，从而自动触发后续任务的执行，形成了一个动态的、响应式的任务执行流程。