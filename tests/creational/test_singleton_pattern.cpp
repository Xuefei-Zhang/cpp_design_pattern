#include "creational/singleton_patterns.h"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

int main() {
    std::cout << "=== Singleton Pattern Demo ===\n\n";
    
    // Test 1: Basic Singleton
    std::cout << "1. Basic Singleton Test:\n";
    auto* basic1 = BasicSingleton::getInstance();
    auto* basic2 = BasicSingleton::getInstance();
    
    std::cout << "Are basic1 and basic2 the same instance? " 
              << (basic1 == basic2 ? "Yes" : "No") << std::endl;
    
    basic1->setData("Modified Basic Data");
    std::cout << "BasicSingleton data from basic1: " << basic1->getData() << std::endl;
    std::cout << "BasicSingleton data from basic2: " << basic2->getData() << std::endl;
    std::cout << std::endl;
    
    // Test 2: Thread-Safe Singleton (Meyers Singleton)
    std::cout << "2. Thread-Safe Singleton (Meyers) Test:\n";
    auto& threadSafe1 = ThreadSafeSingleton::getInstance();
    auto& threadSafe2 = ThreadSafeSingleton::getInstance();
    
    std::cout << "Are threadSafe1 and threadSafe2 the same instance? " 
              << (&threadSafe1 == &threadSafe2 ? "Yes" : "No") << std::endl;
    
    threadSafe1.setData("Thread-Safe Modified Data");
    std::cout << "ThreadSafeSingleton data from threadSafe1: " << threadSafe1.getData() << std::endl;
    std::cout << "ThreadSafeSingleton data from threadSafe2: " << threadSafe2.getData() << std::endl;
    std::cout << "Increment counter: " << threadSafe1.incrementCounter() << std::endl;
    std::cout << "Increment counter again: " << threadSafe2.incrementCounter() << std::endl;
    std::cout << std::endl;
    
    // Test 3: Double-Checked Locking Singleton
    std::cout << "3. Double-Checked Locking Singleton Test:\n";
    auto* doubleChecked1 = DoubleCheckedSingleton::getInstance();
    auto* doubleChecked2 = DoubleCheckedSingleton::getInstance();
    
    std::cout << "Are doubleChecked1 and doubleChecked2 the same instance? " 
              << (doubleChecked1 == doubleChecked2 ? "Yes" : "No") << std::endl;
    
    doubleChecked1->setData("Double-Checked Modified Data");
    std::cout << "DoubleCheckedSingleton data from doubleChecked1: " << doubleChecked1->getData() << std::endl;
    std::cout << "DoubleCheckedSingleton data from doubleChecked2: " << doubleChecked2->getData() << std::endl;
    std::cout << std::endl;
    
    // Test 4: Lazy-Initialized Singleton
    std::cout << "4. Lazy-Initialized Singleton Test:\n";
    auto* lazy1 = LazyInitSingleton::getInstance();
    auto* lazy2 = LazyInitSingleton::getInstance();
    
    std::cout << "Are lazy1 and lazy2 the same instance? " 
              << (lazy1 == lazy2 ? "Yes" : "No") << std::endl;
    
    lazy1->setData("Lazy-Init Modified Data");
    std::cout << "LazyInitSingleton data from lazy1: " << lazy1->getData() << std::endl;
    std::cout << "LazyInitSingleton data from lazy2: " << lazy2->getData() << std::endl;
    std::cout << std::endl;
    
    // Test 5: Generic Template Singleton (Logger example)
    std::cout << "5. Generic Template Singleton (Logger) Test:\n";
    auto& logger1 = Logger::getInstance();
    auto& logger2 = Logger::getInstance();
    
    std::cout << "Are logger1 and logger2 the same instance? " 
              << (&logger1 == &logger2 ? "Yes" : "No") << std::endl;
    
    logger1.setLogData("Logger Modified Data");
    std::cout << "Logger data from logger1: " << logger1.getLogData() << std::endl;
    std::cout << "Logger data from logger2: " << logger2.getLogData() << std::endl;
    
    logger1.log("This is a test log message");
    logger2.log("Another log message from the same instance");
    std::cout << std::endl;
    
    // Test 6: Destroyable Singleton
    std::cout << "6. Destroyable Singleton Test:\n";
    auto* destroyable1 = DestroyableSingleton::getInstance();
    auto* destroyable2 = DestroyableSingleton::getInstance();
    
    std::cout << "Are destroyable1 and destroyable2 the same instance? " 
              << (destroyable1 == destroyable2 ? "Yes" : "No") << std::endl;
    
    destroyable1->setData("Destroyable Singleton Data");
    std::cout << "DestroyableSingleton data: " << destroyable1->getData() << std::endl;
    
    // Destroy the instance and create a new one
    DestroyableSingleton::destroyInstance();
    auto* destroyable3 = DestroyableSingleton::getInstance();
    std::cout << "New instance after destruction: " << (destroyable1 == destroyable3 ? "Same" : "Different") << std::endl;
    std::cout << std::endl;
    
    // Test 7: Configuration Manager (Generic Singleton)
    std::cout << "7. Configuration Manager Singleton Test:\n";
    auto& config1 = ConfigManager::getInstance();
    auto& config2 = ConfigManager::getInstance();
    
    std::cout << "Are config1 and config2 the same instance? " 
              << (&config1 == &config2 ? "Yes" : "No") << std::endl;
    
    config1.setConfig("database_host", "localhost");
    std::string configValue = config2.getConfig("database_host");
    std::cout << "Configuration value: " << configValue << std::endl;
    std::cout << std::endl;
    
    // Test 8: Thread Safety Test
    std::cout << "8. Thread Safety Test:\n";
    std::cout << "Creating 5 threads that will all try to access the ThreadSafeSingleton...\n";
    
    std::vector<std::thread> threads;
    
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([i]() {
            // Each thread gets the singleton instance
            auto& instance = ThreadSafeSingleton::getInstance();
            
            // Each thread increments the counter
            int counter = instance.incrementCounter();
            
            // Each thread updates the data with its ID
            instance.setData("Thread " + std::to_string(i) + " data, counter: " + std::to_string(counter));
            
            std::cout << "Thread " << i << " - Counter: " << counter 
                      << ", Data: " << instance.getData() << std::endl;
            
            // Sleep to allow other threads to execute
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        });
    }
    
    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }
    
    // Check final state
    auto& finalInstance = ThreadSafeSingleton::getInstance();
    std::cout << "Final singleton state: " << finalInstance.getData() << std::endl;
    std::cout << "Final counter value: " << finalInstance.incrementCounter() << std::endl;
    std::cout << std::endl;
    
    // Test 9: Performance Comparison
    std::cout << "9. Performance Test - Accessing Singleton multiple times:\n";
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 1000000; ++i) {
        auto& instance = ThreadSafeSingleton::getInstance();
        // Access the counter to prevent optimization
        int counter = instance.incrementCounter();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "1,000,000 singleton accesses took: " << duration.count() << " microseconds" << std::endl;
    std::cout << std::endl;
    
    // Test 10: Singleton Relationships
    std::cout << "10. Singleton Uniqueness Verification:\n";
    std::cout << "BasicSingleton address: " << basic1 << std::endl;
    std::cout << "ThreadSafeSingleton address: " << &threadSafe1 << std::endl;
    std::cout << "DoubleCheckedSingleton address: " << doubleChecked1 << std::endl;
    std::cout << "LazyInitSingleton address: " << lazy1 << std::endl;
    std::cout << "Logger address: " << &logger1 << std::endl;
    std::cout << "DestroyableSingleton address: " << destroyable1 << std::endl;
    std::cout << "ConfigManager address: " << &config1 << std::endl;
    std::cout << "\nNote: Each singleton type has its own unique instance" << std::endl;
    std::cout << std::endl;
    
    std::cout << "Singleton pattern demo completed successfully!\n";
    std::cout << "\nKey Benefits of Singleton Pattern:\n";
    std::cout << "- Ensures a class has only one instance\n";
    std::cout << "- Provides a global access point to that instance\n";
    std::cout << "- Controls concurrent access in a multithreaded environment\n";
    std::cout << "- Lazy initialization possible\n";
    std::cout << "- Can be extended for specific use cases (logging, configuration, etc.)\n\n";
    
    std::cout << "Important Considerations:\n";
    std::cout << "- Can create tight coupling between classes\n";
    std::cout << "- Can make unit testing difficult\n";
    std::cout << "- Can hide bad design (global state)\n";
    std::cout << "- Should be thread-safe if used in multithreaded applications\n";
    std::cout << "- Consider alternatives like dependency injection for better design\n";
    
    return 0;
}