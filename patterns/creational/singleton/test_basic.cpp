#include "creational/singleton_patterns.h"
#include <iostream>
#include <cassert>

/**
 * Basic Singleton Pattern Test Cases
 *
 * This test covers fundamental singleton behavior:
 * 1. Single instance verification
 * 2. State sharing between references
 * 3. Multiple singleton types
 */

void testBasicSingleton() {
    std::cout << "Test 1: Basic Singleton Instance Verification\n";
    std::cout << "------------------------------------------------\n";

    auto* instance1 = BasicSingleton::getInstance();
    auto* instance2 = BasicSingleton::getInstance();

    // Test: Both pointers should point to the same instance
    assert(instance1 == instance2);
    std::cout << "✓ Both getInstance() calls return the same instance\n";

    // Test: Modifications through one pointer affect the other
    instance1->setData("Test Data");
    assert(instance2->getData() == "Test Data");
    std::cout << "✓ State is shared between references\n";

    std::cout << "✓ Basic Singleton test passed!\n\n";
}

void testThreadSafeSingleton() {
    std::cout << "Test 2: Thread-Safe Singleton (Meyer's Singleton)\n";
    std::cout << "---------------------------------------------------\n";

    auto& instance1 = ThreadSafeSingleton::getInstance();
    auto& instance2 = ThreadSafeSingleton::getInstance();

    // Test: References should point to the same object
    assert(&instance1 == &instance2);
    std::cout << "✓ Both getInstance() calls return the same instance\n";

    // Test: Counter should increment properly
    int count1 = instance1.incrementCounter();
    int count2 = instance2.incrementCounter();
    assert(count2 == count1 + 1);
    std::cout << "✓ Shared state increments correctly: " << count1 << " -> " << count2 << "\n";

    std::cout << "✓ Thread-Safe Singleton test passed!\n\n";
}

void testDoubleCheckedLockingSingleton() {
    std::cout << "Test 3: Double-Checked Locking Singleton\n";
    std::cout << "------------------------------------------\n";

    auto* instance1 = DoubleCheckedSingleton::getInstance();
    auto* instance2 = DoubleCheckedSingleton::getInstance();

    assert(instance1 == instance2);
    std::cout << "✓ Instance uniqueness verified\n";

    instance1->setData("DCL Test");
    assert(instance2->getData() == "DCL Test");
    std::cout << "✓ Data consistency verified\n";

    std::cout << "✓ Double-Checked Locking test passed!\n\n";
}

void testLazyInitSingleton() {
    std::cout << "Test 4: Lazy Initialization Singleton\n";
    std::cout << "---------------------------------------\n";

    auto* instance1 = LazyInitSingleton::getInstance();
    auto* instance2 = LazyInitSingleton::getInstance();

    assert(instance1 == instance2);
    std::cout << "✓ Instance uniqueness verified\n";

    // Test lazy initialization behavior
    instance1->setData("Lazy Init Data");
    assert(instance2->getData() == "Lazy Init Data");
    std::cout << "✓ Lazy initialization works correctly\n";

    std::cout << "✓ Lazy Initialization test passed!\n\n";
}

void testLoggerSingleton() {
    std::cout << "Test 5: Logger Singleton (Practical Example)\n";
    std::cout << "----------------------------------------------\n";

    auto& logger1 = Logger::getInstance();
    auto& logger2 = Logger::getInstance();

    assert(&logger1 == &logger2);
    std::cout << "✓ Logger singleton verified\n";

    logger1.setLogData("System initialized");
    assert(logger2.getLogData() == "System initialized");
    std::cout << "✓ Log data shared correctly\n";

    // Test logging functionality
    logger1.log("Test message 1");
    logger2.log("Test message 2");
    std::cout << "✓ Logging works from both references\n";

    std::cout << "✓ Logger Singleton test passed!\n\n";
}

void testConfigManagerSingleton() {
    std::cout << "Test 6: Configuration Manager Singleton\n";
    std::cout << "-----------------------------------------\n";

    auto& config1 = ConfigManager::getInstance();
    auto& config2 = ConfigManager::getInstance();

    assert(&config1 == &config2);
    std::cout << "✓ Config manager singleton verified\n";

    // Test configuration operations
    config1.setConfig("app_name", "TestApp");
    config1.setConfig("version", "1.0.0");
    config1.setConfig("debug", "true");

    assert(config2.getConfig("app_name") == "TestApp");
    assert(config2.getConfig("version") == "1.0.0");
    assert(config2.getConfig("debug") == "true");
    std::cout << "✓ Configuration values shared correctly\n";

    std::cout << "✓ Configuration Manager test passed!\n\n";
}

void testDestroyableSingleton() {
    std::cout << "Test 7: Destroyable Singleton\n";
    std::cout << "-------------------------------\n";

    auto* instance1 = DestroyableSingleton::getInstance();
    instance1->setData("Original Data");

    auto* instance2 = DestroyableSingleton::getInstance();
    assert(instance1 == instance2);
    assert(instance2->getData() == "Original Data");
    std::cout << "✓ Initial instance created and data set\n";

    // Destroy the singleton
    DestroyableSingleton::destroyInstance();
    std::cout << "✓ Singleton destroyed\n";

    // Create a new instance
    auto* instance3 = DestroyableSingleton::getInstance();
    assert(instance1 != instance3);  // Different instance
    assert(instance3->getData() != "Original Data");  // Fresh state
    std::cout << "✓ New instance created with fresh state\n";

    std::cout << "✓ Destroyable Singleton test passed!\n\n";
}

void testMultipleSingletonCoexistence() {
    std::cout << "Test 8: Multiple Singleton Types Coexistence\n";
    std::cout << "----------------------------------------------\n";

    // Get instances of different singleton types
    auto* basic = BasicSingleton::getInstance();
    auto& threadSafe = ThreadSafeSingleton::getInstance();
    auto& logger = Logger::getInstance();
    auto& config = ConfigManager::getInstance();

    // Each singleton type should have its own unique instance
    std::cout << "BasicSingleton address:      " << basic << "\n";
    std::cout << "ThreadSafeSingleton address: " << &threadSafe << "\n";
    std::cout << "Logger address:              " << &logger << "\n";
    std::cout << "ConfigManager address:       " << &config << "\n";

    // Verify they're all different addresses
    assert(static_cast<void*>(basic) != static_cast<void*>(&threadSafe));
    assert(static_cast<void*>(&logger) != static_cast<void*>(&config));
    std::cout << "✓ Each singleton type has its own unique instance\n";

    std::cout << "✓ Multiple Singleton Coexistence test passed!\n\n";
}

void testSingletonStateIsolation() {
    std::cout << "Test 9: Singleton State Isolation\n";
    std::cout << "-----------------------------------\n";

    // Set different data for different singletons
    BasicSingleton::getInstance()->setData("Basic Data");
    ThreadSafeSingleton::getInstance().setData("ThreadSafe Data");
    Logger::getInstance().setLogData("Logger Data");

    // Verify each maintains its own state
    assert(BasicSingleton::getInstance()->getData() == "Basic Data");
    assert(ThreadSafeSingleton::getInstance().getData() == "ThreadSafe Data");
    assert(Logger::getInstance().getLogData() == "Logger Data");

    std::cout << "✓ Each singleton maintains its own isolated state\n";
    std::cout << "✓ State Isolation test passed!\n\n";
}

void testSingletonNullSafety() {
    std::cout << "Test 10: Singleton Null Safety\n";
    std::cout << "--------------------------------\n";

    // All singleton getInstance methods should never return null
    auto* basic = BasicSingleton::getInstance();
    auto* doubleChecked = DoubleCheckedSingleton::getInstance();
    auto* lazy = LazyInitSingleton::getInstance();

    assert(basic != nullptr);
    assert(doubleChecked != nullptr);
    assert(lazy != nullptr);

    std::cout << "✓ All singleton getInstance() methods return non-null\n";
    std::cout << "✓ Null Safety test passed!\n\n";
}

int main() {
    std::cout << "╔════════════════════════════════════════════════════╗\n";
    std::cout << "║     Singleton Pattern - Basic Test Suite          ║\n";
    std::cout << "╚════════════════════════════════════════════════════╝\n\n";

    try {
        testBasicSingleton();
        testThreadSafeSingleton();
        testDoubleCheckedLockingSingleton();
        testLazyInitSingleton();
        testLoggerSingleton();
        testConfigManagerSingleton();
        testDestroyableSingleton();
        testMultipleSingletonCoexistence();
        testSingletonStateIsolation();
        testSingletonNullSafety();

        std::cout << "╔════════════════════════════════════════════════════╗\n";
        std::cout << "║          ALL TESTS PASSED! ✓                       ║\n";
        std::cout << "╚════════════════════════════════════════════════════╝\n";

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "✗ Test failed with exception: " << e.what() << "\n";
        return 1;
    }
}
