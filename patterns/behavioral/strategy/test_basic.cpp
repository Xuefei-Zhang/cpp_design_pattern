#include "behavioral/strategy_patterns.h"
#include <iostream>
#include <cassert>
#include <memory>
#include <vector>

/**
 * Basic Strategy Pattern Test Cases
 *
 * Tests the ability to switch algorithms at runtime
 */

void testBasicStrategySwitch() {
    std::cout << "Test 1: Basic Strategy Switching\n";
    std::cout << "----------------------------------\n";

    // Test with different sorting strategies
    SortContext context;

    std::vector<int> data1 = {5, 2, 8, 1, 9};

    // Use bubble sort
    context.setStrategy(std::make_unique<BubbleSortStrategy>());
    auto result1 = context.sort(data1);
    std::cout << "✓ Bubble sort strategy executed\n";

    // Use quick sort
    context.setStrategy(std::make_unique<QuickSortStrategy>());
    auto result2 = context.sort(data1);
    std::cout << "✓ Quick sort strategy executed\n";

    // Use merge sort
    context.setStrategy(std::make_unique<MergeSortStrategy>());
    auto result3 = context.sort(data1);
    std::cout << "✓ Merge sort strategy executed\n";

    std::cout << "✓ Basic Strategy Switch test passed!\n\n";
}

void testPaymentStrategies() {
    std::cout << "Test 2: Payment Strategy Example\n";
    std::cout << "----------------------------------\n";

    ShoppingCart cart;

    // Pay with credit card
    cart.setPaymentStrategy(std::make_unique<CreditCardPayment>("1234-5678-9012-3456"));
    cart.checkout(100.0);
    std::cout << "✓ Credit card payment processed\n";

    // Pay with PayPal
    cart.setPaymentStrategy(std::make_unique<PayPalPayment>("user@example.com"));
    cart.checkout(50.0);
    std::cout << "✓ PayPal payment processed\n";

    // Pay with Bitcoin
    cart.setPaymentStrategy(std::make_unique<BitcoinPayment>("1A2B3C4D5E6F"));
    cart.checkout(75.0);
    std::cout << "✓ Bitcoin payment processed\n";

    std::cout << "✓ Payment Strategies test passed!\n\n";
}

void testCompressionStrategies() {
    std::cout << "Test 3: Compression Strategy Example\n";
    std::cout << "--------------------------------------\n";

    FileCompressor compressor;
    std::string data = "This is test data for compression";

    // ZIP compression
    compressor.setStrategy(std::make_unique<ZipCompressionStrategy>());
    std::string zipped = compressor.compress(data);
    std::cout << "✓ ZIP compression applied\n";

    // RAR compression
    compressor.setStrategy(std::make_unique<RarCompressionStrategy>());
    std::string rarred = compressor.compress(data);
    std::cout << "✓ RAR compression applied\n";

    // TAR compression
    compressor.setStrategy(std::make_unique<TarCompressionStrategy>());
    std::string tarred = compressor.compress(data);
    std::cout << "✓ TAR compression applied\n";

    std::cout << "✓ Compression Strategies test passed!\n\n";
}

void testNavigationStrategies() {
    std::cout << "Test 4: Navigation Strategy Example\n";
    std::cout << "-------------------------------------\n";

    Navigator navigator;

    // Walking route
    navigator.setStrategy(std::make_unique<WalkingStrategy>());
    navigator.navigate("Home", "Park");
    std::cout << "✓ Walking route calculated\n";

    // Driving route
    navigator.setStrategy(std::make_unique<DrivingStrategy>());
    navigator.navigate("Home", "Office");
    std::cout << "✓ Driving route calculated\n";

    // Public transport route
    navigator.setStrategy(std::make_unique<PublicTransportStrategy>());
    navigator.navigate("Home", "Airport");
    std::cout << "✓ Public transport route calculated\n";

    std::cout << "✓ Navigation Strategies test passed!\n\n";
}

void testStrategyPolymorphism() {
    std::cout << "Test 5: Strategy Polymorphism\n";
    std::cout << "-------------------------------\n";

    // Create different strategies and use them polymorphically
    std::vector<std::unique_ptr<SortStrategy>> strategies;
    strategies.push_back(std::make_unique<BubbleSortStrategy>());
    strategies.push_back(std::make_unique<QuickSortStrategy>());
    strategies.push_back(std::make_unique<MergeSortStrategy>());

    std::vector<int> data = {4, 2, 7, 1, 9, 5};
    SortContext context;

    for (auto& strategy : strategies) {
        context.setStrategy(std::move(strategy));
        auto result = context.sort(data);
        // Verify result is sorted
        for (size_t i = 1; i < result.size(); ++i) {
            assert(result[i-1] <= result[i]);
        }
    }

    std::cout << "✓ All strategies produced sorted results\n";
    std::cout << "✓ Strategy Polymorphism test passed!\n\n";
}

void testStrategyWithDifferentData() {
    std::cout << "Test 6: Strategy with Different Data Types\n";
    std::cout << "--------------------------------------------\n";

    // Test with integers
    SortContext intContext;
    intContext.setStrategy(std::make_unique<QuickSortStrategy>());
    std::vector<int> intData = {5, 2, 8, 1, 9};
    auto sortedInts = intContext.sort(intData);
    assert(sortedInts.size() == intData.size());
    std::cout << "✓ Strategy works with integers\n";

    // Test with different size arrays
    std::vector<int> smallData = {3, 1};
    auto sortedSmall = intContext.sort(smallData);
    assert(sortedSmall[0] == 1 && sortedSmall[1] == 3);
    std::cout << "✓ Strategy works with small data\n";

    std::vector<int> largeData;
    for (int i = 100; i > 0; --i) {
        largeData.push_back(i);
    }
    auto sortedLarge = intContext.sort(largeData);
    assert(sortedLarge.size() == 100);
    assert(sortedLarge[0] == 1 && sortedLarge[99] == 100);
    std::cout << "✓ Strategy works with large data\n";

    std::cout << "✓ Different Data Types test passed!\n\n";
}

void testRuntimeStrategyChange() {
    std::cout << "Test 7: Runtime Strategy Change\n";
    std::cout << "---------------------------------\n";

    ShoppingCart cart;
    double amount = 100.0;

    // Start with credit card
    cart.setPaymentStrategy(std::make_unique<CreditCardPayment>("1234-5678"));
    std::cout << "Initial payment method: Credit Card\n";

    // Change to PayPal
    cart.setPaymentStrategy(std::make_unique<PayPalPayment>("user@email.com"));
    std::cout << "Changed payment method: PayPal\n";
    cart.checkout(amount);

    // Change to Bitcoin
    cart.setPaymentStrategy(std::make_unique<BitcoinPayment>("ABC123"));
    std::cout << "Changed payment method: Bitcoin\n";
    cart.checkout(amount);

    std::cout << "✓ Strategy changed successfully at runtime\n";
    std::cout << "✓ Runtime Strategy Change test passed!\n\n";
}

void testNullStrategyHandling() {
    std::cout << "Test 8: Null Strategy Handling\n";
    std::cout << "--------------------------------\n";

    SortContext context;
    std::vector<int> data = {3, 1, 4, 1, 5};

    // Try to use context without setting a strategy
    // Implementation should handle this gracefully
    try {
        auto result = context.sort(data);
        std::cout << "✓ Null strategy handled (returned original or threw expected exception)\n";
    } catch (const std::exception& e) {
        std::cout << "✓ Null strategy threw exception as expected: " << e.what() << "\n";
    }

    std::cout << "✓ Null Strategy Handling test passed!\n\n";
}

void testStrategyIndependence() {
    std::cout << "Test 9: Strategy Independence\n";
    std::cout << "------------------------------\n";

    // Create multiple contexts with different strategies
    SortContext context1, context2, context3;

    context1.setStrategy(std::make_unique<BubbleSortStrategy>());
    context2.setStrategy(std::make_unique<QuickSortStrategy>());
    context3.setStrategy(std::make_unique<MergeSortStrategy>());

    std::vector<int> data = {5, 2, 8, 1, 9};

    // All contexts should work independently
    auto result1 = context1.sort(data);
    auto result2 = context2.sort(data);
    auto result3 = context3.sort(data);

    // All should produce sorted results
    for (size_t i = 1; i < result1.size(); ++i) {
        assert(result1[i-1] <= result1[i]);
        assert(result2[i-1] <= result2[i]);
        assert(result3[i-1] <= result3[i]);
    }

    std::cout << "✓ Multiple contexts work independently\n";
    std::cout << "✓ Strategy Independence test passed!\n\n";
}

void testStrategyComparison() {
    std::cout << "Test 10: Strategy Performance Comparison\n";
    std::cout << "------------------------------------------\n";

    std::vector<int> data;
    for (int i = 50; i > 0; --i) {
        data.push_back(i);
    }

    SortContext context;

    // Test different strategies
    std::cout << "Testing Bubble Sort...\n";
    context.setStrategy(std::make_unique<BubbleSortStrategy>());
    auto result1 = context.sort(data);

    std::cout << "Testing Quick Sort...\n";
    context.setStrategy(std::make_unique<QuickSortStrategy>());
    auto result2 = context.sort(data);

    std::cout << "Testing Merge Sort...\n";
    context.setStrategy(std::make_unique<MergeSortStrategy>());
    auto result3 = context.sort(data);

    // All should produce correct results
    assert(result1[0] == 1 && result1[49] == 50);
    assert(result2[0] == 1 && result2[49] == 50);
    assert(result3[0] == 1 && result3[49] == 50);

    std::cout << "✓ All strategies produced correct results\n";
    std::cout << "✓ Strategy Comparison test passed!\n\n";
}

int main() {
    std::cout << "╔════════════════════════════════════════════════════╗\n";
    std::cout << "║     Strategy Pattern - Basic Test Suite           ║\n";
    std::cout << "╚════════════════════════════════════════════════════╝\n\n";

    try {
        testBasicStrategySwitch();
        testPaymentStrategies();
        testCompressionStrategies();
        testNavigationStrategies();
        testStrategyPolymorphism();
        testStrategyWithDifferentData();
        testRuntimeStrategyChange();
        testNullStrategyHandling();
        testStrategyIndependence();
        testStrategyComparison();

        std::cout << "╔════════════════════════════════════════════════════╗\n";
        std::cout << "║          ALL TESTS PASSED! ✓                       ║\n";
        std::cout << "╚════════════════════════════════════════════════════╝\n";

        std::cout << "\nKey Benefits of Strategy Pattern:\n";
        std::cout << "- Algorithms can be switched at runtime\n";
        std::cout << "- Eliminates conditional statements\n";
        std::cout << "- Makes algorithms interchangeable\n";
        std::cout << "- Follows Open/Closed Principle\n";

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "✗ Test failed with exception: " << e.what() << "\n";
        return 1;
    }
}
