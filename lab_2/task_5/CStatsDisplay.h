#ifndef CSTATSDISPLAY_H
#define CSTATSDISPLAY_H

#include <iostream>
#include "CStat.h"
#include "CStatWindDirection.h"
#include "IObserver.h"
#include "SWeatherData.h"

class CStatsDisplay final : public IObserver<SWeatherData>
{
public:
    void Update(const SWeatherData& data, const IObservable<SWeatherData>& observable) override
    {
        m_temperatureStatistics.AddValue(data.temperature);
        m_humidityStatistics.AddValue(data.humidity);
        m_pressureStatistics.AddValue(data.pressure);
        m_windSpeedStatistics.AddValue(data.windSpeed);
        m_windDirectionStatistics.AddValue(data.windDirection);

        std::cout << "Max Temp " << m_temperatureStatistics.GetMax() << std::endl;
        std::cout << "Min Temp " << m_temperatureStatistics.GetMin() << std::endl;
        std::cout << "Average Temp " << m_temperatureStatistics.GetAverage() << std::endl;

        std::cout << "Max Hum " << m_humidityStatistics.GetMax() << std::endl;
        std::cout << "Min Hum " << m_humidityStatistics.GetMin() << std::endl;
        std::cout << "Average Hum " << m_humidityStatistics.GetAverage() << std::endl;

        std::cout << "Max Pressure " << m_pressureStatistics.GetMax() << std::endl;
        std::cout << "Min Pressure " << m_pressureStatistics.GetMin() << std::endl;
        std::cout << "Average Pressure " << m_pressureStatistics.GetAverage() << std::endl;

        std::cout << "Max Wind Speed " << m_windSpeedStatistics.GetMax() << std::endl;
        std::cout << "Min Wind Speed " << m_windSpeedStatistics.GetMin() << std::endl;
        std::cout << "Average Wind Speed " << m_windSpeedStatistics.GetAverage() << std::endl;

        std::cout << "Average Wind Direction " << m_windDirectionStatistics.GetAverage() << " degrees" << std::endl;
        std::cout << "----------------" << std::endl;
    }

    const CStat& GetTemperatureStats() const
    { 
        return m_temperatureStatistics; 
    }
    const CStat& GetHumidityStats() const
    { 
        return m_humidityStatistics; 
    }
    const CStat& GetPressureStats() const
    { 
        return m_pressureStatistics; 
    }
    const CStat& GetWindSpeedStats() const
    { 
        return m_windSpeedStatistics; 
    }
    const CStatWindDirection& GetWindDirectionStats() const
    { 
        return m_windDirectionStatistics; 
    }

private:
    CStat m_temperatureStatistics{};
    CStat m_humidityStatistics{};
    CStat m_pressureStatistics{};
    CStat m_windSpeedStatistics{};
    CStatWindDirection m_windDirectionStatistics{};
};

#endif // CSTATSDISPLAY_H
