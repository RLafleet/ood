#ifndef IOBSERVABLE_H
#define IOBSERVABLE_H
#include "IObserver.h"

template<typename T>
class IObservable {
public:
    virtual ~IObservable() = default;

    virtual void RegisterObserver(unsigned priority, IObserver<T>& observer) = 0;

    virtual void NotifyObservers() = 0;

    virtual void RemoveObserver(IObserver<T>& observer) = 0;
};

#endif //IOBSERVABLE_H