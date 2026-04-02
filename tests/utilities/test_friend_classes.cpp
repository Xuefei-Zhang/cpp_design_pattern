#include "friend_classes_basic.h"
#include "friend_classes_advanced.h"
#include <iostream>

int main() {
    std::cout << "=== Friend Classes in C++ Demo ===\n\n";
    
    // Test 1: Basic friend class example (Account and Bank)
    std::cout << "1. Basic Friend Class Example - Account and Bank:\n";
    Account myAccount("John Doe", 1000.0);
    Bank myBank("First National Bank");
    
    myAccount.display();
    myBank.displayAccountDetails(myAccount);
    myBank.processAccount(myAccount);
    myAccount.display();
    myBank.setBalance(myAccount, 1500.0);
    myAccount.display();
    std::cout << std::endl;
    
    // Test 2: Friend function example (Counter)
    std::cout << "2. Friend Function Example - Counter:\n";
    Counter count(5);
    count.display();
    
    ++count;  // Uses friend operator++
    count.display();
    
    resetCounter(count);  // Uses friend function
    count.display();
    std::cout << std::endl;
    
    // Test 3: Multiple friends example
    std::cout << "3. Multiple Friend Classes Example:\n";
    DataContainer data(42, "Sample Data");
    data.displayPrivate();
    
    DataAnalyzer analyzer;
    analyzer.analyze(data);
    std::cout << "Raw data from analyzer: " << analyzer.getRawData(data) << std::endl;
    
    DataSerializer serializer;
    std::cout << "Serialized: " << serializer.serialize(data) << std::endl;
    
    serializer.modifyData(data, 99);
    data.displayPrivate();
    std::cout << std::endl;
    
    // Test 4: Advanced Matrix operations with friends
    std::cout << "4. Advanced Matrix Operations with Friends:\n";
    Matrix m1({{1, 2}, {3, 4}});
    Matrix m2({{5, 6}, {7, 8}});
    
    std::cout << "Matrix 1:\n";
    m1.display();
    std::cout << "\nMatrix 2:\n";
    m2.display();
    
    Matrix sum = m1 + m2;
    std::cout << "\nSum (m1 + m2):\n";
    sum.display();
    
    Matrix product = m1 * m2;
    std::cout << "\nProduct (m1 * m2):\n";
    product.display();
    
    Matrix transposed = MatrixOperations::transpose(m1);
    std::cout << "\nTranspose of m1:\n";
    transposed.display();
    
    std::cout << "\nRaw data access from friend class:\n";
    MatrixOperations::printRawData(m1);
    
    try {
        double det = MatrixOperations::determinant(m1);
        std::cout << "\nDeterminant of m1: " << det << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error calculating determinant: " << e.what() << std::endl;
    }
    std::cout << std::endl;
    
    // Test 5: Complex number operations with friends
    std::cout << "5. Complex Number Operations with Friends:\n";
    Complex c1(3, 4);
    Complex c2(1, -2);
    
    std::cout << "c1 = " << c1 << std::endl;
    std::cout << "c2 = " << c2 << std::endl;
    
    Complex sum_c = c1 + c2;
    std::cout << "c1 + c2 = " << sum_c << std::endl;
    
    Complex diff = c1 - c2;
    std::cout << "c1 - c2 = " << diff << std::endl;
    
    Complex prod_c = c1 * c2;
    std::cout << "c1 * c2 = " << prod_c << std::endl;
    
    Complex quot = c1 / c2;
    std::cout << "c1 / c2 = " << quot << std::endl;
    
    std::cout << "Magnitude of c1: " << ComplexUtils::magnitude(c1) << std::endl;
    std::cout << "Conjugate of c1: " << ComplexUtils::conjugate(c1) << std::endl;
    std::cout << std::endl;
    
    // Test 6: Template Container with friend Iterator
    std::cout << "6. Template Container with Friend Iterator:\n";
    Container<int> intContainer;
    intContainer.add(10);
    intContainer.add(20);
    intContainer.add(30);
    intContainer.add(40);
    
    std::cout << "Container contents using friend iterator: ";
    for (auto it = intContainer.begin(); it != intContainer.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\nContainer size: " << intContainer.size() << std::endl;
    std::cout << std::endl;
    
    // Test 7: Demonstration of what friend classes can do
    std::cout << "7. Friend Class Capabilities Demonstration:\n";
    std::cout << "- Friend classes can access private members: YES\n";
    std::cout << "- Friend functions can access private members: YES\n";
    std::cout << "- Friend classes can modify private members: YES\n";
    std::cout << "- Friend classes don't inherit from the class they are friends with: YES\n";
    std::cout << "- Multiple classes can be friends: YES (as shown with DataContainer)\n";
    std::cout << "- Friend relationships are NOT mutual: YES (Bank is friend of Account, but Account is not friend of Bank)\n";
    std::cout << "- Friend relationships are NOT inherited: YES\n\n";
    
    // Show that non-friend classes cannot access private members
    std::cout << "8. Non-friend class access (compilation would fail):\n";
    std::cout << "// Attempting to access private members from non-friend class would result in compilation error\n";
    std::cout << "// Example: trying to access Account::balance from a non-friend class -> Compilation Error\n\n";
    
    std::cout << "Friend classes demo completed successfully!\n";
    std::cout << "\nKey Takeaways:\n";
    std::cout << "- Friend classes provide controlled access to private members\n";
    std::cout << "- Useful for operator overloading and related classes\n";
    std::cout << "- Should be used sparingly to maintain encapsulation\n";
    std::cout << "- Common in mathematical operations, iterators, and closely related classes\n";
    
    return 0;
}