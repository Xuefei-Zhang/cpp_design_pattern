#include "behavioral/strategy_patterns.h"

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

namespace {

struct TestStats {
    int passed = 0;
    int failed = 0;
};

bool areVectorsEqual(const std::vector<int>& lhs, const std::vector<int>& rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    }
    for (std::size_t i = 0; i < lhs.size(); ++i) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }
    return true;
}

bool areNear(double a, double b, double epsilon = 1e-9) {
    return std::fabs(a - b) <= epsilon;
}

void check(TestStats& stats, bool condition, const std::string& label) {
    if (condition) {
        ++stats.passed;
        std::cout << "[PASS] " << label << '\n';
    } else {
        ++stats.failed;
        std::cout << "[FAIL] " << label << '\n';
    }
}

void printSection(const std::string& title) {
    std::cout << "\n=== " << title << " ===\n";
}

}

int main() {
    TestStats stats;

    std::cout << "=== Strategy Pattern Demo: Multiple Strategy Families ===\n";

    printSection("1. Sorting Strategy Family");
    SortContext sortContext(std::make_unique<BubbleSortStrategy>());
    const std::vector<int> unsorted{9, 3, 7, 1, 8, 2, 6, 5, 4};
    const std::vector<int> expectedSorted{1, 2, 3, 4, 5, 6, 7, 8, 9};

    auto bubbleSorted = sortContext.execute(unsorted);
    std::cout << "Using: " << sortContext.getStrategyName() << '\n';
    check(stats, areVectorsEqual(bubbleSorted, expectedSorted), "Bubble sort sorts correctly");
    check(stats, areVectorsEqual(sortContext.getLastInput(), unsorted), "SortContext tracks last input");
    check(stats, areVectorsEqual(sortContext.getLastOutput(), expectedSorted), "SortContext tracks last output");

    sortContext.setStrategy(std::make_unique<QuickSortStrategy>());
    auto quickSorted = sortContext.execute(std::vector<int>{5, 5, 3, 2, 4, 1, 1});
    std::cout << "Switched to: " << sortContext.getStrategyName() << '\n';
    check(stats, areVectorsEqual(quickSorted, std::vector<int>({1, 1, 2, 3, 4, 5, 5})),
          "Quick sort handles duplicates");

    sortContext.setStrategy(std::make_unique<MergeSortStrategy>());
    auto mergeSorted = sortContext.execute(std::vector<int>{42});
    std::cout << "Switched to: " << sortContext.getStrategyName() << '\n';
    check(stats, areVectorsEqual(mergeSorted, std::vector<int>({42})),
          "Merge sort handles single element");

    printSection("2. Pricing Strategy Family");
    PricingContext pricingContext(std::make_unique<RegularPricingStrategy>());

    double basePrice = 200.0;
    double regular = pricingContext.execute(basePrice);
    std::cout << "Using: " << pricingContext.getStrategyName() << '\n';
    check(stats, areNear(regular, 200.0), "Regular pricing returns base price");
    check(stats, areNear(pricingContext.getLastBasePrice(), 200.0), "PricingContext stores last base price");
    check(stats, areNear(pricingContext.getLastFinalPrice(), 200.0), "PricingContext stores last final price");

    pricingContext.setStrategy(std::make_unique<PercentageDiscountStrategy>(25.0));
    double percentage = pricingContext.execute(basePrice);
    std::cout << "Switched to: " << pricingContext.getStrategyName() << '\n';
    check(stats, areNear(percentage, 150.0), "Percentage discount applies correctly");

    pricingContext.setStrategy(std::make_unique<FlatDiscountStrategy>(500.0));
    double flat = pricingContext.execute(120.0);
    std::cout << "Switched to: " << pricingContext.getStrategyName() << '\n';
    check(stats, areNear(flat, 0.0), "Flat discount never goes below zero");

    pricingContext.setStrategy(std::make_unique<TieredPricingStrategy>(100.0, 5.0, 20.0));
    double tierLow = pricingContext.execute(80.0);
    double tierHigh = pricingContext.execute(180.0);
    std::cout << "Switched to: " << pricingContext.getStrategyName() << '\n';
    check(stats, areNear(tierLow, 76.0), "Tiered pricing uses low tier under threshold");
    check(stats, areNear(tierHigh, 144.0), "Tiered pricing uses high tier over threshold");

    printSection("3. Compression Strategy Family");
    CompressionContext compressionContext(std::make_unique<IdentityCompressionStrategy>());
    const std::string text = "aaabccccdd";

    std::string identity = compressionContext.execute(text);
    std::cout << "Using: " << compressionContext.getStrategyName() << '\n';
    check(stats, identity == text, "Identity compression keeps text unchanged");
    check(stats, compressionContext.getLastOriginal() == text, "CompressionContext stores original text");
    check(stats, compressionContext.getLastCompressed() == text, "CompressionContext stores compressed text");

    compressionContext.setStrategy(std::make_unique<RunLengthCompressionStrategy>());
    std::string rle = compressionContext.execute(text);
    std::cout << "Switched to: " << compressionContext.getStrategyName() << '\n';
    check(stats, rle == "a3b1c4d2", "Run-length compression output is expected");
    check(stats, compressionContext.getLastCompressionRatio() < 1.0,
          "Run-length compression ratio indicates size reduction for this input");

    compressionContext.setStrategy(std::make_unique<VowelRemovalCompressionStrategy>());
    std::string noVowel = compressionContext.execute("Strategy Pattern In Action");
    std::cout << "Switched to: " << compressionContext.getStrategyName() << '\n';
    check(stats, noVowel == "Strtgy Pttrn n ctn", "Vowel-removal compression strips vowels");

    printSection("4. Payment Strategy Family");
    PaymentContext paymentContext(std::make_unique<CreditCardPaymentStrategy>());

    PaymentResult cardResult = paymentContext.execute(100.0);
    std::cout << "Using: " << paymentContext.getStrategyName() << '\n';
    check(stats, cardResult.success, "Credit card payment succeeds for positive amount");
    check(stats, areNear(cardResult.fee, 2.0), "Credit card fee is 2%");
    check(stats, cardResult.provider == "CreditCard", "Credit card provider label is correct");

    paymentContext.setStrategy(std::make_unique<PayPalPaymentStrategy>());
    PaymentResult paypalResult = paymentContext.execute(100.0);
    std::cout << "Switched to: " << paymentContext.getStrategyName() << '\n';
    check(stats, areNear(paypalResult.fee, 3.3), "PayPal fee formula is correct");
    check(stats, paypalResult.provider == "PayPal", "PayPal provider label is correct");

    paymentContext.setStrategy(std::make_unique<CryptoPaymentStrategy>());
    PaymentResult cryptoResult = paymentContext.execute(100.0);
    std::cout << "Switched to: " << paymentContext.getStrategyName() << '\n';
    check(stats, areNear(cryptoResult.fee, 1.0), "Crypto fee is 1%");
    check(stats, cryptoResult.provider == "Crypto", "Crypto provider label is correct");

    PaymentResult invalidAmount = paymentContext.execute(-50.0);
    check(stats, !invalidAmount.success, "Payment fails for non-positive normalized amount");
    check(stats, areNear(invalidAmount.requestedAmount, 0.0), "Negative amount is normalized to zero");
    check(stats, paymentContext.getLastResult().provider == "Crypto",
          "PaymentContext exposes last result for assertions");

    printSection("5. Runtime Strategy Switching in Same Flow");
    SortContext switchingDemo(std::make_unique<BubbleSortStrategy>());
    auto phase1 = switchingDemo.execute(std::vector<int>{10, 9, 8, 7});
    switchingDemo.setStrategy(std::make_unique<MergeSortStrategy>());
    auto phase2 = switchingDemo.execute(std::vector<int>{7, 3, 9, 1, 7, 2});

    check(stats, areVectorsEqual(phase1, std::vector<int>({7, 8, 9, 10})),
          "Runtime flow phase1 uses initial strategy");
    check(stats, areVectorsEqual(phase2, std::vector<int>({1, 2, 3, 7, 7, 9})),
          "Runtime flow phase2 uses switched strategy");
    check(stats, switchingDemo.getStrategyName() == "MergeSort",
          "Context reports currently active strategy after switch");

    std::cout << "\n=== Summary ===\n";
    std::cout << "Passed: " << stats.passed << '\n';
    std::cout << "Failed: " << stats.failed << '\n';
    std::cout << "Total : " << (stats.passed + stats.failed) << '\n';

    if (stats.failed == 0) {
        std::cout << "Result: ALL STRATEGY TESTS PASSED\n";
        return 0;
    }

    std::cout << "Result: SOME STRATEGY TESTS FAILED\n";
    return 1;
}
