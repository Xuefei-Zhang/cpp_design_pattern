# 设计模式测试套件

本目录包含所有设计模式的完整测试用例和文档。每个设计模式都有独立的子目录，包含：

- README.md - 模式的详细说明
- 多个测试文件 - 覆盖不同方面的测试用例

## 目录结构

```
patterns/
├── creational/          # 创建型模式 (5个)
│   ├── singleton/
│   ├── factory_method/
│   ├── abstract_factory/
│   ├── builder/
│   └── prototype/
├── structural/          # 结构型模式 (7个)
│   ├── adapter/
│   ├── bridge/
│   ├── composite/
│   ├── decorator/
│   ├── facade/
│   ├── flyweight/
│   └── proxy/
└── behavioral/          # 行为型模式 (11个)
    ├── chain_of_responsibility/
    ├── command/
    ├── interpreter/
    ├── iterator/
    ├── mediator/
    ├── memento/
    ├── observer/
    ├── state/
    ├── strategy/
    ├── template_method/
    └── visitor/
```

## 测试类型

每个模式通常包含以下类型的测试：

1. **test_basic.cpp** - 基础功能测试，验证核心行为
2. **test_thread_safety.cpp** - 线程安全测试（适用的模式）
3. **test_performance.cpp** - 性能测试和基准测试
4. **test_edge_cases.cpp** - 边界条件和异常情况测试
5. **test_real_world.cpp** - 真实场景应用示例

## 编译所有测试

### 使用提供的脚本

```bash
# 编译所有测试
./build_all_tests.sh

# 编译特定类别
./build_all_tests.sh creational
./build_all_tests.sh structural
./build_all_tests.sh behavioral

# 编译特定模式
./build_all_tests.sh creational/singleton
```

### 手动编译

```bash
# 编译单个测试
g++ -std=c++14 -I../../include creational/singleton/test_basic.cpp -o test_singleton_basic

# 需要线程支持的测试
g++ -std=c++14 -I../../include -pthread creational/singleton/test_thread_safety.cpp -o test_singleton_thread
```

## 运行所有测试

```bash
# 运行所有测试
./run_all_tests.sh

# 运行特定类别的测试
./run_all_tests.sh creational
```

## 测试覆盖率

每个模式的测试用例覆盖：

- ✓ 基本功能验证
- ✓ 多个使用场景
- ✓ 边界条件处理
- ✓ 错误处理
- ✓ 性能特性
- ✓ 线程安全（如适用）
- ✓ 内存管理

## 设计模式快速参考

### 创建型模式

| 模式 | 用途 | 关键特性 |
|------|------|----------|
| Singleton | 确保单例 | 全局访问点 |
| Factory Method | 创建对象接口 | 延迟到子类 |
| Abstract Factory | 创建相关对象族 | 产品族一致性 |
| Builder | 分步构建复杂对象 | 分离构建与表示 |
| Prototype | 克隆原型 | 避免昂贵创建 |

### 结构型模式

| 模式 | 用途 | 关键特性 |
|------|------|----------|
| Adapter | 接口转换 | 兼容不兼容接口 |
| Bridge | 分离抽象和实现 | 独立变化 |
| Composite | 树形结构 | 统一对待个体和组合 |
| Decorator | 动态添加职责 | 灵活扩展 |
| Facade | 简化接口 | 统一复杂子系统 |
| Flyweight | 共享细粒度对象 | 节省内存 |
| Proxy | 控制访问 | 延迟加载、访问控制 |

### 行为型模式

| 模式 | 用途 | 关键特性 |
|------|------|----------|
| Chain of Responsibility | 请求链传递 | 解耦发送者和接收者 |
| Command | 请求对象化 | 支持撤销/重做 |
| Interpreter | 解释语法 | 简单语言实现 |
| Iterator | 顺序访问 | 不暴露内部结构 |
| Mediator | 中介协调 | 减少对象间耦合 |
| Memento | 保存/恢复状态 | 不破坏封装 |
| Observer | 发布-订阅 | 一对多依赖 |
| State | 状态改变行为 | 状态对象化 |
| Strategy | 算法族 | 算法可互换 |
| Template Method | 算法骨架 | 子类重定义步骤 |
| Visitor | 操作与结构分离 | 易于添加新操作 |

## 学习路径建议

### 初学者
1. 从 Singleton 和 Factory Method 开始
2. 学习 Observer 和 Strategy
3. 掌握 Decorator 和 Adapter

### 中级
1. 深入 Abstract Factory 和 Builder
2. 理解 Composite 和 Proxy
3. 学习 Command 和 State

### 高级
1. 掌握 Visitor 和 Interpreter
2. 理解 Mediator 和 Memento
3. 研究模式组合应用

## 贡献

欢迎贡献更多测试用例！请确保：

1. 测试代码清晰易读
2. 包含详细的注释
3. 覆盖边界条件
4. 遵循现有代码风格
5. 更新相应的 README

## 参考资料

- 《设计模式：可复用面向对象软件的基础》
- 《Head First 设计模式》
- C++ 标准库中的设计模式应用
