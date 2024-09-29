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

template <typename T>
class CStatsCalculator
{
public:
	void Update(T value)
	{
		if (m_min > value)
		{
			m_min = value;
		}
		if (m_max < value)
		{
			m_max = value;
		}
		m_accumulated += value;
		++m_count;
	}	

	T GetMin() const { return m_min; }
	T GetMax() const { return m_max; }
	T GetAverage() const { return m_accumulated / m_count; }

private:
	T m_min = std::numeric_limits<T>::infinity();
	T m_max = -std::numeric_limits<T>::infinity();
	T m_accumulated = 0;
	unsigned m_count = 0;
};

class CStatsDisplay : public IObserver<SWeatherData>
{
private:
	CStatsCalculator<double> m_temperatureStats;
	CStatsCalculator<double> m_humidityStats;
	CStatsCalculator<double> m_pressureStats;

	void Update(SWeatherData const& data) override
	{
		m_temperatureStats.Update(data.temperature);
		m_humidityStats.Update(data.humidity);
		m_pressureStats.Update(data.pressure);

		std::cout << "Temperature Stats - Max: " << m_temperatureStats.GetMax()
			<< " Min: " << m_temperatureStats.GetMin()
			<< " Avg: " << m_temperatureStats.GetAverage() << std::endl;

		std::cout << "Humidity Stats - Max: " << m_humidityStats.GetMax()
			<< " Min: " << m_humidityStats.GetMin()
			<< " Avg: " << m_humidityStats.GetAverage() << std::endl;

		std::cout << "Pressure Stats - Max: " << m_pressureStats.GetMax()
			<< " Min: " << m_pressureStats.GetMin()
			<< " Avg: " << m_pressureStats.GetAverage() << std::endl;

		std::cout << "----------------" << std::endl;
	}
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