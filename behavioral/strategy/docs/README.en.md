# Strategy Pattern in C++

This project demonstrates the Strategy design pattern in C++ with multiple strategy families (sorting, pricing, compression, and payment) and runtime algorithm switching.

## Overview

The Strategy pattern is a behavioral design pattern that defines a family of algorithms, encapsulates each one, and makes them interchangeable. It lets clients switch behavior at runtime without changing context code.

## Key Concepts

### When to use Strategy pattern:
- When you need multiple algorithms for the same task
- When algorithm selection should happen at runtime
- When you want to replace conditional-heavy logic with polymorphism
- When algorithm implementations should evolve independently from client code

### Components of Strategy:
- **Strategy**: Common algorithm interface
- **ConcreteStrategy**: Different algorithm implementations
- **Context**: Holds and uses a strategy through the common interface
- **Client**: Configures which strategy the context uses

## Implementation Variations

### 1. Sorting Strategies
- `BubbleSortStrategy`, `QuickSortStrategy`, `MergeSortStrategy`
- Demonstrates interchangeable sorting algorithms on the same context

### 2. Pricing Strategies
- Regular price, percentage discount, flat discount, tiered discount
- Demonstrates business-rule variation without changing context flow

### 3. Compression and Payment Strategies
- Text compression algorithms and payment provider selection
- Demonstrates strategy pattern across unrelated domains

## Files

- `strategy_patterns.h` - Strategy interfaces, concrete strategies, and contexts
- `test_strategy_pattern.cpp` - Comprehensive demo/tests for all strategy families
- `STRATEGY_PATTERN_README.md` - This documentation

## Key Examples

### 1. Sorting Context with Runtime Switch
```cpp
SortContext sortContext(std::make_unique<BubbleSortStrategy>());
auto result1 = sortContext.execute({9, 3, 7, 1});

sortContext.setStrategy(std::make_unique<MergeSortStrategy>());
auto result2 = sortContext.execute({7, 3, 9, 1, 7, 2});
```

### 2. Pricing Context
```cpp
PricingContext pricing(std::make_unique<RegularPricingStrategy>());
double fullPrice = pricing.execute(200.0);

pricing.setStrategy(std::make_unique<PercentageDiscountStrategy>(25.0));
double discounted = pricing.execute(200.0);
```

### 3. Payment Provider Strategy
```cpp
PaymentContext payment(std::make_unique<CreditCardPaymentStrategy>());
PaymentResult card = payment.execute(100.0);

payment.setStrategy(std::make_unique<CryptoPaymentStrategy>());
PaymentResult crypto = payment.execute(100.0);
```

## Compile and Run

```bash
g++ -std=c++14 -o test_strategy_pattern test_strategy_pattern.cpp
./test_strategy_pattern
```

## Educational Value

This implementation demonstrates:
- Clear replacement of branching logic with interchangeable strategies
- Runtime strategy switching in a shared context
- Multiple realistic domains where Strategy applies
- Context-side observability (`getLastInput`, `getLastOutput`, last result tracking)
- How to keep algorithm code isolated and testable

## Strategy vs Other Patterns

- **State**: Strategy changes algorithm by external selection; State changes behavior by internal state transitions
- **Template Method**: Strategy composes behavior; Template Method inherits behavior skeleton
- **Command**: Command encapsulates invocations; Strategy encapsulates algorithms

## Important Notes

- Keep strategies focused and cohesive (single algorithm intent)
- Avoid leaking context-specific state into strategy implementations
- Favor value-safe normalization for boundary inputs (negative prices/amounts)
- Context can expose last execution results to simplify testability

## Benefits

- **Interchangeability**: Swap algorithms without changing client logic
- **Extensibility**: Add new strategies with minimal impact
- **Maintainability**: Isolated algorithm implementations
- **Testability**: Each strategy can be validated independently
