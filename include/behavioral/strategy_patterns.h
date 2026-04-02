#ifndef STRATEGY_PATTERNS_H
#define STRATEGY_PATTERNS_H

#include <algorithm>
#include <cctype>
#include <cmath>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

class SortingStrategy {
public:
    virtual ~SortingStrategy() = default;
    virtual void sort(std::vector<int>& data) const = 0;
    virtual std::string getName() const = 0;
};

class BubbleSortStrategy : public SortingStrategy {
public:
    void sort(std::vector<int>& data) const override {
        if (data.size() < 2) {
            return;
        }

        for (std::size_t i = 0; i < data.size() - 1; ++i) {
            bool swapped = false;
            for (std::size_t j = 0; j < data.size() - 1 - i; ++j) {
                if (data[j] > data[j + 1]) {
                    std::swap(data[j], data[j + 1]);
                    swapped = true;
                }
            }
            if (!swapped) {
                break;
            }
        }
    }

    std::string getName() const override {
        return "BubbleSort";
    }
};

class QuickSortStrategy : public SortingStrategy {
private:
    static int partition(std::vector<int>& data, int low, int high) {
        int pivot = data[high];
        int i = low - 1;

        for (int j = low; j < high; ++j) {
            if (data[j] <= pivot) {
                ++i;
                std::swap(data[i], data[j]);
            }
        }

        std::swap(data[i + 1], data[high]);
        return i + 1;
    }

    static void quickSort(std::vector<int>& data, int low, int high) {
        if (low >= high) {
            return;
        }

        const int pivotIndex = partition(data, low, high);
        quickSort(data, low, pivotIndex - 1);
        quickSort(data, pivotIndex + 1, high);
    }

public:
    void sort(std::vector<int>& data) const override {
        if (data.empty()) {
            return;
        }

        quickSort(data, 0, static_cast<int>(data.size()) - 1);
    }

    std::string getName() const override {
        return "QuickSort";
    }
};

class MergeSortStrategy : public SortingStrategy {
private:
    static void merge(std::vector<int>& data, int left, int mid, int right) {
        std::vector<int> leftPart(data.begin() + left, data.begin() + mid + 1);
        std::vector<int> rightPart(data.begin() + mid + 1, data.begin() + right + 1);

        std::size_t i = 0;
        std::size_t j = 0;
        int k = left;

        while (i < leftPart.size() && j < rightPart.size()) {
            if (leftPart[i] <= rightPart[j]) {
                data[k++] = leftPart[i++];
            } else {
                data[k++] = rightPart[j++];
            }
        }

        while (i < leftPart.size()) {
            data[k++] = leftPart[i++];
        }

        while (j < rightPart.size()) {
            data[k++] = rightPart[j++];
        }
    }

    static void mergeSort(std::vector<int>& data, int left, int right) {
        if (left >= right) {
            return;
        }

        const int mid = left + (right - left) / 2;
        mergeSort(data, left, mid);
        mergeSort(data, mid + 1, right);
        merge(data, left, mid, right);
    }

public:
    void sort(std::vector<int>& data) const override {
        if (data.empty()) {
            return;
        }

        mergeSort(data, 0, static_cast<int>(data.size()) - 1);
    }

    std::string getName() const override {
        return "MergeSort";
    }
};

class SortContext {
private:
    std::unique_ptr<SortingStrategy> strategy_;
    std::vector<int> lastInput_;
    std::vector<int> lastOutput_;

public:
    explicit SortContext(std::unique_ptr<SortingStrategy> strategy)
        : strategy_(std::move(strategy)) {}

    void setStrategy(std::unique_ptr<SortingStrategy> strategy) {
        strategy_ = std::move(strategy);
    }

    std::vector<int> execute(std::vector<int> data) {
        lastInput_ = data;
        if (strategy_) {
            strategy_->sort(data);
        }
        lastOutput_ = data;
        return data;
    }

    std::string getStrategyName() const {
        return strategy_ ? strategy_->getName() : "NoSortingStrategy";
    }

    const std::vector<int>& getLastInput() const { return lastInput_; }
    const std::vector<int>& getLastOutput() const { return lastOutput_; }
};

class PricingStrategy {
public:
    virtual ~PricingStrategy() = default;
    virtual double calculate(double basePrice) const = 0;
    virtual std::string getName() const = 0;
};

class RegularPricingStrategy : public PricingStrategy {
public:
    double calculate(double basePrice) const override {
        return basePrice < 0.0 ? 0.0 : basePrice;
    }

    std::string getName() const override {
        return "RegularPricing";
    }
};

class PercentageDiscountStrategy : public PricingStrategy {
private:
    double percent_;

public:
    explicit PercentageDiscountStrategy(double percent)
        : percent_(percent < 0.0 ? 0.0 : (percent > 100.0 ? 100.0 : percent)) {}

    double calculate(double basePrice) const override {
        const double safePrice = basePrice < 0.0 ? 0.0 : basePrice;
        return safePrice * (1.0 - percent_ / 100.0);
    }

    std::string getName() const override {
        return "PercentageDiscount";
    }
};

class FlatDiscountStrategy : public PricingStrategy {
private:
    double discount_;

public:
    explicit FlatDiscountStrategy(double discount)
        : discount_(discount < 0.0 ? 0.0 : discount) {}

    double calculate(double basePrice) const override {
        const double safePrice = basePrice < 0.0 ? 0.0 : basePrice;
        return std::max(0.0, safePrice - discount_);
    }

    std::string getName() const override {
        return "FlatDiscount";
    }
};

class TieredPricingStrategy : public PricingStrategy {
private:
    double threshold_;
    double lowPercent_;
    double highPercent_;

public:
    TieredPricingStrategy(double threshold, double lowPercent, double highPercent)
        : threshold_(threshold < 0.0 ? 0.0 : threshold),
          lowPercent_(lowPercent < 0.0 ? 0.0 : (lowPercent > 100.0 ? 100.0 : lowPercent)),
          highPercent_(highPercent < 0.0 ? 0.0 : (highPercent > 100.0 ? 100.0 : highPercent)) {}

    double calculate(double basePrice) const override {
        const double safePrice = basePrice < 0.0 ? 0.0 : basePrice;
        const double applied = (safePrice >= threshold_) ? highPercent_ : lowPercent_;
        return safePrice * (1.0 - applied / 100.0);
    }

    std::string getName() const override {
        return "TieredPricing";
    }
};

class PricingContext {
private:
    std::unique_ptr<PricingStrategy> strategy_;
    double lastBasePrice_;
    double lastFinalPrice_;

public:
    explicit PricingContext(std::unique_ptr<PricingStrategy> strategy)
        : strategy_(std::move(strategy)), lastBasePrice_(0.0), lastFinalPrice_(0.0) {}

    void setStrategy(std::unique_ptr<PricingStrategy> strategy) {
        strategy_ = std::move(strategy);
    }

    double execute(double basePrice) {
        lastBasePrice_ = basePrice;
        lastFinalPrice_ = strategy_ ? strategy_->calculate(basePrice) : basePrice;
        return lastFinalPrice_;
    }

    std::string getStrategyName() const {
        return strategy_ ? strategy_->getName() : "NoPricingStrategy";
    }

    double getLastBasePrice() const { return lastBasePrice_; }
    double getLastFinalPrice() const { return lastFinalPrice_; }
};

class CompressionStrategy {
public:
    virtual ~CompressionStrategy() = default;
    virtual std::string compress(const std::string& input) const = 0;
    virtual std::string getName() const = 0;
};

class IdentityCompressionStrategy : public CompressionStrategy {
public:
    std::string compress(const std::string& input) const override {
        return input;
    }

    std::string getName() const override {
        return "IdentityCompression";
    }
};

class RunLengthCompressionStrategy : public CompressionStrategy {
public:
    std::string compress(const std::string& input) const override {
        if (input.empty()) {
            return "";
        }

        std::string result;
        char current = input[0];
        int count = 1;

        for (std::size_t i = 1; i < input.size(); ++i) {
            if (input[i] == current) {
                ++count;
            } else {
                result.push_back(current);
                result += std::to_string(count);
                current = input[i];
                count = 1;
            }
        }

        result.push_back(current);
        result += std::to_string(count);
        return result;
    }

    std::string getName() const override {
        return "RunLengthCompression";
    }
};

class VowelRemovalCompressionStrategy : public CompressionStrategy {
private:
    static bool isVowel(char ch) {
        const char lower = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
        return lower == 'a' || lower == 'e' || lower == 'i' || lower == 'o' || lower == 'u';
    }

public:
    std::string compress(const std::string& input) const override {
        std::string result;
        result.reserve(input.size());

        for (char ch : input) {
            if (!isVowel(ch)) {
                result.push_back(ch);
            }
        }
        return result;
    }

    std::string getName() const override {
        return "VowelRemovalCompression";
    }
};

class CompressionContext {
private:
    std::unique_ptr<CompressionStrategy> strategy_;
    std::string lastOriginal_;
    std::string lastCompressed_;

public:
    explicit CompressionContext(std::unique_ptr<CompressionStrategy> strategy)
        : strategy_(std::move(strategy)) {}

    void setStrategy(std::unique_ptr<CompressionStrategy> strategy) {
        strategy_ = std::move(strategy);
    }

    std::string execute(const std::string& input) {
        lastOriginal_ = input;
        lastCompressed_ = strategy_ ? strategy_->compress(input) : input;
        return lastCompressed_;
    }

    std::string getStrategyName() const {
        return strategy_ ? strategy_->getName() : "NoCompressionStrategy";
    }

    const std::string& getLastOriginal() const { return lastOriginal_; }
    const std::string& getLastCompressed() const { return lastCompressed_; }

    double getLastCompressionRatio() const {
        if (lastOriginal_.empty()) {
            return 1.0;
        }
        return static_cast<double>(lastCompressed_.size()) /
               static_cast<double>(lastOriginal_.size());
    }
};

struct PaymentResult {
    bool success;
    double requestedAmount;
    double fee;
    double chargedAmount;
    std::string provider;
    std::string message;
};

class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;
    virtual PaymentResult pay(double amount) const = 0;
    virtual std::string getName() const = 0;
};

class CreditCardPaymentStrategy : public PaymentStrategy {
public:
    PaymentResult pay(double amount) const override {
        const double safeAmount = amount < 0.0 ? 0.0 : amount;
        const double fee = safeAmount * 0.02;  // 2% processing fee
        return {safeAmount > 0.0, safeAmount, fee, safeAmount + fee,
                "CreditCard", "Credit card payment processed"};
    }

    std::string getName() const override {
        return "CreditCardPayment";
    }
};

class PayPalPaymentStrategy : public PaymentStrategy {
public:
    PaymentResult pay(double amount) const override {
        const double safeAmount = amount < 0.0 ? 0.0 : amount;
        const double fee = safeAmount * 0.03 + 0.30;  // Typical PayPal-like formula
        return {safeAmount > 0.0, safeAmount, fee, safeAmount + fee,
                "PayPal", "PayPal payment processed"};
    }

    std::string getName() const override {
        return "PayPalPayment";
    }
};

class CryptoPaymentStrategy : public PaymentStrategy {
public:
    PaymentResult pay(double amount) const override {
        const double safeAmount = amount < 0.0 ? 0.0 : amount;
        const double fee = safeAmount * 0.01;  // Lower fee
        return {safeAmount > 0.0, safeAmount, fee, safeAmount + fee,
                "Crypto", "Crypto payment processed"};
    }

    std::string getName() const override {
        return "CryptoPayment";
    }
};

class PaymentContext {
private:
    std::unique_ptr<PaymentStrategy> strategy_;
    PaymentResult lastResult_;

public:
    explicit PaymentContext(std::unique_ptr<PaymentStrategy> strategy)
        : strategy_(std::move(strategy)), lastResult_{false, 0.0, 0.0, 0.0, "None", "No payment executed"} {}

    void setStrategy(std::unique_ptr<PaymentStrategy> strategy) {
        strategy_ = std::move(strategy);
    }

    PaymentResult execute(double amount) {
        if (!strategy_) {
            lastResult_ = {false, amount, 0.0, amount, "None", "No payment strategy set"};
            return lastResult_;
        }

        lastResult_ = strategy_->pay(amount);
        return lastResult_;
    }

    std::string getStrategyName() const {
        return strategy_ ? strategy_->getName() : "NoPaymentStrategy";
    }

    const PaymentResult& getLastResult() const { return lastResult_; }
};

// Inline usage examples (educational comments)
// Example 1 (sorting):
// SortContext sortCtx(std::make_unique<QuickSortStrategy>());
// auto sorted = sortCtx.execute({5, 1, 4, 2, 8});
// sortCtx.setStrategy(std::make_unique<MergeSortStrategy>());  // runtime switch
//
// Example 2 (pricing):
// PricingContext pricingCtx(std::make_unique<RegularPricingStrategy>());
// double price = pricingCtx.execute(120.0);
// pricingCtx.setStrategy(std::make_unique<PercentageDiscountStrategy>(15.0));
//
// Example 3 (compression):
// CompressionContext compCtx(std::make_unique<RunLengthCompressionStrategy>());
// std::string packed = compCtx.execute("aaabcccc");
//
// Example 4 (payment):
// PaymentContext payCtx(std::make_unique<CreditCardPaymentStrategy>());
// PaymentResult result = payCtx.execute(99.0);

#endif // STRATEGY_PATTERNS_H
