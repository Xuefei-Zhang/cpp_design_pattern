#include "template_method_patterns.h"

#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

int main() {
    std::cout << "=== Template Method Pattern Demo: Algorithm Skeleton ===\n\n";

    std::cout << "1. CSV data processing:\n";
    CSVProcessor csvProc;
    std::string csvResult = csvProc.process("a,b,c");
    std::cout << "   Result: " << csvResult << "\n";
    assert(csvResult.find("CSV_OUTPUT") != std::string::npos);
    assert(csvResult.find(";") != std::string::npos); // Commas replaced with semicolons
    std::cout << "   Log: " << csvProc.getLog() << "\n";
    std::cout << "   [OK] CSV processing completed\n\n";

    std::cout << "2. JSON data processing:\n";
    JSONProcessor jsonProc(false);
    std::string jsonResult = jsonProc.process("test_data");
    std::cout << "   Result: " << jsonResult << "\n";
    assert(jsonResult.find("JSON_OUTPUT") != std::string::npos);
    assert(jsonResult.find("{\"data\"") != std::string::npos);
    std::cout << "   Log: " << jsonProc.getLog() << "\n";
    std::cout << "   [OK] JSON processing completed\n\n";

    std::cout << "3. JSON strict validation:\n";
    JSONProcessor strictJsonProc(true);
    std::string strictResult = strictJsonProc.process("invalid");
    std::cout << "   Result: " << strictResult << "\n";
    assert(strictResult.find("INVALID") == std::string::npos); // Should still process
    std::cout << "   [OK] Strict JSON validation applied\n\n";

    std::cout << "4. XML data processing:\n";
    XMLProcessor xmlProc;
    std::string xmlResult = xmlProc.process("content");
    std::cout << "   Result: " << xmlResult << "\n";
    assert(xmlResult.find("XML_OUTPUT") != std::string::npos);
    assert(xmlResult.find("<data>") != std::string::npos);
    std::cout << "   [OK] XML processing completed\n\n";

    std::cout << "5. Tea beverage preparation:\n";
    Tea tea;
    tea.prepareRecipe();
    std::string teaLog = tea.getPreparationLog();
    std::cout << teaLog;
    assert(teaLog.find("Boiling water") != std::string::npos);
    assert(teaLog.find("Steeping the tea") != std::string::npos);
    assert(teaLog.find("Adding lemon") != std::string::npos);
    std::cout << "   [OK] Tea preparation completed\n\n";

    std::cout << "6. Coffee beverage preparation:\n";
    Coffee coffee;
    coffee.prepareRecipe();
    std::string coffeeLog = coffee.getPreparationLog();
    std::cout << coffeeLog;
    assert(coffeeLog.find("Boiling water") != std::string::npos);
    assert(coffeeLog.find("Dripping coffee") != std::string::npos);
    assert(coffeeLog.find("Adding sugar and milk") != std::string::npos);
    assert(coffeeLog.find("enjoy!") != std::string::npos); // Custom final step
    std::cout << "   [OK] Coffee preparation completed\n\n";

    std::cout << "7. Forest level loading:\n";
    ForestLevel forestLevel;
    std::string forestLog = forestLevel.loadLevel("Forest-01");
    std::cout << forestLog;
    assert(forestLog.find("Loading trees") != std::string::npos);
    assert(forestLog.find("forest creatures") != std::string::npos);
    assert(forestLog.find("forest weather") != std::string::npos);
    std::cout << "   [OK] Forest level loaded\n\n";

    std::cout << "8. Dungeon level loading:\n";
    DungeonLevel dungeonLevel;
    std::string dungeonLog = dungeonLevel.loadLevel("Dungeon-01");
    std::cout << dungeonLog;
    assert(dungeonLog.find("dungeon walls") != std::string::npos);
    assert(dungeonLog.find("monsters and traps") != std::string::npos);
    assert(dungeonLog.find("dark dungeon") != std::string::npos);
    assert(dungeonLog.find("combat-heavy") != std::string::npos);
    std::cout << "   [OK] Dungeon level loaded\n\n";

    std::cout << "9. HTML report generation:\n";
    HTMLReport htmlReport;
    std::vector<std::string> data = {"Item 1", "Item 2", "Item 3"};
    std::string htmlOutput = htmlReport.generateReport(data);
    std::cout << htmlOutput;
    assert(htmlOutput.find("<html>") != std::string::npos);
    assert(htmlOutput.find("<li>Item 1</li>") != std::string::npos);
    assert(htmlOutput.find("<li>Item 2</li>") != std::string::npos);
    assert(htmlOutput.find("</html>") != std::string::npos);
    std::cout << "   [OK] HTML report generated\n\n";

    std::cout << "10. Text report generation:\n";
    TextReport textReport;
    std::string textOutput = textReport.generateReport(data);
    std::cout << textOutput;
    assert(textOutput.find("TEXT REPORT") != std::string::npos);
    assert(textOutput.find("* Item 1") != std::string::npos);
    assert(textOutput.find("* Item 2") != std::string::npos);
    assert(textOutput.find("END OF REPORT") != std::string::npos);
    std::cout << "   [OK] Text report generated\n\n";

    std::cout << "11. Polymorphic usage with base class pointer:\n";
    std::unique_ptr<DataProcessor> processor = std::make_unique<CSVProcessor>();
    std::string polymorphicResult = processor->process("x,y,z");
    std::cout << "   Polymorphic result: " << polymorphicResult << "\n";
    assert(polymorphicResult.find("CSV_OUTPUT") != std::string::npos);
    std::cout << "   [OK] Polymorphic template method works\n\n";

    std::cout << "12. Multiple beverage preparations:\n";
    std::vector<std::unique_ptr<Beverage>> beverages;
    beverages.push_back(std::make_unique<Tea>());
    beverages.push_back(std::make_unique<Coffee>());

    int count = 0;
    for (auto& beverage : beverages) {
        beverage->prepareRecipe();
        ++count;
    }
    assert(count == 2);
    std::cout << "   [OK] Prepared " << count << " different beverages\n\n";

    std::cout << "Template Method pattern demo completed successfully!\n";
    std::cout << "\nKey Benefits of Template Method Pattern:\n";
    std::cout << "- Defines algorithm structure in base class\n";
    std::cout << "- Subclasses customize specific steps without changing structure\n";
    std::cout << "- Code reuse through inheritance\n";
    std::cout << "- Enforces consistent algorithm flow\n";
    std::cout << "- Hook methods provide optional customization points\n";

    return 0;
}
