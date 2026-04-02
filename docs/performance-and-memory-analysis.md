# 性能与内存分析指南

本指南介绍如何使用 Linux 下的常用工具分析本项目中的 C++ 设计模式示例。

## 编译配置

为了获得有意义的分析结果，建议使用不同的编译器标志进行构建。

### 调试构建 (Debug Build)
用于调试和 Sanitizer 分析。
```bash
g++ -g -O0 -std=c++14 <file>.cpp -o <output>
```

如果目标程序使用线程（例如 `test_observer_pattern.cpp`、`test_thread_pool.cpp`、`test_enhanced_memory_pool.cpp`），请补上：
```bash
g++ -g -O0 -std=c++14 -pthread <file>.cpp -o <output>
```

### 性能分析构建 (Profile Build)
包含调试符号但开启优化。
```bash
g++ -g -O3 -std=c++14 <file>.cpp -o <output>
```

多线程程序同样建议加上 `-pthread`：
```bash
g++ -g -O3 -std=c++14 -pthread <file>.cpp -o <output>
```

---

## 运行时长分析

### `time`
最基础的耗时分析工具。
```bash
/usr/bin/time -v ./your_program
```
重点关注：`User time`, `System time`, `Percent of CPU this job got`, `Maximum resident set size`.

---

## CPU 与缓存行为分析

### `perf`
强大的 Linux 内核采样分析器。

1. **统计概览**：
   ```bash
   perf stat ./your_program
   ```
   查看 `instructions per cycle` (IPC) 和 `cache-misses`。

2. **记录与报告**：
   ```bash
   perf record -g ./your_program
   perf report
   ```
   查看函数级别的 CPU 占用比例及调用栈。

### `gprof`
经典的 GNU profiler。
1. 编译时加入 `-pg`：`g++ -pg -g <file>.cpp`
2. 运行程序产生 `gmon.out`。
3. 分析：`gprof ./your_program gmon.out`

---

## 内存泄漏与分配分析

### `AddressSanitizer` (ASan)
现代编译器内置的内存检测工具。
```bash
g++ -g -fsanitize=address -std=c++14 <file>.cpp -o asan_test
./asan_test
```
可检测：堆/栈越界、Use-after-free、内存泄漏。

### `valgrind --tool=memcheck`
重量级内存检测，不需要重新编译。
```bash
valgrind --leak-check=full --show-leak-kinds=all ./your_program
```

---

## 堆内存分析

### `valgrind --tool=massif`
分析堆内存随时间的使用情况。
```bash
valgrind --tool=massif ./your_program
ms_print massif.out.<pid>
```
查看内存使用峰值及对应的分配函数。

### `heaptrack`
更现代、更高效的堆内存分析工具。
```bash
heaptrack ./your_program
heaptrack --analyze heaptrack.your_program.gz
```
提供直观的可视化报告（需要 GUI）。

---

## 运行时异常检测

### `UndefinedBehaviorSanitizer` (UBSan)
检测 C++ 标准中的未定义行为。
```bash
g++ -g -fsanitize=undefined -std=c++14 <file>.cpp -o ubsan_test
./ubsan_test
```
可检测：整数溢出、解引用空指针、类型下转换错误等。

---

## Mermaid 图表指南

文档中使用标准 Mermaid 语法来可视化设计模式的结构和行为。

### 常用图表类型

1. **类图 (classDiagram)**
   - **用途**：展示模式的静态结构。
   - **适用**：所有 GoF 模式的 UML 表示。
   - **示例**：
     ```mermaid
     classDiagram
         class Target {
             <<interface>>
             +request()
         }
         class Adapter {
             +request()
         }
         Target <|-- Adapter
     ```

2. **时序图 (sequenceDiagram)**
   - **用途**：展示对象间的交互。
   - **适用**：观察者模式、命令模式等涉及复杂协作的场景。

3. **流程图 (flowchart)**
   - **用途**：展示算法逻辑。
   - **适用**：策略模式的选择逻辑、责任链的传递。

4. **状态图 (stateDiagram-v2)**
   - **用途**：展示状态机的变化。
   - **适用**：状态模式 (State Pattern)。

### 兼容性说明
本仓库的所有设计模式文档均采用标准的 Markdown 围栏代码块 (` ```mermaid `) 标记 Mermaid 图表，以确保在 GitHub 和大多数现代 Markdown 编辑器中获得最佳显示效果。
