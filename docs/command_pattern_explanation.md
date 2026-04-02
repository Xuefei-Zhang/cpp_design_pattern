# Command Pattern in C++

## Overview

The Command Pattern is a behavioral design pattern that turns a request into a stand-alone object that contains all information about the request. This transformation allows for parameterization of methods with different requests, queuing of requests, and logging of the requests. It also provides the ability to undo operations.

## Key Components

### 1. Command Interface
```cpp
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual std::string getName() const = 0;
};
```

### 2. Concrete Commands
Implementation of specific commands like MoveShapeCommand, ResizeShapeCommand, etc.

### 3. Receiver
The object that performs the actual operations (in this case, the Shape objects).

### 4. Invoker
The CommandManager class that manages the execution of commands, maintains history, and handles undo/redo operations.

### 5. Client
The main function that creates and executes commands.

## Implementation Details

### Command Manager Features:
- **Command Execution**: Executes commands and maintains history
- **Undo/Redo**: Maintains stacks for undo and redo operations
- **History Tracking**: Keeps track of all executed commands
- **Command Sequencing**: Ensures proper order of operations

### Undo/Redo Mechanism:
- `undoStack`: Contains commands that can be undone
- `redoStack`: Contains commands that can be redone after undo
- When a new command is executed, the redo stack is cleared
- When a command is undone, it's moved to the redo stack
- When a command is redone, it's moved back to the undo stack

## Benefits of Command Pattern

1. **Decoupling**: Separates the object that invokes the operation from the object that knows how to perform it
2. **Extensibility**: Easy to add new commands without changing existing code
3. **Transaction-like Operations**: Supports complex operations that can be rolled back
4. **Macro Commands**: Can combine multiple commands into a single operation
5. **Undo/Redo Functionality**: Built-in support for reversal of operations
6. **Logging**: Can maintain a history of operations for audit purposes

## Use Cases

- GUI applications (menu items, buttons)
- Text editors (typing, formatting, undo/redo)
- Game engines (actions, movements, spell casting)
- File system operations (copy, move, delete)
- Database transactions
- Any system requiring audit trails or rollback capabilities

## Real World Examples

- Menu systems in applications
- Ctrl+Z / Cmd+Z functionality
- Wizard-based installations
- Macro recording in software
- Command-line interfaces
- API call queuing systems

## Advantages

1. **Flexibility**: Easy to add new commands
2. **Encapsulation**: Each command encapsulates its own execution logic  
3. **Composability**: Commands can be combined to form complex operations
4. **Maintainability**: Changes to command logic don't affect other parts
5. **Testability**: Commands are easier to unit test in isolation

## Disadvantages

1. **Complexity**: Can increase the complexity of simple operations
2. **Memory Usage**: Maintaining command history can use significant memory
3. **Performance**: Additional layers of abstraction may impact performance
4. **Implementation Overhead**: Requires careful design for complex operations

## Comparison with Other Patterns

- **Strategy Pattern**: Strategy defines a family of algorithms, Command encapsulates a request
- **Memento Pattern**: Memento can be used with Command to store state for undo operations
- **Observer Pattern**: Commands can notify observers when executed
- **Composite Pattern**: Commands can be combined into composite commands