#ifndef WEATHERDATA_H
#define WEATHERDATA_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include "Observer.h"

struct SWeatherData
{
	double temperature = 0;
	double humidity = 0;
	double pressure = 0;
};

class CDisplay : public IObserver<SWeatherData>
{
private:
	/* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
		Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
		остается публичным
	*/
	void Update(SWeatherData const& data) override
	{
		std::cout << "Current Temp " << data.temperature << std::endl;
		std::cout << "Current Hum " << data.humidity << std::endl;
		std::cout << "Current Pressure " << data.pressure << std::endl;
		std::cout << "----------------" << std::endl;
	}
};

class CStat
{
public:
	void AddValue(const double value)
	{
		if (m_minValue > value)
		{
			m_minValue = value;
		}
		if (m_maxValue < value)
		{
			m_maxValue = value;
		}
		m_accValue += value;
		++m_countAcc;
	}

	[[nodiscard]] double GetAverage() const
	{
		return m_accValue / m_countAcc;
	}

	[[nodiscard]] double GetMin() const
	{
		return m_minValue;
	}

	[[nodiscard]] double GetMax() const
	{
		return m_maxValue;
	}

private:
	double m_minValue = std::numeric_limits<double>::infinity();
	double m_maxValue = -std::numeric_limits<double>::infinity();
	double m_accValue = 0;
	unsigned m_countAcc = 0;
};


class CStatsDisplay final : public IObserver<SWeatherData>
{
private:
	/* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
	Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
	остается публичным
	*/
	void Update(SWeatherData const& data) override
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

	CStat m_temperatureStatistics{};
	CStat m_humidityStatistics{};
	CStat m_pressureStatistics{};
};

class CWeatherData : public CObservable<SWeatherData>
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

	void MeasurementsChanged()
	{
		NotifyObservers();
	}

	void SetMeasurements(double temp, double humidity, double pressure)
	{
		m_humidity = humidity;
		m_temperature = temp;
		m_pressure = pressure;

		MeasurementsChanged();
	}
protected:
	SWeatherData GetChangedData()const override
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

#endif //WEATHERDATA_H