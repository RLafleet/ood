#ifndef CDISPLAY_H
#define CDISPLAY_H

#include <iostream>
#include "IObserver.h"
#include "SWeatherData.h"
#include "CObservable.h"

class CDisplay final : public IObserver<SWeatherData>
{
public:
    CDisplay() : m_weatherDataIn(nullptr), m_weatherDataOut(nullptr) {}

    CDisplay(
        CObservable<SWeatherData>* weatherDataIn,
        CObservable<SWeatherData>* weatherDataOut
    ) : m_weatherDataIn(weatherDataIn), m_weatherDataOut(weatherDataOut)
    {}
    /*  Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
        Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
        остается публичным
    */
private:
    void Update(const SWeatherData& data, const IObservable<SWeatherData>* observable) override
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

        std::cout << "Current Location " << location << std::endl;
        std::cout << "Current Temp " << data.temperature << std::endl;
        std::cout << "Current Hum " << data.humidity << std::endl;
        std::cout << "Current Pressure " << data.pressure << std::endl;
        std::cout << "----------------" << std::endl;
    }

    const CObservable<SWeatherData>* m_weatherDataIn;
    const CObservable<SWeatherData>* m_weatherDataOut;
};

#endif //CDISPLAY_H