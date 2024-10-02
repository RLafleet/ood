#ifndef IOBSERVABLE_H
#define IOBSERVABLE_H
#include "IObserver.h"
#include "Events.h"

/*
Шаблонный интерфейс IObservable. Позволяет подписаться и отписаться на оповещения, а также
инициировать рассылку уведомлений зарегистрированным наблюдателям.
*/
template<typename T>
class IObservable
{
public:
    virtual ~IObservable() = default;

    virtual void RegisterObserver(unsigned priority, IObserver<T>& observer, EventType eventType) = 0;

    virtual void RemoveObserver(IObserver<T>& observer, EventType eventType) = 0;

protected:
    virtual void NotifyObservers(EventType eventType) = 0;
};

#endif //IOBSERVABLE_H