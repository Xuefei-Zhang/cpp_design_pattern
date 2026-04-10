# 中介者模式 (Mediator Pattern)

## 概述

用中介对象来封装对象间的交互

## 实现

头文件位置: `include/behavioral/mediator_*.h`

## 测试用例

本目录包含以下测试用例：

1. **test_basic.cpp** - 基础功能测试
2. **test_advanced.cpp** - 高级特性测试
3. **test_edge_cases.cpp** - 边界条件测试
4. **test_real_world.cpp** - 实际应用示例

## 编译和运行

```bash
# 基础测试
g++ -std=c++14 -I../../../include test_basic.cpp -o test_basic
./test_basic

# 高级测试
g++ -std=c++14 -I../../../include test_advanced.cpp -o test_advanced
./test_advanced

# 如果需要线程支持
g++ -std=c++14 -I../../../include -pthread test_thread_safety.cpp -o test_thread_safety
./test_thread_safety
```

## 关键特性

### 优点
- 待完善：根据具体模式填写

### 缺点
- 待完善：根据具体模式填写

## 使用场景

- 待完善：根据具体模式填写

## 最佳实践

1. 遵循SOLID原则
2. 保持代码简洁
3. 注意线程安全（如适用）
4. 合理使用设计模式，避免过度设计

## 相关模式

- 待完善：列出相关的设计模式

## 参考资料

- GoF《设计模式》
- 项目主README: ../../../docs/README.md
