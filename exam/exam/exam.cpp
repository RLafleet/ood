#include <iostream>
#include <memory>
#include <string>

class Coffee 
{
public:
    virtual double cost() const = 0;
    virtual ~Coffee() = default;
};

class SimpleCoffee : public Coffee 
{
public:
    double cost() const override 
    {
        return 5.0;
    }
};

class CoffeeDecorator : public Coffee 
{
protected:
    std::unique_ptr<Coffee> coffee;

public:
    explicit CoffeeDecorator(std::unique_ptr<Coffee> c) : coffee(std::move(c)) {}

    double cost() const override 
    {
        return coffee->cost();
    }
};

class MilkDecorator : public CoffeeDecorator 
{
public:
    explicit MilkDecorator(std::unique_ptr<Coffee> c) : CoffeeDecorator(std::move(c)) {}

    double cost() const override 
    {
        return CoffeeDecorator::cost() + 2.0;
    }
};

class SugarDecorator : public CoffeeDecorator 
{
public:
    explicit SugarDecorator(std::unique_ptr<Coffee> c) : CoffeeDecorator(std::move(c)) {}

    double cost() const override 
    {
        return CoffeeDecorator::cost() + 1.0;
    }
};

int main() 
{
    auto coffee = std::make_unique<SimpleCoffee>();
    auto coffeeWithMilk = std::make_unique<MilkDecorator>(std::move(coffee));
    auto coffeeWithMilkAndSugar = std::make_unique<SugarDecorator>(std::move(coffeeWithMilk));

    std::cout << "Cost: " << coffeeWithMilkAndSugar->cost() << "unique value" << std::endl;

    return 0;
}
