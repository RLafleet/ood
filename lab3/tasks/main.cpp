#include "Beverages.h"
#include "Condiments.h"

#include <iostream>
#include <string>
#include <functional>

using namespace std;


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

struct MakeCream
{
    auto operator()(IBeveragePtr&& beverage) const
    {
        return make_unique<CCream>(std::move(beverage));
    }
};

struct MakeChocolate
{
    explicit MakeChocolate(unsigned quantity)
        : m_quantity(quantity > 5 ? 5 : quantity) 
    {}

    auto operator()(IBeveragePtr&& beverage) const
    {
        return make_unique<CChocolate>(std::move(beverage), m_quantity);
    }

private:
    unsigned m_quantity;
};

struct MakeLiqueur
{
    explicit MakeLiqueur(LiqueurType type)
        : m_type(type)
    {}

    auto operator()(IBeveragePtr&& beverage) const
    {
        return make_unique<CLiqueur>(std::move(beverage), m_type);
    }

private:
    LiqueurType m_type;
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
    return [=](auto&& b) {
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
        cout << "Choose tea type (1 - Special edition, 2 - Green, 3 - Gray): ";

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
        if (cubeChoice > 2 || cubeChoice < 1)
        {
            cout << "Invalid choice, try again)" << endl;
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
        if (syrupChoice > 2 || syrupChoice < 1)
        {
            cout << "Invalid Syrup Type choice" << endl;
            return false;
        }

        beverage = std::move(beverage) << MakeCondiment<CSyrup>
            (syrupChoice == 1 ? SyrupType::Maple : SyrupType::Chocolate);
        return true;

    case 7:
        beverage = std::move(beverage) << MakeCream();
        return true;

    case 8:
        cout << "How many pieces of chocolate would you like? (Max: 5): ";
        int chocolateQuantity;
        cin >> chocolateQuantity;

        if (chocolateQuantity < 1 )
        {
            cout << "Invalid quantity choice" << endl;
            return false;
        }

        beverage = std::move(beverage) << MakeChocolate(chocolateQuantity);
        return true;

    case 9:
    {
        int liqueurChoice;

        cout << "Choose Liqueur Type (1 - Nut, 2 - Chocolate): ";

        cin >> liqueurChoice;

        if (liqueurChoice < 1 || liqueurChoice > 2)
        {
            cout << "Invalid Liqueur Type choice" << endl;
            return false;
        }

        auto liqueurType = static_cast<LiqueurType>(liqueurChoice - 1);

        beverage = std::move(beverage) << MakeLiqueur(liqueurType);
        return true;
    }

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

    cout << "==============================\n";
    cout << "    Choose Your Beverage      \n";
    cout << "==============================\n";
    cout << "1 - Coffee                  \n";
    cout << "2 - Cappuccino              \n";
    cout << "3 - Latte                   \n";
    cout << "4 - Tea                     \n";
    cout << "5 - Milkshake               \n";
    cout << "==============================\n";

    int beverageChoice;
    cin >> beverageChoice;

    if (CompleteBeverageChoice(beverage, beverageChoice))
    {
        while (true)
        {
            cout << "==============================\n";
            cout << "      Choose Your Condiment   \n";
            cout << "==============================\n";
            cout << "1 - Lemon                  \n";
            cout << "2 - Cinnamon               \n";
            cout << "3 - Ice Cubes             \n";
            cout << "4 - Chocolate Crumbs       \n";
            cout << "5 - Coconut Flakes         \n";
            cout << "6 - Syrup                 \n";
            cout << "7 - Cream                 \n";
            cout << "8 - Chocolate Slice        \n";
            cout << "9 - Liqueur               \n";
            cout << "0 - It'all                 \n";
            cout << "==============================\n";

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
        cout << "==============================\n";
        cout << "      Choose Your Condiment   \n";
        cout << "==============================\n";
        cout << "1 - Lemon                  \n";
        cout << "2 - Cinnamon               \n";
        cout << "0 - Checkout               \n";
        cout << "==============================\n";
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