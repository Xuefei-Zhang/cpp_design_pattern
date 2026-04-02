#ifndef OBSERVER_PATTERNS_H
#define OBSERVER_PATTERNS_H

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>

class Subject;

class Observer {
public:
    virtual ~Observer() = default;

    virtual void update(const Subject& subject,
                        const std::string& eventName,
                        const std::string& payload) = 0;
};

class Subject {
public:
    virtual ~Subject() = default;

    virtual void attachObserver(const std::shared_ptr<Observer>& observer) = 0;
    virtual void detachObserver(const std::shared_ptr<Observer>& observer) = 0;
    virtual void notifyObservers(const std::string& eventName,
                                 const std::string& payload) = 0;
    virtual std::size_t getObserverCount() const = 0;
};

class ThreadSafeSubject : public Subject {
private:
    mutable std::mutex observersMutex_;
    mutable std::vector<std::weak_ptr<Observer>> observers_;

protected:
    static std::vector<std::shared_ptr<Observer>> collectLiveObservers(
        std::vector<std::weak_ptr<Observer>>& weakObservers) {
        std::vector<std::shared_ptr<Observer>> live;
        live.reserve(weakObservers.size());

        weakObservers.erase(
            std::remove_if(weakObservers.begin(), weakObservers.end(),
                           [&](const std::weak_ptr<Observer>& weakObserver) {
                               std::shared_ptr<Observer> locked = weakObserver.lock();
                               if (locked) {
                                   live.push_back(std::move(locked));
                                   return false;
                               }
                               return true;
                           }),
            weakObservers.end());

        return live;
    }

public:
    void attachObserver(const std::shared_ptr<Observer>& observer) override {
        if (!observer) {
            return;
        }

        std::lock_guard<std::mutex> lock(observersMutex_);
        std::vector<std::shared_ptr<Observer>> live = collectLiveObservers(observers_);

        const Observer* raw = observer.get();
        const bool exists = std::any_of(
            live.begin(), live.end(),
            [&](const std::shared_ptr<Observer>& current) { return current.get() == raw; });

        if (!exists) {
            observers_.push_back(observer);
        }
    }

    void detachObserver(const std::shared_ptr<Observer>& observer) override {
        std::lock_guard<std::mutex> lock(observersMutex_);
        const Observer* raw = observer ? observer.get() : nullptr;

        observers_.erase(
            std::remove_if(observers_.begin(), observers_.end(),
                           [&](const std::weak_ptr<Observer>& weakObserver) {
                               std::shared_ptr<Observer> locked = weakObserver.lock();
                               if (!locked) {
                                   return true;
                               }
                               return locked.get() == raw;
                           }),
            observers_.end());
    }

    void notifyObservers(const std::string& eventName,
                         const std::string& payload) override {
        std::vector<std::shared_ptr<Observer>> snapshot;
        {
            std::lock_guard<std::mutex> lock(observersMutex_);
            snapshot = collectLiveObservers(observers_);
        }

        for (const std::shared_ptr<Observer>& observer : snapshot) {
            observer->update(*this, eventName, payload);
        }
    }

    std::size_t getObserverCount() const override {
        std::lock_guard<std::mutex> lock(observersMutex_);
        const std::vector<std::shared_ptr<Observer>> live = collectLiveObservers(observers_);
        return live.size();
    }
};

class WeatherStation : public ThreadSafeSubject {
private:
    std::string stationName_;
    double temperature_;
    double humidity_;
    double pressure_;

public:
    explicit WeatherStation(const std::string& stationName)
        : stationName_(stationName), temperature_(0.0), humidity_(0.0), pressure_(0.0) {}

    void setMeasurements(double temperature, double humidity, double pressure) {
        temperature_ = temperature;
        humidity_ = humidity;
        pressure_ = pressure;
        notifyObservers("weather.updated", buildPushPayload());
    }

    const std::string& getStationName() const { return stationName_; }
    double getTemperature() const { return temperature_; }
    double getHumidity() const { return humidity_; }
    double getPressure() const { return pressure_; }

private:
    std::string buildPushPayload() const {
        std::ostringstream out;
        out << std::fixed << std::setprecision(1)
            << "station=" << stationName_
            << ", temp=" << temperature_
            << ", humidity=" << humidity_
            << ", pressure=" << pressure_;
        return out.str();
    }
};

class WeatherConsoleDisplay : public Observer {
private:
    std::string displayName_;
    std::string lastMessage_;
    std::size_t updateCount_;

public:
    explicit WeatherConsoleDisplay(const std::string& displayName)
        : displayName_(displayName), updateCount_(0) {}

    void update(const Subject&, const std::string& eventName, const std::string& payload) override {
        ++updateCount_;
        std::ostringstream out;
        out << "[PushDisplay:" << displayName_ << "] " << eventName << " -> " << payload;
        lastMessage_ = out.str();
        std::cout << lastMessage_ << std::endl;
    }

    const std::string& getLastMessage() const { return lastMessage_; }
    std::size_t getUpdateCount() const { return updateCount_; }
};

class WeatherStatisticsDisplay : public Observer {
private:
    std::size_t sampleCount_;
    double averageTemperature_;
    std::string lastStation_;

public:
    WeatherStatisticsDisplay() : sampleCount_(0), averageTemperature_(0.0) {}

    void update(const Subject& subject,
                const std::string& eventName,
                const std::string&) override {
        if (eventName != "weather.updated") {
            return;
        }

        const WeatherStation* station = dynamic_cast<const WeatherStation*>(&subject);
        if (!station) {
            return;
        }

        ++sampleCount_;
        const double previousTotal = averageTemperature_ * static_cast<double>(sampleCount_ - 1);
        averageTemperature_ = (previousTotal + station->getTemperature()) /
                              static_cast<double>(sampleCount_);
        lastStation_ = station->getStationName();

        std::cout << "[PullStats] station=" << lastStation_
                  << ", samples=" << sampleCount_
                  << ", avg_temp=" << std::fixed << std::setprecision(2) << averageTemperature_
                  << std::endl;
    }

    std::size_t getSampleCount() const { return sampleCount_; }
    double getAverageTemperature() const { return averageTemperature_; }
    const std::string& getLastStation() const { return lastStation_; }
};

class EventCenter : public ThreadSafeSubject {
private:
    std::string centerName_;

public:
    explicit EventCenter(const std::string& centerName) : centerName_(centerName) {}

    void publishEvent(const std::string& eventType, const std::string& details) {
        std::string payload = "center=" + centerName_ + ", details=" + details;
        notifyObservers(eventType, payload);
    }
};

class EventLogger : public Observer {
private:
    std::vector<std::string> eventLog_;

public:
    void update(const Subject&, const std::string& eventName, const std::string& payload) override {
        const std::string line = "[EventLogger] " + eventName + " -> " + payload;
        eventLog_.push_back(line);
        std::cout << line << std::endl;
    }

    std::size_t getLogSize() const { return eventLog_.size(); }
    const std::string& getLastEvent() const { return eventLog_.back(); }
};

class NumericModel : public ThreadSafeSubject {
private:
    std::string modelName_;
    int value_;

public:
    NumericModel(const std::string& modelName, int initialValue)
        : modelName_(modelName), value_(initialValue) {}

    void setValue(int value) {
        value_ = value;
        notifyObservers("model.value.changed", "name=" + modelName_);
    }

    int getValue() const { return value_; }
    const std::string& getModelName() const { return modelName_; }
};

class ModelValueView : public Observer {
private:
    std::string viewName_;
    int lastValue_;
    std::size_t renderCount_;

public:
    explicit ModelValueView(const std::string& viewName)
        : viewName_(viewName), lastValue_(0), renderCount_(0) {}

    void update(const Subject& subject, const std::string& eventName, const std::string&) override {
        if (eventName != "model.value.changed") {
            return;
        }

        const NumericModel* model = dynamic_cast<const NumericModel*>(&subject);
        if (!model) {
            return;
        }

        ++renderCount_;
        lastValue_ = model->getValue();
        std::cout << "[View:" << viewName_ << "] "
                  << model->getModelName() << " value=" << lastValue_ << std::endl;
    }

    int getLastValue() const { return lastValue_; }
    std::size_t getRenderCount() const { return renderCount_; }
};

/* Inline usage examples:
   Push:
   auto station = std::make_shared<WeatherStation>("Downtown");
   auto phoneDisplay = std::make_shared<WeatherConsoleDisplay>("Phone");
   station->attachObserver(phoneDisplay);
   station->setMeasurements(22.5, 50.0, 1012.0);

   Pull:
   auto stats = std::make_shared<WeatherStatisticsDisplay>();
   station->attachObserver(stats);
   station->setMeasurements(24.0, 48.0, 1009.0);
*/

#endif
