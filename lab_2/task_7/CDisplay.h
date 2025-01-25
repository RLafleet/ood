#ifndef CDISPLAY_H
#define CDISPLAY_H

#include "Events.h"
#include <iostream>
#include "IObserver.h"
#include "SWeatherData.h"
#include "CObservable.h"

class CDisplay final : public IObserver<SWeatherData, EventType>
{
public:
    CDisplay() : m_weatherDataIn(nullptr), m_weatherDataOut(nullptr) {}

    CDisplay(
        CObservable<SWeatherData, EventType>* weatherDataIn,
        CObservable<SWeatherData, EventType>* weatherDataOut
    ) : m_weatherDataIn(weatherDataIn), m_weatherDataOut(weatherDataOut)
    {}
    /*  Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
        Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
        остается публичным
    */
private:
    void Update(const SWeatherData& data, const IObservable<SWeatherData, EventType>* observable, EventType eventType) override
    {
        std::string location = "unknown";
        if (observable == m_weatherDataIn)
        {
            location = "inside";
        }
        else if (observable == m_weatherDataOut)
        {
            location = "outside";
        }

        if (eventType == EventType::Temperature)
        {
            std::cout << "Current Temperature: " << data.temperature << std::endl;
        }
        else if (eventType == EventType::Pressure)
        {
            std::cout << "Current Pressure: " << data.pressure << std::endl;
        }

        std::cout << "Current Humidity: " << data.humidity << std::endl;
        std::cout << "Current Location " << location << std::endl;
        std::cout << "Current Hum " << data.humidity << std::endl;
        std::cout << "----------------" << std::endl;
    }

    const CObservable<SWeatherData, EventType>* m_weatherDataIn;
    const CObservable<SWeatherData, EventType>* m_weatherDataOut;
};

#endif //CDISPLAY_H