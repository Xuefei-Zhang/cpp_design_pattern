# C++ 设计模式 (GoF) 索引

欢迎来到本项目的 C++ 设计模式实现库。本目录包含经典的 GoF 设计模式实现，旨在展示清晰的现代 C++ 实践。

## 设计模式分类

### 创建型模式 (Creational Patterns)
这些模式涉及对象的实例化，提供了解耦客户端和对象创建过程的方法。

| 模式名称 | 核心实现 | 测试/示例 |
| :--- | :--- | :--- |
| [**单例模式 (Singleton)**](./singleton.md) | `singleton_patterns.h` | `test_singleton_pattern.cpp` |
| [**工厂方法 (Factory Method)**](./factory-method.md) | `factory_method_shapes.h` | `test_factory_method.cpp` |
| [**抽象工厂 (Abstract Factory)**](./abstract-factory.md) | `abstract_factory_shapes.h` | `test_abstract_factory.cpp` |
| [**生成器模式 (Builder)**](./builder.md) | `builder_shapes.h` | `test_builder_pattern.cpp` |
| [**原型模式 (Prototype)**](./prototype.md) | `prototype_shapes.h` | `test_prototype_pattern.cpp` |

### 结构型模式 (Structural Patterns)
这些模式关注如何组合类和对象以形成更大的结构。

| 模式名称 | 核心实现 | 测试/示例 |
| :--- | :--- | :--- |
| [**适配器模式 (Adapter)**](./adapter.md) | `adapter_patterns.h` | `test_adapter_pattern.cpp` |
| [**桥接模式 (Bridge)**](./bridge.md) | `bridge_patterns.h` | `test_bridge_pattern.cpp` |
| [**组合模式 (Composite)**](./composite.md) | `composite_patterns.h` | `test_composite_pattern.cpp` |
| [**装饰器模式 (Decorator)**](./decorator.md) | `decorator_patterns.h` | `test_decorator_pattern.cpp` |
| [**外观模式 (Facade)**](./facade.md) | `graphics_facade.h` / `facade_subsystems.h` | `test_facade_pattern.cpp` |
| [**享元模式 (Flyweight)**](./flyweight.md) | `flyweight_shapes.h` | `test_flyweight_pattern_final.cpp` |
| [**代理模式 (Proxy)**](./proxy.md) | `proxy_shapes.h` | `test_proxy_pattern_corrected.cpp` |

### 行为型模式 (Behavioral Patterns)
这些模式关注对象之间的通信以及职责分配。

| 模式名称 | 核心实现 | 测试/示例 |
| :--- | :--- | :--- |
| [**责任链模式 (Chain of Responsibility)**](./chain-of-responsibility.md) | `chain_of_responsibility_shapes_fixed.h` | `test_chain_of_responsibility_final.cpp` |
| [**命令模式 (Command)**](./command.md) | `command_patterns.h` | `test_command_pattern_final.cpp` |
| [**观察者模式 (Observer)**](./observer.md) | `observer_patterns.h` | `test_observer_pattern.cpp` |
| [**策略模式 (Strategy)**](./strategy.md) | `strategy_patterns.h` | `test_strategy_pattern.cpp` |
| [**状态模式 (State)**](./state.md) | `state_patterns.h` | `test_state_pattern.cpp` |

---

## 相关指南

- [性能与内存分析指南](./performance-and-memory-analysis.md)
- [Mermaid 图表使用说明](./performance-and-memory-analysis.md#mermaid-图表指南)

## 如何使用

每个模式的测试文件都是独立的 C++ 程序。你可以使用 `g++` 编译并运行它们，例如：

```bash
g++ -std=c++14 test_strategy_pattern.cpp -o strategy_test
./strategy_test
```

对于内存和性能分析，请参考上述指南。
