# C++ 设计模式实现库

本仓库包含 23 个 GoF 设计模式的完整 C++ 实现，以及实用工具类和示例程序。每个设计模式都集中在自己的目录中，包含头文件、测试、文档和示例，便于按模式直接学习。

## 📁 项目结构

```
cpp_design_pattern/
├── creational/             # 创建型模式（每个子目录含 include/tests/docs/examples）
├── structural/             # 结构型模式
├── behavioral/             # 行为型模式
├── utilities/              # 实用工具模式
├── examples/               # 跨模式示例（如 pipelines、practical_applications）
├── scripts/                # 本地构建/测试脚本
├── docs/                   # 索引、性能指南及历史文档
└── .github/workflows/      # CI 配置
```

## 🎨 设计模式分类

### 创建型模式 (Creational Patterns) - 5个

| 模式 | 头文件 | 测试文件 |
|------|--------|----------|
| **单例模式** (Singleton) | `creational/singleton/include/singleton_patterns.h` | `creational/singleton/tests/test_singleton_pattern.cpp` |
| **工厂方法** (Factory Method) | `creational/factory_method/include/factory_method_shapes.h` | `creational/factory_method/tests/test_factory_method.cpp` |
| **抽象工厂** (Abstract Factory) | `creational/abstract_factory/include/abstract_factory_shapes.h` | `creational/abstract_factory/tests/test_abstract_factory.cpp` |
| **生成器** (Builder) | `creational/builder/include/builder_shapes.h` | `creational/builder/tests/test_builder_pattern.cpp` |
| **原型** (Prototype) | `creational/prototype/include/prototype_shapes.h` | `creational/prototype/tests/test_prototype_pattern.cpp` |

### 结构型模式 (Structural Patterns) - 7个

| 模式 | 头文件 | 测试文件 |
|------|--------|----------|
| **适配器** (Adapter) | `structural/adapter/include/adapter_patterns.h` | `structural/adapter/tests/test_adapter_pattern.cpp` |
| **桥接** (Bridge) | `structural/bridge/include/bridge_patterns.h` | `structural/bridge/tests/test_bridge_pattern.cpp` |
| **组合** (Composite) | `structural/composite/include/composite_patterns.h` | `structural/composite/tests/test_composite_pattern.cpp` |
| **装饰器** (Decorator) | `structural/decorator/include/decorator_patterns.h` | `structural/decorator/tests/test_decorator_pattern.cpp` |
| **外观** (Facade) | `structural/facade/include/graphics_facade.h` | `structural/facade/tests/test_facade_pattern.cpp` |
| **享元** (Flyweight) | `structural/flyweight/include/flyweight_patterns.h` | `structural/flyweight/tests/test_flyweight_pattern_final.cpp` |
| **代理** (Proxy) | `structural/proxy/include/proxy_patterns.h` | `structural/proxy/tests/test_proxy_pattern_final.cpp` |

### 行为型模式 (Behavioral Patterns) - 11个

| 模式 | 头文件 | 测试文件 |
|------|--------|----------|
| **责任链** (Chain of Responsibility) | `behavioral/chain_of_responsibility/include/chain_of_responsibility_shapes_fixed.h` | `behavioral/chain_of_responsibility/tests/test_chain_of_responsibility_final.cpp` |
| **命令** (Command) | `behavioral/command/include/command_patterns.h` | `behavioral/command/tests/test_command_pattern_final.cpp` |
| **解释器** (Interpreter) | `behavioral/interpreter/include/interpreter_patterns.h` | `behavioral/interpreter/tests/test_interpreter_pattern.cpp` |
| **迭代器** (Iterator) | `behavioral/iterator/include/iterator_patterns.h` | `behavioral/iterator/tests/test_iterator_pattern.cpp` |
| **中介者** (Mediator) | `behavioral/mediator/include/mediator_patterns.h` | `behavioral/mediator/tests/test_mediator_pattern.cpp` |
| **备忘录** (Memento) | `behavioral/memento/include/memento_patterns.h` | `behavioral/memento/tests/test_memento_pattern.cpp` |
| **观察者** (Observer) | `behavioral/observer/include/observer_patterns.h` | `behavioral/observer/tests/test_observer_pattern.cpp` |
| **状态** (State) | `behavioral/state/include/state_patterns.h` | `behavioral/state/tests/test_state_pattern.cpp` |
| **策略** (Strategy) | `behavioral/strategy/include/strategy_patterns.h` | `behavioral/strategy/tests/test_strategy_pattern.cpp` |
| **模板方法** (Template Method) | `behavioral/template_method/include/template_method_patterns.h` | `behavioral/template_method/tests/test_template_method_pattern.cpp` |
| **访问者** (Visitor) | `behavioral/visitor/include/visitor_patterns.h` | `behavioral/visitor/tests/test_visitor_pattern.cpp` |

### 实用工具 (Utilities) - 6个

| 工具 | 头文件 | 测试文件 |
|------|--------|----------|
| **内存池** (Memory Pool) | `utilities/memory_pool/include/memory_pool.h` | `utilities/memory_pool/tests/test_memory_pool.cpp` |
| **增强内存池** | `utilities/enhanced_memory_pool/include/enhanced_memory_pool.h` | `utilities/enhanced_memory_pool/tests/test_enhanced_memory_pool.cpp` |
| **线程池** (Thread Pool) | `utilities/thread_pool/include/thread_pool.h` | `utilities/thread_pool/tests/test_thread_pool.cpp` |
| **增强线程池** | `utilities/enhanced_thread_pool/include/enhanced_thread_pool.h` | `utilities/enhanced_thread_pool/tests/test_enhanced_thread_pool.cpp` |
| **友元类** (Friend Classes) | `utilities/friend_classes/include/friend_classes_basic.h` | `utilities/friend_classes/tests/test_friend_classes.cpp` |
| **场景构建器** | `utilities/scene_builder/include/scene_builder.h` | `utilities/scene_builder/tests/test_scene_builder.cpp` |

## 🚀 快速开始

### 运行全部测试

```bash
./scripts/run_tests.sh
```

所有二进制会输出到 `build/tests/<category>/<pattern>/`，脚本会自动编译并执行。

### 编译单个测试

```bash
mkdir -p build/tests
g++ -std=c++14 -pthread \\
    -I ./behavioral/observer/include \\
    behavioral/observer/tests/test_observer_pattern.cpp \\
    -o build/tests/behavioral/observer/test_observer_pattern
./build/tests/behavioral/observer/test_observer_pattern
```

### 使用模式

```cpp
#include "singleton_patterns.h"           // 编译时添加 -I ./creational/singleton/include
#include "adapter_patterns.h"             // -I ./structural/adapter/include
#include "observer_patterns.h"            // -I ./behavioral/observer/include

auto& instance = SingletonClass::getInstance();

auto subject = std::make_shared<ConcreteSubject>();
auto observer = std::make_shared<ConcreteObserver>();
subject->attach(observer);
```

### 实战示例

订单结算流水线示例（策略 + 工厂 + 观察者 + 命令）：

```bash
mkdir -p build/practical
g++ -std=c++14 -pthread \\
    -I ./behavioral/observer/include \\
    -I ./behavioral/strategy/include \\
    examples/practical_applications/order_checkout/order_checkout.cpp \\
    -o build/practical/order_checkout
./build/practical/order_checkout
```

## 🧪 运行测试

- 本地：推荐执行 `./scripts/run_tests.sh`，它会自动发现所有 `test_*.cpp` 并运行。
- CI：GitHub Actions (`.github/workflows/pattern-tests.yml`) 会调用同一个脚本，保持本地与 CI 一致。

## 📚 文档

- 中文索引：`docs/README.zh-cn.md`
- 性能与内存分析：`docs/performance-and-memory-analysis.md`
- 每个模式的文档位于对应目录的 `docs/` 子目录，包含中英文说明，例如 `creational/abstract_factory/docs/`。
- 历史概览和旧结构文档保留在 `docs/legacy/`。

## 🔧 开发指南

### 添加新模式

1. 在对应分类下创建模式目录：`<category>/<pattern>/{include,tests,docs,examples}`。
2. 将头文件放在 `include/`，测试命名为 `test_*.cpp` 放在 `tests/`（CI 会自动发现）。
3. 在 `docs/` 中补充 README（可含中英文），并根据需要添加示例到 `examples/`。
4. 更新本 README 中的分类表，确保路径正确。

### 代码规范

- 使用 C++14 标准
- 头文件使用 include guards
- 遵循现有代码的命名约定
- 每个模式独立，避免跨模式耦合
- 提供完整的测试用例

## 🤝 贡献

欢迎贡献！请查看 [分支保护指南](.github/BRANCH_PROTECTION_GUIDE.md) 了解如何提交 PR。

### 贡献流程

1. Fork 本仓库
2. 创建功能分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 创建 Pull Request
6. 等待 CI 测试通过
7. 代码审查后合并

## 📖 参考资料

- [Design Patterns: Elements of Reusable Object-Oriented Software](https://en.wikipedia.org/wiki/Design_Patterns) - GoF 经典书籍
- [C++ 设计模式](https://refactoringguru.cn/design-patterns/cpp) - 在线教程
- [现代 C++ 设计模式](https://github.com/topics/design-patterns-cpp) - GitHub 资源

## 📝 许可证

本项目采用 MIT 许可证。详见 LICENSE 文件。

## ⭐ Star History

如果这个项目对你有帮助，请给个 Star ⭐️！
