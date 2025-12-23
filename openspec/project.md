# Project Context

## Purpose
This project is a comprehensive C++ learning and reference repository focused on implementing and demonstrating design patterns, system architecture concepts, and advanced C++ programming techniques. It serves as both an educational resource and a practical implementation guide for various software engineering patterns and principles.

## Tech Stack
- C++ (C++14/C++17/C++20 features)
- Standard Template Library (STL)
- STL threading and synchronization primitives (std::thread, std::mutex, std::atomic, etc.)
- STL smart pointers (std::unique_ptr, std::shared_ptr)
- STL containers (std::vector, std::queue, std::unordered_map, etc.)
- Modern C++ idioms (RAII, move semantics, const correctness)

## Project Conventions

### Code Style
- Header files use .h extension, implementation files use .cpp extension
- Header guards follow the pattern: #ifndef FILE_NAME_H / #define FILE_NAME_H / #endif
- Use of modern C++ smart pointers instead of raw pointers where possible
- RAII (Resource Acquisition Is Initialization) pattern for resource management
- Consistent use of const correctness
- Prefer initialization lists in constructors
- Use of std::move for efficient transfers of ownership
- Naming conventions: PascalCase for classes, camelCase for methods and variables
- Extensive use of inline documentation within code files

### Architecture Patterns
- Design patterns implementation (Abstract Factory, Adapter, Bridge, Builder, Command, Composite, Decorator, Facade, Factory Method, Flyweight, Proxy, Singleton, etc.)
- Object-oriented design principles (SOLID principles)
- Separation of concerns with clear class responsibilities
- Use of abstract interfaces and concrete implementations
- Resource management patterns (memory pools, buffer pools, thread pools)
- Pipeline and task scheduling systems

### Testing Strategy
- Dedicated test files for each major component (test_*.cpp)
- Executable test programs that demonstrate functionality
- Unit testing approach with direct instantiation and method calls
- Integration testing through complete system demonstrations

### Git Workflow
- Feature-based development with descriptive commit messages
- Each design pattern or system component in separate, focused files
- README files accompanying major implementations

## Domain Context
AI assistants working on this project need to understand:
- Modern C++ memory management with smart pointers
- Threading and concurrency patterns with STL primitives
- Design patterns implementation in C++
- Resource management techniques (memory pools, buffer pools)
- System architecture patterns for performance-critical applications
- RAII and exception-safe programming practices

## Important Constraints
- Code must be compatible with C++14 and later standards
- Thread safety is critical for shared resources
- Memory efficiency is prioritized in pool implementations
- Maintainability and educational value are key requirements
- Performance considerations for concurrent systems

## External Dependencies
- Standard C++ Library (no external dependencies beyond STL)
- Standard C++ compiler with C++14 or later support
- Platform support for threading (POSIX threads on Linux/macOS, Windows threads on Windows)
