#ifndef CWEATHERDATA_H
#define CWEATHERDATA_H

#include "CObservable.h"
#include "SWeatherData.h"

class CWeatherData final : public CObservable<SWeatherData>
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

    void MeasurementsChanged()
    {
        NotifyObservers();
    }

    void SetMeasurements(double temp, double humidity, double pressure, double windSpeed = 0, double windDirection = 0)
    {
        m_humidity = humidity;
        m_temperature = temp;
        m_pressure = pressure;
        m_windSpeed = windSpeed;
        m_windDirection = windDirection;

        MeasurementsChanged();
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