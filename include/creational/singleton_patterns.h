#ifndef SINGLETON_PATTERNS_H
#define SINGLETON_PATTERNS_H

#include <iostream>
#include <string>
#include <mutex>
#include <thread>
#include <chrono>
#include <memory>

// 1. Basic Singleton - Simple implementation (not thread-safe)
class BasicSingleton {
private:
    static BasicSingleton* instance;
    std::string data;
    
    // Private constructor to prevent instantiation
    BasicSingleton() : data("Basic Singleton Data") {}
    
    // Delete copy constructor and assignment operator
    BasicSingleton(const BasicSingleton&) = delete;
    BasicSingleton& operator=(const BasicSingleton&) = delete;

public:
    static BasicSingleton* getInstance() {
        if (instance == nullptr) {
            instance = new BasicSingleton();
        }
        return instance;
    }
    
    void setData(const std::string& newData) { data = newData; }
    std::string getData() const { return data; }
    void printInfo() const { std::cout << "BasicSingleton: " << data << std::endl; }
};

// Initialize static member
BasicSingleton* BasicSingleton::instance = nullptr;

// 2. Thread-Safe Singleton using mutex (Meyers Singleton alternative)
class ThreadSafeSingleton {
private:
    std::string data;
    mutable std::mutex mtx; // mutable to allow locking in const methods
    
    // Private constructor
    ThreadSafeSingleton() : data("Thread-Safe Singleton Data") {}
    
    // Delete copy constructor and assignment operator
    ThreadSafeSingleton(const ThreadSafeSingleton&) = delete;
    ThreadSafeSingleton& operator=(const ThreadSafeSingleton&) = delete;

public:
    static ThreadSafeSingleton& getInstance() {
        static ThreadSafeSingleton instance; // Thread-safe in C++11 and later
        return instance;
    }
    
    void setData(const std::string& newData) {
        std::lock_guard<std::mutex> lock(mtx);
        data = newData;
    }
    
    std::string getData() const {
        std::lock_guard<std::mutex> lock(mtx);
        return data;
    }
    
    void printInfo() const {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "ThreadSafeSingleton: " << data << std::endl;
    }
    
    // Thread-safe counter example
    int incrementCounter() const {
        static int counter = 0;
        std::lock_guard<std::mutex> lock(mtx);
        return ++counter;
    }
};

// 3. Double-Checked Locking Singleton (for older C++ standards)
class DoubleCheckedSingleton {
private:
    static DoubleCheckedSingleton* instance;
    static std::mutex mtx;
    std::string data;
    
    DoubleCheckedSingleton() : data("Double-Checked Singleton Data") {}
    
    // Delete copy constructor and assignment operator
    DoubleCheckedSingleton(const DoubleCheckedSingleton&) = delete;
    DoubleCheckedSingleton& operator=(const DoubleCheckedSingleton&) = delete;

public:
    static DoubleCheckedSingleton* getInstance() {
        if (instance == nullptr) {
            std::lock_guard<std::mutex> lock(mtx);
            if (instance == nullptr) { // Double check
                instance = new DoubleCheckedSingleton();
            }
        }
        return instance;
    }
    
    void setData(const std::string& newData) {
        std::lock_guard<std::mutex> lock(mtx);
        data = newData;
    }
    
    std::string getData() const {
        std::lock_guard<std::mutex> lock(mtx);
        return data;
    }
    
    void printInfo() const {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "DoubleCheckedSingleton: " << data << std::endl;
    }
};

// Initialize static members
DoubleCheckedSingleton* DoubleCheckedSingleton::instance = nullptr;
std::mutex DoubleCheckedSingleton::mtx;

// 4. Lazy-Initialized Singleton with std::call_once (C++11)
class LazyInitSingleton {
private:
    static std::once_flag onceFlag;
    static LazyInitSingleton* instance;
    std::string data;
    
    LazyInitSingleton() : data("Lazy-Initialized Singleton Data") {}
    
    // Delete copy constructor and assignment operator
    LazyInitSingleton(const LazyInitSingleton&) = delete;
    LazyInitSingleton& operator=(const LazyInitSingleton&) = delete;

public:
    static LazyInitSingleton* getInstance() {
        std::call_once(onceFlag, []() {
            instance = new LazyInitSingleton();
        });
        return instance;
    }
    
    void setData(const std::string& newData) { data = newData; }
    std::string getData() const { return data; }
    void printInfo() const { std::cout << "LazyInitSingleton: " << data << std::endl; }
};

// Initialize static members
std::once_flag LazyInitSingleton::onceFlag;
LazyInitSingleton* LazyInitSingleton::instance = nullptr;

// 5. Template-based Singleton (Generic approach)
template<typename T>
class GenericSingleton {
protected:
    static T* instance;
    
    GenericSingleton() = default;
    
public:
    static T& getInstance() {
        static T instance_val; // Meyers singleton - thread-safe in C++11
        return instance_val;
    }
    
    // Delete copy constructor and assignment operator
    GenericSingleton(const GenericSingleton&) = delete;
    GenericSingleton& operator=(const GenericSingleton&) = delete;
};

template<typename T>
T* GenericSingleton<T>::instance = nullptr;

// Example derived class for template singleton
class Logger : public GenericSingleton<Logger> {
private:
    std::string logData;
    mutable std::mutex logMutex;
    
public:
    Logger() : logData("Logger initialized") {}
    
    void log(const std::string& message) {
        std::lock_guard<std::mutex> lock(logMutex);
        std::cout << "[LOG] " << message << std::endl;
    }
    
    void setLogData(const std::string& data) { logData = data; }
    std::string getLogData() const { return logData; }
};

// 6. Destroyable Singleton (allows cleanup)
class DestroyableSingleton {
private:
    static DestroyableSingleton* instance;
    static std::mutex mtx;
    std::string data;
    
    DestroyableSingleton() : data("Destroyable Singleton Data") {}
    
    // Delete copy constructor and assignment operator
    DestroyableSingleton(const DestroyableSingleton&) = delete;
    DestroyableSingleton& operator=(const DestroyableSingleton&) = delete;

public:
    static DestroyableSingleton* getInstance() {
        std::lock_guard<std::mutex> lock(mtx);
        if (instance == nullptr) {
            instance = new DestroyableSingleton();
        }
        return instance;
    }
    
    static void destroyInstance() {
        std::lock_guard<std::mutex> lock(mtx);
        if (instance != nullptr) {
            delete instance;
            instance = nullptr;
        }
    }
    
    void setData(const std::string& newData) {
        std::lock_guard<std::mutex> lock(mtx);
        data = newData;
    }
    
    std::string getData() const {
        std::lock_guard<std::mutex> lock(mtx);
        return data;
    }
    
    void printInfo() const {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "DestroyableSingleton: " << data << std::endl;
    }
};

DestroyableSingleton* DestroyableSingleton::instance = nullptr;
std::mutex DestroyableSingleton::mtx;

// 7. Singleton with additional functionality (Configuration Manager example)
class ConfigManager : public GenericSingleton<ConfigManager> {
private:
    std::string configData;
    mutable std::mutex configMutex;
    
public:
    ConfigManager() : configData("Default Config Data") {}
    
    void setConfig(const std::string& key, const std::string& value) {
        std::lock_guard<std::mutex> lock(configMutex);
        configData = key + "=" + value;
        std::cout << "Config set: " << configData << std::endl;
    }
    
    std::string getConfig(const std::string& key) const {
        std::lock_guard<std::mutex> lock(configMutex);
        // In a real implementation, you would parse the config
        return configData;
    }
    
    void reset() {
        std::lock_guard<std::mutex> lock(configMutex);
        configData = "Default Config Data";
    }
};

#endif // SINGLETON_PATTERNS_H