#ifndef CWEATHERDATA_H
#define CWEATHERDATA_H

#include "CObservable.h"
#include "SWeatherData.h"

class CWeatherData final : public CObservable<SWeatherData>
{
public:
    // Температура в градусах Цельсия
    [[nodiscard]] double GetTemperature() const 
    {
        return m_temperature;
    }

    // Относительная влажность (0...100)
    [[nodiscard]] double GetHumidity() const 
    {
        return m_humidity;
    }

    // Атмосферное давление (в мм.рт.ст)
    [[nodiscard]] double GetPressure() const 
    {
        return m_pressure;
    }

    void MeasurementsChanged() 
    {
        NotifyObservers();
    }

    void SetMeasurements(const double temp, const double humidity, const double pressure) 
    {
        m_humidity = humidity;
        m_temperature = temp;
        m_pressure = pressure;

        MeasurementsChanged();
    }

protected:
    [[nodiscard]] SWeatherData GetChangedData() const override
    {
        SWeatherData info;
        info.temperature = GetTemperature();
        info.humidity = GetHumidity();
        info.pressure = GetPressure();
        return info;
    }

private:
    double m_temperature = 0.0;
    double m_humidity = 0.0;
    double m_pressure = 760.0;
};

#endif //CWEATHERDATA_H