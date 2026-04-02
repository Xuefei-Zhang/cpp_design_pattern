# C++ 设计模式 (GoF) 索引

欢迎来到本项目的 C++ 设计模式实现库。本目录索引了所有模式的文档、头文件和测试位置，便于快速跳转。

## 设计模式分类

### 创建型模式 (Creational Patterns)

| 模式名称 | 文档 | 核心实现 | 测试/示例 |
| :--- | :--- | :--- | :--- |
| **单例模式 (Singleton)** | [`docs/zh-cn`](../creational/singleton/docs/zh-cn.md) | `creational/singleton/include/singleton_patterns.h` | `creational/singleton/tests/test_singleton_pattern.cpp` |
| **工厂方法 (Factory Method)** | [`docs/zh-cn`](../creational/factory_method/docs/zh-cn.md) | `creational/factory_method/include/factory_method_shapes.h` | `creational/factory_method/tests/test_factory_method.cpp` |
| **抽象工厂 (Abstract Factory)** | [`docs/zh-cn`](../creational/abstract_factory/docs/zh-cn.md) | `creational/abstract_factory/include/abstract_factory_shapes.h` | `creational/abstract_factory/tests/test_abstract_factory.cpp` |
| **生成器模式 (Builder)** | [`docs/zh-cn`](../creational/builder/docs/zh-cn.md) | `creational/builder/include/builder_shapes.h` | `creational/builder/tests/test_builder_pattern.cpp` |
| **原型模式 (Prototype)** | [`docs/zh-cn`](../creational/prototype/docs/zh-cn.md) | `creational/prototype/include/prototype_shapes.h` | `creational/prototype/tests/test_prototype_pattern.cpp` |

### 结构型模式 (Structural Patterns)

| 模式名称 | 文档 | 核心实现 | 测试/示例 |
| :--- | :--- | :--- | :--- |
| **适配器模式 (Adapter)** | [`docs/zh-cn`](../structural/adapter/docs/zh-cn.md) | `structural/adapter/include/adapter_patterns.h` | `structural/adapter/tests/test_adapter_pattern.cpp` |
| **桥接模式 (Bridge)** | [`docs/zh-cn`](../structural/bridge/docs/zh-cn.md) | `structural/bridge/include/bridge_patterns.h` | `structural/bridge/tests/test_bridge_pattern.cpp` |
| **组合模式 (Composite)** | [`docs/zh-cn`](../structural/composite/docs/zh-cn.md) | `structural/composite/include/composite_patterns.h` | `structural/composite/tests/test_composite_pattern.cpp` |
| **装饰器模式 (Decorator)** | [`docs/zh-cn`](../structural/decorator/docs/zh-cn.md) | `structural/decorator/include/decorator_patterns.h` | `structural/decorator/tests/test_decorator_pattern.cpp` |
| **外观模式 (Facade)** | [`docs/zh-cn`](../structural/facade/docs/zh-cn.md) | `structural/facade/include/graphics_facade.h` | `structural/facade/tests/test_facade_pattern.cpp` |
| **享元模式 (Flyweight)** | [`docs/zh-cn`](../structural/flyweight/docs/zh-cn.md) | `structural/flyweight/include/flyweight_patterns.h` | `structural/flyweight/tests/test_flyweight_pattern_final.cpp` |
| **代理模式 (Proxy)** | [`docs/zh-cn`](../structural/proxy/docs/zh-cn.md) | `structural/proxy/include/proxy_patterns.h` | `structural/proxy/tests/test_proxy_pattern_final.cpp` |

### 行为型模式 (Behavioral Patterns)

| 模式名称 | 文档 | 核心实现 | 测试/示例 |
| :--- | :--- | :--- | :--- |
| **责任链模式 (Chain of Responsibility)** | [`docs/zh-cn`](../behavioral/chain_of_responsibility/docs/zh-cn.md) | `behavioral/chain_of_responsibility/include/chain_of_responsibility_shapes_fixed.h` | `behavioral/chain_of_responsibility/tests/test_chain_of_responsibility_final.cpp` |
| **命令模式 (Command)** | [`docs/zh-cn`](../behavioral/command/docs/zh-cn.md) | `behavioral/command/include/command_patterns.h` | `behavioral/command/tests/test_command_pattern_final.cpp` |
| **解释器模式 (Interpreter)** | [`docs/zh-cn`](../behavioral/interpreter/docs/zh-cn.md) | `behavioral/interpreter/include/interpreter_patterns.h` | `behavioral/interpreter/tests/test_interpreter_pattern.cpp` |
| **迭代器模式 (Iterator)** | [`docs/zh-cn`](../behavioral/iterator/docs/zh-cn.md) | `behavioral/iterator/include/iterator_patterns.h` | `behavioral/iterator/tests/test_iterator_pattern.cpp` |
| **中介者模式 (Mediator)** | [`docs/zh-cn`](../behavioral/mediator/docs/zh-cn.md) | `behavioral/mediator/include/mediator_patterns.h` | `behavioral/mediator/tests/test_mediator_pattern.cpp` |
| **备忘录模式 (Memento)** | [`docs/zh-cn`](../behavioral/memento/docs/zh-cn.md) | `behavioral/memento/include/memento_patterns.h` | `behavioral/memento/tests/test_memento_pattern.cpp` |
| **观察者模式 (Observer)** | [`docs/zh-cn`](../behavioral/observer/docs/zh-cn.md) | `behavioral/observer/include/observer_patterns.h` | `behavioral/observer/tests/test_observer_pattern.cpp` |
| **状态模式 (State)** | [`docs/zh-cn`](../behavioral/state/docs/zh-cn.md) | `behavioral/state/include/state_patterns.h` | `behavioral/state/tests/test_state_pattern.cpp` |
| **策略模式 (Strategy)** | [`docs/zh-cn`](../behavioral/strategy/docs/zh-cn.md) | `behavioral/strategy/include/strategy_patterns.h` | `behavioral/strategy/tests/test_strategy_pattern.cpp` |
| **模板方法模式 (Template Method)** | [`docs/zh-cn`](../behavioral/template_method/docs/zh-cn.md) | `behavioral/template_method/include/template_method_patterns.h` | `behavioral/template_method/tests/test_template_method_pattern.cpp` |
| **访问者模式 (Visitor)** | [`docs/zh-cn`](../behavioral/visitor/docs/zh-cn.md) | `behavioral/visitor/include/visitor_patterns.h` | `behavioral/visitor/tests/test_visitor_pattern.cpp` |

---

## 相关指南

- [性能与内存分析指南](./performance-and-memory-analysis.md)
- 每个模式目录下的 `docs/` 子目录包含更详细的英文/中文说明。

## 如何使用

每个模式的测试文件都是独立的 C++ 程序。可以使用统一脚本：

```bash
./scripts/run_tests.sh
```

或编译单个测试（示例：策略模式）：

```bash
g++ -std=c++14 -pthread \\
    -I ./behavioral/strategy/include \\
    behavioral/strategy/tests/test_strategy_pattern.cpp \\
    -o build/tests/behavioral/strategy/test_strategy_pattern
./build/tests/behavioral/strategy/test_strategy_pattern
```

如需分析性能与内存，请参考性能指南。
