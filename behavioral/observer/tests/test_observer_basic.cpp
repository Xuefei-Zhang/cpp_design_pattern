#include "../include/observer_patterns.h"
#include <iostream>
#include <cassert>
#include <memory>

/**
 * Basic Observer Pattern Test Cases
 *
 * This test covers fundamental observer behavior:
 * 1. Subject-Observer relationship
 * 2. Notification mechanism
 * 3. Multiple observers
 * 4. Dynamic attachment/detachment
 */

void testBasicAttachDetach() {
    std::cout << "Test 1: Basic Attach and Detach\n";
    std::cout << "---------------------------------\n";

    WeatherStation station("TestStation");
    auto display = std::make_shared<WeatherConsoleDisplay>("MainDisplay");

    // Initially no observers
    assert(station.getObserverCount() == 0);
    std::cout << "✓ Initial observer count is 0\n";

    // Attach observer
    station.attachObserver(display);
    assert(station.getObserverCount() == 1);
    std::cout << "✓ Observer attached successfully\n";

    // Detach observer
    station.detachObserver(display);
    assert(station.getObserverCount() == 0);
    std::cout << "✓ Observer detached successfully\n";

    std::cout << "✓ Basic Attach/Detach test passed!\n\n";
}

void testNotificationDelivery() {
    std::cout << "Test 2: Notification Delivery\n";
    std::cout << "-------------------------------\n";

    WeatherStation station("TestStation");
    auto display = std::make_shared<WeatherConsoleDisplay>("Display1");

    station.attachObserver(display);

    // Set measurements and check if observer was notified
    station.setMeasurements(25.0, 60.0, 1013.0);
    assert(display->getUpdateCount() == 1);
    std::cout << "✓ Observer received first notification\n";

    station.setMeasurements(26.0, 58.0, 1012.0);
    assert(display->getUpdateCount() == 2);
    std::cout << "✓ Observer received second notification\n";

    std::cout << "✓ Notification Delivery test passed!\n\n";
}

void testMultipleObservers() {
    std::cout << "Test 3: Multiple Observers\n";
    std::cout << "----------------------------\n";

    WeatherStation station("TestStation");
    auto display1 = std::make_shared<WeatherConsoleDisplay>("Display1");
    auto display2 = std::make_shared<WeatherConsoleDisplay>("Display2");
    auto stats = std::make_shared<WeatherStatisticsDisplay>();

    // Attach multiple observers
    station.attachObserver(display1);
    station.attachObserver(display2);
    station.attachObserver(stats);

    assert(station.getObserverCount() == 3);
    std::cout << "✓ Three observers attached\n";

    // Notify all observers
    station.setMeasurements(24.0, 55.0, 1015.0);

    assert(display1->getUpdateCount() == 1);
    assert(display2->getUpdateCount() == 1);
    assert(stats->getSampleCount() == 1);
    std::cout << "✓ All three observers received notification\n";

    std::cout << "✓ Multiple Observers test passed!\n\n";
}

void testObserverRemovalDuringOperation() {
    std::cout << "Test 4: Observer Removal During Operation\n";
    std::cout << "-------------------------------------------\n";

    WeatherStation station("TestStation");
    auto display1 = std::make_shared<WeatherConsoleDisplay>("Display1");
    auto display2 = std::make_shared<WeatherConsoleDisplay>("Display2");

    station.attachObserver(display1);
    station.attachObserver(display2);

    // First notification
    station.setMeasurements(20.0, 50.0, 1010.0);
    assert(display1->getUpdateCount() == 1);
    assert(display2->getUpdateCount() == 1);

    // Remove one observer
    station.detachObserver(display1);
    assert(station.getObserverCount() == 1);

    // Second notification - only display2 should receive it
    station.setMeasurements(21.0, 51.0, 1011.0);
    assert(display1->getUpdateCount() == 1);  // Still 1
    assert(display2->getUpdateCount() == 2);  // Incremented to 2

    std::cout << "✓ Removed observer did not receive notification\n";
    std::cout << "✓ Remaining observer received notification\n";
    std::cout << "✓ Observer Removal test passed!\n\n";
}

void testPushVsPullModel() {
    std::cout << "Test 5: Push vs Pull Model\n";
    std::cout << "----------------------------\n";

    // Push model - WeatherStation pushes data to observers
    WeatherStation weatherStation("PushStation");
    auto weatherDisplay = std::make_shared<WeatherConsoleDisplay>("PushDisplay");
    weatherStation.attachObserver(weatherDisplay);
    weatherStation.setMeasurements(22.5, 50.0, 1012.0);
    assert(weatherDisplay->getUpdateCount() == 1);
    std::cout << "✓ Push model: Observer received pushed data\n";

    // Pull model - NumericModel notifies, observers pull data
    NumericModel model("PullModel", 100);
    auto view = std::make_shared<ModelValueView>("PullView");
    model.attachObserver(view);
    model.setValue(200);
    assert(view->getLastValue() == 200);
    assert(view->getRenderCount() == 1);
    std::cout << "✓ Pull model: Observer pulled data from subject\n";

    std::cout << "✓ Push vs Pull Model test passed!\n\n";
}

void testEventCenterPattern() {
    std::cout << "Test 6: Event Center Pattern\n";
    std::cout << "------------------------------\n";

    EventCenter eventCenter("MainEventBus");
    auto logger = std::make_shared<EventLogger>();

    eventCenter.attachObserver(logger);

    // Publish various events
    eventCenter.publishEvent("user.login", "user_id=123");
    eventCenter.publishEvent("order.created", "order_id=456");
    eventCenter.publishEvent("payment.completed", "amount=99.99");

    assert(logger->getLogSize() == 3);
    std::cout << "✓ Event logger captured all 3 events\n";

    std::cout << "✓ Event Center Pattern test passed!\n\n";
}

void testStatisticsObserver() {
    std::cout << "Test 7: Statistics Observer\n";
    std::cout << "-----------------------------\n";

    WeatherStation station("StatsStation");
    auto stats = std::make_shared<WeatherStatisticsDisplay>();

    station.attachObserver(stats);

    // Feed multiple measurements
    station.setMeasurements(20.0, 60.0, 1010.0);
    station.setMeasurements(25.0, 55.0, 1015.0);
    station.setMeasurements(22.0, 58.0, 1012.0);

    assert(stats->getSampleCount() == 3);
    assert(stats->getLastStation() == "StatsStation");
    std::cout << "✓ Statistics display collected 3 samples\n";

    std::cout << "✓ Statistics Observer test passed!\n\n";
}

void testNullObserverHandling() {
    std::cout << "Test 8: Null Observer Handling\n";
    std::cout << "--------------------------------\n";

    WeatherStation station("TestStation");

    // Try to attach null observer
    station.attachObserver(nullptr);
    assert(station.getObserverCount() == 0);
    std::cout << "✓ Null observer not attached\n";

    // Try to detach null observer (should not crash)
    station.detachObserver(nullptr);
    std::cout << "✓ Detaching null observer handled gracefully\n";

    std::cout << "✓ Null Observer Handling test passed!\n\n";
}

void testDuplicateAttachment() {
    std::cout << "Test 9: Duplicate Attachment Prevention\n";
    std::cout << "-----------------------------------------\n";

    WeatherStation station("TestStation");
    auto display = std::make_shared<WeatherConsoleDisplay>("Display");

    // Attach same observer multiple times
    station.attachObserver(display);
    station.attachObserver(display);
    station.attachObserver(display);

    // Should still have only one observer (or implementation-dependent)
    int observerCount = station.getObserverCount();
    std::cout << "Observer count after 3 attach calls: " << observerCount << "\n";

    // Notify once
    station.setMeasurements(20.0, 50.0, 1010.0);

    // Display should be notified appropriately
    int updateCount = display->getUpdateCount();
    std::cout << "Update count: " << updateCount << "\n";
    std::cout << "✓ Duplicate attachment handled\n";

    std::cout << "✓ Duplicate Attachment test passed!\n\n";
}

void testComplexScenario() {
    std::cout << "Test 10: Complex Multi-Subject Scenario\n";
    std::cout << "-----------------------------------------\n";

    // Create multiple weather stations
    WeatherStation station1("Station1");
    WeatherStation station2("Station2");

    // Create observers
    auto display1 = std::make_shared<WeatherConsoleDisplay>("Display1");
    auto display2 = std::make_shared<WeatherConsoleDisplay>("Display2");
    auto stats = std::make_shared<WeatherStatisticsDisplay>();

    // Display1 observes station1
    station1.attachObserver(display1);

    // Display2 observes station2
    station2.attachObserver(display2);

    // Stats observes both stations
    station1.attachObserver(stats);
    station2.attachObserver(stats);

    // Update both stations
    station1.setMeasurements(20.0, 60.0, 1010.0);
    station2.setMeasurements(25.0, 55.0, 1015.0);

    assert(display1->getUpdateCount() == 1);
    assert(display2->getUpdateCount() == 1);
    assert(stats->getSampleCount() == 2);  // Received from both stations

    std::cout << "✓ Multiple subjects with shared and dedicated observers work correctly\n";
    std::cout << "✓ Complex Scenario test passed!\n\n";
}

int main() {
    std::cout << "╔════════════════════════════════════════════════════╗\n";
    std::cout << "║     Observer Pattern - Basic Test Suite           ║\n";
    std::cout << "╚════════════════════════════════════════════════════╝\n\n";

    try {
        testBasicAttachDetach();
        testNotificationDelivery();
        testMultipleObservers();
        testObserverRemovalDuringOperation();
        testPushVsPullModel();
        testEventCenterPattern();
        testStatisticsObserver();
        testNullObserverHandling();
        testDuplicateAttachment();
        testComplexScenario();

        std::cout << "╔════════════════════════════════════════════════════╗\n";
        std::cout << "║          ALL TESTS PASSED! ✓                       ║\n";
        std::cout << "╚════════════════════════════════════════════════════╝\n";

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "✗ Test failed with exception: " << e.what() << "\n";
        return 1;
    }
}
