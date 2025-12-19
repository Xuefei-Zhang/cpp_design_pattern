# Advanced Builder Pattern: Scene Graph Builder

This project demonstrates an advanced implementation of the Builder design pattern using scene graph construction as an example, creating complex hierarchical visual scenes.

## Overview

This Builder implementation creates complex scene graphs with parent-child relationships between objects, demonstrating how the Builder pattern can be used to construct very complex objects step-by-step with many interdependent parts.

## Key Concepts

### When to use Advanced Builder patterns:
- When constructing complex hierarchical objects
- When objects have many interdependent parts that need to be created in a specific order
- When you need to manage relationships between objects in the final structure
- When the construction process itself is complex and needs to be separated from representation

### Components of this Advanced Builder:
- **Product**: Scene with hierarchical SceneNode structure
- **Builder**: Abstract interface for building scenes step-by-step
- **ConcreteBuilder**: SceneBuilderImpl that implements the construction steps
- **Director**: SceneDirector that orchestrates complex scene construction
- **Fluent Interface**: FluentSceneBuilder for more readable construction

## Advanced Features

### 1. Hierarchical Structure Building
- Creates parent-child relationships in the scene graph
- Handles transformation matrices and positioning relative to parents
- Maintains proper scene hierarchy during construction

### 2. Named Object Management
- Tracks named objects for easy reference during construction
- Allows setting properties on objects after they're created
- Manages relationships between named entities

### 3. Complex Scene Construction
- Builds scenes with multiple nested groups
- Handles positioning in scene graph hierarchy
- Manages transformations across parent-child relationships

### 4. Fluent Interface
- Provides readable, chainable method calls
- Makes complex scene construction more maintainable
- Follows best practices for fluent builder patterns

## Files

- `scene_builder.h` - Advanced Builder pattern implementation
- `test_scene_builder.cpp` - Comprehensive test program
- `SCENE_BUILDER_README.md` - This documentation

## Key Examples

### 1. Basic Scene Builder Usage
```cpp
auto builder = std::make_unique<SceneBuilderImpl>();
auto scene = builder->reset()
                    ->setName("My Scene")
                    ->addShape("Circle", std::make_unique<Circle>(5.0, "red"))
                    ->setTransform("Circle", 10.0, 10.0, 1.0, 0.0)
                    ->getResult();
```

### 2. Fluent Builder Usage
```cpp
auto scene = FluentSceneBuilder()
                .name("Fluent Scene")
                .addCircle("Sun", 10.0, "yellow")
                .transform("Sun", 50.0, 50.0, 1.0, 0.0)
                .addRectangle("Sign", 15.0, 8.0, "white")
                .transform("Sign", 100.0, 100.0, 1.2, 15.0)
                .build();
```

### 3. Director Pattern Usage
```cpp
SceneDirector director(std::make_unique<SceneBuilderImpl>());
auto complexScene = director.createHierarchicalScene();
```

## Compile and Run

```bash
g++ -std=c++14 -o test_scene_builder test_scene_builder.cpp
./test_scene_builder
```

## Educational Value

This implementation demonstrates:
- How Builder can create complex hierarchical objects
- Management of parent-child relationships during construction
- How to handle interdependent object relationships
- Advanced fluent interface design
- Scene graph construction techniques
- State management in complex builders
- Real-world application of Builder for scene management

## Advanced Builder vs Simple Builder

- **Simple Builder**: Constructs simple objects with optional parameters
- **Advanced Builder**: Constructs complex objects with hierarchical relationships and interdependencies
- **Scene Builder**: Handles complex scene graph structures with transformations and positioning

## Important Notes

- The Builder manages the complexity of creating hierarchical structures
- Transformations and positioning are calculated relative to parent nodes
- The scene graph maintains parent-child relationships for rendering
- Fluent interface makes complex scene construction more readable
- Director pattern provides predefined complex scene templates

## Benefits

- **Complex Object Construction**: Handles very complex object creation
- **Relationship Management**: Manages interdependencies between parts
- **Hierarchical Structure**: Maintains parent-child relationships
- **Readable Construction**: Fluent interface for better code readability
- **Reusable Construction**: Same builder can create multiple scenes
- **Separation of Concerns**: Construction logic separated from representation