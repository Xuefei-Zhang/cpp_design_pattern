#include "structural/decorator_patterns.h"
#include <iostream>
#include <memory>
#include <vector>
#include <chrono>

int main() {
    std::cout << "=== Decorator Pattern Demo: Shape Enhancement System ===\n\n";
    
    // Test 1: Basic Shape Creation
    std::cout << "1. Basic Shape Creation:\n";
    auto circle = std::make_unique<Circle>(5.0, "red");
    circle->draw();
    std::cout << "Description: " << circle->getDescription() << std::endl;
    std::cout << "Area: " << circle->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 2: Single Decorator Application
    std::cout << "2. Single Decorator Application:\n";
    auto circleWithBorder = std::make_unique<BorderDecorator>(
        std::make_unique<Circle>(4.0, "blue"), 3, "black"
    );
    circleWithBorder->draw();
    std::cout << "Description: " << circleWithBorder->getDescription() << std::endl;
    std::cout << "Area: " << circleWithBorder->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 3: Multiple Decorators Applied Sequentially
    std::cout << "3. Multiple Decorators Applied Sequentially:\n";
    auto decoratedCircle = std::make_unique<ShadowDecorator>(
        std::make_unique<FillDecorator>(
            std::make_unique<BorderDecorator>(
                std::make_unique<Circle>(3.0, "green"), 2, "darkgreen"
            ), 
            "lightgreen", true
        ), 
        4, "gray"
    );
    decoratedCircle->draw();
    std::cout << "Description: " << decoratedCircle->getDescription() << std::endl;
    std::cout << "Area: " << decoratedCircle->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 4: Rectangle with Multiple Decorators
    std::cout << "4. Rectangle with Multiple Decorators:\n";
    auto decoratedRect = std::make_unique<GradientDecorator>(
        std::make_unique<BorderDecorator>(
            std::make_unique<Rectangle>(8.0, 6.0, "blue"), 1, "orange"
        ),
        "yellow", "purple", "vertical"
    );
    decoratedRect->draw();
    std::cout << "Description: " << decoratedRect->getDescription() << std::endl;
    std::cout << "Area: " << decoratedRect->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 5: Fluent Decorator Usage
    std::cout << "5. Fluent Decorator Usage:\n";
    auto fluentCircle = FluentDecorator(std::make_unique<Circle>(2.0, "yellow"))
                            .border(2, "red")
                            .fill("orange", true)
                            .shadow(3, "lightgray")
                            .scale(1.5, 1.5)
                            .animate("rotate", 1.2)
                            .build();
    fluentCircle->draw();
    std::cout << "Description: " << fluentCircle->getDescription() << std::endl;
    std::cout << std::endl;
    
    // Test 6: Decorator Factory Usage
    std::cout << "6. Decorator Factory Usage:\n";
    auto originalTriangle = std::make_unique<Triangle>(6.0, 4.0, "cyan");
    std::cout << "Original: ";
    originalTriangle->draw();
    
    auto factoryDecorated = DecoratorFactory::addEffects(
        std::make_unique<Triangle>(6.0, 4.0, "cyan"),
        true, 2, "darkblue",  // border
        true, "lightblue", true,  // fill
        true, 3, "lightgray"  // shadow
    );
    
    std::cout << "Factory decorated: ";
    factoryDecorated->draw();
    std::cout << "Description: " << factoryDecorated->getDescription() << std::endl;
    std::cout << std::endl;
    
    // Test 7: Complex Decoration Patterns
    std::cout << "7. Complex Decoration Patterns (Nested Decorators):\n";
    auto complexShape = std::make_unique<AnimationDecorator>(
        std::make_unique<ScaleDecorator>(
            std::make_unique<GradientDecorator>(
                std::make_unique<ShadowDecorator>(
                    std::make_unique<BorderDecorator>(
                        std::make_unique<Rectangle>(5.0, 3.0, "purple"),
                        2, "white"
                    ),
                    3, "darkgray"
                ),
                "red", "blue", "diagonal"
            ),
            1.2, 1.2
        ),
        "pulse", 0.8
    );
    
    complexShape->draw();
    std::cout << "Description: " << complexShape->getDescription() << std::endl;
    std::cout << "Area: " << complexShape->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 8: Triangle with Compound Decorator
    std::cout << "8. Triangle with Compound Decorator:\n";
    auto compoundTriangle = std::make_unique<CompoundDecorator>(
        std::make_unique<Triangle>(7.0, 5.0, "magenta"),
        true, 3, "darkmagenta",  // border
        true, "pink", true,     // fill
        true, 4, "lightgray",   // shadow
        true, "bounce", 1.5     // animation
    );
    
    compoundTriangle->draw();
    std::cout << "Description: " << compoundTriangle->getDescription() << std::endl;
    std::cout << std::endl;
    
    // Test 9: Comparing Decorated vs Undecorated
    std::cout << "9. Comparing Decorated vs Undecorated:\n";
    
    std::cout << "Undecorated Circle:\n";
    auto plainCircle = std::make_unique<Circle>(3.0, "red");
    plainCircle->draw();
    std::cout << "Area: " << plainCircle->getArea() << std::endl;
    
    std::cout << "\nHeavily Decorated Circle:\n";
    auto heavilyDecoratedCircle = std::make_unique<ScaleDecorator>(
        std::make_unique<AnimationDecorator>(
            std::make_unique<GradientDecorator>(
                std::make_unique<ShadowDecorator>(
                    std::make_unique<BorderDecorator>(
                        std::make_unique<FillDecorator>(
                            std::make_unique<Circle>(3.0, "red"), "orange", true
                        ),
                        3, "black"
                    ),
                    5, "gray"
                ),
                "red", "yellow", "radial"
            ),
            "spin", 2.0
        ),
        1.8, 1.8
    );
    heavilyDecoratedCircle->draw();
    std::cout << "Area: " << heavilyDecoratedCircle->getArea() << std::endl;
    std::cout << "Description: " << heavilyDecoratedCircle->getDescription() << std::endl;
    std::cout << std::endl;
    
    // Test 10: Rectangle Variations
    std::cout << "10. Rectangle Variations with Different Decorators:\n";
    
    std::vector<std::unique_ptr<Shape>> rectangles;
    
    // Plain rectangle
    rectangles.push_back(std::make_unique<Rectangle>(4.0, 3.0, "white"));
    
    // Border only
    rectangles.push_back(std::make_unique<BorderDecorator>(
        std::make_unique<Rectangle>(4.0, 3.0, "white"), 2, "black"
    ));
    
    // Fill only
    rectangles.push_back(std::make_unique<FillDecorator>(
        std::make_unique<Rectangle>(4.0, 3.0, "white"), "yellow", true
    ));
    
    // Border + Fill
    rectangles.push_back(std::make_unique<FillDecorator>(
        std::make_unique<BorderDecorator>(
            std::make_unique<Rectangle>(4.0, 3.0, "white"), 1, "blue"
        ),
        "lightblue", true
    ));
    
    // Border + Fill + Shadow
    rectangles.push_back(std::make_unique<ShadowDecorator>(
        std::make_unique<FillDecorator>(
            std::make_unique<BorderDecorator>(
                std::make_unique<Rectangle>(4.0, 3.0, "white"), 1, "blue"
            ),
            "lightblue", true
        ),
        3, "gray"
    ));
    
    for (size_t i = 0; i < rectangles.size(); ++i) {
        std::cout << "Rectangle " << i + 1 << ": ";
        rectangles[i]->draw();
        std::cout << "  Description: " << rectangles[i]->getDescription() << std::endl;
    }
    std::cout << std::endl;
    
    // Test 11: Fluency and Chaining
    std::cout << "11. Fluency and Chaining with Different Shapes:\n";
    
    auto redCircle = FluentDecorator(std::make_unique<Circle>(3.0, "red"))
                         .border(2, "darkred")
                         .fill("lightcoral", true)
                         .gradient("red", "darkred", "radial")
                         .build();

    auto blueRect = FluentDecorator(std::make_unique<Rectangle>(6.0, 4.0, "blue"))
                        .shadow(4, "darkblue")
                        .border(1, "white")
                        .animate("pulse", 1.0)
                        .build();

    std::cout << "Fluent Red Circle: ";
    redCircle->draw();
    std::cout << "Description: " << redCircle->getDescription() << std::endl;

    std::cout << "Fluent Blue Rectangle: ";
    blueRect->draw();
    std::cout << "Description: " << blueRect->getDescription() << std::endl;
    std::cout << std::endl;
    
    // Test 12: Area Calculation with Decorators
    std::cout << "12. Area Calculation with Scaling Decorator:\n";
    
    std::cout << "Original Circle (radius 3): Area = " << Circle(3.0, "test").getArea() << std::endl;
    
    auto scaledCircle1 = std::make_unique<ScaleDecorator>(
        std::make_unique<Circle>(3.0, "test"), 2.0, 2.0
    );
    std::cout << "Scaled 2x,2x: Area = " << scaledCircle1->getArea() << std::endl;
    
    auto scaledCircle2 = std::make_unique<ScaleDecorator>(
        std::make_unique<ScaleDecorator>(
            std::make_unique<Circle>(3.0, "test"), 2.0, 2.0
        ),
        1.5, 1.5
    );
    std::cout << "Scaled 2x,2x then 1.5x,1.5x: Area = " << scaledCircle2->getArea() << std::endl;
    std::cout << "Expected: " << 3.14159 * 3 * 3 * 2 * 2 * 1.5 * 1.5 << " (manual calc)" << std::endl;
    std::cout << std::endl;
    
    // Test 13: Performance Test - Creating Many Decorated Shapes
    std::cout << "13. Performance Test - Creating Many Decorated Shapes:\n";
    
    auto start = std::chrono::high_resolution_clock::now();
    
    std::vector<std::unique_ptr<Shape>> decoratedShapes;
    for (int i = 0; i < 1000; ++i) {
        decoratedShapes.push_back(
            std::make_unique<BorderDecorator>(
                std::make_unique<FillDecorator>(
                    std::make_unique<Circle>(1.0 + (i % 5), "color" + std::to_string(i % 3)),
                    "fill_color" + std::to_string(i % 4), true
                ),
                1, "border_color" + std::to_string(i % 2)
            )
        );
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Created 1000 decorated shapes in: " << duration.count() << " microseconds" << std::endl;
    std::cout << "First few shapes:\n";
    for (int i = 0; i < 3 && i < 1000; ++i) {
        std::cout << "  " << i + 1 << ": " << decoratedShapes[i]->getDescription() << std::endl;
    }
    std::cout << std::endl;
    
    // Test 14: Decorator Pattern Benefits Demonstration
    std::cout << "14. Decorator Pattern Benefits Demonstration:\n";
    
    std::cout << "- Single Responsibility: Each decorator focuses on one enhancement\n";
    std::cout << "- Runtime Flexibility: Can add/remove decorations dynamically\n";
    std::cout << "- Open/Closed Principle: Open for extension, closed for modification\n";
    std::cout << "- Transparency: Decorated objects behave like original objects\n";
    std::cout << "- Composition Over Inheritance: Build complexity through composition\n";
    std::cout << "- Reusability: Same decorators can be applied to different shapes\n";
    std::cout << std::endl;
    
    // Test 15: Real-world Scenario - UI Component Styling
    std::cout << "15. Real-world Scenario - UI Component Styling:\n";
    
    // Simulate a UI button with various visual enhancements
    auto uiButton = std::make_unique<CompoundDecorator>(
        std::make_unique<Rectangle>(100.0, 30.0, "lightblue"),
        true, 2, "darkblue",  // border
        true, "skyblue", true,  // fill
        true, 3, "gray",  // shadow
        false  // no animation
    );
    
    uiButton->draw();
    std::cout << "Button description: " << uiButton->getDescription() << std::endl;
    
    // Simulate a UI panel
    auto uiPanel = FluentDecorator(std::make_unique<Rectangle>(300.0, 200.0, "white"))
                       .border(3, "gray")
                       .fill("lightgray", true)
                       .shadow(5, "darkgray")
                       .build();

    uiPanel->draw();
    std::cout << "Panel description: " << uiPanel->getDescription() << std::endl;
    std::cout << std::endl;
    
    // Test 16: Dynamic Decoration Switching
    std::cout << "16. Dynamic Decoration Switching:\n";
    
    auto baseShape = std::make_unique<Circle>(3.0, "original");
    std::cout << "Base shape: " << baseShape->getDescription() << std::endl;
    
    // Apply basic decoration
    auto basicDecorator = std::make_unique<BorderDecorator>(std::move(baseShape), 1, "black");
    std::cout << "With border: " << basicDecorator->getDescription() << std::endl;
    
    // In a real application, you might replace decorator chains dynamically
    // (Here we're just demonstrating the concept)
    std::cout << "Simulating dynamic decoration switching:\n";
    auto fancyDecorator = std::make_unique<GradientDecorator>(
        std::make_unique<ShadowDecorator>(
            std::make_unique<FillDecorator>(
                std::make_unique<BorderDecorator>(
                    std::make_unique<Circle>(3.0, "original"), 2, "purple"
                ),
                "lavender", true
            ),
            4, "plum"
        ),
        "white", "purple", "radial"
    );
    
    fancyDecorator->draw();
    std::cout << "Dynamic decoration: " << fancyDecorator->getDescription() << std::endl;
    std::cout << std::endl;
    
    // Test 17: Chain of Decorators with Different Types
    std::cout << "17. Chain of Decorators with Different Types:\n";
    
    auto complexChain = std::make_unique<AnimationDecorator>(
        std::make_unique<ScaleDecorator>(
            std::make_unique<GradientDecorator>(
                std::make_unique<ShadowDecorator>(
                    std::make_unique<FillDecorator>(
                        std::make_unique<BorderDecorator>(
                            std::make_unique<Triangle>(8.0, 6.0, "turquoise"), 2, "teal"
                        ),
                        "aqua", true
                    ),
                    5, "slategray"
                ),
                "cyan", "deepskyblue", "horizontal"
            ),
            1.3, 1.1
        ),
        "sway", 0.7
    );
    
    complexChain->draw();
    std::cout << "Complex chain area: " << complexChain->getArea() << std::endl;
    std::cout << "Complex chain description: " << complexChain->getDescription() << std::endl;
    std::cout << std::endl;
    
    std::cout << "Decorator pattern demo completed successfully!\n";
    std::cout << "\nKey Benefits of Decorator Pattern:\n";
    std::cout << "- Dynamic addition of responsibilities to objects\n";
    std::cout << "- More flexibility than static inheritance\n";
    std::cout << "- Ability to combine behaviors by stacking decorators\n";
    std::cout << "- Transparency to the client code\n";
    std::cout << "- Clear, understandable object composition\n";
    std::cout << "- Supports Open/Closed Principle (open for extension, closed for modification)\n";
    std::cout << "- Can be combined with other patterns for enhanced functionality\n";
    
    return 0;
}