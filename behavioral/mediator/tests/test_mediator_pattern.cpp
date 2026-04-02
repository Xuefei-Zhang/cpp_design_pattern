#include "../include/mediator_patterns.h"

#include <cassert>
#include <iostream>
#include <memory>

int main() {
    std::cout << "=== Mediator Pattern Demo: Centralized Communication ===\n\n";

    std::cout << "1. Chat room with multiple users:\n";
    auto chatRoom = std::make_shared<ChatRoom>();
    auto alice = std::make_shared<User>("Alice");
    auto bob = std::make_shared<User>("Bob");
    auto charlie = std::make_shared<User>("Charlie");

    chatRoom->registerColleague(alice);
    chatRoom->registerColleague(bob);
    chatRoom->registerColleague(charlie);

    alice->send("Hello everyone!");
    bob->send("Hi Alice!");
    charlie->send("Hey folks!");

    assert(chatRoom->getMessageCount() == 3);
    assert(alice->getInboxSize() == 2); // Received from Bob and Charlie
    assert(bob->getInboxSize() == 2);   // Received from Alice and Charlie
    assert(charlie->getInboxSize() == 2); // Received from Alice and Bob
    std::cout << "   [OK] Messages distributed correctly\n\n";

    std::cout << "2. Chat room colleague count:\n";
    std::cout << "   Total colleagues: " << chatRoom->getColleagueCount() << "\n";
    assert(chatRoom->getColleagueCount() == 3);
    std::cout << "   [OK] Colleague count is correct\n\n";

    std::cout << "3. Air traffic control coordination:\n";
    auto atc = std::make_shared<AirTrafficControl>();
    auto flight1 = std::make_shared<Aircraft>("UA123");
    auto flight2 = std::make_shared<Aircraft>("AA456");
    auto flight3 = std::make_shared<Aircraft>("DL789");

    atc->registerColleague(flight1);
    atc->registerColleague(flight2);
    atc->registerColleague(flight3);

    assert(atc->getAircraftCount() == 3);
    std::cout << "   [OK] " << atc->getAircraftCount() << " aircraft registered\n\n";

    std::cout << "4. Aircraft communications through ATC:\n";
    flight1->send("request landing");
    flight2->send("request takeoff");

    assert(flight1->getCommunicationCount() == 1); // Received clearance
    assert(flight2->getCommunicationCount() == 1); // Received clearance
    std::cout << "   [OK] ATC coordinated aircraft communications\n\n";

    std::cout << "5. Smart home automation:\n";
    auto hub = std::make_shared<SmartHomeHub>();
    auto motionSensor = std::make_shared<SmartDevice>("MotionSensor");
    auto light1 = std::make_shared<SmartDevice>("Light-LivingRoom");
    auto light2 = std::make_shared<SmartDevice>("Light-Kitchen");
    auto ac = std::make_shared<SmartDevice>("AC-Main");

    hub->registerColleague(motionSensor);
    hub->registerColleague(light1);
    hub->registerColleague(light2);
    hub->registerColleague(ac);

    assert(hub->getDeviceCount() == 4);
    std::cout << "   [OK] " << hub->getDeviceCount() << " devices registered\n\n";

    std::cout << "6. Motion detection triggers lights:\n";
    motionSensor->send("motion detected");

    assert(light1->getActionCount() >= 1);
    assert(light2->getActionCount() >= 1);
    std::cout << "   [OK] Motion triggered lights through hub\n\n";

    std::cout << "7. Temperature control:\n";
    auto tempSensor = std::make_shared<SmartDevice>("TempSensor");
    hub->registerColleague(tempSensor);
    tempSensor->send("temperature high");

    assert(ac->getActionCount() >= 1);
    std::cout << "   [OK] Temperature sensor triggered AC through hub\n\n";

    std::cout << "8. Additional chat messages:\n";
    alice->send("How's everyone doing?");
    std::cout << "   Bob's inbox size: " << bob->getInboxSize() << "\n";
    std::cout << "   Charlie's inbox size: " << charlie->getInboxSize() << "\n";
    assert(bob->getInboxSize() == 3);
    assert(charlie->getInboxSize() == 3);
    std::cout << "   [OK] Additional messages delivered\n\n";

    std::cout << "9. Message log verification:\n";
    const auto& log = chatRoom->getMessageLog();
    std::cout << "   Total messages in log: " << log.size() << "\n";
    assert(log.size() == 4);
    std::cout << "   First message: " << log[0] << "\n";
    assert(log[0] == "Alice: Hello everyone!");
    std::cout << "   [OK] Message log is accurate\n\n";

    std::cout << "10. Multiple mediators independence:\n";
    auto chatRoom2 = std::make_shared<ChatRoom>();
    auto david = std::make_shared<User>("David");
    chatRoom2->registerColleague(david);
    david->send("Testing separate chat room");

    assert(chatRoom2->getMessageCount() == 1);
    assert(chatRoom->getMessageCount() == 4); // Unchanged
    std::cout << "   [OK] Multiple mediators work independently\n\n";

    std::cout << "11. Weak pointer cleanup simulation:\n";
    {
        auto tempUser = std::make_shared<User>("Temporary");
        chatRoom->registerColleague(tempUser);
        assert(chatRoom->getColleagueCount() == 4);
    }
    // tempUser destroyed, weak_ptr should be cleaned up on next access
    alice->send("Cleanup test");
    std::size_t currentCount = chatRoom->getColleagueCount();
    std::cout << "   Colleague count after cleanup: " << currentCount << "\n";
    assert(currentCount == 3); // tempUser should be cleaned up
    std::cout << "   [OK] Weak pointer cleanup works\n\n";

    std::cout << "Mediator pattern demo completed successfully!\n";
    std::cout << "\nKey Benefits of Mediator Pattern:\n";
    std::cout << "- Reduces coupling between colleague objects\n";
    std::cout << "- Centralizes control logic in one place\n";
    std::cout << "- Simplifies object protocols and communication\n";
    std::cout << "- Makes system behavior easier to understand and modify\n";
    std::cout << "- Colleagues don't need to know about each other\n";

    return 0;
}
