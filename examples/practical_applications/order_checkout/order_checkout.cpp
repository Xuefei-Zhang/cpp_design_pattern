#include <iomanip>
#include <iostream>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "behavioral/observer_patterns.h"
#include "behavioral/strategy_patterns.h"

struct OrderItem {
    std::string name;
    double unitPrice;
    int quantity;
};

struct OrderTotals {
    double subtotal{0.0};
    double discounted{0.0};
    double tax{0.0};
    double finalTotal{0.0};
};

std::string formatMoney(double value) {
    std::ostringstream out;
    out << std::fixed << std::setprecision(2) << value;
    return out.str();
}

double calculateSubtotal(const std::vector<OrderItem>& items) {
    return std::accumulate(
        items.begin(), items.end(), 0.0,
        [](double sum, const OrderItem& item) { return sum + item.unitPrice * item.quantity; });
}

class PaymentProcessor {
public:
    virtual ~PaymentProcessor() = default;
    virtual bool pay(double amount) = 0;
    virtual std::string name() const = 0;
};

class CreditCardProcessor : public PaymentProcessor {
public:
    bool pay(double amount) override {
        std::cout << "[Payment] Charging credit card: $" << formatMoney(amount) << std::endl;
        return true;
    }

    std::string name() const override { return "credit_card"; }
};

class WeChatPayProcessor : public PaymentProcessor {
public:
    bool pay(double amount) override {
        std::cout << "[Payment] WeChat Pay processed: ¥" << formatMoney(amount) << std::endl;
        return true;
    }

    std::string name() const override { return "wechat_pay"; }
};

class BankTransferProcessor : public PaymentProcessor {
public:
    bool pay(double amount) override {
        std::cout << "[Payment] Bank transfer scheduled: $" << formatMoney(amount) << std::endl;
        return true;
    }

    std::string name() const override { return "bank_transfer"; }
};

class PaymentProcessorFactory {
public:
    static std::shared_ptr<PaymentProcessor> create(const std::string& channel) {
        if (channel == "wechat" || channel == "wechat_pay") {
            return std::make_shared<WeChatPayProcessor>();
        }
        if (channel == "bank") {
            return std::make_shared<BankTransferProcessor>();
        }
        return std::make_shared<CreditCardProcessor>();
    }
};

class OrderEventBus : public ThreadSafeSubject {
public:
    void emit(const std::string& event, const std::string& payload) {
        notifyObservers(event, payload);
    }
};

class AuditLogger : public Observer {
public:
    void update(const Subject&, const std::string& eventName, const std::string& payload) override {
        std::cout << "[Audit] " << eventName << " -> " << payload << std::endl;
    }
};

class MetricsCollector : public Observer {
private:
    std::size_t successCount_{0};
    std::size_t failureCount_{0};

public:
    void update(const Subject&, const std::string& eventName, const std::string&) override {
        if (eventName.find("failure") != std::string::npos) {
            ++failureCount_;
        } else if (eventName.find("success") != std::string::npos) {
            ++successCount_;
        }
    }

    void print() const {
        std::cout << "[Metrics] success=" << successCount_ << ", failure=" << failureCount_
                  << std::endl;
    }
};

class WorkflowCommand {
public:
    virtual ~WorkflowCommand() = default;
    virtual bool execute() = 0;
    virtual std::string name() const = 0;
};

class ReserveInventoryCommand : public WorkflowCommand {
private:
    const std::vector<OrderItem>& items_;
    OrderEventBus& events_;

public:
    ReserveInventoryCommand(const std::vector<OrderItem>& items, OrderEventBus& events)
        : items_(items), events_(events) {}

    bool execute() override {
        events_.emit("inventory.reserved", "items=" + std::to_string(items_.size()));
        return true;
    }

    std::string name() const override { return "ReserveInventory"; }
};

class ChargePaymentCommand : public WorkflowCommand {
private:
    std::shared_ptr<PaymentProcessor> processor_;
    double amount_;
    OrderEventBus& events_;

public:
    ChargePaymentCommand(std::shared_ptr<PaymentProcessor> processor,
                         double amount,
                         OrderEventBus& events)
        : processor_(std::move(processor)), amount_(amount), events_(events) {}

    bool execute() override {
        const bool ok = processor_->pay(amount_);
        events_.emit(ok ? "payment.success" : "payment.failure",
                     processor_->name() + " amount=" + formatMoney(amount_));
        return ok;
    }

    std::string name() const override { return "ChargePayment"; }
};

class EmitEventCommand : public WorkflowCommand {
private:
    OrderEventBus& events_;
    std::string event_;
    std::string payload_;

public:
    EmitEventCommand(OrderEventBus& events, std::string event, std::string payload)
        : events_(events), event_(std::move(event)), payload_(std::move(payload)) {}

    bool execute() override {
        events_.emit(event_, payload_);
        return true;
    }

    std::string name() const override { return "EmitEvent"; }
};

class WorkflowPipeline {
private:
    std::vector<std::unique_ptr<WorkflowCommand>> steps_;

public:
    void addStep(std::unique_ptr<WorkflowCommand> cmd) {
        steps_.push_back(std::move(cmd));
    }

    bool run() {
        for (auto& step : steps_) {
            std::cout << "[Workflow] " << step->name() << std::endl;
            if (!step->execute()) {
                std::cout << "[Workflow] aborted at step: " << step->name() << std::endl;
                return false;
            }
        }
        return true;
    }
};

std::string summarizeItems(const std::vector<OrderItem>& items) {
    std::ostringstream out;
    for (std::size_t i = 0; i < items.size(); ++i) {
        if (i > 0) {
            out << ", ";
        }
        out << items[i].name << "x" << items[i].quantity;
    }
    return out.str();
}

int main() {
    std::vector<OrderItem> items{
        {"显示器", 1299.0, 1},
        {"机械键盘", 499.0, 1},
        {"Type-C 线缆", 39.0, 2},
    };

    PricingContext pricing(std::make_unique<TieredPricingStrategy>(1500.0, 5.0, 12.0));
    const bool hasCoupon = true;
    std::string pricingTrace = pricing.getStrategyName();

    OrderTotals totals;
    totals.subtotal = calculateSubtotal(items);
    totals.discounted = pricing.execute(totals.subtotal);

    if (hasCoupon) {
        pricing.setStrategy(std::make_unique<FlatDiscountStrategy>(80.0));
        totals.discounted = pricing.execute(totals.discounted);
        pricingTrace += " -> " + pricing.getStrategyName();
    }

    totals.tax = totals.discounted * 0.06;
    totals.finalTotal = totals.discounted + totals.tax;

    OrderEventBus eventBus;
    auto audit = std::make_shared<AuditLogger>();
    auto metrics = std::make_shared<MetricsCollector>();
    eventBus.attachObserver(audit);
    eventBus.attachObserver(metrics);

    const std::string payChannel = "wechat";
    std::shared_ptr<PaymentProcessor> processor = PaymentProcessorFactory::create(payChannel);

    WorkflowPipeline pipeline;
    pipeline.addStep(std::make_unique<ReserveInventoryCommand>(items, eventBus));
    pipeline.addStep(
        std::make_unique<ChargePaymentCommand>(processor, totals.finalTotal, eventBus));
    pipeline.addStep(std::make_unique<EmitEventCommand>(
        eventBus, "order.confirmed",
        "items=" + summarizeItems(items) + ", total=" + formatMoney(totals.finalTotal)));
    pipeline.addStep(std::make_unique<EmitEventCommand>(
        eventBus, "notification.email", "order ready, channel=" + processor->name()));

    std::cout << "=== 订单结算示例 ===" << std::endl;
    std::cout << "商品清单: " << summarizeItems(items) << std::endl;
    std::cout << "定价策略: " << pricingTrace << std::endl;
    std::cout << "小计: $" << formatMoney(totals.subtotal)
              << " → 优惠后: $" << formatMoney(totals.discounted)
              << " → 含税总额: $" << formatMoney(totals.finalTotal) << std::endl;
    std::cout << "支付渠道: " << processor->name() << std::endl;

    const bool ok = pipeline.run();
    std::cout << "流水线执行结果: " << (ok ? "成功" : "失败") << std::endl;
    metrics->print();
    return 0;
}
