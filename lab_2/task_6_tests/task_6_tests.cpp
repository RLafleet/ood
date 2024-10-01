#define CATCH_CONFIG_MAIN
#include "../../Catch2/catch.hpp"
#include "../task_6/CWeatherData.h"
#include "../task_6/CStatsDisplay.h"

TEST_CASE("Wind speed measurement works correctly")
{
    CWeatherData weatherDataIn;
    CWeatherData weatherDataOut;
    CStatsDisplay statsDisplay(&weatherDataIn, &weatherDataOut);

    // Подписываем дисплей на обновления погоды
    weatherDataOut.RegisterObserver(1, statsDisplay);

    // Устанавливаем измерения с разной скоростью ветра
    weatherDataOut.SetMeasurements(25.0, 65.0, 750.0, 10.0, 45.0);
    weatherDataOut.SetMeasurements(20.0, 60.0, 755.0, 15.0, 90.0);
    weatherDataOut.SetMeasurements(30.0, 70.0, 760.0, 20.0, 135.0);

    // Проверяем минимальное, максимальное и среднее значения скорости ветра
    REQUIRE(statsDisplay.GetWindSpeedStatsOut().GetMin() == 10.0);
    REQUIRE(statsDisplay.GetWindSpeedStatsOut().GetMax() == 20.0);
    REQUIRE(statsDisplay.GetWindSpeedStatsOut().GetAverage() == Approx(15.0).epsilon(0.01));
}

// Тестирование направления ветра
TEST_CASE("Temperature statistics are calculated separately for indoor and outdoor stations") {
    CWeatherData weatherDataIn;
    CWeatherData weatherDataOut;
    CStatsDisplay statsDisplay(&weatherDataIn, &weatherDataOut);

    weatherDataIn.RegisterObserver(1, statsDisplay);
    weatherDataOut.RegisterObserver(1, statsDisplay);

    weatherDataIn.SetMeasurements(22.0, 50.0, 760.0);
    weatherDataOut.SetMeasurements(15.0, 60.0, 750.0, 12.0, 180.0);

    REQUIRE(statsDisplay.GetTemperatureStatsIn().GetAverage() == 22.0);

    REQUIRE(statsDisplay.GetTemperatureStatsOut().GetAverage() == 15.0);
}


TEST_CASE("Wind speed statistics calculation", "[CStatsDisplay][Wind]")
{
    CWeatherData weatherDataOut;
    CStatsDisplay statsDisplay(nullptr, &weatherDataOut);

    weatherDataOut.RegisterObserver(1, statsDisplay);

    SECTION("Initial wind speed statistics should be zero")
    {
        REQUIRE(statsDisplay.GetWindSpeedStatsOut().GetMax() == -std::numeric_limits<double>::infinity());
        REQUIRE(statsDisplay.GetWindSpeedStatsOut().GetMin() == std::numeric_limits<double>::infinity());
    }

    SECTION("Single measurement should update wind speed statistics")
    {
        weatherDataOut.SetMeasurements(20.0, 65.0, 755.0, 10.0, 45.0);
        REQUIRE(statsDisplay.GetWindSpeedStatsOut().GetAverage() == 10.0);
        REQUIRE(statsDisplay.GetWindSpeedStatsOut().GetMax() == 10.0);
        REQUIRE(statsDisplay.GetWindSpeedStatsOut().GetMin() == 10.0);
    }

    SECTION("Multiple wind speed measurements should update statistics correctly")
    {
        weatherDataOut.SetMeasurements(18.0, 70.0, 760.0, 12.0, 90.0);
        weatherDataOut.SetMeasurements(15.0, 60.0, 750.0, 20.0, 180.0);
        weatherDataOut.SetMeasurements(22.0, 55.0, 765.0, 5.0, 270.0);

        REQUIRE(statsDisplay.GetWindSpeedStatsOut().GetAverage() == Approx(12.33).epsilon(0.01));
        REQUIRE(statsDisplay.GetWindSpeedStatsOut().GetMax() == 20.0);
        REQUIRE(statsDisplay.GetWindSpeedStatsOut().GetMin() == 5.0);
    }

    SECTION("Wind direction statistics should calculate average direction correctly")
    {
        weatherDataOut.SetMeasurements(18.0, 70.0, 760.0, 15.0, 90.0);
        weatherDataOut.SetMeasurements(15.0, 60.0, 750.0, 10.0, 180.0);
        weatherDataOut.SetMeasurements(22.0, 55.0, 765.0, 20.0, 270.0);

        // Средний угол должен быть 180, так как 90 и 270 уравновешиваются
        REQUIRE(statsDisplay.GetWindDirectionStatsOut().GetAverage() == Approx(180.0).epsilon(0.01));
    }

    SECTION("Wind direction statistics should handle full-circle averaging")
    {
        weatherDataOut.SetMeasurements(22.0, 55.0, 765.0, 15.0, 350.0); // Направление близко к 360
        weatherDataOut.SetMeasurements(20.0, 50.0, 760.0, 10.0, 10.0);  // Направление близко к 0

        // Средний угол должен быть 0, так как 350 и 10 уравновешиваются через круг
        REQUIRE(statsDisplay.GetWindDirectionStatsOut().GetAverage() == Approx(0.0).epsilon(0.01));
    }
}
