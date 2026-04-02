#!/bin/bash

# Script to generate README files for all design patterns
# This creates comprehensive documentation for each pattern

# Pattern definitions with Chinese and English names
declare -A CREATIONAL_PATTERNS=(
    ["singleton"]="单例模式|Singleton Pattern|确保一个类只有一个实例，并提供全局访问点"
    ["factory_method"]="工厂方法模式|Factory Method Pattern|定义创建对象的接口，让子类决定实例化哪个类"
    ["abstract_factory"]="抽象工厂模式|Abstract Factory Pattern|提供创建相关或依赖对象族的接口"
    ["builder"]="建造者模式|Builder Pattern|将复杂对象的构建与表示分离"
    ["prototype"]="原型模式|Prototype Pattern|通过复制原型实例创建新对象"
)

declare -A STRUCTURAL_PATTERNS=(
    ["adapter"]="适配器模式|Adapter Pattern|将一个类的接口转换成客户期望的另一个接口"
    ["bridge"]="桥接模式|Bridge Pattern|将抽象部分与实现部分分离"
    ["composite"]="组合模式|Composite Pattern|将对象组合成树形结构表示部分-整体层次"
    ["decorator"]="装饰器模式|Decorator Pattern|动态地给对象添加额外职责"
    ["facade"]="外观模式|Facade Pattern|为子系统提供统一的高层接口"
    ["flyweight"]="享元模式|Flyweight Pattern|运用共享技术有效支持大量细粒度对象"
    ["proxy"]="代理模式|Proxy Pattern|为其他对象提供一种代理以控制对这个对象的访问"
)

declare -A BEHAVIORAL_PATTERNS=(
    ["chain_of_responsibility"]="责任链模式|Chain of Responsibility Pattern|沿着链传递请求，直到有对象处理它"
    ["command"]="命令模式|Command Pattern|将请求封装成对象"
    ["interpreter"]="解释器模式|Interpreter Pattern|定义语法的表示和解释器"
    ["iterator"]="迭代器模式|Iterator Pattern|提供一种方法顺序访问聚合对象中的元素"
    ["mediator"]="中介者模式|Mediator Pattern|用中介对象来封装对象间的交互"
    ["memento"]="备忘录模式|Memento Pattern|在不破坏封装的前提下捕获对象内部状态"
    ["observer"]="观察者模式|Observer Pattern|定义对象间的一对多依赖关系"
    ["state"]="状态模式|State Pattern|允许对象在内部状态改变时改变行为"
    ["strategy"]="策略模式|Strategy Pattern|定义一系列算法，使它们可以相互替换"
    ["template_method"]="模板方法模式|Template Method Pattern|定义算法骨架，延迟部分步骤到子类"
    ["visitor"]="访问者模式|Visitor Pattern|在不改变元素类的前提下定义作用于元素的新操作"
)

# Function to generate README for a pattern
generate_readme() {
    local pattern_key="$1"
    local category="$2"
    local pattern_info="$3"

    IFS='|' read -r cn_name en_name description <<< "$pattern_info"

    local header_file=""
    if [ "$category" = "creational" ]; then
        header_file="include/creational/${pattern_key}_*.h"
    elif [ "$category" = "structural" ]; then
        header_file="include/structural/${pattern_key}_*.h"
    elif [ "$category" = "behavioral" ]; then
        header_file="include/behavioral/${pattern_key}_*.h"
    fi

    cat > "patterns/$category/$pattern_key/README.md" <<EOF
# $cn_name ($en_name)

## 概述

$description

## 实现

头文件位置: \`$header_file\`

## 测试用例

本目录包含以下测试用例：

1. **test_basic.cpp** - 基础功能测试
2. **test_advanced.cpp** - 高级特性测试
3. **test_edge_cases.cpp** - 边界条件测试
4. **test_real_world.cpp** - 实际应用示例

## 编译和运行

\`\`\`bash
# 基础测试
g++ -std=c++14 -I../../../include test_basic.cpp -o test_basic
./test_basic

# 高级测试
g++ -std=c++14 -I../../../include test_advanced.cpp -o test_advanced
./test_advanced

# 如果需要线程支持
g++ -std=c++14 -I../../../include -pthread test_thread_safety.cpp -o test_thread_safety
./test_thread_safety
\`\`\`

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
- 项目主README: ../../../README.md
EOF

    echo "Generated README for $category/$pattern_key"
}

# Create directories and READMEs
echo "Generating README files for all design patterns..."

# Creational patterns
for pattern in "${!CREATIONAL_PATTERNS[@]}"; do
    mkdir -p "patterns/creational/$pattern"
    generate_readme "$pattern" "creational" "${CREATIONAL_PATTERNS[$pattern]}"
done

# Structural patterns
for pattern in "${!STRUCTURAL_PATTERNS[@]}"; do
    mkdir -p "patterns/structural/$pattern"
    generate_readme "$pattern" "structural" "${STRUCTURAL_PATTERNS[$pattern]}"
done

# Behavioral patterns
for pattern in "${!BEHAVIORAL_PATTERNS[@]}"; do
    mkdir -p "patterns/behavioral/$pattern"
    generate_readme "$pattern" "behavioral" "${BEHAVIORAL_PATTERNS[$pattern]}"
done

echo "README generation complete!"
echo "Total patterns documented: $((${#CREATIONAL_PATTERNS[@]} + ${#STRUCTURAL_PATTERNS[@]} + ${#BEHAVIORAL_PATTERNS[@]}))"
