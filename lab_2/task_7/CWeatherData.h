#ifndef CWEATHERDATA_H
#define CWEATHERDATA_H

#include "CObservable.h"
#include "SWeatherData.h"
#include "Events.h"

class CWeatherData final : public CObservable<SWeatherData, EventType>
{
public:
    // Температура в градусах Цельсия
    double GetTemperature()const
    {
        return m_temperature;
    }
    // Относительная влажность (0...100)
    double GetHumidity()const
    {
        return m_humidity;
    }
    // Атмосферное давление (в мм.рт.ст)
    double GetPressure()const
    {
        return m_pressure;
    }

    double GetWindSpeed()const
    {
        return m_windSpeed;
    }

    double GetWindDirection()const
    {
        return m_windDirection;
    }

    void MeasurementsChanged(EventType eventType)
    {
        NotifyObservers(eventType);
    }

    void SetTemp(double temp)
    {
        m_temperature = temp;

        MeasurementsChanged(EventType::Temperature);
    }

    void SetHumidity(double humidity)
    {
        m_humidity = humidity;

        MeasurementsChanged(EventType::Humidity);
    }

    void SetPressure(double pressure)
    {
        m_pressure = pressure;

        MeasurementsChanged(EventType::Pressure);
    }


    void SetMeasurements(double temp, double humidity, double pressure, double windSpeed = 0, double windDirection = 0)
    {
        m_humidity = humidity;
        m_temperature = temp;
        m_pressure = pressure;
        m_windSpeed = windSpeed;
        m_windDirection = windDirection;

        MeasurementsChanged(EventType::Temperature);
        MeasurementsChanged(EventType::Pressure);
        MeasurementsChanged(EventType::Humidity);
    }

protected:
    [[nodiscard]] SWeatherData GetChangedData() const override
    {
        SWeatherData info;
        info.temperature = GetTemperature();
        info.humidity = GetHumidity();
        info.pressure = GetPressure();
        info.windSpeed = GetWindSpeed();
        info.windDirection = GetWindDirection();
        return info;
    }

private:
    double m_temperature = 0.0;
    double m_humidity = 0.0;
    double m_pressure = 760.0;
    double m_windSpeed = 0.0;
    double m_windDirection = 0.0;
};

#endif //CWEATHERDATA_H