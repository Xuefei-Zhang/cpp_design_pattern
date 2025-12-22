# Pipeline Scheduler System Architecture

## System Overview

The Pipeline Scheduler System is a concurrent task processing system that manages tasks through a pipeline architecture with scheduler and executor components.

## Class Relationships

```mermaid
classDiagram
    class IDrawable {
        <<interface>>
        +~virtual void draw() const
        +~virtual void resize(double factor)
        +~virtual double getArea() const
    }
    
    class Shape {
        -double x, y
        -double scale
        -string id
        +virtual string getType() const
        +virtual void draw() const
        +virtual double getArea() const
    }
    
    class Circle {
        -double radius
        +void draw() const
        +string getType() const
        +double getArea() const
    }
    
    class Rectangle {
        -double width, height
        +void draw() const
        +string getType() const
        +double getArea() const
    }
    
    class Triangle {
        -double base, height
        +void draw() const
        +string getType() const
        +double getArea() const
    }
    
    class ShapeFactory {
        +ShapeType enum
        +createShape(ShapeType, double, double, double, double, string): unique_ptr~Shape~
    }
    
    class Task {
        +string taskId
        +string taskName
        +TaskState state
        +vector~string~ dependencies
        +vector~string~ dependents
        +execute() virtual
        +getTaskType() virtual
        +addDependency(string)
        +isReady() bool
    }
    
    class GenerateShapeTask {
        +unique_ptr~Shape~ shape
        +ShapeFactory::ShapeType shapeType
        +execute()
        +getTaskType()
    }
    
    class ValidateShapeTask {
        +unique_ptr~Shape~ shape
        +execute()
        +getTaskType()
    }
    
    class TransformShapeTask {
        +unique_ptr~Shape~ shape
        +double scaleX, scaleY
        +execute()
        +getTaskType()
    }
    
    class RenderShapeTask {
        +unique_ptr~Shape~ shape
        +execute()
        +getTaskType()
    }
    
    class TaskExecutor {
        -string executorId
        -atomic~bool~ shouldStop
        +executeTask(shared_ptr~Task~): bool
        +getId(): string
    }
    
    class TaskScheduler {
        -vector~unique_ptr~TaskExecutor~~ executors
        -vector~shared_ptr~Task~~ taskQueue
        -vector~shared_ptr~Task~~ readyQueue
        -vector~shared_ptr~Task~~ runningTasks
        -map~string, shared_ptr~Task~~ taskMap
        -mutex schedulerMutex
        +addTask(shared_ptr~Task~)
        +addDependency(string, string)
        +start()
        +schedulerLoop()
        +executorLoop(size_t)
    }
    
    class Pipeline {
        -string pipelineName
        -shared_ptr~TaskScheduler~ scheduler
        -vector~string~ stageNames
        +submitShapeProcessingTask(ShapeFactory::ShapeType, double, double, double, double): string
        +printStatus()
    }
    
    IDrawable <|-- Shape
    Shape <|-- Circle
    Shape <|-- Rectangle
    Shape <|-- Triangle
    
    ShapeFactory ..> Shape
    
    Task <|-- GenerateShapeTask
    Task <|-- ValidateShapeTask
    Task <|-- TransformShapeTask
    Task <|-- RenderShapeTask
    
    GenerateShapeTask --> Shape
    ValidateShapeTask --> Shape
    TransformShapeTask --> Shape
    RenderShapeTask --> Shape
    
    TaskScheduler o-- TaskExecutor
    TaskScheduler o-- Task
    
    Pipeline o-- TaskScheduler
    Pipeline ..> Task
```

## Multi-threading Flow Diagram

```mermaid
sequenceDiagram
    participant User
    participant Pipeline
    participant TaskScheduler
    participant Executor1 as Executor 1
    participant Executor2 as Executor 2
    participant Task1 as Generate Task
    participant Task2 as Validate Task
    participant Task3 as Transform Task
    participant Task4 as Render Task
    
    User->>Pipeline: submitShapeProcessingTask()
    Pipeline->>TaskScheduler: addTask(), addDependency()
    
    Note over TaskScheduler: TaskQueue: [Gen, Val, Trans, Render]
    Note over TaskScheduler: Dependencies: Gen->Val->Trans->Render
    
    TaskScheduler->>TaskScheduler: updateReadyTasks()
    TaskScheduler->>TaskScheduler: start() - spawns scheduler thread
    
    loop Process Tasks
        TaskScheduler->>TaskScheduler: schedulerLoop()
        TaskScheduler->>Executor1: getNextReadyTask()
        alt Task Available
            Executor1->>Task1: executeTask()
            Task1->>Task1: execute()
            Task1->>TaskScheduler: onTaskStateChanged(COMPLETED)
            TaskScheduler->>TaskScheduler: updateDependencies()
            TaskScheduler->>Task2: Mark as READY
        else No Tasks Available
            TaskScheduler->>TaskScheduler: Wait for new tasks (100ms)
        end
    end
    
    TaskScheduler->>Executor2: getNextReadyTask() for next available task
    Executor2->>Task2: executeTask()
    Task2->>Task2: execute()
    Task2->>TaskScheduler: onTaskStateChanged(COMPLETED)
```

## Task State Machine

```mermaid
stateDiagram-v2
    [*] --> PENDING: Task Created
    PENDING --> READY: No Dependencies
    READY --> RUNNING: Executor Takes Task
    RUNNING --> COMPLETED: Execution Success
    RUNNING --> FAILED: Execution Error
    COMPLETED --> [*]
    FAILED --> [*]
```

## Data Flow Between Tasks

```mermaid
graph LR
    A[GenerateShapeTask] -->|shape data| B(ValidateShapeTask)
    B -->|valid shape| C(TransformShapeTask)
    C -->|transformed shape| D(RenderShapeTask)
    
    style A fill:#e1f5fe
    style B fill:#f3e5f5
    style C fill:#e8f5e8
    style D fill:#fff3e0
```

## Component Interaction Overview

```mermaid
graph TB
    subgraph "Pipeline Layer"
        P[Pipeline]
    end
    
    subgraph "Scheduler Layer"
        TS[TaskScheduler]
    end
    
    subgraph "Execution Layer"
        TE1[TaskExecutor 1]
        TE2[TaskExecutor N]
    end
    
    subgraph "Task Layer"
        T1[Generate Task]
        T2[Validate Task]
        T3[Transform Task]
        T4[Render Task]
    end
    
    P --> TS
    TS --> TE1
    TS --> TE2
    TS --> T1
    TS --> T2
    TS --> T3
    TS --> T4
```