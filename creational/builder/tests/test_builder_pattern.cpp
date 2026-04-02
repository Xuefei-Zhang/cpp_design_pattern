#include "../include/builder_shapes.h"
#include <iostream>
#include <memory>
#include <vector>

int main() {
    std::cout << "=== Builder Pattern Demo: Shape Creation ===\n\n";
    
    // Test 1: Basic Circle Builder usage
    std::cout << "1. Basic Circle Builder Usage:\n";
    auto circleBuilder = std::make_unique<CircleBuilder>();
    
    auto circle1 = circleBuilder->setRadius(5.0)
                                ->setColor("red")
                                ->setStyle("solid")
                                ->setFilled(true)
                                ->getResult();
    circle1->draw();
    circle1->printInfo();
    std::cout << std::endl;
    
    // Create another circle with different parameters using the same builder
    auto circle2 = circleBuilder->setRadius(3.0)
                                ->setColor("blue")
                                ->setStyle("dashed")
                                ->setFilled(false)
                                ->getResult();
    circle2->draw();
    circle2->printInfo();
    std::cout << std::endl;
    
    // Test 2: Rectangle Builder usage
    std::cout << "2. Rectangle Builder Usage:\n";
    auto rectBuilder = std::make_unique<RectangleBuilder>();
    
    auto rect1 = rectBuilder->setWidth(10.0)
                            ->setHeight(8.0)
                            ->setColor("green")
                            ->setStyle("solid")
                            ->setFilled(true)
                            ->setBorderWidth(2)
                            ->getResult();
    rect1->draw();
    rect1->printInfo();
    std::cout << std::endl;
    
    // Test 3: Triangle Builder usage
    std::cout << "3. Triangle Builder Usage:\n";
    auto triBuilder = std::make_unique<TriangleBuilder>();
    
    auto tri1 = triBuilder->setBase(6.0)
                          ->setHeight(4.0)
                          ->setColor("purple")
                          ->setStyle("dotted")
                          ->setFilled(false)
                          ->setLabel("Acute Triangle")
                          ->getResult();
    tri1->draw();
    tri1->printInfo();
    std::cout << std::endl;
    
    // Test 4: Director usage
    std::cout << "4. Director Pattern Usage:\n";
    auto director = std::make_unique<ShapeDirector>(std::make_unique<CircleBuilder>());
    
    auto standardCircle = director->createStandardCircle();
    std::cout << "Standard circle: ";
    standardCircle->draw();
    standardCircle->printInfo();
    
    auto largeCircle = director->createLargeFilledCircle();
    std::cout << "Large filled circle: ";
    largeCircle->draw();
    largeCircle->printInfo();
    
    // Change builder to rectangle
    director->setBuilder(std::make_unique<RectangleBuilder>());
    auto complexRect = director->createComplexRectangle();
    std::cout << "Complex rectangle: ";
    complexRect->draw();
    complexRect->printInfo();
    
    // Change builder to triangle
    director->setBuilder(std::make_unique<TriangleBuilder>());
    auto labeledTri = director->createLabeledTriangle();
    std::cout << "Labeled triangle: ";
    labeledTri->draw();
    labeledTri->printInfo();
    std::cout << std::endl;
    
    // Test 5: Fluent Builder usage
    std::cout << "5. Fluent Builder Usage:\n";
    FluentShapeBuilder fluentBuilder;
    
    auto circleFluent = fluentBuilder.createCircleBuilder()
                                   .radius(7.0)
                                   .color("orange")
                                   .style("solid")
                                   .filled(true)
                                   .build();
    std::cout << "Fluent circle: ";
    circleFluent->draw();
    circleFluent->printInfo();
    
    auto rectFluent = fluentBuilder.createRectangleBuilder()
                                  .width(12.0)
                                  .height(6.0)
                                  .color("cyan")
                                  .style("dashed")
                                  .filled(false)
                                  .borderWidth(1)
                                  .build();
    std::cout << "Fluent rectangle: ";
    rectFluent->draw();
    rectFluent->printInfo();
    
    auto triFluent = fluentBuilder.createTriangleBuilder()
                                 .base(8.0)
                                 .height(6.0)
                                 .color("magenta")
                                 .style("solid")
                                 .filled(true)
                                 .label("Isosceles Triangle")
                                 .build();
    std::cout << "Fluent triangle: ";
    triFluent->draw();
    triFluent->printInfo();
    std::cout << std::endl;
    
    // Test 6: Complex Shape Builder
    std::cout << "6. Complex Shape Builder Usage:\n";
    auto complexBuilder = std::make_unique<ComplexShapeBuilder>("House");
    
    // Add multiple shapes to the complex shape
    complexBuilder->addShape(std::make_unique<Rectangle>(10, 8, "brown", "solid", true, 2))
                   ->addShape(std::make_unique<Triangle>(10, 5, "red", "solid", true, "Roof"))
                   ->addShape(std::make_unique<Rectangle>(3, 4, "blue", "solid", true, 1));
    
    std::cout << "Complex shape created with " << complexBuilder->getShapeCount() << " components\n";
    // Note: In a real implementation, we would have a proper composite shape handler
    std::cout << std::endl;
    
    // Test 7: Comparison with Factory Pattern approach
    std::cout << "7. Builder vs Direct Construction Comparison:\n";
    
    // Using Builder - step-by-step construction
    std::cout << "Using Builder: ";
    auto builderCircle = std::make_unique<CircleBuilder>()
                            ->setRadius(4.0)
                            ->setColor("yellow")
                            ->setStyle("solid")
                            ->setFilled(false)
                            ->getResult();
    builderCircle->draw();
    
    // Direct construction - all parameters at once
    std::cout << "Direct construction: ";
    auto directCircle = std::make_unique<Circle>(4.0, "yellow", "solid", false);
    directCircle->draw();
    
    std::cout << "\nComparison:\n";
    std::cout << "- Builder: More flexible, allows partial construction\n";
    std::cout << "- Builder: Can reuse the same builder for multiple similar objects\n";
    std::cout << "- Builder: Better for complex objects with many optional parameters\n";
    std::cout << "- Direct: Simpler for objects with few required parameters\n";
    std::cout << std::endl;
    
    // Test 8: Builder Factory usage
    std::cout << "8. Builder Factory Usage:\n";
    auto factoryCircleBuilder = ShapeFactoryBuilder::createCircleBuilder();
    auto factoryCircle = factoryCircleBuilder->setRadius(9.0)
                                            ->setColor("lime")
                                            ->setStyle("dotted")
                                            ->setFilled(true)
                                            ->getResult();
    std::cout << "Factory-built circle: ";
    factoryCircle->draw();
    factoryCircle->printInfo();
    std::cout << std::endl;
    
    // Test 9: Builder with method chaining demonstration
    std::cout << "9. Method Chaining Demonstration:\n";
    auto chainedCircle = std::make_unique<CircleBuilder>()
                            ->setRadius(6.5)
                            ->setColor("pink")
                            ->setStyle("dashed")
                            ->setFilled(true)
                            ->getResult();
    chainedCircle->draw();
    chainedCircle->printInfo();
    std::cout << std::endl;
    
    // Test 10: Builder reset functionality
    std::cout << "10. Builder Reset Functionality:\n";
    auto resetBuilder = std::make_unique<CircleBuilder>();
    
    // First build
    auto circleA = resetBuilder->setRadius(1.0)->setColor("black")->getResult();
    std::cout << "First circle: ";
    circleA->printInfo();
    
    // Second build with same builder (should be reset)
    auto circleB = resetBuilder->setRadius(2.0)->setColor("white")->getResult();
    std::cout << "Second circle: ";
    circleB->printInfo();
    
    std::cout << "Builder reset ensured clean state for each build\n\n";
    
    std::cout << "Builder pattern demo completed successfully!\n";
    std::cout << "\nKey Benefits of Builder Pattern:\n";
    std::cout << "- Separates construction of complex objects from their representation\n";
    std::cout << "- Allows step-by-step construction of objects\n";
    std::cout << "- Can create different representations with the same construction process\n";
    std::cout << "- Provides control over the construction process\n";
    std::cout << "- Supports method chaining for fluent interfaces\n";
    
    return 0;
}