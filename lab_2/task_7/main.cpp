#include "CWeatherData.h"
#include "CDisplay.h"
#include "CStatsDisplay.h"
#include "Events.h"

int main() {
    CWeatherData wd;

    CDisplay display;
    wd.RegisterObserver(1, display, EventType::Temperature);

    wd.SetMeasurements(25, 0.4, 760);

    wd.SetMeasurements(10, 0.8, 761);

    return 0;
}