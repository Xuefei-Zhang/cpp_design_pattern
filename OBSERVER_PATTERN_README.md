# Observer Pattern in C++

This project demonstrates the Observer design pattern in C++ using event-driven examples and thread-safe subject/observer coordination.

## Overview

The Observer pattern is a behavioral design pattern that defines a one-to-many dependency between objects. When a subject changes state, all registered observers are notified automatically.

## Key Concepts

### When to use Observer pattern:
- When one object change must notify many dependent objects
- When you need loose coupling between event producers and event consumers
- When your system has broadcast-style updates (UI refresh, domain events, monitoring)
- When subscribers should be attachable/detachable at runtime

### Components of Observer:
- **Subject**: Defines registration and notification operations
- **Observer**: Defines the update callback contract
- **ConcreteSubject**: Stores state and triggers notifications
- **ConcreteObserver**: Reacts to updates from the subject

## Implementation Variations

### 1. Push Model
- Subject pushes event name and payload directly to observers
- Good for event streams where observers should not query extra state

### 2. Pull Model
- Subject notifies observers, and observers pull required state from subject
- Good for richer domain models where observers choose which state to read

### 3. Thread-Safe Subject
- Uses a mutex-protected observer list
- Uses snapshot notification to avoid deadlocks during callbacks
- Supports attach/detach while notifications are running

## Files

- `observer_patterns.h` - Observer interfaces and concrete implementations
- `test_observer_pattern.cpp` - Comprehensive demo/tests (push, pull, detach, concurrency)
- `OBSERVER_PATTERN_README.md` - This documentation

## Key Examples

### 1. Subject/Observer Interfaces
```cpp
class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const Subject& subject,
                        const std::string& eventName,
                        const std::string& payload) = 0;
};

class Subject {
public:
    virtual ~Subject() = default;
    virtual void attachObserver(const std::shared_ptr<Observer>& observer) = 0;
    virtual void detachObserver(const std::shared_ptr<Observer>& observer) = 0;
    virtual void notifyObservers(const std::string& eventName,
                                 const std::string& payload) = 0;
};
```

### 2. Push Notification Example
```cpp
WeatherStation station("CityCenter");
auto phoneDisplay = std::make_shared<WeatherConsoleDisplay>("Phone");

station.attachObserver(phoneDisplay);
station.setMeasurements(22.5, 50.0, 1012.0);  // emits weather.updated
```

### 3. Pull Notification Example
```cpp
NumericModel model("TemperatureThreshold", 10);
auto view = std::make_shared<ModelValueView>("A");

model.attachObserver(view);
model.setValue(30);  // view pulls model value via typed subject access
```

## Compile and Run

```bash
g++ -std=c++14 -pthread -o test_observer_pattern test_observer_pattern.cpp
./test_observer_pattern
```

## Educational Value

This implementation demonstrates:
- Push and pull notification styles in one pattern family
- Loose coupling between event source and listeners
- Safe runtime attach/detach behavior
- Re-entrant callback safety (observer detaching itself)
- Thread-safe notification for concurrent systems

## Observer vs Other Patterns

- **Mediator**: Centralizes interactions, while Observer broadcasts to subscribers
- **Command**: Encapsulates requests; Observer handles event propagation
- **State**: Changes behavior by internal state; Observer coordinates external reactions

## Important Notes

- Keep observer callbacks fast to avoid long notification latency
- Avoid retaining stale observer references (weak_ptr cleanup is used here)
- Snapshot-then-notify helps avoid lock reentrancy issues
- Prefer domain-specific event names and payload conventions

## Benefits

- **Loose Coupling**: Publishers do not depend on concrete subscribers
- **Extensibility**: New observers can be added without changing subjects
- **Runtime Flexibility**: Dynamic subscribe/unsubscribe behavior
- **Scalability**: Works well for event-driven architectures
