#ifndef INC_1_CONDIMENTS_H
#define INC_1_CONDIMENTS_H

#pragma once

#include "IBeverage.h"

// Базовый декоратор "Добавка к напитку". Также является напитком
class CCondimentDecorator : public IBeverage
{
public:
    std::string GetDescription()const override
    {
        // Описание декорированного напитка добавляется к описанию оборачиваемого напитка
        return m_beverage->GetDescription() + ", " + GetCondimentDescription();
    }

    double GetCost()const override
    {
        // Стоимость складывается из стоимости добавки и стоимости декорируемого напитка
        return m_beverage->GetCost() + GetCondimentCost();
    }

    // Стоимость и описание добавки вычисляется в классах конкретных декораторов
    virtual std::string GetCondimentDescription()const = 0;
    virtual double GetCondimentCost()const = 0;
protected:
    CCondimentDecorator(IBeveragePtr&& beverage)
        : m_beverage(move(beverage))
    {
    }
private:
    IBeveragePtr m_beverage;
};

// Добавка из корицы
class CCinnamon : public CCondimentDecorator
{
public:
    CCinnamon(IBeveragePtr&& beverage)
        : CCondimentDecorator(move(beverage))
    {}
protected:
    double GetCondimentCost()const override
    {
        return 20;
    }

    std::string GetCondimentDescription()const override
    {
        return "Cinnamon";
    }
};

// Лимонная добавка
class CLemon : public CCondimentDecorator
{
public:
    CLemon(IBeveragePtr&& beverage, unsigned quantity = 1)
        : CCondimentDecorator(move(beverage))
        , m_quantity(quantity)
    {}
protected:
    double GetCondimentCost()const override
    {
        return 10.0 * m_quantity;
    }
    std::string GetCondimentDescription()const override
    {
        return "Lemon x " + std::to_string(m_quantity);
    }
private:
    unsigned m_quantity;
};


enum class IceCubeType
{
    Dry,	// Сухой лед (для суровых сибирских мужиков)
    Water	// Обычные кубики из воды
};

// Добавка "Кубики льда". Определяется типом и количеством кубиков, что влияет на стоимость
// и описание
class CIceCubes : public CCondimentDecorator
{
public:
    CIceCubes(IBeveragePtr&& beverage, unsigned quantity, IceCubeType type = IceCubeType::Water)
        : CCondimentDecorator(move(beverage))
        , m_quantity(quantity)
        , m_type(type)
    {}
protected:
    double GetCondimentCost()const override
    {
        // Чем больше кубиков, тем больше стоимость.
        // Сухой лед стоит дороже
        return (m_type == IceCubeType::Dry ? 10 : 5) * m_quantity;
    }
    std::string GetCondimentDescription()const override
    {
        return std::string(m_type == IceCubeType::Dry ? "Dry" : "Water")
            + " ice cubes x " + std::to_string(m_quantity);
    }
private:
    unsigned m_quantity;
    IceCubeType m_type;
};

// Тип сиропа
enum class SyrupType
{
    Chocolate,	// Шоколадный
    Maple,		// Кленовый
};

// Добавка "Сироп"
class CSyrup : public CCondimentDecorator
{
public:
    CSyrup(IBeveragePtr&& beverage, SyrupType syrupType)
        : CCondimentDecorator(move(beverage))
        , m_syrupType(syrupType)
    {}
protected:
    double GetCondimentCost()const override
    {
        return 15;
    }
    std::string GetCondimentDescription()const override
    {
        return std::string(m_syrupType == SyrupType::Chocolate ? "Chocolate" : "Maple")
            + " syrup";
    }
private:
    SyrupType m_syrupType;
};

// Шоколадная крошка
class CChocolateCrumbs : public CCondimentDecorator
{
public:
    CChocolateCrumbs(IBeveragePtr&& beverage, unsigned mass)
        :CCondimentDecorator(move(beverage))
        , m_mass(mass)
    {
    }

    double GetCondimentCost()const override
    {
        return 2.0 * m_mass;
    }

    std::string GetCondimentDescription()const override
    {
        return "Chocolate crumbs " + std::to_string(m_mass) + "g";
    }
private:
    unsigned m_mass;
};

// Кокосовая стружка
class CCoconutFlakes : public CCondimentDecorator
{
public:
    CCoconutFlakes(IBeveragePtr&& beverage, unsigned mass)
        : CCondimentDecorator(move(beverage))
        , m_mass(mass)
    {}

protected:
    double GetCondimentCost()const override
    {
        return 1.0 * m_mass;
    }
    std::string GetCondimentDescription()const override
    {
        return "Coconut flakes " + std::to_string(m_mass) + "g";
    }
private:
    unsigned m_mass;
};

// Добавка "Сливки"
class CCream : public CCondimentDecorator
{
public:
    CCream(IBeveragePtr&& beverage)
        : CCondimentDecorator(move(beverage))
    {}

protected:
    double GetCondimentCost()const override
    {
        return 25.0; 
    }

    std::string GetCondimentDescription()const override
    {
        return "Cream"; 
    }
};

// Добавка "Шоколад"
class CChocolate : public CCondimentDecorator
{
public:
    CChocolate(IBeveragePtr&& beverage, unsigned quantity)
        : CCondimentDecorator(move(beverage)), m_quantity(quantity > 5 ? 5 : quantity) 
    {}

protected:
    double GetCondimentCost()const override
    {
        return 10.0 * m_quantity; 
    }

    std::string GetCondimentDescription()const override
    {
        return "Chocolate x " + std::to_string(m_quantity); 
    }

private:
    unsigned m_quantity; 
};

// Тип ликера
enum class LiqueurType
{
    Nut,      
    Chocolate 
};

// Добавка "Ликер"
class CLiqueur : public CCondimentDecorator
{
public:
    CLiqueur(IBeveragePtr&& beverage, LiqueurType liqueurType)
        : CCondimentDecorator(move(beverage)), m_liqueurType(liqueurType)
    {}

protected:
    double GetCondimentCost()const override
    {
        return 50.0;
    }

    std::string GetCondimentDescription()const override
    {
        return std::string(m_liqueurType == LiqueurType::Nut ? "Nut liqueur" : "Chocolate liqueur");
    }

private:
    LiqueurType m_liqueurType; // Тип ликера
};

#endif //INC_1_CONDIMENTS_H