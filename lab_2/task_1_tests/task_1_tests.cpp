#define CATCH_CONFIG_MAIN
#include "../../Catch2/catch.hpp"
#include "../task_1/Observer.h"
#include "../task_1/WeatherData.h"

class CTestObserver : public IObserver<SWeatherData>
{
public:
	CTestObserver(CObservable<SWeatherData>& observable)
		: m_observable(observable)
	{
	}

	void Update(SWeatherData const&) override
	{
		m_observable.RemoveObserver(*this);
		m_updateCalled = true;
	}

	bool WasUpdateCalled() const
	{
		return m_updateCalled;
	}

	void ResetUpdateCalled()
	{
		m_updateCalled = false;
	}

private:
	CObservable<SWeatherData>& m_observable;
	bool m_updateCalled = false;
};

class CTestWeatherData : public CObservable<SWeatherData>
{
protected:
	SWeatherData GetChangedData() const override
	{
		return SWeatherData{ 25.0, 65.0, 760.0 };
	}
};

TEST_CASE("Observer can remove itself during NotifyObservers call without causing undefined behavior")
{
	CTestWeatherData weatherData;

	CTestObserver observer(weatherData);
	weatherData.RegisterObserver(observer);

	weatherData.NotifyObservers();

	REQUIRE(observer.WasUpdateCalled());  

	observer.ResetUpdateCalled();  

	weatherData.NotifyObservers();

	REQUIRE_FALSE(observer.WasUpdateCalled());  
}