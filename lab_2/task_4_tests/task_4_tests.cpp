#define CATCH_CONFIG_MAIN
#include "../../Catch2/catch.hpp"
#include "../Task_4/CWeatherData.h"
#include "../Task_4/IObserver.h"

class MockDisplay : public IObserver<SWeatherData>
{
public:
    void Update(const SWeatherData& data, const IObservable<SWeatherData>& observable) override
    {
        lastReceivedData = data;
        lastObservable = &observable;
    }

    SWeatherData lastReceivedData;
    const IObservable<SWeatherData>* lastObservable = nullptr;
};

TEST_CASE("Weather Station Duo sends updates from both stations")
{
    CWeatherData indoorStation;
    CWeatherData outdoorStation;

    MockDisplay display;

    indoorStation.RegisterObserver(1, display);
    outdoorStation.RegisterObserver(1, display);

    indoorStation.SetMeasurements(22.5, 40, 750);
    REQUIRE(display.lastObservable == &indoorStation);
    REQUIRE(display.lastReceivedData.temperature == 22.5);

    outdoorStation.SetMeasurements(15.0, 80, 760);
    REQUIRE(display.lastObservable == &outdoorStation);
    REQUIRE(display.lastReceivedData.temperature == 15.0);
}
