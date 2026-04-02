# 订单结算与履约示例

用一个常见的电商结算流程展示多种设计模式如何协同：定价策略 → 支付渠道选择 → 工作流执行 → 事件通知与审计。

## 涉及的模式

- 策略模式：`PricingContext` 动态切换定价/促销策略（阶梯折扣、满减等）。
- 工厂方法：`PaymentProcessorFactory` 按配置选择支付渠道实现，新增渠道不影响调用方。
- 观察者：`OrderEventBus` 向审计与指标采集器推送事件，消费端与业务解耦。
- 命令模式：`WorkflowPipeline` 将库存预占、收款、通知等步骤串成可维护的流水线。

## 运行示例

```bash
mkdir -p build/practical
g++ -std=c++14 -pthread -I./include examples/practical_applications/order_checkout/order_checkout.cpp -o build/practical/order_checkout
./build/practical/order_checkout
```

输出会展示价格计算、支付选择、事件通知与流水线执行顺序。

## 可以这样扩展

- 新增促销或税费策略：实现新的 `PricingStrategy`，在主流程里 `pricing.setStrategy(...)` 动态切换。
- 接入更多支付渠道：实现 `PaymentProcessor` 并在工厂注册，无需改动业务代码。
- 丰富事件监听：实现 `Observer`（如风控、埋点、延迟队列），通过 `eventBus.attachObserver` 挂载。
- 调整工作流：在 `WorkflowPipeline` 里新增/重排命令（如风控审核、库存校验、补偿任务）。
