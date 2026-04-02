#include "utilities/scene_builder.h"
#include <iostream>
#include <memory>

int main() {
    std::cout << "=== Advanced Builder Pattern Demo: Scene Graph Builder ===\n\n";
    
    // Test 1: Basic Scene Builder Usage
    std::cout << "1. Basic Scene Builder Usage:\n";
    auto basicBuilder = std::make_unique<SceneBuilderImpl>();
    
    basicBuilder->reset()
                ->setName("Basic Scene")
                ->addShape("Main Circle", std::make_unique<Circle>(5.0, "red"))
                ->setTransform("Main Circle", 10.0, 10.0, 1.0, 0.0)
                ->addShape("Main Rectangle", std::make_unique<Rectangle>(8.0, 6.0, "blue"))
                ->setTransform("Main Rectangle", 20.0, 15.0, 1.5, 45.0);
    
    auto basicScene = basicBuilder->getResult();
    basicScene->draw();
    basicScene->printSceneStructure();
    std::cout << std::endl;
    
    // Test 2: Director Pattern Usage
    std::cout << "2. Director Pattern Usage:\n";
    SceneDirector director(std::make_unique<SceneBuilderImpl>());
    
    std::cout << "Creating Simple Scene:\n";
    auto simpleScene = director.createSimpleScene();
    simpleScene->draw();
    simpleScene->printSceneStructure();
    std::cout << std::endl;
    
    std::cout << "Creating Complex Scene:\n";
    auto complexScene = director.createComplexScene();
    complexScene->draw();
    complexScene->printSceneStructure();
    std::cout << std::endl;
    
    std::cout << "Creating Hierarchical Scene:\n";
    auto hierarchicalScene = director.createHierarchicalScene();
    hierarchicalScene->draw();
    hierarchicalScene->printSceneStructure();
    std::cout << std::endl;
    
    // Test 3: Fluent Builder Usage
    std::cout << "3. Fluent Builder Usage:\n";
    auto fluentScene = FluentSceneBuilder()
                          .name("Fluent Scene")
                          .background("lightgreen")
                          .addCircle("Sun", 10.0, "yellow")
                          .transform("Sun", 50.0, 50.0, 1.0, 0.0)
                          .addRectangle("Sign", 15.0, 8.0, "white")
                          .transform("Sign", 100.0, 100.0, 1.2, 15.0)
                          .group("Vehicle")
                          .transform("Vehicle", 200.0, 200.0, 1.0, 0.0)
                          .addRectangleToGroup("Vehicle", "Body", 20.0, 10.0, "red")
                          .transform("Body", 0.0, 0.0, 1.0, 0.0)
                          .addCircleToGroup("Vehicle", "Wheel1", 3.0, "black")
                          .transform("Wheel1", -8.0, -6.0, 1.0, 0.0)
                          .addCircleToGroup("Vehicle", "Wheel2", 3.0, "black")
                          .transform("Wheel2", 8.0, -6.0, 1.0, 0.0)
                          .build();
    
    fluentScene->draw();
    fluentScene->printSceneStructure();
    std::cout << std::endl;
    
    // Test 4: Reusing Builder for Multiple Scenes
    std::cout << "4. Reusing Builder for Multiple Scenes:\n";
    auto reusableBuilder = std::make_unique<SceneBuilderImpl>();
    
    // First scene
    reusableBuilder->reset()
                   ->setName("Reused Scene 1")
                   ->addShape("Circle 1", std::make_unique<Circle>(3.0, "purple"))
                   ->setTransform("Circle 1", 5.0, 5.0, 1.0, 0.0);
    auto scene1 = reusableBuilder->getResult();
    std::cout << "First scene:\n";
    scene1->draw();
    
    // Second scene with same builder
    reusableBuilder->reset()
                   ->setName("Reused Scene 2")
                   ->addShape("Rectangle 1", std::make_unique<Rectangle>(7.0, 4.0, "orange"))
                   ->setTransform("Rectangle 1", 15.0, 15.0, 2.0, 90.0);
    auto scene2 = reusableBuilder->getResult();
    std::cout << "\nSecond scene:\n";
    scene2->draw();
    std::cout << std::endl;
    
    // Test 5: Complex Hierarchical Scene Construction
    std::cout << "5. Complex Hierarchical Scene Construction:\n";
    auto complexBuilder = std::make_unique<SceneBuilderImpl>();
    
    complexBuilder->reset()
                  ->setName("Robot Scene")
                  ->setBackgroundColor("white")
                  ->addGroup("Robot")
                  ->setTransform("Robot", 100.0, 100.0, 1.0, 0.0)
                  
                  // Robot body
                  ->addShapeToGroup("Robot", "Body", std::make_unique<Rectangle>(20.0, 30.0, "gray"))
                  ->setTransform("Body", 0.0, 0.0, 1.0, 0.0)
                  
                  // Robot head
                  ->addShapeToGroup("Robot", "Head", std::make_unique<Circle>(8.0, "lightgray"))
                  ->setTransform("Head", 0.0, -20.0, 1.0, 0.0)
                  
                  // Robot arms
                  ->addGroup("LeftArm")
                  ->setTransform("LeftArm", -15.0, -5.0, 1.0, 0.0)
                  ->addShapeToGroup("LeftArm", "UpperArm", std::make_unique<Rectangle>(5.0, 15.0, "gray"))
                  ->setTransform("UpperArm", 0.0, -7.5, 1.0, 0.0)
                  ->addShapeToGroup("LeftArm", "LowerArm", std::make_unique<Rectangle>(5.0, 12.0, "darkgray"))
                  ->setTransform("LowerArm", 0.0, -22.5, 1.0, 0.0)
                  
                  ->addGroup("RightArm")
                  ->setTransform("RightArm", 15.0, -5.0, 1.0, 0.0)
                  ->addShapeToGroup("RightArm", "UpperArm", std::make_unique<Rectangle>(5.0, 15.0, "gray"))
                  ->setTransform("UpperArm", 0.0, -7.5, 1.0, 0.0)
                  ->addShapeToGroup("RightArm", "LowerArm", std::make_unique<Rectangle>(5.0, 12.0, "darkgray"))
                  ->setTransform("LowerArm", 0.0, -22.5, 1.0, 0.0)
                  
                  // Robot legs
                  ->addGroup("LeftLeg")
                  ->setTransform("LeftLeg", -7.0, 20.0, 1.0, 0.0)
                  ->addShapeToGroup("LeftLeg", "Thigh", std::make_unique<Rectangle>(6.0, 18.0, "gray"))
                  ->setTransform("Thigh", 0.0, 9.0, 1.0, 0.0)
                  ->addShapeToGroup("LeftLeg", "Shin", std::make_unique<Rectangle>(6.0, 15.0, "darkgray"))
                  ->setTransform("Shin", 0.0, 28.5, 1.0, 0.0)
                  
                  ->addGroup("RightLeg")
                  ->setTransform("RightLeg", 7.0, 20.0, 1.0, 0.0)
                  ->addShapeToGroup("RightLeg", "Thigh", std::make_unique<Rectangle>(6.0, 18.0, "gray"))
                  ->setTransform("Thigh", 0.0, 9.0, 1.0, 0.0)
                  ->addShapeToGroup("RightLeg", "Shin", std::make_unique<Rectangle>(6.0, 15.0, "darkgray"))
                  ->setTransform("Shin", 0.0, 28.5, 1.0, 0.0);
    
    auto robotScene = complexBuilder->getResult();
    robotScene->draw();
    robotScene->printSceneStructure();
    std::cout << std::endl;
    
    // Test 6: Builder with Different Scenes from Same Director
    std::cout << "6. Different Scenes from Same Director:\n";
    SceneDirector sceneDirector(std::make_unique<SceneBuilderImpl>());
    
    std::vector<std::unique_ptr<Scene>> scenes;
    scenes.push_back(sceneDirector.createSimpleScene());
    scenes.push_back(sceneDirector.createComplexScene());
    scenes.push_back(sceneDirector.createHierarchicalScene());
    
    for (size_t i = 0; i < scenes.size(); ++i) {
        std::cout << "Scene " << i + 1 << ":\n";
        scenes[i]->draw();
        std::cout << std::endl;
    }
    
    // Test 7: Performance Comparison - Direct Construction vs Builder
    std::cout << "7. Performance Consideration - Builder vs Direct Construction:\n";
    
    std::cout << "\nDirect Construction (for a simple scene):\n";
    auto directScene = std::make_unique<Scene>("Direct Scene");
    auto root = directScene->getRoot();
    auto circleNode = std::make_unique<SceneNode>("Direct Circle", std::make_unique<Circle>(5.0, "red"));
    circleNode->setLocalTransform(10.0, 10.0, 1.0, 0.0);
    root->addChild(std::move(circleNode));
    directScene->draw();
    
    std::cout << "\nUsing Builder (for a complex scene):\n";
    auto builderScene = FluentSceneBuilder()
                           .name("Builder Scene")
                           .addCircle("Circle 1", 5.0, "red")
                           .transform("Circle 1", 10.0, 10.0, 1.0, 0.0)
                           .addRectangle("Rect 1", 8.0, 6.0, "blue")
                           .transform("Rect 1", 20.0, 15.0, 1.5, 45.0)
                           .build();
    builderScene->draw();
    std::cout << std::endl;
    
    // Test 8: Builder Reusability and State Management
    std::cout << "8. Builder Reusability and State Management:\n";
    auto testBuilder = std::make_unique<SceneBuilderImpl>();
    
    // Build first scene
    testBuilder->reset()
               ->setName("Test Scene 1")
               ->addShape("Shape A", std::make_unique<Circle>(2.0));
    
    // Check that we can get result and builder is reset
    auto testScene1 = testBuilder->getResult();
    std::cout << "First scene name: " << testScene1->getName() << std::endl;
    
    // Build second scene 
    testBuilder->addShape("Shape B", std::make_unique<Rectangle>(4.0, 3.0));
    auto testScene2 = testBuilder->getResult();
    std::cout << "Second scene name: " << testScene2->getName() << std::endl;
    std::cout << std::endl;
    
    // Test 9: Real-world Scenario - UI Component Builder
    std::cout << "9. Real-world Scenario - UI Dashboard Builder:\n";
    auto dashboard = FluentSceneBuilder()
                        .name("Dashboard")
                        .background("lightgray")
                        .group("Header")
                        .transform("Header", 0.0, 0.0, 1.0, 0.0)
                        .addRectangleToGroup("Header", "TitleBar", 800.0, 50.0, "navy")
                        .addRectangleToGroup("Header", "Logo", 100.0, 40.0, "white")
                        .transform("Logo", 10.0, 5.0, 1.0, 0.0)
                        .group("MainContent")
                        .transform("MainContent", 0.0, 50.0, 1.0, 0.0)
                        .addRectangleToGroup("MainContent", "ContentPanel", 800.0, 500.0, "white")
                        .group("Charts")
                        .transform("Charts", 20.0, 70.0, 1.0, 0.0)
                        .addRectangleToGroup("Charts", "Chart1", 200.0, 150.0, "lightblue")
                        .transform("Chart1", 0.0, 0.0, 1.0, 0.0)
                        .addRectangleToGroup("Charts", "Chart2", 200.0, 150.0, "lightgreen")
                        .transform("Chart2", 220.0, 0.0, 1.0, 0.0)
                        .build();
    
    dashboard->printSceneStructure();
    std::cout << std::endl;
    
    std::cout << "Advanced Builder Pattern demo completed successfully!\n";
    std::cout << "\nKey Benefits of this Advanced Builder Pattern:\n";
    std::cout << "- Builds complex hierarchical objects (scene graphs with parent-child relationships)\n";
    std::cout << "- Manages relationships between objects in the hierarchy\n";
    std::cout << "- Supports both imperative and fluent interfaces\n";
    std::cout << "- Includes a Director for predefined complex constructions\n";
    std::cout << "- Handles transformation matrices and positioning in scene graph\n";
    std::cout << "- Demonstrates how Builder can construct very complex objects step-by-step\n";
    
    return 0;
}