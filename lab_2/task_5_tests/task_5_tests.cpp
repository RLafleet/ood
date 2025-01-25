#define CATCH_CONFIG_MAIN
#include "../../Catch2/catch.hpp"
#include "../task_5/CWeatherData.h"
#include "../task_5/CStatsDisplay.h"
#include "../task_5/CDisplay.h"
#include <vector>
#include <algorithm>

template<typename T>
class MockObservable : public IObservable<T>
{
public:
    void RegisterObserver(unsigned priority, IObserver<T>& observer) override
    {
        observers.push_back(&observer);
    }

    void RemoveObserver(IObserver<T>& observer) override
    {
        observers.erase(std::remove(observers.begin(), observers.end(), &observer), observers.end());
    }

    void NotifyObservers() override
    {
        for (auto& observer : observers)
        {
            observer->Update(T{}, *this);
        }
    }


private:
    std::vector<IObserver<T>*> observers;
};


// Тест 1: Проверка расчёта средней температуры
TEST_CASE("Average temperature is calculated correctly", "[CStatsDisplay]")
{
    CStatsDisplay statsDisplay;
    MockObservable<SWeatherData> observable;

    SWeatherData data1 = { 25.0, 65.0, 760.0, 5.0, 90.0 };
    SWeatherData data2 = { 30.0, 70.0, 765.0, 10.0, 180.0 };

    statsDisplay.Update(data1, observable);
    statsDisplay.Update(data2, observable);

    REQUIRE(statsDisplay.GetTemperatureStats().GetAverage() == Approx(27.5));
}

// Тест 2: Проверка расчёта минимальной температуры
TEST_CASE("Minimum temperature is calculated correctly", "[CStatsDisplay]")
{
    CStatsDisplay statsDisplay;
    MockObservable<SWeatherData> observable;

    SWeatherData data1 = { 15.0, 60.0, 755.0, 5.0, 45.0 };
    SWeatherData data2 = { 20.0, 65.0, 760.0, 5.0, 90.0 };
    statsDisplay.Update(data1, observable);
    statsDisplay.Update(data2, observable);

    REQUIRE(statsDisplay.GetTemperatureStats().GetMin() == 15.0);
}

// Тест 3: Проверка расчёта максимальной температуры
TEST_CASE("Maximum temperature is calculated correctly", "[CStatsDisplay]")
{
    CStatsDisplay statsDisplay;
    MockObservable<SWeatherData> observable;

    SWeatherData data1 = { 25.0, 55.0, 750.0, 10.0, 180.0 };
    SWeatherData data2 = { 32.0, 60.0, 760.0, 8.0, 270.0 };
    statsDisplay.Update(data1, observable);
    statsDisplay.Update(data2, observable);

    REQUIRE(statsDisplay.GetTemperatureStats().GetMax() == 32.0);
}

// Тест 4: Проверка расчёта средней влажности
TEST_CASE("Average humidity is calculated correctly", "[CStatsDisplay]")
{
    CStatsDisplay statsDisplay;
    MockObservable<SWeatherData> observable;

    SWeatherData data1 = { 22.0, 65.0, 760.0, 5.0, 0.0 };
    SWeatherData data2 = { 25.0, 70.0, 765.0, 8.0, 90.0 };
    statsDisplay.Update(data1, observable);
    statsDisplay.Update(data2, observable);

    REQUIRE(statsDisplay.GetHumidityStats().GetAverage() == Approx(67.5));
}

TEST_CASE("Average wind speed is calculated correctly", "[CStatsDisplay]")
{
    CStatsDisplay statsDisplay;
    MockObservable<SWeatherData> observable;

    SWeatherData data1 = { 25.0, 65.0, 760.0, 5.0, 90.0 };  // windSpeed = 5.0
    SWeatherData data2 = { 30.0, 70.0, 765.0, 10.0, 180.0 }; // windSpeed = 10.0
    statsDisplay.Update(data1, observable);
    statsDisplay.Update(data2, observable);

    REQUIRE(statsDisplay.GetWindSpeedStats().GetAverage() == Approx(7.5));
}

TEST_CASE("Minimum wind speed is calculated correctly", "[CStatsDisplay]") 
{
    CStatsDisplay statsDisplay;
    MockObservable<SWeatherData> observable;

    SWeatherData data1 = { 22.0, 60.0, 755.0, 3.0, 0.0 };
    SWeatherData data2 = { 25.0, 65.0, 765.0, 7.0, 90.0 };
    statsDisplay.Update(data1, observable);
    statsDisplay.Update(data2, observable);

    REQUIRE(statsDisplay.GetWindSpeedStats().GetMin() == 3.0);
}

// Тест 7: Проверка расчёта среднего направления ветра при противоположных направлениях
TEST_CASE("Average wind direction for opposite directions (0° and 180°)", "[CStatsDisplay]") 
{
    CStatsDisplay statsDisplay;
    MockObservable<SWeatherData> observable;
  
    SWeatherData data1 = { 22.0, 60.0, 760.0, 10.0, 0.0 };   // windDir = 0°
    SWeatherData data2 = { 25.0, 65.0, 765.0, 10.0, 180.0 }; // windDir = 180°
    statsDisplay.Update(data1, observable);
    statsDisplay.Update(data2, observable);

    REQUIRE(statsDisplay.GetWindDirectionStats().GetAverage() == Approx(90.0)); // Среднее между 0° и 180° — это 90°
}

// Тест 8: Проверка расчёта среднего направления ветра для близких значений (350° и 10°)
TEST_CASE("Average wind direction for close values (350° and 10°)", "[CStatsDisplay]") 
{
    CStatsDisplay statsDisplay;
    MockObservable<SWeatherData> observable;

    SWeatherData data1 = { 22.0, 60.0, 760.0, 10.0, 350.0 }; // windDir = 350°
    SWeatherData data2 = { 25.0, 65.0, 765.0, 10.0, 10.0 };  // windDir = 10°
    statsDisplay.Update(data1, observable);
    statsDisplay.Update(data2, observable);

    REQUIRE(statsDisplay.GetWindDirectionStats().GetAverage() == Approx(0.0)); // Среднее между 350° и 10° — это 0°
}

// Тест 9: Проверка расчёта среднего направления ветра для одного значения (45°)
TEST_CASE("Average wind direction with one value (45°)", "[CStatsDisplay]") 
{
    CStatsDisplay statsDisplay;
    MockObservable<SWeatherData> observable;
   
    SWeatherData data = { 22.0, 60.0, 760.0, 5.0, 45.0 }; // windDir = 45°
    statsDisplay.Update(data, observable);

    REQUIRE(statsDisplay.GetWindDirectionStats().GetAverage() == Approx(45.0)); // Среднее — это само значение
}

// Тест 10: Проверка корректного расчёта среднего направления при отсутствии данных
TEST_CASE("Average wind direction with no data should be NaN", "[CStatsDisplay]") 
{
    CStatsDisplay statsDisplay;

    REQUIRE(std::isnan(statsDisplay.GetWindDirectionStats().GetAverage())); // Без данных результат должен быть NaN
}
