#define CATCH_CONFIG_MAIN
#include "../../Catch2/catch.hpp"
#include "../task_3/SWeatherData.h"
#include "../task_3/CObservable.h"
#include "../task_3/CWeatherData.h"
#include "../task_3/CDisplay.h"

class SelfRemovingObserver final : public IObserver<SWeatherData> {
public:
    explicit SelfRemovingObserver(CObservable<SWeatherData>& observable) : m_observable(observable) {
    }

    void Update(SWeatherData const& data) override {
        std::cout << "SelfRemovingObserver received update" << std::endl;
        m_observable.RemoveObserver(*this);
    }

private:
    CObservable<SWeatherData>& m_observable;
};

class MockObserver final : public IObserver<SWeatherData> {
public:
    explicit MockObserver(std::string name) : m_name(std::move(name)) {}

    void Update(SWeatherData const& data) override {
        m_order.push_back(m_name);
    }

    static void ResetOrder() {
        m_order.clear();
    }

    static std::vector<std::string> GetOrder() {
        return m_order;
    }

private:
    std::string m_name;
    static std::vector<std::string> m_order;
};

std::vector<std::string> MockObserver::m_order = {};

// Тест на удаление наблюдателя при уведомлении
TEST_CASE("SelfRemovingObserver removes itself on update", "[CObservable]") {
    CWeatherData wd;

    SelfRemovingObserver selfRemovingObserver(wd);
    wd.RegisterObserver(1, selfRemovingObserver);

    CDisplay display;
    wd.RegisterObserver(1, display);

    std::cout << "First change" << std::endl;
    wd.SetMeasurements(3, .7, 760);

    std::cout << "Second change" << std::endl;
    wd.SetMeasurements(-10, .8, 761);
}

// Тест с несколькими самоустраняющимися наблюдателями
TEST_CASE("Multiple self-removing observers", "[CObservable]") {
    CWeatherData weatherData;

    SelfRemovingObserver selfRemovingObserver1(weatherData);
    SelfRemovingObserver selfRemovingObserver2(weatherData);

    weatherData.RegisterObserver(1, selfRemovingObserver1);
    weatherData.RegisterObserver(2, selfRemovingObserver2);

    CDisplay display;
    weatherData.RegisterObserver(3, display);

    std::cout << "First change" << std::endl;
    weatherData.SetMeasurements(5, 70, 765);

    std::cout << "Second change" << std::endl;
    weatherData.SetMeasurements(10, 75, 770);
}

// Тест на уведомление наблюдателей с учетом приоритета
TEST_CASE("Observers notified in priority order", "[CObservable]") {
    CWeatherData weatherData;

    MockObserver observer1("Observer1");
    MockObserver observer2("Observer2");
    MockObserver observer3("Observer3");

    weatherData.RegisterObserver(2, observer2);
    weatherData.RegisterObserver(1, observer1);
    weatherData.RegisterObserver(3, observer3);

    MockObserver::ResetOrder();

    weatherData.SetMeasurements(25, 65, 760);

    std::vector<std::string> expectedOrder = { "Observer1", "Observer2", "Observer3" };
    REQUIRE(MockObserver::GetOrder() == expectedOrder);
}

// Тест на уведомление наблюдателей с одинаковым приоритетом
TEST_CASE("Observers with same priority notified in registration order", "[CObservable]") {
    CWeatherData weatherData;

    MockObserver observer1("Observer1");
    MockObserver observer2("Observer2");
    MockObserver observer3("Observer3");

    weatherData.RegisterObserver(1, observer1);
    weatherData.RegisterObserver(1, observer2);
    weatherData.RegisterObserver(1, observer3);

    MockObserver::ResetOrder();

    weatherData.SetMeasurements(22, 60, 755);

    std::vector<std::string> expectedOrder = { "Observer1", "Observer2", "Observer3" };
    REQUIRE(MockObserver::GetOrder() == expectedOrder);
}

// Тест на уведомление наблюдателей после удаления
TEST_CASE("Observers notified after one is removed", "[CObservable]") {
    CWeatherData weatherData;

    MockObserver observer1("Observer1");
    MockObserver observer2("Observer2");

    weatherData.RegisterObserver(1, observer1);
    weatherData.RegisterObserver(2, observer2);

    MockObserver::ResetOrder();

    weatherData.SetMeasurements(20, 65, 760);

    std::vector<std::string> expectedOrder = { "Observer1", "Observer2" };
    REQUIRE(MockObserver::GetOrder() == expectedOrder);

    weatherData.RemoveObserver(observer1);

    MockObserver::ResetOrder();

    weatherData.SetMeasurements(30, 70, 765);

    expectedOrder = { "Observer2" };
    REQUIRE(MockObserver::GetOrder() == expectedOrder);
}
