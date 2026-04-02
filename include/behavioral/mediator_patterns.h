#ifndef MEDIATOR_PATTERNS_H
#define MEDIATOR_PATTERNS_H

#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

// Forward declarations
class Colleague;
class Mediator;

// Abstract Mediator Interface
class Mediator {
public:
    virtual ~Mediator() = default;
    virtual void registerColleague(const std::shared_ptr<Colleague>& colleague) = 0;
    virtual void sendMessage(const std::string& message, Colleague* sender) = 0;
};

// Abstract Colleague
class Colleague {
protected:
    Mediator* mediator_;
    std::string name_;

public:
    Colleague(const std::string& name) : mediator_(nullptr), name_(name) {}
    virtual ~Colleague() = default;

    void setMediator(Mediator* mediator) {
        mediator_ = mediator;
    }

    const std::string& getName() const {
        return name_;
    }

    virtual void send(const std::string& message) = 0;
    virtual void receive(const std::string& message, const std::string& from) = 0;
};

// Concrete Mediator - Chat Room
class ChatRoom : public Mediator {
private:
    std::vector<std::weak_ptr<Colleague>> colleagues_;
    std::vector<std::string> messageLog_;

public:
    void registerColleague(const std::shared_ptr<Colleague>& colleague) override {
        if (!colleague) {
            return;
        }

        colleague->setMediator(this);
        colleagues_.push_back(colleague);
    }

    void sendMessage(const std::string& message, Colleague* sender) override {
        if (!sender) {
            return;
        }

        std::string logEntry = sender->getName() + ": " + message;
        messageLog_.push_back(logEntry);

        // Send to all colleagues except sender
        for (auto it = colleagues_.begin(); it != colleagues_.end();) {
            std::shared_ptr<Colleague> colleague = it->lock();
            if (!colleague) {
                it = colleagues_.erase(it);
                continue;
            }

            if (colleague.get() != sender) {
                colleague->receive(message, sender->getName());
            }
            ++it;
        }
    }

    std::size_t getMessageCount() const {
        return messageLog_.size();
    }

    const std::vector<std::string>& getMessageLog() const {
        return messageLog_;
    }

    std::size_t getColleagueCount() const {
        std::size_t count = 0;
        for (const auto& weak : colleagues_) {
            if (weak.lock()) {
                ++count;
            }
        }
        return count;
    }
};

// Concrete Colleague - User
class User : public Colleague {
private:
    std::vector<std::string> inbox_;

public:
    explicit User(const std::string& name) : Colleague(name) {}

    void send(const std::string& message) override {
        if (mediator_) {
            std::cout << "[" << name_ << " sends]: " << message << "\n";
            mediator_->sendMessage(message, this);
        }
    }

    void receive(const std::string& message, const std::string& from) override {
        std::string fullMessage = "[" + from + " -> " + name_ + "]: " + message;
        inbox_.push_back(fullMessage);
        std::cout << fullMessage << "\n";
    }

    std::size_t getInboxSize() const {
        return inbox_.size();
    }

    const std::vector<std::string>& getInbox() const {
        return inbox_;
    }
};

// Air Traffic Control Example
class Aircraft;

class AirTrafficControl : public Mediator {
private:
    std::vector<std::weak_ptr<Colleague>> aircraft_;
    std::map<std::string, std::string> airspaceStatus_;

public:
    void registerColleague(const std::shared_ptr<Colleague>& colleague) override {
        if (!colleague) {
            return;
        }

        colleague->setMediator(this);
        aircraft_.push_back(colleague);
        std::cout << "[ATC] Aircraft " << colleague->getName() << " registered\n";
    }

    void sendMessage(const std::string& message, Colleague* sender) override {
        if (!sender) {
            return;
        }

        std::cout << "[ATC receives from " << sender->getName() << "]: " << message << "\n";

        // Process the message and respond
        if (message == "request landing") {
            std::string response = "Landing cleared, runway 27";
            sender->receive(response, "ATC");
        } else if (message == "request takeoff") {
            std::string response = "Takeoff cleared, runway 09";
            sender->receive(response, "ATC");
        } else {
            sender->receive("Message received", "ATC");
        }
    }

    std::size_t getAircraftCount() const {
        std::size_t count = 0;
        for (const auto& weak : aircraft_) {
            if (weak.lock()) {
                ++count;
            }
        }
        return count;
    }
};

class Aircraft : public Colleague {
private:
    std::vector<std::string> communications_;

public:
    explicit Aircraft(const std::string& callSign) : Colleague(callSign) {}

    void send(const std::string& message) override {
        if (mediator_) {
            std::cout << "[" << name_ << " transmits]: " << message << "\n";
            mediator_->sendMessage(message, this);
        }
    }

    void receive(const std::string& message, const std::string& from) override {
        std::string fullMessage = "[" + from + " -> " + name_ + "]: " + message;
        communications_.push_back(fullMessage);
        std::cout << fullMessage << "\n";
    }

    std::size_t getCommunicationCount() const {
        return communications_.size();
    }
};

// Smart Home Example
class SmartDevice;

class SmartHomeHub : public Mediator {
private:
    std::vector<std::weak_ptr<Colleague>> devices_;
    std::map<std::string, std::string> deviceStates_;

public:
    void registerColleague(const std::shared_ptr<Colleague>& colleague) override {
        if (!colleague) {
            return;
        }

        colleague->setMediator(this);
        devices_.push_back(colleague);
        deviceStates_[colleague->getName()] = "idle";
    }

    void sendMessage(const std::string& message, Colleague* sender) override {
        if (!sender) {
            return;
        }

        std::cout << "[Hub] " << sender->getName() << " reports: " << message << "\n";

        // Coordinate between devices
        if (message == "motion detected") {
            // Turn on lights
            notifyDevicesByType("Light", "turn on");
        } else if (message == "no motion") {
            // Turn off lights after delay
            notifyDevicesByType("Light", "turn off");
        } else if (message == "temperature high") {
            notifyDevicesByType("AC", "cool down");
        }

        deviceStates_[sender->getName()] = message;
    }

    void notifyDevicesByType(const std::string& type, const std::string& command) {
        for (auto it = devices_.begin(); it != devices_.end();) {
            std::shared_ptr<Colleague> device = it->lock();
            if (!device) {
                it = devices_.erase(it);
                continue;
            }

            if (device->getName().find(type) != std::string::npos) {
                device->receive(command, "Hub");
            }
            ++it;
        }
    }

    std::size_t getDeviceCount() const {
        std::size_t count = 0;
        for (const auto& weak : devices_) {
            if (weak.lock()) {
                ++count;
            }
        }
        return count;
    }
};

class SmartDevice : public Colleague {
private:
    std::vector<std::string> actions_;

public:
    explicit SmartDevice(const std::string& name) : Colleague(name) {}

    void send(const std::string& message) override {
        if (mediator_) {
            mediator_->sendMessage(message, this);
        }
    }

    void receive(const std::string& message, const std::string& from) override {
        std::string action = name_ + " received command: " + message;
        actions_.push_back(action);
        std::cout << "[" << name_ << "] Executing: " << message << "\n";
    }

    std::size_t getActionCount() const {
        return actions_.size();
    }
};

// Inline usage examples:
// auto chatRoom = std::make_shared<ChatRoom>();
// auto user1 = std::make_shared<User>("Alice");
// auto user2 = std::make_shared<User>("Bob");
// chatRoom->registerColleague(user1);
// chatRoom->registerColleague(user2);
// user1->send("Hello everyone!");

#endif // MEDIATOR_PATTERNS_H
