#ifndef FRIEND_CLASSES_ADVANCED_H
#define FRIEND_CLASSES_ADVANCED_H

#include <iostream>
#include <vector>
#include <memory>
#include <cmath>

// Advanced example: Matrix class with friend operations
class Matrix {
private:
    std::vector<std::vector<double>> data;
    size_t rows, cols;
    
public:
    Matrix(size_t r, size_t c) : rows(r), cols(c) {
        data.resize(rows, std::vector<double>(cols, 0.0));
    }
    
    Matrix(const std::initializer_list<std::initializer_list<double>>& init) {
        rows = init.size();
        cols = init.begin()->size();
        data.resize(rows, std::vector<double>(cols, 0.0));
        
        size_t i = 0;
        for (const auto& row : init) {
            size_t j = 0;
            for (double val : row) {
                data[i][j] = val;
                j++;
            }
            i++;
        }
    }
    
    size_t getRows() const { return rows; }
    size_t getCols() const { return cols; }
    
    double& operator()(size_t r, size_t c) { return data[r][c]; }
    const double& operator()(size_t r, size_t c) const { return data[r][c]; }
    
    // Friend function for matrix addition
    friend Matrix operator+(const Matrix& a, const Matrix& b);
    
    // Friend function for matrix multiplication
    friend Matrix operator*(const Matrix& a, const Matrix& b);
    
    // Friend class for advanced operations
    friend class MatrixOperations;
    
    void display() const {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                std::cout << data[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
};

// Friend function implementations
Matrix operator+(const Matrix& a, const Matrix& b) {
    if (a.rows != b.rows || a.cols != b.cols) {
        throw std::invalid_argument("Matrix dimensions must match for addition");
    }
    
    Matrix result(a.rows, a.cols);
    for (size_t i = 0; i < a.rows; ++i) {
        for (size_t j = 0; j < a.cols; ++j) {
            result.data[i][j] = a.data[i][j] + b.data[i][j];  // Access private data
        }
    }
    return result;
}

Matrix operator*(const Matrix& a, const Matrix& b) {
    if (a.cols != b.rows) {
        throw std::invalid_argument("Matrix dimensions incompatible for multiplication");
    }
    
    Matrix result(a.rows, b.cols);
    for (size_t i = 0; i < a.rows; ++i) {
        for (size_t j = 0; j < b.cols; ++j) {
            for (size_t k = 0; k < a.cols; ++k) {
                result.data[i][j] += a.data[i][k] * b.data[k][j];  // Access private data
            }
        }
    }
    return result;
}

class MatrixOperations {
public:
    // Friend class can access private members
    static Matrix transpose(const Matrix& m) {
        Matrix result(m.cols, m.rows);  // Access private dimensions
        for (size_t i = 0; i < m.rows; ++i) {
            for (size_t j = 0; j < m.cols; ++j) {
                result.data[j][i] = m.data[i][j];  // Access private data
            }
        }
        return result;
    }
    
    static double determinant(const Matrix& m) {
        if (m.rows != m.cols) {
            throw std::invalid_argument("Determinant only defined for square matrices");
        }
        
        // Simplified determinant calculation for 2x2 and 3x3 matrices
        if (m.rows == 2) {
            return m.data[0][0] * m.data[1][1] - m.data[0][1] * m.data[1][0];  // Access private data
        } else if (m.rows == 3) {
            return m.data[0][0] * (m.data[1][1] * m.data[2][2] - m.data[1][2] * m.data[2][1])
                 - m.data[0][1] * (m.data[1][0] * m.data[2][2] - m.data[1][2] * m.data[2][0])
                 + m.data[0][2] * (m.data[1][0] * m.data[2][1] - m.data[1][1] * m.data[2][0]);  // Access private data
        }
        // For larger matrices, more complex algorithms would be needed
        return 0.0;
    }
    
    static void printRawData(const Matrix& m) {
        std::cout << "Raw data access from friend class:" << std::endl;
        for (size_t i = 0; i < m.rows; ++i) {  // Access private dimensions
            for (size_t j = 0; j < m.cols; ++j) {  // Access private dimensions
                std::cout << m.data[i][j] << " ";  // Access private data member directly
            }
            std::cout << std::endl;
        }
    }
};

// Advanced example: Complex number class with friend operations
class Complex {
private:
    double real, imag;
    
public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}
    
    // Friend functions for arithmetic operations
    friend Complex operator+(const Complex& a, const Complex& b);
    friend Complex operator-(const Complex& a, const Complex& b);
    friend Complex operator*(const Complex& a, const Complex& b);
    friend Complex operator/(const Complex& a, const Complex& b);
    
    // Friend function for stream output
    friend std::ostream& operator<<(std::ostream& os, const Complex& c);
    
    // Friend class for complex number utilities
    friend class ComplexUtils;
    
    double getReal() const { return real; }
    double getImag() const { return imag; }
};

// Friend function implementations
Complex operator+(const Complex& a, const Complex& b) {
    return Complex(a.real + b.real, a.imag + b.imag);  // Access private members
}

Complex operator-(const Complex& a, const Complex& b) {
    return Complex(a.real - b.real, a.imag - b.imag);  // Access private members
}

Complex operator*(const Complex& a, const Complex& b) {
    // (a + bi) * (c + di) = (ac - bd) + (ad + bc)i
    return Complex(
        a.real * b.real - a.imag * b.imag,  // Access private members
        a.real * b.imag + a.imag * b.real   // Access private members
    );
}

Complex operator/(const Complex& a, const Complex& b) {
    // (a + bi) / (c + di) = [(a + bi) * (c - di)] / (c² + d²)
    double denominator = b.real * b.real + b.imag * b.imag;
    if (denominator == 0) {
        throw std::invalid_argument("Division by zero complex number");
    }
    
    return Complex(
        (a.real * b.real + a.imag * b.imag) / denominator,  // Access private members
        (a.imag * b.real - a.real * b.imag) / denominator   // Access private members
    );
}

std::ostream& operator<<(std::ostream& os, const Complex& c) {
    os << c.real << (c.imag >= 0 ? "+" : "") << c.imag << "i";  // Access private members
    return os;
}

class ComplexUtils {
public:
    static double magnitude(const Complex& c) {
        return std::sqrt(c.real * c.real + c.imag * c.imag);  // Access private members
    }
    
    static Complex conjugate(const Complex& c) {
        return Complex(c.real, -c.imag);  // Access private members
    }
    
    static double getRealPart(const Complex& c) {
        return c.real;  // Access private member
    }
    
    static double getImagPart(const Complex& c) {
        return c.imag;  // Access private member
    }
};

// Advanced example: Iterator pattern with friend access
template<typename T>
class Container {
private:
    std::vector<T> elements;
    
public:
    Container() = default;
    
    void add(const T& item) {
        elements.push_back(item);
    }
    
    size_t size() const { return elements.size(); }
    
    // Friend iterator class
    friend class Iterator;
    
    class Iterator {
    private:
        Container* container;
        size_t index;
        
    public:
        Iterator(Container* c, size_t i) : container(c), index(i) {}
        
        T& operator*() {
            return container->elements[index];  // Access private member of Container
        }
        
        const T& operator*() const {
            return container->elements[index];  // Access private member of Container
        }
        
        Iterator& operator++() {
            ++index;
            return *this;
        }
        
        bool operator!=(const Iterator& other) const {
            return index != other.index || container != other.container;
        }
    };
    
    Iterator begin() { return Iterator(this, 0); }
    Iterator end() { return Iterator(this, elements.size()); }
};

#endif // FRIEND_CLASSES_ADVANCED_H