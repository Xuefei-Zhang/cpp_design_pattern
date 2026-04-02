#include "structural/composite_patterns.h"
#include <iostream>
#include <vector>
#include <memory>
#include <chrono>

int main() {
    std::cout << "=== Composite Pattern Demo: Shape Grouping and Hierarchies ===\n\n";
    
    // Test 1: Basic Composite Usage
    std::cout << "1. Basic Composite Usage:\n";
    
    // Create individual shapes (leaf components)
    auto circle1 = std::make_unique<Circle>(5.0, "red", "Red Circle");
    auto rect1 = std::make_unique<Rectangle>(10.0, 8.0, "blue", "Blue Rectangle");
    auto triangle1 = std::make_unique<Triangle>(6.0, 4.0, "green", "Green Triangle");
    
    // Display individual shapes
    std::cout << "Individual shapes:\n";
    circle1->draw();
    rect1->draw();
    triangle1->draw();
    std::cout << std::endl;
    
    // Create a composite to group shapes
    auto shapeGroup = std::make_unique<ShapeComposite>("Main Group");
    
    // Add shapes to the composite
    shapeGroup->add(std::make_unique<Circle>(3.0, "yellow", "Small Yellow Circle"));
    shapeGroup->add(std::make_unique<Rectangle>(5.0, 3.0, "purple", "Small Purple Rectangle"));
    
    // Add the existing shapes to the group
    shapeGroup->add(std::make_unique<Triangle>(4.0, 3.0, "orange", "Small Orange Triangle"));
    
    std::cout << "Composite group with individual shapes:\n";
    shapeGroup->draw();
    std::cout << "Composite area: " << shapeGroup->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 2: Nested Composites
    std::cout << "2. Nested Composites:\n";
    
    // Create a subgroup
    auto subGroup = std::make_unique<Group>("Sub Group", 10.0, 10.0);
    subGroup->add(std::make_unique<Circle>(2.0, "cyan", "Tiny Cyan Circle"));
    subGroup->add(std::make_unique<Line>(0, 0, 5, 5, "magenta", "Diagonal Line"));
    
    // Add the subgroup to the main group
    auto mainGroup = std::make_unique<Group>("Main Group", 0.0, 0.0);
    mainGroup->add(std::make_unique<Circle>(4.0, "red", "Main Circle"));
    mainGroup->add(std::make_unique<Rectangle>(8.0, 6.0, "blue", "Main Rectangle"));
    mainGroup->add(std::move(subGroup));  // Add the nested group
    
    std::cout << "Main group with nested subgroup:\n";
    mainGroup->draw();
    std::cout << "Main group area: " << mainGroup->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 3: Tree Structure Operations
    std::cout << "3. Tree Structure Operations:\n";
    
    auto scene = std::make_unique<SceneNode>("Scene Root", 0, 0, 1.0, 1.0, 0.0);
    
    // Add some shapes to the scene
    scene->add(std::make_unique<Circle>(3.0, "white", "Background Circle"));
    
    // Add a layer group
    auto layer1 = std::make_unique<Layer>("Foreground Layer", true, 0.8);
    layer1->add(std::make_unique<Rectangle>(5.0, 4.0, "yellow", "Sun"));
    layer1->add(std::make_unique<Triangle>(3.0, 2.0, "green", "Tree"));
    
    scene->add(std::move(layer1));
    
    std::cout << "Scene with layers:\n";
    scene->draw();
    std::cout << std::endl;
    
    // Test 4: Complex Shape Composition
    std::cout << "4. Complex Shape Composition:\n";
    
    auto house = std::make_unique<ComplexShapeComposite>("House", 0, 0);
    house->buildHouse();
    
    std::cout << "Predefined House:\n";
    house->draw();
    std::cout << "House area: " << house->getArea() << std::endl;
    std::cout << std::endl;
    
    auto car = std::make_unique<ComplexShapeComposite>("Car", 50, 50);
    car->buildCar();
    
    std::cout << "Predefined Car:\n";
    car->draw();
    std::cout << "Car area: " << car->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 5: Manipulating Composites
    std::cout << "5. Manipulating Composite Components:\n";
    
    auto manipulationDemo = std::make_unique<Group>("Manipulation Demo");
    manipulationDemo->add(std::make_unique<Circle>(2.0, "red", "Circle 1"));
    manipulationDemo->add(std::make_unique<Rectangle>(3.0, 2.0, "blue", "Rectangle 1"));
    
    std::cout << "Initial state:\n";
    manipulationDemo->draw();
    
    // Add another shape
    manipulationDemo->add(std::make_unique<Triangle>(2.0, 2.0, "green", "Triangle 1"));
    std::cout << "\nAfter adding Triangle:\n";
    manipulationDemo->draw();
    
    std::cout << "Total area: " << manipulationDemo->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 6: Finding Elements in Composite
    std::cout << "6. Finding Elements in Composite:\n";
    
    auto searchGroup = std::make_unique<Group>("Search Group");
    auto circleToFind = std::make_unique<Circle>(5.0, "red", "Target Circle");
    auto rectToFind = std::make_unique<Rectangle>(6.0, 4.0, "blue", "Target Rectangle");
    
    searchGroup->add(std::make_unique<Circle>(3.0, "yellow", "Circle 1"));
    searchGroup->add(std::move(circleToFind));
    searchGroup->add(std::move(rectToFind));
    
    std::cout << "Searching for elements:\n";
    auto* foundCircle = searchGroup->findByName("Target Circle");
    if (foundCircle) {
        std::cout << "Found: " << foundCircle->getType() << std::endl;
    }
    
    auto* foundRect = searchGroup->findByName("Target Rectangle");
    if (foundRect) {
        std::cout << "Found: " << foundRect->getType() << std::endl;
    }
    
    auto* notFound = searchGroup->findByName("Nonexistent");
    if (notFound) {
        std::cout << "Found: " << notFound->getType() << std::endl;
    } else {
        std::cout << "Element not found: Nonexistent\n";
    }
    std::cout << std::endl;
    
    // Test 7: Layer Visibility Control
    std::cout << "7. Layer Visibility Control:\n";
    
    auto visibleLayer = std::make_unique<Layer>("Visible Layer", true, 1.0);
    visibleLayer->add(std::make_unique<Circle>(2.0, "red", "Visible Circle"));
    visibleLayer->add(std::make_unique<Rectangle>(3.0, 2.0, "blue", "Visible Rectangle"));
    
    std::cout << "Visible layer:\n";
    visibleLayer->draw();
    
    auto invisibleLayer = std::make_unique<Layer>("Invisible Layer", false, 0.0);
    invisibleLayer->add(std::make_unique<Triangle>(3.0, 2.0, "green", "Invisible Triangle"));
    invisibleLayer->add(std::make_unique<Line>(0, 0, 5, 5, "black", "Invisible Line"));
    
    std::cout << "Invisible layer:\n";
    invisibleLayer->draw();
    std::cout << std::endl;
    
    // Test 8: Uniform Treatment of Individual Objects and Collections
    std::cout << "8. Uniform Treatment of Individual Objects and Collections:\n";
    
    std::vector<std::unique_ptr<ShapeComponent>> components;
    
    // Add individual shapes
    components.push_back(std::make_unique<Circle>(4.0, "red", "Individual Circle"));
    components.push_back(std::make_unique<Rectangle>(6.0, 4.0, "blue", "Individual Rectangle"));
    
    // Add a composite
    auto miniGroup = std::make_unique<Group>("Mini Group");
    miniGroup->add(std::make_unique<Triangle>(3.0, 2.0, "green", "Triangle in Group"));
    miniGroup->add(std::make_unique<Line>(0, 0, 3, 3, "black", "Line in Group"));
    components.push_back(std::move(miniGroup));
    
    std::cout << "Processing all components uniformly:\n";
    for (const auto& component : components) {
        std::cout << "Component type: " << component->getType() 
                  << ", Area: " << component->getArea() << std::endl;
        component->draw();
    }
    std::cout << std::endl;
    
    // Test 9: Scene Graph with Transformations
    std::cout << "9. Scene Graph with Transformations:\n";
    
    auto sceneGraph = std::make_unique<SceneNode>("Root Scene", 0, 0, 1.0, 1.0, 0);
    
    // Add a group with transformations
    auto characterGroup = std::make_unique<SceneNode>("Character", 100, 100, 1.5, 1.5, 15);
    characterGroup->add(std::make_unique<Circle>(2.0, "pink", "Head"));
    characterGroup->add(std::make_unique<Rectangle>(4.0, 6.0, "gray", "Body"));
    
    // Add limbs
    auto leftArm = std::make_unique<SceneNode>("Left Arm", -3, -2, 1.0, 1.0, -10);
    leftArm->add(std::make_unique<Rectangle>(1.0, 3.0, "gray", "Arm Segment"));
    characterGroup->add(std::move(leftArm));
    
    auto rightArm = std::make_unique<SceneNode>("Right Arm", 3, -2, 1.0, 1.0, 10);
    rightArm->add(std::make_unique<Rectangle>(1.0, 3.0, "gray", "Arm Segment"));
    characterGroup->add(std::move(rightArm));
    
    sceneGraph->add(std::move(characterGroup));
    sceneGraph->draw();
    std::cout << std::endl;
    
    // Test 10: Performance and Memory Efficiency
    std::cout << "10. Performance Test - Creating Many Nested Composites:\n";
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Create a deep hierarchy
    std::unique_ptr<ShapeComponent> root = std::make_unique<ShapeComposite>("Root");
    ShapeComponent* current = root.get();
    
    // Create 5 levels of nesting with 3 children at each level
    for (int level = 0; level < 5; ++level) {
        if (auto* comp = dynamic_cast<ShapeComposite*>(current)) {
            for (int child = 0; child < 3; ++child) {
                if (level == 4) { // At the bottom level, add leaf nodes
                    comp->add(std::make_unique<Circle>(1.0, "red", "Leaf Circle " + std::to_string(level)));
                } else {
                    auto newComp = std::make_unique<ShapeComposite>("Level " + std::to_string(level) + " Group");
                    auto* newCompPtr = newComp.get();
                    comp->add(std::move(newComp));
                    
                    // Add a few leafs to each intermediate level
                    if (child % 2 == 0) { // Add leaf every other child
                        comp->add(std::make_unique<Rectangle>(2.0, 1.0, "blue", "Intermediate Rect"));
                    }
                    current = newCompPtr;
                }
            }
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Created complex hierarchy in: " << duration.count() << " microseconds\n";
    std::cout << "Root area: " << root->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 11: Real-world Scenario - UI Component Hierarchy
    std::cout << "11. Real-world Scenario - UI Component Hierarchy:\n";
    
    auto window = std::make_unique<ShapeComposite>("MainWindow");
    window->add(std::make_unique<Rectangle>(800, 600, "white", "Window Background", 0, 0));
    
    // Add menu bar
    auto menuBar = std::make_unique<ShapeComposite>("MenuBar");
    menuBar->add(std::make_unique<Rectangle>(800, 30, "lightgray", "Menu Background", 0, 0));
    menuBar->add(std::make_unique<Rectangle>(40, 20, "lightblue", "File Menu Item", 5, 5));
    menuBar->add(std::make_unique<Rectangle>(40, 20, "lightblue", "Edit Menu Item", 50, 5));
    window->add(std::move(menuBar));
    
    // Add content area
    auto contentArea = std::make_unique<ShapeComposite>("ContentArea");
    contentArea->add(std::make_unique<Rectangle>(780, 400, "white", "Content Background", 10, 40));
    contentArea->add(std::make_unique<Circle>(10, "red", "Content Circle", 50, 80));
    contentArea->add(std::make_unique<Rectangle>(100, 30, "yellow", "Button", 100, 100));
    window->add(std::move(contentArea));
    
    // Add status bar
    auto statusBar = std::make_unique<ShapeComposite>("StatusBar");
    statusBar->add(std::make_unique<Rectangle>(800, 20, "darkgray", "Status Background", 0, 450));
    statusBar->add(std::make_unique<Rectangle>(100, 16, "lightgray", "Status Text", 5, 452));
    window->add(std::move(statusBar));
    
    std::cout << "UI Window Hierarchy:\n";
    window->draw();
    std::cout << "Total UI area: " << window->getArea() << std::endl;
    std::cout << std::endl;
    
    // Test 12: Composite Pattern Benefits Demonstration
    std::cout << "12. Composite Pattern Benefits Demonstration:\n";
    
    std::cout << "- Uniform Interface: All components treated the same way\n";
    std::cout << "- Recursive Composition: Composites can contain other composites\n";
    std::cout << "- Flexibility: Easy to add/remove components\n";
    std::cout << "- Extensibility: New shapes can be added without changing existing code\n";
    std::cout << "- Tree Structure: Natural representation of hierarchical data\n";
    std::cout << "- Transparency: Clients don't need to know if they're dealing with leaf or composite\n";
    std::cout << std::endl;
    
    // Test 13: Container vs Individual Behavior
    std::cout << "13. Container vs Individual Behavior:\n";
    
    auto individualCircle = std::make_unique<Circle>(5.0, "red", "Individual Circle");
    std::cout << "Individual Circle - Is Composite: " << individualCircle->isComposite() << std::endl;
    std::cout << "Individual Circle - Children: ";
    try {
        const auto& children = individualCircle->getChildren();
        std::cout << children.size() << std::endl;
    } catch (...) {
        std::cout << "Not supported (leaf node)" << std::endl;
    }
    individualCircle->draw();
    
    auto compositeGroup = std::make_unique<Group>("Composite Group");
    compositeGroup->add(std::make_unique<Circle>(2.0, "green", "Child Circle"));
    compositeGroup->add(std::make_unique<Rectangle>(3.0, 2.0, "blue", "Child Rectangle"));
    
    std::cout << "Composite Group - Is Composite: " << compositeGroup->isComposite() << std::endl;
    std::cout << "Composite Group - Children: " << compositeGroup->getChildCount() << std::endl;
    compositeGroup->draw();
    std::cout << std::endl;
    
    // Test 14: Hierarchical Traversal Example
    std::cout << "14. Hierarchical Traversal Example:\n";
    
    auto hierarchy = std::make_unique<ShapeComposite>("Hierarchy Root");
    auto level1 = std::make_unique<ShapeComposite>("Level 1 Group");
    auto level2 = std::make_unique<ShapeComposite>("Level 2 Group");
    
    level2->add(std::make_unique<Circle>(1.0, "red", "Deep Circle"));
    level2->add(std::make_unique<Rectangle>(2.0, 1.0, "green", "Deep Rectangle"));
    
    level1->add(std::move(level2));
    level1->add(std::make_unique<Triangle>(2.0, 1.5, "blue", "Mid Triangle"));
    
    hierarchy->add(std::move(level1));
    hierarchy->add(std::make_unique<Line>(0, 0, 5, 5, "black", "Root Line"));
    
    std::cout << "Traversing complete hierarchy:\n";
    hierarchy->draw();
    std::cout << "Total area of hierarchy: " << hierarchy->getArea() << std::endl;
    std::cout << std::endl;
    
    std::cout << "Composite pattern demo completed successfully!\n";
    std::cout << "\nKey Benefits of Composite Pattern:\n";
    std::cout << "- Treat individual objects and compositions uniformly\n";
    std::cout << "- Build hierarchical tree structures\n";
    std::cout << "- Define class hierarchies with primitive and composite objects\n";
    std::cout << "- Add new components without changing existing code\n";
    std::cout << "- Promote transparency and flexibility\n";
    std::cout << "- Enable recursive operations on the entire hierarchy\n";
    
    return 0;
}