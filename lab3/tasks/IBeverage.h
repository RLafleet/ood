#ifndef INC_1_IBEVERAGE_H
#define INC_1_IBEVERAGE_H

#pragma once

#include <string>
#include <memory>


// Интерфейс "напиток"
class IBeverage
{
public:
    virtual std::string GetDescription() const = 0;
    virtual double GetCost()const = 0;
    virtual ~IBeverage() = default;
};

typedef std::unique_ptr<IBeverage> IBeveragePtr;

#endif //INC_1_IBEVERAGE_H