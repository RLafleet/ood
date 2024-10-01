#ifndef COBSERVABLE_H
#define COBSERVABLE_H

#include <map>
#include "IObservable.h"

template<class T>
class CObservable : public IObservable<T> {
public:
    typedef IObserver<T> ObserverType;

    void RegisterObserver(unsigned priority, ObserverType& observer) override {
        m_observers.insert({ priority, &observer });
    }

    void NotifyObservers() override {
        T data = GetChangedData();
        auto observers = m_observers;
        for (auto& [priority, observer] : observers) {
            observer->Update(data);
        }
    }

    void RemoveObserver(ObserverType& observer) override {
        for (auto it = m_observers.begin(); it != m_observers.end();) {
            if (it->second == &observer) {
                it = m_observers.erase(it);
            }
            else {
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