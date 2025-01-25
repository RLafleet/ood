#ifndef COBSERVABLE_H
#define COBSERVABLE_H

#include <map>
#include <algorithm>
#include <vector>
#include <unordered_set>
#include "IObservable.h"

// Реализация интерфейса IObservable
template<class T, typename EventType>
class CObservable : public IObservable<T, EventType>
{
public:
    typedef IObserver<T, EventType> ObserverType;

    void RegisterObserver(unsigned priority, ObserverType& observer, EventType eventType) override
    {
        if (!m_observers.contains(eventType))
        {
            m_observers.insert({ eventType, {} });
        }
        if (!m_observers.at(eventType).contains(priority))
        {
            m_observers.at(eventType).insert({ priority, {} });
        }

        m_observers.at(eventType).at(priority).insert(&observer);
    }

    void RemoveObserver(ObserverType& observer, EventType eventType) override
    {
        for (auto& [priority, observers] : m_observers[eventType])
        {
            if (observers.erase(&observer))
            {
                return;
            }
        }
    }

protected:
    virtual T GetChangedData() const = 0;

    void NotifyObservers(EventType eventType)
    {
        T data = GetChangedData();
        auto observersCopy = m_observers;
        for (auto& [priority, observers] : m_observers[eventType])
        {
            for (auto& observer : observers)
            {
                observer->Update(data, static_cast<const IObservable<T, EventType>*>(this), eventType);
            }
        }
    }

private:
    std::map<EventType, std::map<unsigned, std::unordered_set<ObserverType*>>> m_observers;
};

#endif //COBSERVABLE_H