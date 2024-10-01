#ifndef COBSERVABLE_H
#define COBSERVABLE_H

#include <map>
#include <algorithm>
#include <vector>
#include "IObservable.h"

template<class T>
class CObservable : public IObservable<T> 
{
public:
    typedef IObserver<T> ObserverType;

    void RegisterObserver(unsigned priority, ObserverType& observer) override 
    {
        m_observers.insert({ priority, &observer });
    }

    void NotifyObservers() override
    {
        std::vector<std::pair<unsigned, IObserver<T>*>> observersCopy(m_observers.begin(), m_observers.end());
        std::sort(observersCopy.begin(), observersCopy.end(), [](const auto& lhs, const auto& rhs) {
            return lhs.first > rhs.first;
            });

        for (auto& [priority, observer] : observersCopy)
        {
            observer->Update(GetChangedData(), *this);  
        }
    }


    void RemoveObserver(ObserverType& observer) override 
    {
        for (auto it = m_observers.begin(); it != m_observers.end();) 
        {
            if (it->second == &observer) 
            {
                it = m_observers.erase(it);
            }
            else 
            {
                ++it;
            }
        }
    }

protected:
    virtual T GetChangedData() const = 0;

private:
    std::multimap<unsigned, ObserverType*> m_observers;
};

#endif //COBSERVABLE_H