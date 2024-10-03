#include "Beverages.h"
#include "Condiments.h"

#include <iostream>
#include <string>
#include <functional>

using namespace std;


/*
Функциональный объект, создающий лимонную добавку
*/
struct MakeLemon
{
    explicit MakeLemon(unsigned quantity)
        :m_quantity(quantity)
    {}

    auto operator()(IBeveragePtr&& beverage)const
    {
        return make_unique<CLemon>(std::move(beverage), m_quantity);
    }
private:
    unsigned m_quantity;
};

function<IBeveragePtr(IBeveragePtr&&)> MakeCinnamon()
{
    return [](IBeveragePtr&& b) {
        return make_unique<CCinnamon>(std::move(b));
        };
}

template <typename Condiment, typename... Args>
auto MakeCondiment(const Args&...args)
{
    // Возвращаем функцию, декорирующую напиток, переданный ей в качестве аргумента
    // Дополнительные аргументы декоратора, захваченные лямбда-функцией, передаются
    // конструктору декоратора через make_unique
    return [=](auto&& b) {
        // Функции make_unique передаем b вместе со списком аргументов внешней функции
        return make_unique<Condiment>(std::forward<decltype(b)>(b), args...);
        };
}

template <typename Component, typename Decorator>
auto operator << (Component&& component, const Decorator& decorate)
{
    return decorate(std::forward<Component>(component));
}

bool CompleteBeverageChoice(unique_ptr<IBeverage>& beverage, int beverageChoice)
{
    switch (beverageChoice)
    {
    case 1:
        beverage = make_unique<CCoffee>();
        return true;
    case 2:
        cout << "Choose Cappuccino portion (1 - Standard, 2 - Double): ";
        int cappuccinoChoice;

        cin >> cappuccinoChoice;
        if (cappuccinoChoice > 4 or cappuccinoChoice < 1)
        {
            cout << "Invalid Cappuccino portion choice";
            return false;
        }

        beverage = make_unique<CCappuccino>(cappuccinoChoice == 2);
        return true;
    case 3:
        cout << "Choose Latte portion (1 - Standard, 2 - Double): ";

        int latteChoice;
        cin >> latteChoice;

        if (latteChoice > 4 or latteChoice < 1)
        {
            cout << "Invalid Latte portion choice";
            return false;
        }

        beverage = make_unique<CLatte>(latteChoice == 2);
        return true;
    case 4:
    {
        cout << "Choose tea type (1 - Black, 2 - Green, 3 - Red): ";

        int teaChoice;
        cin >> teaChoice;

        if (teaChoice > 4 or teaChoice < 1)
        {
            cout << "Invalid tea type choice";
            return false;
        }

        auto teaType = static_cast<TeaType>(teaChoice - 1);
        beverage = make_unique<CTea>(teaType);
        return true;
    }
    case 5:
    {
        cout << "Choose milkshake size (1 - Small, 2 - Medium, 3 - Large): ";

        int sizeChoice;
        cin >> sizeChoice;

        if (sizeChoice > 3 or sizeChoice < 1)
        {
            cout << "Invalid size milkshake choice";
            return false;
        }

        beverage = make_unique<CMilkshake>(static_cast<MilkshakeSizeType>(sizeChoice - 1));
        return true;
    }
    default:
        cout << "Invalid choice beverage" << endl;
        return false;
    }
}

bool CompleteCondimentChoice(unique_ptr<IBeverage>& beverage, int condimentChoice)
{
    switch (condimentChoice)
    {
    case 1:
        beverage = std::move(beverage) << MakeCondiment<CLemon>(2);
        return true;
    case 2:
        beverage = std::move(beverage) << MakeCondiment<CCinnamon>();
        return true;
    case 3:
        cout << "Choose Ice Cubes Type (1 - Water, 2 - Dry): ";
        int cubeChoice;
        cin >> cubeChoice;
        if (cubeChoice > 2 or cubeChoice < 1)
        {
            cout << "Invalid choice, try again)";
            return false;
        }
        beverage = std::move(beverage) << MakeCondiment<CIceCubes>
            (2, cubeChoice == 1 ? IceCubeType::Water : IceCubeType::Dry);
        return true;
    case 4:
        beverage = std::move(beverage) << MakeCondiment<CChocolateCrumbs>(5);
        return true;
    case 5:
        beverage = std::move(beverage) << MakeCondiment<CCoconutFlakes>(5);
        return true;
    case 6:
        cout << "Choose Syrup Type (1 - Maple, 2 - Chocolate): ";
        int syrupChoice;

        cin >> syrupChoice;
        if (syrupChoice > 2 or syrupChoice < 1)
        {
            cout << "Invalid Syrup Type choice";
            return false;
        }

        beverage = std::move(beverage) << MakeCondiment<CSyrup>
            (syrupChoice == 1 ? SyrupType::Maple : SyrupType::Chocolate);
        return true;
    case 0:
        cout << "Break" << endl;
        cout << beverage->GetDescription() << ", cost: " << beverage->GetCost() << endl;
        return false;
    default:
        cout << "Invalid choice condiment" << endl;
        return true;
    }
}

void DialogWithUser()
{
    unique_ptr<IBeverage> beverage;

    cout << "Choose your base beverage:\n";
    cout << "1 - Coffee\n2 - Cappuccino\n3 - Latte\n4 - Tea\n5 - Milkshake\n";

    int beverageChoice;
    cin >> beverageChoice;

    if (CompleteBeverageChoice(beverage, beverageChoice))
    {
        while (true)
        {
            cout << "Choose your condiment:\n";
            cout << "1 - Lemon\n2 - Cinnamon\n3 - Ice Cubes\n4 - Chocolate Crumbs\n";
            cout << "5 - Coconut Flakes\n6 - Syrup\n";
            cout << "0 - Break\n";

            int condimentChoice;
            cin >> condimentChoice;

            if (!CompleteCondimentChoice(beverage, condimentChoice))
            {
                break;
            }
        }
    }
}

void DialogWithUserDeprecated()
{
    cout << "Type 1 for coffee or 2 for tea\n";
    int beverageChoice;
    cin >> beverageChoice;

    unique_ptr<IBeverage> beverage;

    if (beverageChoice == 1)
    {
        beverage = make_unique<CCoffee>();
    }
    else if (beverageChoice == 2)
    {
        beverage = make_unique<CTea>(TeaType::BLACK);
    }
    else
    {
        return;
    }

    int condimentChoice;
    for (;;)
    {
        cout << "1 - Lemon, 2 - Cinnamon, 0 - Checkout" << endl;
        cin >> condimentChoice;

        if (condimentChoice == 1)
        {
            beverage = move(beverage) << MakeCondiment<CLemon>(2);
        }
        else if (condimentChoice == 2)
        {
            beverage = move(beverage) << MakeCondiment<CCinnamon>();
        }
        else if (condimentChoice == 0)
        {
            break;
        }
        else
        {
            return;
        }
    }

    cout << beverage->GetDescription() << ", cost: " << beverage->GetCost() << endl;
}


int main()
{
    DialogWithUser();
    cout << endl;
   
}