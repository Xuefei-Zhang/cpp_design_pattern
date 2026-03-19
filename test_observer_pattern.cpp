#include "observer_patterns.h"

#include <atomic>
#include <cassert>
#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

class CountingObserver : public Observer {
private:
    mutable std::mutex mutex_;
    std::size_t count_;

public:
    CountingObserver() : count_(0) {}

    void update(const Subject&, const std::string&, const std::string&) override {
        std::lock_guard<std::mutex> lock(mutex_);
        ++count_;
    }

    std::size_t getCount() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return count_;
    }
};

class SelfDetachingObserver : public Observer {
private:
    Subject* subject_;
    std::shared_ptr<Observer> selfKeepAlive_;
    std::atomic<bool> detached_;

public:
    SelfDetachingObserver() : subject_(nullptr), detached_(false) {}

    void bind(Subject* subject, const std::shared_ptr<Observer>& self) {
        subject_ = subject;
        selfKeepAlive_ = self;
    }

    void update(const Subject&, const std::string&, const std::string&) override {
        if (subject_ && !detached_.exchange(true)) {
            subject_->detachObserver(selfKeepAlive_);
        }
    }

    bool isDetached() const { return detached_.load(); }
};

int main() {
    std::cout << "=== Observer Pattern Demo: Thread-Safe Push/Pull Notifications ===\n\n";

    std::cout << "1. Basic push + pull weather notifications:\n";
    WeatherStation station("CityCenter");
    auto phoneDisplay = std::make_shared<WeatherConsoleDisplay>("Phone");
    auto lobbyDisplay = std::make_shared<WeatherConsoleDisplay>("Lobby");
    auto statsDisplay = std::make_shared<WeatherStatisticsDisplay>();

    station.attachObserver(phoneDisplay);
    station.attachObserver(lobbyDisplay);
    station.attachObserver(statsDisplay);

    station.setMeasurements(22.5, 50.0, 1012.0);
    station.setMeasurements(24.0, 48.0, 1009.0);

    assert(station.getObserverCount() == 3);
    assert(phoneDisplay->getUpdateCount() == 2);
    assert(lobbyDisplay->getUpdateCount() == 2);
    assert(statsDisplay->getSampleCount() == 2);
    assert(statsDisplay->getLastStation() == "CityCenter");
    std::cout << "   [OK] Push and pull observers received updates.\n\n";

    std::cout << "2. Detach behavior and boundary condition checks:\n";
    station.detachObserver(lobbyDisplay);
    station.detachObserver(nullptr);
    station.setMeasurements(26.0, 45.0, 1006.0);

    assert(station.getObserverCount() == 2);
    assert(phoneDisplay->getUpdateCount() == 3);
    assert(lobbyDisplay->getUpdateCount() == 2);
    assert(statsDisplay->getSampleCount() == 3);
    std::cout << "   [OK] detachObserver removed target and ignored null safely.\n\n";

    std::cout << "3. Event center example (push-oriented event stream):\n";
    EventCenter eventCenter("MainBus");
    auto logger = std::make_shared<EventLogger>();
    eventCenter.attachObserver(logger);
    eventCenter.publishEvent("user.login", "id=alice");
    eventCenter.publishEvent("order.created", "id=ORD-1001");

    assert(logger->getLogSize() == 2);
    std::cout << "   [OK] Event logger captured two events.\n\n";

    std::cout << "4. Model-view pull example:\n";
    NumericModel model("TemperatureThreshold", 10);
    auto viewA = std::make_shared<ModelValueView>("A");
    auto viewB = std::make_shared<ModelValueView>("B");
    model.attachObserver(viewA);
    model.attachObserver(viewB);
    model.setValue(20);
    model.setValue(30);

    assert(viewA->getLastValue() == 30);
    assert(viewB->getLastValue() == 30);
    assert(viewA->getRenderCount() == 2);
    assert(viewB->getRenderCount() == 2);
    std::cout << "   [OK] Views pulled latest model value correctly.\n\n";

    std::cout << "5. Re-entrant detach during notify (deadlock safety check):\n";
    WeatherStation reentrantStation("Reentrant");
    auto counter = std::make_shared<CountingObserver>();
    auto selfDetaching = std::make_shared<SelfDetachingObserver>();
    selfDetaching->bind(&reentrantStation, selfDetaching);

    reentrantStation.attachObserver(counter);
    reentrantStation.attachObserver(selfDetaching);

    reentrantStation.setMeasurements(10.0, 30.0, 1000.0);
    reentrantStation.setMeasurements(11.0, 31.0, 1001.0);

    assert(selfDetaching->isDetached());
    assert(reentrantStation.getObserverCount() == 1);
    assert(counter->getCount() == 2);
    std::cout << "   [OK] Observer detached itself during callback without deadlock.\n\n";

    std::cout << "6. Concurrent attach/detach/notify stress test:\n";
    WeatherStation concurrentStation("ConcurrentStation");
    std::vector<std::shared_ptr<CountingObserver>> observers;
    observers.reserve(64);
    for (int i = 0; i < 64; ++i) {
        observers.push_back(std::make_shared<CountingObserver>());
    }

    std::atomic<bool> start{false};
    std::atomic<bool> done{false};
    std::atomic<int> notificationsSent{0};

    std::thread notifier([&]() {
        while (!start.load()) {
            std::this_thread::yield();
        }
        for (int i = 0; i < 200; ++i) {
            concurrentStation.setMeasurements(15.0 + i * 0.1, 40.0 + i * 0.05, 1000.0 + i * 0.2);
            ++notificationsSent;
            if (i % 10 == 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }
        done.store(true);
    });

    std::thread attacher1([&]() {
        while (!start.load()) {
            std::this_thread::yield();
        }
        for (std::size_t i = 0; i < observers.size(); ++i) {
            if ((i % 2) == 0) {
                concurrentStation.attachObserver(observers[i]);
            }
        }
    });

    std::thread attacher2([&]() {
        while (!start.load()) {
            std::this_thread::yield();
        }
        for (std::size_t i = 0; i < observers.size(); ++i) {
            if ((i % 2) == 1) {
                concurrentStation.attachObserver(observers[i]);
            }
        }
    });

    std::thread flapper([&]() {
        while (!start.load()) {
            std::this_thread::yield();
        }

        std::size_t rounds = 0;
        while (!done.load()) {
            for (std::size_t i = 0; i < observers.size(); i += 3) {
                concurrentStation.detachObserver(observers[i]);
                concurrentStation.attachObserver(observers[i]);
            }
            ++rounds;
            if (rounds % 8 == 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }
    });

    start.store(true);

    notifier.join();
    attacher1.join();
    attacher2.join();
    flapper.join();

    const std::size_t countAfter = concurrentStation.getObserverCount();
    assert(countAfter <= observers.size());

    std::size_t totalReceived = 0;
    for (const auto& observer : observers) {
        totalReceived += observer->getCount();
    }

    assert(notificationsSent.load() == 200);
    assert(totalReceived > 0);
    std::cout << "   notifications sent: " << notificationsSent.load() << "\n";
    std::cout << "   observer count after stress: " << countAfter << "\n";
    std::cout << "   total observer callbacks observed: " << totalReceived << "\n";
    std::cout << "   [OK] Concurrent operations completed safely.\n\n";

    std::cout << "Observer pattern demo completed successfully!\n";
    std::cout << "\nKey Benefits of Observer Pattern:\n";
    std::cout << "- Loose coupling between subjects and observers\n";
    std::cout << "- Broadcast-style one-to-many updates\n";
    std::cout << "- Supports both push and pull notification models\n";
    std::cout << "- Thread-safe registration and notification for concurrent systems\n";
    std::cout << "- Easy extension by adding new observer types\n";

    return 0;
}
