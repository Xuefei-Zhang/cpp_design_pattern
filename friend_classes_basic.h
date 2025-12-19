#ifndef FRIEND_CLASSES_BASIC_H
#define FRIEND_CLASSES_BASIC_H

#include <iostream>
#include <string>

// Basic friend class example
class Account {
private:
    std::string owner;
    double balance;
    
public:
    Account(const std::string& o, double b) : owner(o), balance(b) {}
    
    // Declare Bank as a friend class - Bank can access private members
    friend class Bank;
    
    // Regular public method
    void display() const {
        std::cout << "Account owner: " << owner << ", Balance: $" << balance << std::endl;
    }
};

class Bank {
private:
    std::string name;
    
public:
    Bank(const std::string& n) : name(n) {}
    
    // This method can access private members of Account because it's a friend
    void displayAccountDetails(const Account& acc) const {
        std::cout << "From Bank " << name << ": ";
        std::cout << "Owner: " << acc.owner << ", Balance: $" << acc.balance << std::endl;  // Access private members
    }
    
    // This method can modify private members of Account
    void setBalance(Account& acc, double newBalance) {
        acc.balance = newBalance;  // Access private member
        std::cout << "Bank " << name << " set balance to $" << newBalance << std::endl;
    }
    
    // Method to process account operations
    void processAccount(Account& acc) {
        acc.balance *= 1.05; // Add 5% interest - access private member
        std::cout << "Bank " << name << " processed account for " << acc.owner << std::endl;
    }
};

// Basic friend function example
class Counter {
private:
    int value;
    
public:
    Counter(int v = 0) : value(v) {}
    
    // Declare a function as friend
    friend void resetCounter(Counter& c);
    
    // Friend function to increment
    friend Counter& operator++(Counter& c);
    
    int getValue() const { return value; }
    
    void display() const {
        std::cout << "Counter value: " << value << std::endl;
    }
};

// Friend function definitions
void resetCounter(Counter& c) {
    c.value = 0;  // Access private member
    std::cout << "Counter reset to 0" << std::endl;
}

Counter& operator++(Counter& c) {
    c.value++;  // Access private member
    return c;
}

// Example with two friend classes
class DataContainer {
private:
    int data;
    std::string description;
    
public:
    DataContainer(int d, const std::string& desc) : data(d), description(desc) {}
    
    // Declare multiple classes as friends
    friend class DataAnalyzer;
    friend class DataSerializer;
    
    void displayPrivate() const {
        std::cout << "Data: " << data << ", Description: " << description << std::endl;
    }
};

class DataAnalyzer {
public:
    // Can access private members of DataContainer
    void analyze(const DataContainer& dc) const {
        std::cout << "Analyzing data: " << dc.data << " (" << dc.description << ")" << std::endl;
        std::cout << "Data is " << (dc.data % 2 == 0 ? "even" : "odd") << std::endl;
    }
    
    int getRawData(const DataContainer& dc) const {
        return dc.data;  // Access private member
    }
};

class DataSerializer {
public:
    // Can access private members of DataContainer
    std::string serialize(const DataContainer& dc) const {
        return "Data: " + std::to_string(dc.data) + ", Desc: " + dc.description;
    }
    
    void modifyData(DataContainer& dc, int newValue) const {
        dc.data = newValue;  // Modify private member
    }
};

#endif // FRIEND_CLASSES_BASIC_H