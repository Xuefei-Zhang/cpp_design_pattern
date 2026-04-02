#include "behavioral/visitor_patterns.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>

int main() {
    std::cout << "=== Visitor Pattern Demo: Operations on Object Structures ===\n\n";

    std::cout << "1. Creating a drawing with multiple shapes:\n";
    Drawing drawing;
    drawing.addShape(std::make_unique<Circle>(5.0));
    drawing.addShape(std::make_unique<Rectangle>(4.0, 3.0));
    drawing.addShape(std::make_unique<Triangle>(6.0, 4.0));

    std::cout << "   Added " << drawing.getShapeCount() << " shapes to drawing\n";
    assert(drawing.getShapeCount() == 3);
    std::cout << "   [OK] Drawing created\n\n";

    std::cout << "2. Calculate areas using AreaCalculator visitor:\n";
    AreaCalculator areaCalc;
    drawing.accept(&areaCalc);

    double totalArea = areaCalc.getTotalArea();
    std::cout << "   Total area: " << totalArea << "\n";
    assert(totalArea > 0.0);
    std::cout << "   [OK] Area calculation completed\n\n";

    std::cout << "3. Calculate perimeters using PerimeterCalculator visitor:\n";
    PerimeterCalculator perimeterCalc;
    drawing.accept(&perimeterCalc);

    double totalPerimeter = perimeterCalc.getTotalPerimeter();
    std::cout << "   Total perimeter: " << totalPerimeter << "\n";
    assert(totalPerimeter > 0.0);
    std::cout << "   [OK] Perimeter calculation completed\n\n";

    std::cout << "4. Export to XML using XMLExporter visitor:\n";
    XMLExporter xmlExporter;
    drawing.accept(&xmlExporter);

    std::string xml = xmlExporter.getXML();
    std::cout << xml;
    assert(xml.find("<circle") != std::string::npos);
    assert(xml.find("<rectangle") != std::string::npos);
    assert(xml.find("<triangle") != std::string::npos);
    std::cout << "   [OK] XML export completed\n\n";

    std::cout << "5. Export to JSON using JSONExporter visitor:\n";
    JSONExporter jsonExporter;
    drawing.accept(&jsonExporter);

    std::string json = jsonExporter.getJSON();
    std::cout << "   JSON: " << json << "\n";
    assert(json.find("\"type\":\"circle\"") != std::string::npos);
    assert(json.find("\"type\":\"rectangle\"") != std::string::npos);
    assert(json.find("\"type\":\"triangle\"") != std::string::npos);
    assert(jsonExporter.getCount() == 3);
    std::cout << "   [OK] JSON export completed\n\n";

    std::cout << "6. Render drawing using DrawingRenderer visitor:\n";
    DrawingRenderer renderer;
    drawing.accept(&renderer);

    assert(renderer.getCommandCount() == 3);
    const auto& commands = renderer.getCommands();
    std::cout << "   Generated " << commands.size() << " drawing commands\n";
    std::cout << "   [OK] Drawing rendering completed\n\n";

    std::cout << "7. Scale shapes using ScaleTransformer visitor:\n";
    Drawing scalableDrawing;
    scalableDrawing.addShape(std::make_unique<Circle>(10.0));
    scalableDrawing.addShape(std::make_unique<Rectangle>(20.0, 10.0));

    ScaleTransformer scaler(0.5); // Scale by 50%
    scalableDrawing.accept(&scaler);

    std::cout << "   [OK] Shapes scaled successfully\n\n";

    std::cout << "8. Multiple visitors on same structure:\n";
    Drawing multiVisitorDrawing;
    multiVisitorDrawing.addShape(std::make_unique<Circle>(3.0));
    multiVisitorDrawing.addShape(std::make_unique<Circle>(4.0));

    AreaCalculator areaCalc2;
    JSONExporter jsonExporter2;
    DrawingRenderer renderer2;

    multiVisitorDrawing.accept(&areaCalc2);
    multiVisitorDrawing.accept(&jsonExporter2);
    multiVisitorDrawing.accept(&renderer2);

    std::cout << "   Applied 3 different visitors to same structure\n";
    std::cout << "   Total area: " << areaCalc2.getTotalArea() << "\n";
    std::cout << "   JSON objects: " << jsonExporter2.getCount() << "\n";
    std::cout << "   Render commands: " << renderer2.getCommandCount() << "\n";

    assert(areaCalc2.getTotalArea() > 0.0);
    assert(jsonExporter2.getCount() == 2);
    assert(renderer2.getCommandCount() == 2);
    std::cout << "   [OK] Multiple visitors applied successfully\n\n";

    std::cout << "9. Single shape visitation:\n";
    auto singleCircle = std::make_unique<Circle>(7.0);
    AreaCalculator singleCalc;

    singleCircle->accept(&singleCalc);
    std::cout << "   Single circle area: " << singleCalc.getTotalArea() << "\n";
    assert(singleCalc.getTotalArea() > 150.0 && singleCalc.getTotalArea() < 155.0);
    std::cout << "   [OK] Single shape visitation works\n\n";

    std::cout << "10. Visitor with state reset:\n";
    AreaCalculator resetCalc;

    Drawing drawing1;
    drawing1.addShape(std::make_unique<Circle>(2.0));
    drawing1.accept(&resetCalc);
    double area1 = resetCalc.getTotalArea();
    std::cout << "   First calculation: " << area1 << "\n";

    resetCalc.reset();

    Drawing drawing2;
    drawing2.addShape(std::make_unique<Rectangle>(5.0, 5.0));
    drawing2.accept(&resetCalc);
    double area2 = resetCalc.getTotalArea();
    std::cout << "   After reset calculation: " << area2 << "\n";

    assert(area2 == 25.0); // Should only be rectangle area, not accumulated
    std::cout << "   [OK] Visitor reset functionality works\n\n";

    std::cout << "11. Different shape types with same visitor:\n";
    Drawing mixedDrawing;
    mixedDrawing.addShape(std::make_unique<Circle>(1.0));
    mixedDrawing.addShape(std::make_unique<Rectangle>(2.0, 2.0));
    mixedDrawing.addShape(std::make_unique<Triangle>(3.0, 3.0));
    mixedDrawing.addShape(std::make_unique<Circle>(2.0));

    JSONExporter mixedExporter;
    mixedDrawing.accept(&mixedExporter);

    std::cout << "   Exported " << mixedExporter.getCount() << " shapes\n";
    assert(mixedExporter.getCount() == 4);
    std::cout << "   [OK] Mixed shape types handled correctly\n\n";

    std::cout << "12. Transform and then export:\n";
    Drawing transformDrawing;
    transformDrawing.addShape(std::make_unique<Circle>(5.0));

    std::cout << "   Before scaling:\n";
    XMLExporter beforeExport;
    transformDrawing.accept(&beforeExport);
    std::cout << beforeExport.getXML();

    ScaleTransformer doubler(2.0);
    transformDrawing.accept(&doubler);

    std::cout << "   After scaling:\n";
    XMLExporter afterExport;
    transformDrawing.accept(&afterExport);
    std::cout << afterExport.getXML();

    std::cout << "   [OK] Transform followed by export works\n\n";

    std::cout << "Visitor pattern demo completed successfully!\n";
    std::cout << "\nKey Benefits of Visitor Pattern:\n";
    std::cout << "- Add new operations without modifying element classes\n";
    std::cout << "- Groups related operations in visitor classes\n";
    std::cout << "- Visitor can accumulate state while traversing\n";
    std::cout << "- Makes adding new operations easy\n";
    std::cout << "- Separates algorithm from object structure\n";

    return 0;
}
