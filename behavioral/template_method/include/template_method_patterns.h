#ifndef TEMPLATE_METHOD_PATTERNS_H
#define TEMPLATE_METHOD_PATTERNS_H

#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

// Abstract class defining template method
class DataProcessor {
public:
    virtual ~DataProcessor() = default;

    // Template method defines the algorithm skeleton
    std::string process(const std::string& input) {
        std::string data = readData(input);
        std::string validated = validateData(data);
        std::string processed = processData(validated);
        std::string formatted = formatOutput(processed);
        saveResults(formatted);
        return formatted;
    }

protected:
    // Abstract methods - must be implemented by subclasses
    virtual std::string readData(const std::string& source) = 0;
    virtual std::string processData(const std::string& data) = 0;

    // Hook methods - can be overridden but have default implementation
    virtual std::string validateData(const std::string& data) {
        // Default validation
        return data.empty() ? "INVALID" : data;
    }

    virtual std::string formatOutput(const std::string& data) {
        // Default formatting
        return data;
    }

    virtual void saveResults(const std::string& results) {
        // Default: do nothing
        (void)results; // Suppress unused parameter warning
    }
};

// Concrete implementation - CSV Processor
class CSVProcessor : public DataProcessor {
private:
    std::string outputLog_;

protected:
    std::string readData(const std::string& source) override {
        return "CSV_DATA:" + source;
    }

    std::string processData(const std::string& data) override {
        // Simulate CSV processing
        std::string result = data;
        std::replace(result.begin(), result.end(), ',', ';');
        return result;
    }

    std::string formatOutput(const std::string& data) override {
        return "[CSV_OUTPUT] " + data;
    }

    void saveResults(const std::string& results) override {
        outputLog_ = "Saved CSV: " + results;
    }

public:
    const std::string& getLog() const {
        return outputLog_;
    }
};

// Concrete implementation - JSON Processor
class JSONProcessor : public DataProcessor {
private:
    bool strict_;
    std::string outputLog_;

public:
    explicit JSONProcessor(bool strict = false) : strict_(strict) {}

protected:
    std::string readData(const std::string& source) override {
        return "{\"data\":\"" + source + "\"}";
    }

    std::string validateData(const std::string& data) override {
        if (strict_ && data.find("{") == std::string::npos) {
            return "INVALID_JSON";
        }
        return DataProcessor::validateData(data);
    }

    std::string processData(const std::string& data) override {
        // Simulate JSON processing
        return data + "_PROCESSED";
    }

    std::string formatOutput(const std::string& data) override {
        return "[JSON_OUTPUT] " + data;
    }

    void saveResults(const std::string& results) override {
        outputLog_ = "Saved JSON: " + results;
    }

public:
    const std::string& getLog() const {
        return outputLog_;
    }
};

// Concrete implementation - XML Processor
class XMLProcessor : public DataProcessor {
protected:
    std::string readData(const std::string& source) override {
        return "<data>" + source + "</data>";
    }

    std::string processData(const std::string& data) override {
        // Simulate XML processing
        std::string result = data;
        std::size_t pos = result.find("><");
        if (pos != std::string::npos) {
            result.insert(pos + 1, "processed");
        }
        return result;
    }

    std::string formatOutput(const std::string& data) override {
        return "[XML_OUTPUT] " + data;
    }
};

// Beverage preparation example
class Beverage {
public:
    virtual ~Beverage() = default;

    // Template method
    void prepareRecipe() {
        boilWater();
        brew();
        pourInCup();
        addCondiments();
        finalStep();
    }

    std::string getPreparationLog() const {
        return preparationLog_;
    }

protected:
    std::string preparationLog_;

    void boilWater() {
        preparationLog_ += "Boiling water\n";
    }

    void pourInCup() {
        preparationLog_ += "Pouring into cup\n";
    }

    // Abstract methods
    virtual void brew() = 0;
    virtual void addCondiments() = 0;

    // Hook method with default implementation
    virtual void finalStep() {
        preparationLog_ += "Beverage ready!\n";
    }
};

class Tea : public Beverage {
protected:
    void brew() override {
        preparationLog_ += "Steeping the tea\n";
    }

    void addCondiments() override {
        preparationLog_ += "Adding lemon\n";
    }
};

class Coffee : public Beverage {
protected:
    void brew() override {
        preparationLog_ += "Dripping coffee through filter\n";
    }

    void addCondiments() override {
        preparationLog_ += "Adding sugar and milk\n";
    }

    void finalStep() override {
        preparationLog_ += "Coffee is ready - enjoy!\n";
    }
};

// Game level loading example
class LevelLoader {
public:
    virtual ~LevelLoader() = default;

    // Template method
    std::string loadLevel(const std::string& levelName) {
        std::string log;
        log += "=== Loading " + levelName + " ===\n";

        log += loadAssets();
        log += initializeEntities();
        log += setupEnvironment();
        log += configureGameplay();
        log += finalizeLevel();

        log += "=== Level loaded successfully ===\n";
        return log;
    }

protected:
    // Abstract methods
    virtual std::string loadAssets() = 0;
    virtual std::string initializeEntities() = 0;

    // Hook methods with defaults
    virtual std::string setupEnvironment() {
        return "Setting up default environment\n";
    }

    virtual std::string configureGameplay() {
        return "Configuring standard gameplay\n";
    }

    virtual std::string finalizeLevel() {
        return "Finalizing level\n";
    }
};

class ForestLevel : public LevelLoader {
protected:
    std::string loadAssets() override {
        return "Loading trees, rocks, and animals\n";
    }

    std::string initializeEntities() override {
        return "Spawning forest creatures\n";
    }

    std::string setupEnvironment() override {
        return "Setting up forest weather and lighting\n";
    }
};

class DungeonLevel : public LevelLoader {
protected:
    std::string loadAssets() override {
        return "Loading dungeon walls, torches, and treasures\n";
    }

    std::string initializeEntities() override {
        return "Spawning monsters and traps\n";
    }

    std::string setupEnvironment() override {
        return "Setting up dark dungeon atmosphere\n";
    }

    std::string configureGameplay() override {
        return "Configuring combat-heavy gameplay\n";
    }
};

// Report generation example
class ReportGenerator {
public:
    virtual ~ReportGenerator() = default;

    std::string generateReport(const std::vector<std::string>& data) {
        std::ostringstream report;
        report << openReport();
        report << writeHeader();

        for (const auto& item : data) {
            report << formatDataItem(item);
        }

        report << writeFooter();
        report << closeReport();

        return report.str();
    }

protected:
    virtual std::string openReport() = 0;
    virtual std::string closeReport() = 0;
    virtual std::string formatDataItem(const std::string& item) = 0;

    virtual std::string writeHeader() {
        return "--- Report Start ---\n";
    }

    virtual std::string writeFooter() {
        return "--- Report End ---\n";
    }
};

class HTMLReport : public ReportGenerator {
protected:
    std::string openReport() override {
        return "<html><body>\n";
    }

    std::string closeReport() override {
        return "</body></html>\n";
    }

    std::string writeHeader() override {
        return "<h1>Report</h1>\n<ul>\n";
    }

    std::string formatDataItem(const std::string& item) override {
        return "<li>" + item + "</li>\n";
    }

    std::string writeFooter() override {
        return "</ul>\n";
    }
};

class TextReport : public ReportGenerator {
protected:
    std::string openReport() override {
        return "TEXT REPORT\n";
    }

    std::string closeReport() override {
        return "END OF REPORT\n";
    }

    std::string formatDataItem(const std::string& item) override {
        return "* " + item + "\n";
    }
};

// Inline usage examples:
// CSVProcessor csvProc;
// std::string result = csvProc.process("a,b,c");
//
// Coffee coffee;
// coffee.prepareRecipe();

#endif // TEMPLATE_METHOD_PATTERNS_H
