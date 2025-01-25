#ifndef CSTATSDISPLAY_H
#define CSTATSDISPLAY_H

#include <iostream>
#include "CStat.h"
#include "IObserver.h"
#include "SWeatherData.h"

class CStatsDisplay final : public IObserver<SWeatherData>
{
public: 
    CStatsDisplay() : m_weatherDataIn(nullptr), m_weatherDataOut(nullptr) {}

    CStatsDisplay(
        const CObservable<SWeatherData>* weatherDataIn,
        const CObservable<SWeatherData>* weatherDataOut
    ) : m_weatherDataIn(weatherDataIn), m_weatherDataOut(weatherDataOut)
    {}

    const CStat& GetTemperatureStatsIn() const
    {
        return m_temperatureStatisticsIn;
    }
    const CStat& GetHumidityStatsIn() const
    {
        return m_humidityStatisticsIn;
    }
    const CStat& GetPressureStatsIn() const
    {
        return m_pressureStatisticsIn;
    }

    const CStat& GetTemperatureStatsOut() const
    {
        return m_temperatureStatisticsOut;
    }
    const CStat& GetHumidityStatsOut() const
    {
        return m_humidityStatisticsOut;
    }
    const CStat& GetPressureStatsOut() const
    {
        return m_pressureStatisticsOut;
    }
private:
    CStat m_temperatureStatisticsIn{};
    CStat m_humidityStatisticsIn{};
    CStat m_pressureStatisticsIn{};

    CStat m_temperatureStatisticsOut{};
    CStat m_humidityStatisticsOut{};
    CStat m_pressureStatisticsOut{};

    const CObservable<SWeatherData>* m_weatherDataIn;
    const CObservable<SWeatherData>* m_weatherDataOut;

    void Update(const SWeatherData& data, const IObservable<SWeatherData>* observable) override
    {
        if (observable == m_weatherDataIn)
        {
            m_temperatureStatisticsIn.AddValue(data.temperature);
            m_humidityStatisticsIn.AddValue(data.humidity);
            m_pressureStatisticsIn.AddValue(data.pressure);

            std::cout << "Current Location inside" << std::endl;

            std::cout << "Max Temp " << m_temperatureStatisticsIn.GetMax() << std::endl;
            std::cout << "Min Temp " << m_temperatureStatisticsIn.GetMin() << std::endl;
            std::cout << "Average Temp " << m_temperatureStatisticsIn.GetAverage() << std::endl;

            std::cout << "Max Hum " << m_humidityStatisticsIn.GetMax() << std::endl;
            std::cout << "Min Hum " << m_humidityStatisticsIn.GetMin() << std::endl;
            std::cout << "Average Hum " << m_humidityStatisticsIn.GetAverage() << std::endl;

            std::cout << "Max Pressure " << m_pressureStatisticsIn.GetMax() << std::endl;
            std::cout << "Min Pressure " << m_pressureStatisticsIn.GetMin() << std::endl;
            std::cout << "Average Pressure " << m_pressureStatisticsIn.GetAverage() << std::endl;

            std::cout << "----------------" << std::endl;
        }
        else if (observable == m_weatherDataOut)
        {
            m_temperatureStatisticsOut.AddValue(data.temperature);
            m_humidityStatisticsOut.AddValue(data.humidity);
            m_pressureStatisticsOut.AddValue(data.pressure);

            std::cout << "Current Location outside" << std::endl;

            std::cout << "Max Temp " << m_temperatureStatisticsOut.GetMax() << std::endl;
            std::cout << "Min Temp " << m_temperatureStatisticsOut.GetMin() << std::endl;
            std::cout << "Average Temp " << m_temperatureStatisticsOut.GetAverage() << std::endl;

            std::cout << "Max Hum " << m_humidityStatisticsOut.GetMax() << std::endl;
            std::cout << "Min Hum " << m_humidityStatisticsOut.GetMin() << std::endl;
            std::cout << "Average Hum " << m_humidityStatisticsOut.GetAverage() << std::endl;

            std::cout << "Max Pressure " << m_pressureStatisticsOut.GetMax() << std::endl;
            std::cout << "Min Pressure " << m_pressureStatisticsOut.GetMin() << std::endl;
            std::cout << "Average Pressure " << m_pressureStatisticsOut.GetAverage() << std::endl;
        }
        else
        {
            std::cout << "Unknown weather station" << std::endl;
        }

        std::cout << "----------------" << std::endl;
    }
};

#endif //CSTATSDISPLAY_H