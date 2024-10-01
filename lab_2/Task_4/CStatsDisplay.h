#ifndef CSTATSDISPLAY_H
#define CSTATSDISPLAY_H

#include <iostream>
#include "CStat.h"
#include "IObserver.h"
#include "SWeatherData.h"

class CStatsDisplay final : public IObserver<SWeatherData>
{

private:
    CStat m_temperatureStatistics{};
    CStat m_humidityStatistics{};
    CStat m_pressureStatistics{};

    void Update(const SWeatherData& data, const IObservable<SWeatherData>& observable) override
    {
        m_temperatureStatistics.AddValue(data.temperature);
        m_humidityStatistics.AddValue(data.humidity);
        m_pressureStatistics.AddValue(data.pressure);

        std::cout << "Max Temp " << m_temperatureStatistics.GetMax() << std::endl;
        std::cout << "Min Temp " << m_temperatureStatistics.GetMin() << std::endl;
        std::cout << "Average Temp " << m_temperatureStatistics.GetAverage() << std::endl;

        std::cout << "Max Hum " << m_humidityStatistics.GetMax() << std::endl;
        std::cout << "Min Hum " << m_humidityStatistics.GetMin() << std::endl;
        std::cout << "Average Hum " << m_humidityStatistics.GetAverage() << std::endl;

        std::cout << "Max Pressure " << m_pressureStatistics.GetMax() << std::endl;
        std::cout << "Min Pressure " << m_pressureStatistics.GetMin() << std::endl;
        std::cout << "Average Pressure " << m_pressureStatistics.GetAverage() << std::endl;

        std::cout << "----------------" << std::endl;
    }
};

#endif //CSTATSDISPLAY_H