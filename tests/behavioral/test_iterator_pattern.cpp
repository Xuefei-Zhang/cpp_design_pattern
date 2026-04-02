#include "iterator_patterns.h"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::cout << "=== Iterator Pattern Demo: Custom Collection Traversal ===\n\n";

    std::cout << "1. Basic forward iteration:\n";
    Collection<int> numbers;
    numbers.add(1);
    numbers.add(2);
    numbers.add(3);
    numbers.add(4);
    numbers.add(5);

    auto forwardIter = numbers.createForwardIterator();
    std::cout << "   Forward iteration: ";
    std::vector<int> forwardResult;
    while (forwardIter->hasNext()) {
        int val = forwardIter->next();
        std::cout << val << " ";
        forwardResult.push_back(val);
    }
    std::cout << "\n";
    assert(forwardResult.size() == 5);
    assert(forwardResult[0] == 1 && forwardResult[4] == 5);
    std::cout << "   [OK] Forward iteration completed\n\n";

    std::cout << "2. Backward iteration:\n";
    auto backwardIter = numbers.createBackwardIterator();
    std::cout << "   Backward iteration: ";
    std::vector<int> backwardResult;
    while (backwardIter->hasNext()) {
        int val = backwardIter->next();
        std::cout << val << " ";
        backwardResult.push_back(val);
    }
    std::cout << "\n";
    assert(backwardResult.size() == 5);
    assert(backwardResult[0] == 5 && backwardResult[4] == 1);
    std::cout << "   [OK] Backward iteration completed\n\n";

    std::cout << "3. Skip iterator (every 2nd element):\n";
    auto skipIter = numbers.createSkipIterator(2);
    std::cout << "   Skip iteration (step=2): ";
    std::vector<int> skipResult;
    while (skipIter->hasNext()) {
        int val = skipIter->next();
        std::cout << val << " ";
        skipResult.push_back(val);
    }
    std::cout << "\n";
    assert(skipResult.size() == 3);
    assert(skipResult[0] == 1 && skipResult[1] == 3 && skipResult[2] == 5);
    std::cout << "   [OK] Skip iteration completed\n\n";

    std::cout << "4. Iterator reset functionality:\n";
    auto resetIter = numbers.createForwardIterator();
    int first1 = resetIter->next();
    int second1 = resetIter->next();
    resetIter->reset();
    int first2 = resetIter->next();
    std::cout << "   First element before reset: " << first1 << "\n";
    std::cout << "   Second element before reset: " << second1 << "\n";
    std::cout << "   First element after reset: " << first2 << "\n";
    assert(first1 == first2);
    assert(first1 == 1 && second1 == 2);
    std::cout << "   [OK] Iterator reset works correctly\n\n";

    std::cout << "5. Book collection example:\n";
    BookCollection library;
    library.addBook(Book("The C Programming Language", "Kernighan & Ritchie", 1978));
    library.addBook(Book("Design Patterns", "Gang of Four", 1994));
    library.addBook(Book("Clean Code", "Robert Martin", 2008));
    library.addBook(Book("The Pragmatic Programmer", "Hunt & Thomas", 1999));

    auto bookIter = library.createIterator();
    std::cout << "   Books in library:\n";
    int bookCount = 0;
    while (bookIter->hasNext()) {
        Book book = bookIter->next();
        std::cout << "   - " << book.toString() << "\n";
        ++bookCount;
    }
    assert(bookCount == 4);
    std::cout << "   [OK] Iterated through " << bookCount << " books\n\n";

    std::cout << "6. Reverse chronological book iteration:\n";
    auto reverseBookIter = library.createReverseChronologicalIterator();
    std::cout << "   Books in reverse order:\n";
    std::vector<int> years;
    while (reverseBookIter->hasNext()) {
        Book book = reverseBookIter->next();
        std::cout << "   - " << book.toString() << "\n";
        years.push_back(book.year);
    }
    assert(years[0] == 1999);
    assert(years[3] == 1978);
    std::cout << "   [OK] Reverse iteration completed\n\n";

    std::cout << "7. Filter iterator (even numbers only):\n";
    Collection<int> allNumbers;
    for (int i = 1; i <= 10; ++i) {
        allNumbers.add(i);
    }

    auto evenFilter = [](int n) { return n % 2 == 0; };
    FilterIterator<int, decltype(evenFilter)> evenIter(allNumbers.getItems(), evenFilter);

    std::cout << "   Even numbers: ";
    std::vector<int> evenResult;
    while (evenIter.hasNext()) {
        int val = evenIter.next();
        std::cout << val << " ";
        evenResult.push_back(val);
    }
    std::cout << "\n";
    assert(evenResult.size() == 5);
    assert(evenResult[0] == 2 && evenResult[4] == 10);
    std::cout << "   [OK] Filter iterator works correctly\n\n";

    std::cout << "8. String collection iteration:\n";
    Collection<std::string> fruits;
    fruits.add("Apple");
    fruits.add("Banana");
    fruits.add("Cherry");
    fruits.add("Date");

    auto fruitIter = fruits.createIterator();
    std::cout << "   Fruits: ";
    int fruitCount = 0;
    while (fruitIter->hasNext()) {
        std::string fruit = fruitIter->next();
        std::cout << fruit << " ";
        ++fruitCount;
    }
    std::cout << "\n";
    assert(fruitCount == 4);
    std::cout << "   [OK] String iteration completed\n\n";

    std::cout << "9. Empty collection iteration:\n";
    Collection<int> empty;
    auto emptyIter = empty.createIterator();
    assert(!emptyIter->hasNext());
    std::cout << "   [OK] Empty collection has no elements to iterate\n\n";

    std::cout << "10. Exception handling (iterating past end):\n";
    Collection<int> small;
    small.add(42);
    auto smallIter = small.createIterator();
    smallIter->next(); // Valid call

    try {
        smallIter->next(); // Should throw
        assert(false); // Should not reach here
    } catch (const std::out_of_range& e) {
        std::cout << "   Caught expected exception: " << e.what() << "\n";
        std::cout << "   [OK] Exception thrown when iterating past end\n\n";
    }

    std::cout << "Iterator pattern demo completed successfully!\n";
    std::cout << "\nKey Benefits of Iterator Pattern:\n";
    std::cout << "- Provides uniform interface for traversing different collections\n";
    std::cout << "- Encapsulates collection internal structure\n";
    std::cout << "- Supports multiple simultaneous traversals\n";
    std::cout << "- Enables different traversal strategies (forward, backward, filtered)\n";
    std::cout << "- Separates collection from traversal algorithms\n";

    return 0;
}
