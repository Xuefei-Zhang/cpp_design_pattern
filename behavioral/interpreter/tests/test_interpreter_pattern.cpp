#include "../include/interpreter_patterns.h"

#include <cassert>
#include <iostream>
#include <memory>

int main() {
    std::cout << "=== Interpreter Pattern Demo: Expression Evaluation ===\n\n";

    std::cout << "1. Basic arithmetic expression evaluation:\n";
    Context ctx1;
    ctx1.setVariable("x", 10);
    ctx1.setVariable("y", 5);

    // x + y
    auto expr1 = std::make_unique<AddExpression>(
        std::make_unique<VariableExpression>("x"),
        std::make_unique<VariableExpression>("y")
    );

    int result1 = expr1->interpret(ctx1);
    std::cout << "   Expression: " << expr1->toString() << " = " << result1 << "\n";
    assert(result1 == 15);
    std::cout << "   [OK] x + y = 15\n\n";

    std::cout << "2. Subtraction expression:\n";
    // x - y
    auto expr2 = std::make_unique<SubtractExpression>(
        std::make_unique<VariableExpression>("x"),
        std::make_unique<VariableExpression>("y")
    );

    int result2 = expr2->interpret(ctx1);
    std::cout << "   Expression: " << expr2->toString() << " = " << result2 << "\n";
    assert(result2 == 5);
    std::cout << "   [OK] x - y = 5\n\n";

    std::cout << "3. Complex nested expression:\n";
    // (x + y) * (x - y) = (10 + 5) * (10 - 5) = 15 * 5 = 75
    auto expr3 = std::make_unique<MultiplyExpression>(
        std::make_unique<AddExpression>(
            std::make_unique<VariableExpression>("x"),
            std::make_unique<VariableExpression>("y")
        ),
        std::make_unique<SubtractExpression>(
            std::make_unique<VariableExpression>("x"),
            std::make_unique<VariableExpression>("y")
        )
    );

    int result3 = expr3->interpret(ctx1);
    std::cout << "   Expression: " << expr3->toString() << " = " << result3 << "\n";
    assert(result3 == 75);
    std::cout << "   [OK] (x + y) * (x - y) = 75\n\n";

    std::cout << "4. Division expression:\n";
    Context ctx2;
    ctx2.setVariable("a", 20);
    ctx2.setVariable("b", 4);

    // a / b
    auto expr4 = std::make_unique<DivideExpression>(
        std::make_unique<VariableExpression>("a"),
        std::make_unique<VariableExpression>("b")
    );

    int result4 = expr4->interpret(ctx2);
    std::cout << "   Expression: " << expr4->toString() << " = " << result4 << "\n";
    assert(result4 == 5);
    std::cout << "   [OK] a / b = 5\n\n";

    std::cout << "5. Using number literals:\n";
    // 100 + 50
    auto expr5 = std::make_unique<AddExpression>(
        std::make_unique<NumberExpression>(100),
        std::make_unique<NumberExpression>(50)
    );

    Context emptyCtx;
    int result5 = expr5->interpret(emptyCtx);
    std::cout << "   Expression: " << expr5->toString() << " = " << result5 << "\n";
    assert(result5 == 150);
    std::cout << "   [OK] 100 + 50 = 150\n\n";

    std::cout << "6. Boolean comparison expressions:\n";
    Context ctx3;
    ctx3.setVariable("m", 10);
    ctx3.setVariable("n", 20);

    // m < n
    auto expr6 = std::make_unique<LessThanExpression>(
        std::make_unique<VariableExpression>("m"),
        std::make_unique<VariableExpression>("n")
    );

    int result6 = expr6->interpret(ctx3);
    std::cout << "   Expression: " << expr6->toString() << " = " << (result6 ? "true" : "false") << "\n";
    assert(result6 == 1);
    std::cout << "   [OK] m < n = true\n\n";

    // m > n
    auto expr7 = std::make_unique<GreaterThanExpression>(
        std::make_unique<VariableExpression>("m"),
        std::make_unique<VariableExpression>("n")
    );

    int result7 = expr7->interpret(ctx3);
    std::cout << "   Expression: " << expr7->toString() << " = " << (result7 ? "true" : "false") << "\n";
    assert(result7 == 0);
    std::cout << "   [OK] m > n = false\n\n";

    std::cout << "7. Using ExpressionEvaluator:\n";
    ExpressionEvaluator evaluator;
    evaluator.setVariable("price", 100);
    evaluator.setVariable("discount", 20);

    // price - discount
    auto expr8 = std::make_unique<SubtractExpression>(
        std::make_unique<VariableExpression>("price"),
        std::make_unique<VariableExpression>("discount")
    );

    int finalPrice = evaluator.evaluate(*expr8);
    std::cout << "   Expression: " << evaluator.getExpressionString(*expr8) << " = " << finalPrice << "\n";
    assert(finalPrice == 80);
    std::cout << "   [OK] price - discount = 80\n\n";

    std::cout << "8. Error handling (division by zero):\n";
    try {
        Context ctx4;
        ctx4.setVariable("val", 10);

        auto exprErr = std::make_unique<DivideExpression>(
            std::make_unique<VariableExpression>("val"),
            std::make_unique<NumberExpression>(0)
        );

        exprErr->interpret(ctx4);
        assert(false); // Should not reach here
    } catch (const std::runtime_error& e) {
        std::cout << "   Caught expected exception: " << e.what() << "\n";
        std::cout << "   [OK] Division by zero handled correctly\n\n";
    }

    std::cout << "9. Error handling (undefined variable):\n";
    try {
        Context ctx5;
        auto exprErr2 = std::make_unique<VariableExpression>("undefined");
        exprErr2->interpret(ctx5);
        assert(false); // Should not reach here
    } catch (const std::runtime_error& e) {
        std::cout << "   Caught expected exception: " << e.what() << "\n";
        std::cout << "   [OK] Undefined variable handled correctly\n\n";
    }

    std::cout << "10. Complex formula: (a + b) * c / d:\n";
    Context ctx6;
    ctx6.setVariable("a", 10);
    ctx6.setVariable("b", 5);
    ctx6.setVariable("c", 4);
    ctx6.setVariable("d", 2);

    // ((a + b) * c) / d
    auto expr9 = std::make_unique<DivideExpression>(
        std::make_unique<MultiplyExpression>(
            std::make_unique<AddExpression>(
                std::make_unique<VariableExpression>("a"),
                std::make_unique<VariableExpression>("b")
            ),
            std::make_unique<VariableExpression>("c")
        ),
        std::make_unique<VariableExpression>("d")
    );

    int result9 = expr9->interpret(ctx6);
    std::cout << "   Expression: " << expr9->toString() << " = " << result9 << "\n";
    assert(result9 == 30);
    std::cout << "   [OK] ((a + b) * c) / d = 30\n\n";

    std::cout << "Interpreter pattern demo completed successfully!\n";
    std::cout << "\nKey Benefits of Interpreter Pattern:\n";
    std::cout << "- Defines a grammar for language interpretation\n";
    std::cout << "- Easy to add new expressions by creating new classes\n";
    std::cout << "- Separates grammar rules from interpretation logic\n";
    std::cout << "- Good for domain-specific languages and expression evaluation\n";
    std::cout << "- Composite structure makes complex expressions possible\n";

    return 0;
}
