#ifndef IOBSERVER_H
#define IOBSERVER_H

#include "IObservable.h"

template<typename T>
class IObservable;
/*
Шаблонный интерфейс IObserver. Его должен реализовывать класс,
желающий получать уведомления от соответствующего IObservable
Параметром шаблона является тип аргумента,
передаваемого Наблюдателю в метод Update
*/

template<typename T>
class IObserver 
{
public:
    virtual ~IObserver() = default;

    virtual void Update(const T& data, const IObservable<T>* observable) = 0;
};

#endif //IOBSERVER_H