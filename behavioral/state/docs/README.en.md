# State Pattern in C++

This project demonstrates the State design pattern in C++ using a document workflow (Draft → Review → Published → Archived) with state-specific behavior and controlled transitions.

## Overview

The State pattern is a behavioral design pattern that lets an object alter its behavior when its internal state changes. The object appears to change its class because behavior is delegated to state objects.

## Key Concepts

### When to use State pattern:
- When object behavior depends heavily on internal lifecycle phase
- When large conditional blocks are used to branch by mode/state
- When valid transitions should be explicit and enforced
- When each state needs distinct behavior for the same operations

### Components of State:
- **State**: Common interface for state-specific behavior
- **ConcreteState**: Implements behavior for one lifecycle state
- **Context**: Holds current state and delegates operations
- **Client**: Uses context operations without state branching logic

## Implementation Variations

### 1. Workflow State Machine
- `DocumentWorkflow` context delegates operations to active state
- `DraftState`, `ReviewState`, `PublishedState`, `ArchivedState`

### 2. Valid/Invalid Transition Handling
- Valid transitions call `transitionTo(...)`
- Invalid operations remain in current state and log intent

### 3. Revision Lifecycle
- Published documents can spawn a new draft revision via edit
- Approval increments version and clears dirty flag

## Files

- `state_patterns.h` - State interface, context, and concrete states
- `test_state_pattern.cpp` - Workflow test coverage for valid/invalid transitions
- `STATE_PATTERN_README.md` - This documentation

## Key Examples

### 1. Context and Initialization
```cpp
DocumentWorkflow doc("Architecture Decision");
doc.initialize();

// Initial state is Draft
doc.edit("Initial draft content\n");
```

### 2. Review and Publish Flow
```cpp
doc.submitForReview();
doc.approve();

// State becomes Published, version increments
```

### 3. Published Revision and Archive
```cpp
doc.edit("Patch after release\n"); // transitions back to Draft revision
doc.submitForReview();
doc.approve();
doc.archive();
```

## Compile and Run

```bash
g++ -std=c++14 -o test_state_pattern test_state_pattern.cpp
./test_state_pattern
```

## Educational Value

This implementation demonstrates:
- Replacing condition-heavy workflow logic with polymorphic states
- Explicit transition rules per state
- Context delegation for consistent external API
- Handling invalid operations without corrupting state
- Lifecycle metadata management (`version`, `dirty`)

## State vs Other Patterns

- **Strategy**: Strategy is selected externally; State evolves internally by transitions
- **Command**: Command encapsulates actions; State controls how actions behave now
- **Observer**: Observer propagates updates; State controls internal behavior switching

## Important Notes

- Keep transition ownership in state methods to localize lifecycle rules
- Ensure invalid transitions are safe no-ops or explicit feedback paths
- Avoid direct client mutation of current state outside context APIs
- Preserve context invariants (`version`, `dirty`, content integrity)

## Benefits

- **Readability**: Removes sprawling state-condition branches
- **Maintainability**: State logic is localized per concrete state
- **Correctness**: Transition constraints are explicit and testable
- **Extensibility**: New states can be introduced with minimal ripple effects
