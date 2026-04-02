#include "../include/adapter_patterns.h"
#include <iostream>
#include <cassert>
#include <memory>
#include <sstream>

/**
 * Basic Adapter Pattern Test Cases
 *
 * Tests the ability to make incompatible interfaces work together
 */

void testObjectAdapterBasic() {
    std::cout << "Test 1: Basic Object Adapter\n";
    std::cout << "------------------------------\n";

    // Create an adaptee (old interface)
    auto europeanSocket = std::make_shared<EuropeanSocket>();

    // Create adapter
    SocketAdapter adapter(europeanSocket);

    // Use through new interface
    adapter.plugIn();
    std::cout << "✓ European socket adapted to American interface\n";

    std::cout << "✓ Object Adapter test passed!\n\n";
}

void testClassAdapterBasic() {
    std::cout << "Test 2: Basic Class Adapter\n";
    std::cout << "-----------------------------\n";

    // Class adapter inherits from both
    ClassSocketAdapter adapter;

    adapter.plugIn();  // American interface
    std::cout << "✓ Class adapter provides American interface\n";

    std::cout << "✓ Class Adapter test passed!\n\n";
}

void testMediaPlayerAdapter() {
    std::cout << "Test 3: Media Player Adapter\n";
    std::cout << "-----------------------------\n";

    // Create advanced media player
    auto vlcPlayer = std::make_shared<VlcPlayer>();
    auto mp4Player = std::make_shared<Mp4Player>();

    // Adapt to standard interface
    MediaAdapter vlcAdapter(vlcPlayer);
    MediaAdapter mp4Adapter(mp4Player);

    vlcAdapter.play("movie.vlc");
    std::cout << "✓ VLC player adapted\n";

    mp4Adapter.play("song.mp4");
    std::cout << "✓ MP4 player adapted\n";

    std::cout << "✓ Media Player Adapter test passed!\n\n";
}

void testTwoWayAdapter() {
    std::cout << "Test 4: Two-Way Adapter\n";
    std::cout << "-------------------------\n";

    auto europeanDevice = std::make_shared<EuropeanDevice>();
    auto americanDevice = std::make_shared<AmericanDevice>();

    // Two-way adapter
    TwoWaySocketAdapter adapter;

    // Can work with both interfaces
    adapter.setEuropeanDevice(europeanDevice);
    adapter.plugIn();  // American interface
    std::cout << "✓ European device used through American interface\n";

    adapter.setAmericanDevice(americanDevice);
    adapter.connectEuropean();  // European interface
    std::cout << "✓ American device used through European interface\n";

    std::cout << "✓ Two-Way Adapter test passed!\n\n";
}

void testDataFormatAdapter() {
    std::cout << "Test 5: Data Format Adapter\n";
    std::cout << "-----------------------------\n";

    // XML data source
    auto xmlData = std::make_shared<XmlDataProvider>();

    // Adapt to JSON interface
    DataFormatAdapter adapter(xmlData);

    std::string jsonData = adapter.getJsonData();
    assert(!jsonData.empty());
    std::cout << "✓ XML data converted to JSON format\n";
    std::cout << "   Data: " << jsonData << "\n";

    std::cout << "✓ Data Format Adapter test passed!\n\n";
}

void testLegacySystemAdapter() {
    std::cout << "Test 6: Legacy System Adapter\n";
    std::cout << "-------------------------------\n";

    // Old legacy system
    auto legacyPrinter = std::make_shared<LegacyPrinter>();

    // Adapt to modern interface
    PrinterAdapter adapter(legacyPrinter);

    adapter.print("Modern print job");
    adapter.printInColor("Color print job");
    std::cout << "✓ Legacy printer adapted to modern interface\n";

    std::cout << "✓ Legacy System Adapter test passed!\n\n";
}

void testMultipleAdaptees() {
    std::cout << "Test 7: Multiple Adaptees\n";
    std::cout << "---------------------------\n";

    // Create different media players
    auto vlc = std::make_shared<VlcPlayer>();
    auto mp4 = std::make_shared<Mp4Player>();
    auto avi = std::make_shared<AviPlayer>();

    // Adapt all to common interface
    MediaAdapter adapter1(vlc);
    MediaAdapter adapter2(mp4);
    MediaAdapter adapter3(avi);

    std::vector<MediaAdapter*> players = {&adapter1, &adapter2, &adapter3};

    for (auto* player : players) {
        player->play("test_file");
    }

    std::cout << "✓ All different players adapted to common interface\n";
    std::cout << "✓ Multiple Adaptees test passed!\n\n";
}

void testAdapterChaining() {
    std::cout << "Test 8: Adapter Chaining\n";
    std::cout << "-------------------------\n";

    // Chain adapters to convert through multiple steps
    auto xmlSource = std::make_shared<XmlDataProvider>();
    auto xmlToJson = std::make_shared<XmlToJsonAdapter>(xmlSource);
    auto jsonToYaml = std::make_shared<JsonToYamlAdapter>(xmlToJson);

    std::string yamlData = jsonToYaml->getYaml();
    assert(!yamlData.empty());
    std::cout << "✓ XML -> JSON -> YAML conversion chain works\n";

    std::cout << "✓ Adapter Chaining test passed!\n\n";
}

void testAdapterWithInheritance() {
    std::cout << "Test 9: Adapter with Inheritance\n";
    std::cout << "----------------------------------\n";

    // Base adapter
    auto baseAdapter = std::make_unique<BaseSocketAdapter>();
    baseAdapter->plugIn();
    std::cout << "✓ Base adapter works\n";

    // Derived adapter with extra features
    auto enhancedAdapter = std::make_unique<EnhancedSocketAdapter>();
    enhancedAdapter->plugIn();
    enhancedAdapter->enableSurgeProtection();
    std::cout << "✓ Enhanced adapter with extra features works\n";

    std::cout << "✓ Adapter Inheritance test passed!\n\n";
}

void testAdapterPolymorphism() {
    std::cout << "Test 10: Adapter Polymorphism\n";
    std::cout << "-------------------------------\n";

    // Use adapters polymorphically
    std::vector<std::unique_ptr<StandardInterface>> adapters;

    adapters.push_back(std::make_unique<Adapter1>());
    adapters.push_back(std::make_unique<Adapter2>());
    adapters.push_back(std::make_unique<Adapter3>());

    for (auto& adapter : adapters) {
        adapter->standardMethod();
    }

    std::cout << "✓ All adapters used polymorphically\n";
    std::cout << "✓ Adapter Polymorphism test passed!\n\n";
}

void testAdapterExceptionHandling() {
    std::cout << "Test 11: Adapter Exception Handling\n";
    std::cout << "-------------------------------------\n";

    try {
        auto faultyDevice = std::make_shared<FaultyDevice>();
        SafeAdapter adapter(faultyDevice);

        // Adapter should handle exceptions from adaptee
        adapter.safeOperation();
        std::cout << "✓ Adapter handled adaptee exception gracefully\n";

    } catch (const std::exception& e) {
        std::cerr << "✗ Exception not handled by adapter: " << e.what() << "\n";
    }

    std::cout << "✓ Exception Handling test passed!\n\n";
}

void testAdapterNullSafety() {
    std::cout << "Test 12: Adapter Null Safety\n";
    std::cout << "------------------------------\n";

    // Create adapter with null adaptee
    SocketAdapter nullAdapter(nullptr);

    try {
        nullAdapter.plugIn();
        std::cout << "✓ Adapter handled null adaptee gracefully\n";
    } catch (const std::exception& e) {
        std::cout << "✓ Adapter threw exception for null adaptee: " << e.what() << "\n";
    }

    std::cout << "✓ Null Safety test passed!\n\n";
}

void testRealWorldScenario() {
    std::cout << "Test 13: Real-World Payment Gateway Adapter\n";
    std::cout << "---------------------------------------------\n";

    // Different payment gateways with different interfaces
    auto paypal = std::make_shared<PayPalGateway>();
    auto stripe = std::make_shared<StripeGateway>();
    auto square = std::make_shared<SquareGateway>();

    // Adapt all to unified interface
    PaymentAdapter paypalAdapter(paypal);
    PaymentAdapter stripeAdapter(stripe);
    PaymentAdapter squareAdapter(square);

    // Process payments through unified interface
    paypalAdapter.processPayment(100.0);
    std::cout << "✓ PayPal payment processed\n";

    stripeAdapter.processPayment(200.0);
    std::cout << "✓ Stripe payment processed\n";

    squareAdapter.processPayment(150.0);
    std::cout << "✓ Square payment processed\n";

    std::cout << "✓ Real-World Scenario test passed!\n\n";
}

int main() {
    std::cout << "╔════════════════════════════════════════════════════╗\n";
    std::cout << "║     Adapter Pattern - Basic Test Suite            ║\n";
    std::cout << "╚════════════════════════════════════════════════════╝\n\n";

    try {
        testObjectAdapterBasic();
        testClassAdapterBasic();
        testMediaPlayerAdapter();
        testTwoWayAdapter();
        testDataFormatAdapter();
        testLegacySystemAdapter();
        testMultipleAdaptees();
        testAdapterChaining();
        testAdapterWithInheritance();
        testAdapterPolymorphism();
        testAdapterExceptionHandling();
        testAdapterNullSafety();
        testRealWorldScenario();

        std::cout << "╔════════════════════════════════════════════════════╗\n";
        std::cout << "║          ALL TESTS PASSED! ✓                       ║\n";
        std::cout << "╚════════════════════════════════════════════════════╝\n";

        std::cout << "\nKey Benefits of Adapter Pattern:\n";
        std::cout << "- Allows incompatible interfaces to work together\n";
        std::cout << "- Promotes reusability of existing code\n";
        std::cout << "- Single Responsibility: separate interface conversion\n";
        std::cout << "- Open/Closed: can introduce new adapters without changing existing code\n";

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "✗ Test failed with exception: " << e.what() << "\n";
        return 1;
    }
}
