# C++ 设计模式实现库

本仓库包含 23 个 GoF 设计模式的完整 C++ 实现，以及实用的工具类和示例程序。

## 📁 项目结构

```
cpp_design_pattern/
├── include/                    # 头文件目录
│   ├── creational/            # 创建型模式
│   ├── structural/            # 结构型模式
│   ├── behavioral/            # 行为型模式
│   └── utilities/             # 实用工具
├── tests/                      # 测试文件目录
│   ├── creational/            # 创建型模式测试
│   ├── structural/            # 结构型模式测试
│   ├── behavioral/            # 行为型模式测试
│   └── utilities/             # 实用工具测试
├── examples/                   # 示例程序
│   └── practical_applications/ # 实战场景示例（把模式用在业务流程里）
├── docs/                       # 文档
│   └── design-patterns/
│       └── zh-cn/             # 中文文档
├── .github/                    # GitHub 配置
│   ├── workflows/             # CI/CD 工作流
│   └── BRANCH_PROTECTION_GUIDE.md
└── README.md                   # 本文件
```

## 🎨 设计模式分类

### 创建型模式 (Creational Patterns) - 5个

这些模式关注对象的创建机制。

| 模式 | 头文件 | 测试文件 |
|------|--------|----------|
| **单例模式** (Singleton) | `include/creational/singleton_patterns.h` | `tests/creational/test_singleton_pattern.cpp` |
| **工厂方法** (Factory Method) | `include/creational/factory_method_shapes.h` | `tests/creational/test_factory_method.cpp` |
| **抽象工厂** (Abstract Factory) | `include/creational/abstract_factory_shapes.h` | `tests/creational/test_abstract_factory.cpp` |
| **生成器** (Builder) | `include/creational/builder_shapes.h` | `tests/creational/test_builder_pattern.cpp` |
| **原型** (Prototype) | `include/creational/prototype_shapes.h` | `tests/creational/test_prototype_pattern.cpp` |

### 结构型模式 (Structural Patterns) - 7个

这些模式关注类和对象的组合。

| 模式 | 头文件 | 测试文件 |
|------|--------|----------|
| **适配器** (Adapter) | `include/structural/adapter_patterns.h` | `tests/structural/test_adapter_pattern.cpp` |
| **桥接** (Bridge) | `include/structural/bridge_patterns.h` | `tests/structural/test_bridge_pattern.cpp` |
| **组合** (Composite) | `include/structural/composite_patterns.h` | `tests/structural/test_composite_pattern.cpp` |
| **装饰器** (Decorator) | `include/structural/decorator_patterns.h` | `tests/structural/test_decorator_pattern.cpp` |
| **外观** (Facade) | `include/structural/facade_subsystems.h` | `tests/structural/test_facade_pattern.cpp` |
| **享元** (Flyweight) | `include/structural/flyweight_patterns.h` | `tests/structural/test_flyweight_pattern_final.cpp` |
| **代理** (Proxy) | `include/structural/proxy_patterns.h` | `tests/structural/test_proxy_pattern_corrected.cpp` |

### 行为型模式 (Behavioral Patterns) - 11个

这些模式关注对象之间的通信。

| 模式 | 头文件 | 测试文件 |
|------|--------|----------|
| **责任链** (Chain of Responsibility) | `include/behavioral/chain_of_responsibility_shapes_fixed.h` | `tests/behavioral/test_chain_of_responsibility_final.cpp` |
| **命令** (Command) | `include/behavioral/command_patterns.h` | `tests/behavioral/test_command_pattern_final.cpp` |
| **解释器** (Interpreter) | `include/behavioral/interpreter_patterns.h` | `tests/behavioral/test_interpreter_pattern.cpp` |
| **迭代器** (Iterator) | `include/behavioral/iterator_patterns.h` | `tests/behavioral/test_iterator_pattern.cpp` |
| **中介者** (Mediator) | `include/behavioral/mediator_patterns.h` | `tests/behavioral/test_mediator_pattern.cpp` |
| **备忘录** (Memento) | `include/behavioral/memento_patterns.h` | `tests/behavioral/test_memento_pattern.cpp` |
| **观察者** (Observer) | `include/behavioral/observer_patterns.h` | `tests/behavioral/test_observer_pattern.cpp` |
| **状态** (State) | `include/behavioral/state_patterns.h` | `tests/behavioral/test_state_pattern.cpp` |
| **策略** (Strategy) | `include/behavioral/strategy_patterns.h` | `tests/behavioral/test_strategy_pattern.cpp` |
| **模板方法** (Template Method) | `include/behavioral/template_method_patterns.h` | `tests/behavioral/test_template_method_pattern.cpp` |
| **访问者** (Visitor) | `include/behavioral/visitor_patterns.h` | `tests/behavioral/test_visitor_pattern.cpp` |

### 实用工具 (Utilities) - 6个

额外的实用模式和工具类。

| 工具 | 头文件 | 测试文件 |
|------|--------|----------|
| **内存池** (Memory Pool) | `include/utilities/memory_pool.h` | `tests/utilities/test_memory_pool.cpp` |
| **增强内存池** | `include/utilities/enhanced_memory_pool.h` | `tests/utilities/test_enhanced_memory_pool.cpp` |
| **线程池** (Thread Pool) | `include/utilities/thread_pool.h` | `tests/utilities/test_thread_pool.cpp` |
| **增强线程池** | `include/utilities/enhanced_thread_pool.h` | `tests/utilities/test_enhanced_thread_pool.cpp` |
| **友元类** (Friend Classes) | `include/utilities/friend_classes_*.h` | `tests/utilities/test_friend_classes.cpp` |
| **场景构建器** | `include/utilities/scene_builder.h` | `tests/utilities/test_scene_builder.cpp` |

## 🚀 快速开始

### 编译单个测试

```bash
# 编译创建型模式测试
g++ -std=c++14 -I./include tests/creational/test_singleton_pattern.cpp -o test_singleton
./test_singleton

# 编译结构型模式测试
g++ -std=c++14 -I./include tests/structural/test_adapter_pattern.cpp -o test_adapter
./test_adapter

# 编译行为型模式测试
g++ -std=c++14 -I./include tests/behavioral/test_observer_pattern.cpp -o test_observer
./test_observer

# 编译实用工具测试（需要线程支持）
g++ -std=c++14 -pthread -I./include tests/utilities/test_thread_pool.cpp -o test_thread_pool
./test_thread_pool
```

### 使用模式

在你的项目中使用这些模式：

```cpp
#include "creational/singleton_patterns.h"
#include "structural/adapter_patterns.h"
#include "behavioral/observer_patterns.h"

// 使用单例模式
auto& instance = SingletonClass::getInstance();

// 使用观察者模式
auto subject = std::make_shared<ConcreteSubject>();
auto observer = std::make_shared<ConcreteObserver>();
subject->attach(observer);
```

### 实战示例

订单结算流水线示例（策略 + 工厂 + 观察者 + 命令）：

```bash
mkdir -p build/practical
g++ -std=c++14 -pthread -I./include examples/practical_applications/order_checkout/order_checkout.cpp -o build/practical/order_checkout
./build/practical/order_checkout
```

## 🧪 运行测试

### 本地测试

```bash
# 测试所有创建型模式
for test in tests/creational/test_*.cpp; do
    g++ -std=c++14 -I./include "$test" -o "${test%.cpp}" && "./${test%.cpp}"
done

# 测试所有结构型模式
for test in tests/structural/test_*.cpp; do
    g++ -std=c++14 -I./include "$test" -o "${test%.cpp}" && "./${test%.cpp}"
done

# 测试所有行为型模式
for test in tests/behavioral/test_*.cpp; do
    g++ -std=c++14 -I./include "$test" -o "${test%.cpp}" && "./${test%.cpp}"
done
```

### CI/CD 测试

本项目使用 GitHub Actions 进行持续集成测试。每次提交和 PR 都会自动运行所有 29 个测试。

查看 `.github/workflows/pattern-tests.yml` 了解详细配置。

## 📚 文档

详细的中文文档位于 `docs/design-patterns/zh-cn/` 目录：

- [设计模式索引](docs/design-patterns/zh-cn/README.md)
- 每个模式都有独立的文档，包含：
  - 模式定义和使用场景
  - UML 类图和序列图
  - 代码示例和解释
  - 优缺点分析

## 🔧 开发指南

### 添加新模式

1. 在相应的 `include/` 子目录中创建头文件
2. 在相应的 `tests/` 子目录中创建测试文件
3. 更新 `.github/workflows/pattern-tests.yml` 添加新测试
4. 在 `docs/design-patterns/zh-cn/` 添加文档

### 代码规范

- 使用 C++14 标准
- 头文件使用 include guards
- 遵循现有代码的命名约定
- 每个模式应该是独立的，不依赖其他模式
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

---

**完成度**: 23/23 GoF 设计模式 (100%) ✅
