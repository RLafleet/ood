#include "CWeatherData.h"
#include "CDisplay.h"
#include "CStatsDisplay.h"

int main() {
    CWeatherData wd;

    CDisplay display;
    wd.RegisterObserver(2, display);

    CStatsDisplay statsDisplay;
    wd.RegisterObserver(1, statsDisplay);

    wd.SetMeasurements(3, 0.7, 760);
    wd.SetMeasurements(4, 0.8, 761);

    wd.RemoveObserver(statsDisplay);

    wd.SetMeasurements(10, 0.8, 761);
    wd.SetMeasurements(-10, 0.8, 761);
    return 0;
}