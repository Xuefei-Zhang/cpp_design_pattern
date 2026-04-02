#ifndef ITERATOR_PATTERNS_H
#define ITERATOR_PATTERNS_H

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// Forward declarations
template<typename T>
class Collection;

// Abstract Iterator Interface
template<typename T>
class Iterator {
public:
    virtual ~Iterator() = default;
    virtual bool hasNext() const = 0;
    virtual T next() = 0;
    virtual void reset() = 0;
};

// Abstract Collection Interface
template<typename T>
class Iterable {
public:
    virtual ~Iterable() = default;
    virtual std::unique_ptr<Iterator<T>> createIterator() = 0;
};

// Concrete Forward Iterator
template<typename T>
class ForwardIterator : public Iterator<T> {
private:
    const std::vector<T>& data_;
    std::size_t position_;

public:
    explicit ForwardIterator(const std::vector<T>& data)
        : data_(data), position_(0) {}

    bool hasNext() const override {
        return position_ < data_.size();
    }

    T next() override {
        if (!hasNext()) {
            throw std::out_of_range("No more elements");
        }
        return data_[position_++];
    }

    void reset() override {
        position_ = 0;
    }
};

// Concrete Backward Iterator
template<typename T>
class BackwardIterator : public Iterator<T> {
private:
    const std::vector<T>& data_;
    std::size_t position_;

public:
    explicit BackwardIterator(const std::vector<T>& data)
        : data_(data), position_(data.size()) {}

    bool hasNext() const override {
        return position_ > 0;
    }

    T next() override {
        if (!hasNext()) {
            throw std::out_of_range("No more elements");
        }
        return data_[--position_];
    }

    void reset() override {
        position_ = data_.size();
    }
};

// Concrete Skip Iterator (every N-th element)
template<typename T>
class SkipIterator : public Iterator<T> {
private:
    const std::vector<T>& data_;
    std::size_t position_;
    std::size_t step_;

public:
    SkipIterator(const std::vector<T>& data, std::size_t step)
        : data_(data), position_(0), step_(step > 0 ? step : 1) {}

    bool hasNext() const override {
        return position_ < data_.size();
    }

    T next() override {
        if (!hasNext()) {
            throw std::out_of_range("No more elements");
        }
        T result = data_[position_];
        position_ += step_;
        return result;
    }

    void reset() override {
        position_ = 0;
    }
};

// Concrete Collection
template<typename T>
class Collection : public Iterable<T> {
private:
    std::vector<T> items_;

public:
    void add(const T& item) {
        items_.push_back(item);
    }

    std::size_t size() const {
        return items_.size();
    }

    const T& get(std::size_t index) const {
        return items_.at(index);
    }

    std::unique_ptr<Iterator<T>> createIterator() override {
        return std::make_unique<ForwardIterator<T>>(items_);
    }

    std::unique_ptr<Iterator<T>> createForwardIterator() {
        return std::make_unique<ForwardIterator<T>>(items_);
    }

    std::unique_ptr<Iterator<T>> createBackwardIterator() {
        return std::make_unique<BackwardIterator<T>>(items_);
    }

    std::unique_ptr<Iterator<T>> createSkipIterator(std::size_t step) {
        return std::make_unique<SkipIterator<T>>(items_, step);
    }

    const std::vector<T>& getItems() const {
        return items_;
    }
};

// Book example for demonstration
struct Book {
    std::string title;
    std::string author;
    int year;

    Book(const std::string& t, const std::string& a, int y)
        : title(t), author(a), year(y) {}

    std::string toString() const {
        return "\"" + title + "\" by " + author + " (" + std::to_string(year) + ")";
    }
};

// BookCollection with specialized iterators
class BookCollection : public Iterable<Book> {
private:
    std::vector<Book> books_;

public:
    void addBook(const Book& book) {
        books_.push_back(book);
    }

    std::size_t getCount() const {
        return books_.size();
    }

    std::unique_ptr<Iterator<Book>> createIterator() override {
        return std::make_unique<ForwardIterator<Book>>(books_);
    }

    std::unique_ptr<Iterator<Book>> createChronologicalIterator() {
        // For simplicity, we assume books are already sorted by year
        return std::make_unique<ForwardIterator<Book>>(books_);
    }

    std::unique_ptr<Iterator<Book>> createReverseChronologicalIterator() {
        return std::make_unique<BackwardIterator<Book>>(books_);
    }
};

// Filter Iterator (filters elements based on predicate)
template<typename T, typename Predicate>
class FilterIterator : public Iterator<T> {
private:
    const std::vector<T>& data_;
    Predicate predicate_;
    std::size_t position_;

    void advanceToNext() {
        while (position_ < data_.size() && !predicate_(data_[position_])) {
            ++position_;
        }
    }

public:
    FilterIterator(const std::vector<T>& data, Predicate pred)
        : data_(data), predicate_(pred), position_(0) {
        advanceToNext();
    }

    bool hasNext() const override {
        return position_ < data_.size();
    }

    T next() override {
        if (!hasNext()) {
            throw std::out_of_range("No more elements");
        }
        T result = data_[position_++];
        advanceToNext();
        return result;
    }

    void reset() override {
        position_ = 0;
        advanceToNext();
    }
};

// Inline usage examples:
// Collection<int> numbers;
// numbers.add(1); numbers.add(2); numbers.add(3);
// auto iter = numbers.createIterator();
// while (iter->hasNext()) {
//     std::cout << iter->next() << std::endl;
// }

#endif // ITERATOR_PATTERNS_H
