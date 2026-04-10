# Design Patterns in C++

This comprehensive list covers the major design patterns categorized by their purpose. Each pattern addresses a specific problem in software design and provides a reusable solution.

## Creational Patterns

These patterns focus on how objects are created, making systems independent of how objects are created, composed, and represented.

### 1. **Singleton**
- Ensures a class has only one instance and provides a global point of access to it
- Useful for logging, configuration settings, thread pools, database connections

### 2. **Factory Method**
- Defines an interface for creating an object, but lets subclasses alter the type of objects that will be created
- Promotes loose coupling by eliminating the need to bind application-specific classes into the code

### 3. **Abstract Factory**
- Provides an interface for creating families of related or dependent objects without specifying their concrete classes
- Useful when you need to create objects that belong to different but related families

### 4. **Builder**
- Separates the construction of a complex object from its representation, allowing the same construction process to create different representations
- Good for objects with many optional parameters or complex initialization

### 5. **Prototype**
- Specifies the kinds of objects to create using a prototypical instance, and creates new objects by copying this prototype
- Useful when object creation is expensive or when you want to avoid subclassing

## Structural Patterns

These patterns focus on how classes and objects are composed to form larger structures.

### 6. **Adapter**
- Converts the interface of a class into another interface that clients expect
- Allows classes to work together that couldn't otherwise because of incompatible interfaces

### 7. **Bridge**
- Decouples an abstraction from its implementation so that the two can vary independently
- Useful when you need to avoid permanent binding between an abstraction and its implementation

### 8. **Composite**
- Composes objects into tree structures to represent part-whole hierarchies
- Allows clients to treat individual objects and compositions uniformly

### 9. **Decorator**
- Attaches additional responsibilities to an object dynamically
- Provides a flexible alternative to subclassing for extending functionality

### 10. **Facade**
- Provides a unified interface to a set of interfaces in a subsystem
- Defines a higher-level interface that makes the subsystem easier to use

### 11. **Flyweight**
- Uses sharing to support large numbers of fine-grained objects efficiently
- Useful when you need to create a large number of similar objects

### 12. **Proxy**
- Provides a surrogate or placeholder for another object to control access to it
- Can provide lazy initialization, access control, logging, or other features

## Behavioral Patterns

These patterns focus on communication between objects.

### 13. **Chain of Responsibility**
- Passes a request along a chain of handlers, where upon receiving a request, each handler decides either to process the request or to pass it to the next handler in the chain

### 14. **Command**
- Encapsulates a request as an object, thereby allowing for parameterization of clients with queues, requests, and operations
- Enables features like undo/redo functionality

### 15. **Interpreter**
- Defines a representation for a grammar along with an interpreter that uses the representation to interpret sentences in the grammar
- Best used for small languages or expression evaluation

### 16. **Iterator**
- Provides a way to access the elements of an aggregate object sequentially without exposing its underlying representation
- Standard library iterators in C++ are examples of this pattern

### 17. **Mediator**
- Defines an object that encapsulates how a set of objects interact
- Promotes loose coupling by keeping objects from referring to each other explicitly

### 18. **Memento**
- Captures and externalizes an object's internal state without violating encapsulation, so the object can be restored to this state later
- Useful for implementing undo mechanisms

### 19. **Observer**
- Defines a one-to-many dependency between objects so that when one object changes state, all dependents are notified automatically
- Widely used in event handling systems and Model-View architectures

### 20. **State**
- Allows an object to alter its behavior when its internal state changes
- The object will appear to change its class

### 21. **Strategy**
- Defines a family of algorithms, encapsulates each one, and makes them interchangeable
- Allows the algorithm to vary independently from clients that use it

### 22. **Template Method**
- Defines the skeleton of an algorithm in an operation, deferring some steps to subclasses
- Allows subclasses to redefine certain steps of an algorithm without changing the algorithm's structure

### 23. **Visitor**
- Represents an operation to be performed on the elements of an object structure
- Allows defining new operations without changing the classes of the elements on which it operates

## Modern C++ Specific Patterns

### 24. **RAII (Resource Acquisition Is Initialization)**
- Manages resource lifecycle using object lifetime
- Ensures resources are properly released when objects go out of scope

### 25. **PIMPL (Pointer to IMPLementation)**
- Hides implementation details and reduces compilation dependencies
- Helps with binary compatibility in library design

### 26. **CRTP (Curiously Recurring Template Pattern)**
- Achieves static polymorphism through templates
- Provides performance benefits of polymorphism without virtual function overhead

## Pattern Categories Summary

| Category | Purpose | Examples |
|----------|---------|----------|
| **Creational** | How objects are created | Singleton, Factory, Builder |
| **Structural** | How objects are structured | Adapter, Facade, Proxy |
| **Behavioral** | How objects communicate | Observer, Strategy, Command |

## Benefits of Using Design Patterns

- **Reusability**: Proven solutions that can be applied to similar problems
- **Communication**: Common vocabulary for discussing solutions
- **Maintainability**: Well-structured, organized code
- **Scalability**: Design patterns promote loose coupling
- **Best Practices**: Incorporate lessons learned from experienced developers

## Important Considerations

- Don't force patterns where they're not needed
- Understand the problem first, then choose the appropriate pattern
- Patterns should improve code, not make it more complex
- Consider the trade-offs of each pattern
- Some patterns may not be needed in languages with built-in features