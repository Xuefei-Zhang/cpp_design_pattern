#ifndef INTERPRETER_PATTERNS_H
#define INTERPRETER_PATTERNS_H

#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// Context for storing variable values
class Context {
private:
    std::map<std::string, int> variables_;

public:
    void setVariable(const std::string& name, int value) {
        variables_[name] = value;
    }

    int getVariable(const std::string& name) const {
        auto it = variables_.find(name);
        if (it == variables_.end()) {
            throw std::runtime_error("Variable not found: " + name);
        }
        return it->second;
    }

    bool hasVariable(const std::string& name) const {
        return variables_.find(name) != variables_.end();
    }

    void clear() {
        variables_.clear();
    }

    std::size_t size() const {
        return variables_.size();
    }
};

// Abstract Expression
class Expression {
public:
    virtual ~Expression() = default;
    virtual int interpret(const Context& context) const = 0;
    virtual std::string toString() const = 0;
};

// Terminal Expression: Number
class NumberExpression : public Expression {
private:
    int value_;

public:
    explicit NumberExpression(int value) : value_(value) {}

    int interpret(const Context&) const override {
        return value_;
    }

    std::string toString() const override {
        return std::to_string(value_);
    }
};

// Terminal Expression: Variable
class VariableExpression : public Expression {
private:
    std::string name_;

public:
    explicit VariableExpression(const std::string& name) : name_(name) {}

    int interpret(const Context& context) const override {
        return context.getVariable(name_);
    }

    std::string toString() const override {
        return name_;
    }
};

// Non-terminal Expression: Addition
class AddExpression : public Expression {
private:
    std::unique_ptr<Expression> left_;
    std::unique_ptr<Expression> right_;

public:
    AddExpression(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : left_(std::move(left)), right_(std::move(right)) {}

    int interpret(const Context& context) const override {
        return left_->interpret(context) + right_->interpret(context);
    }

    std::string toString() const override {
        return "(" + left_->toString() + " + " + right_->toString() + ")";
    }
};

// Non-terminal Expression: Subtraction
class SubtractExpression : public Expression {
private:
    std::unique_ptr<Expression> left_;
    std::unique_ptr<Expression> right_;

public:
    SubtractExpression(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : left_(std::move(left)), right_(std::move(right)) {}

    int interpret(const Context& context) const override {
        return left_->interpret(context) - right_->interpret(context);
    }

    std::string toString() const override {
        return "(" + left_->toString() + " - " + right_->toString() + ")";
    }
};

// Non-terminal Expression: Multiplication
class MultiplyExpression : public Expression {
private:
    std::unique_ptr<Expression> left_;
    std::unique_ptr<Expression> right_;

public:
    MultiplyExpression(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : left_(std::move(left)), right_(std::move(right)) {}

    int interpret(const Context& context) const override {
        return left_->interpret(context) * right_->interpret(context);
    }

    std::string toString() const override {
        return "(" + left_->toString() + " * " + right_->toString() + ")";
    }
};

// Non-terminal Expression: Division
class DivideExpression : public Expression {
private:
    std::unique_ptr<Expression> left_;
    std::unique_ptr<Expression> right_;

public:
    DivideExpression(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : left_(std::move(left)), right_(std::move(right)) {}

    int interpret(const Context& context) const override {
        int rightValue = right_->interpret(context);
        if (rightValue == 0) {
            throw std::runtime_error("Division by zero");
        }
        return left_->interpret(context) / rightValue;
    }

    std::string toString() const override {
        return "(" + left_->toString() + " / " + right_->toString() + ")";
    }
};

// Boolean Expression Base Class
class BooleanExpression : public Expression {
public:
    virtual ~BooleanExpression() = default;
    // Returns 1 for true, 0 for false
};

// Greater Than Expression
class GreaterThanExpression : public BooleanExpression {
private:
    std::unique_ptr<Expression> left_;
    std::unique_ptr<Expression> right_;

public:
    GreaterThanExpression(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : left_(std::move(left)), right_(std::move(right)) {}

    int interpret(const Context& context) const override {
        return left_->interpret(context) > right_->interpret(context) ? 1 : 0;
    }

    std::string toString() const override {
        return "(" + left_->toString() + " > " + right_->toString() + ")";
    }
};

// Less Than Expression
class LessThanExpression : public BooleanExpression {
private:
    std::unique_ptr<Expression> left_;
    std::unique_ptr<Expression> right_;

public:
    LessThanExpression(std::unique_ptr<Expression> left, std::unique_ptr<Expression> right)
        : left_(std::move(left)), right_(std::move(right)) {}

    int interpret(const Context& context) const override {
        return left_->interpret(context) < right_->interpret(context) ? 1 : 0;
    }

    std::string toString() const override {
        return "(" + left_->toString() + " < " + right_->toString() + ")";
    }
};

// Simple Expression Evaluator
class ExpressionEvaluator {
private:
    Context context_;

public:
    void setVariable(const std::string& name, int value) {
        context_.setVariable(name, value);
    }

    int evaluate(const Expression& expr) const {
        return expr.interpret(context_);
    }

    std::string getExpressionString(const Expression& expr) const {
        return expr.toString();
    }

    const Context& getContext() const {
        return context_;
    }
};

// Inline usage examples:
// Context ctx;
// ctx.setVariable("x", 10);
// ctx.setVariable("y", 5);
// auto expr = std::make_unique<AddExpression>(
//     std::make_unique<VariableExpression>("x"),
//     std::make_unique<VariableExpression>("y")
// );
// int result = expr->interpret(ctx); // result = 15

#endif // INTERPRETER_PATTERNS_H
