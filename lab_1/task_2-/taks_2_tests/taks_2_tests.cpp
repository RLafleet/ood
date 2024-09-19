#define CATCH_CONFIG_MAIN
#include "../../../Catch2/catch.hpp"

#include "../DuckFunctions.h"
#include "../Duck/Duck.h"
#include "../Duck/Dance/DanceNoWay.h"
#include "../Duck/Fly/IFlyBehavior.h"
#include "../Duck/Quack/IQuakBehavior.h"
#include "../IQuackSound.h"
#include <memory>

class StubFlyBehavior : public IFlyBehavior {
public:
    int flightCount = 0;

    void Fly() override {
        flightCount++;
    }

    int GetFlightCount() const override {
        return flightCount;
    }

    int GetNextFlightCount() const override {
        return flightCount + 1;
    }
};

class StubNoFlyBehavior : public IFlyBehavior {
public:
    void Fly() override {
    }

    int GetFlightCount() const override {
        return 0;  
    }

    int GetNextFlightCount() const override {
        return 0;  
    }
};

class StubQuackBehavior : public IQuackBehavior {
public:
    bool quacked = false;

    void Quack() override {
        quacked = true;
    }

    bool HasQuacked() const {
        return quacked;
    }
};

class StubDanceBehavior : public IDanceBehavior {
public:
    bool danced = false;

    void Dance() override {
        danced = true;
    }

    bool HasDanced() const {
        return danced;
    }
};

// код отфарматировать
class ModelTestDuck : public Duck {
public:
    ModelTestDuck(std::unique_ptr<IFlyBehavior>&& flyBehavior,
        std::unique_ptr<IQuackBehavior>&& quackBehavior,
        std::unique_ptr<IDanceBehavior>&& danceBehavior)
        : Duck(std::move(flyBehavior), std::move(danceBehavior), std::move(quackBehavior)) {}

    void Display() const override {
    }
};

TEST_CASE("Duck can dance") {
    auto danceBehavior = std::make_unique<StubDanceBehavior>();

    ModelTestDuck duck(
        std::make_unique<StubFlyBehavior>(),
        std::make_unique<StubQuackBehavior>(),
        std::move(danceBehavior)
    );

    duck.Dance();

    REQUIRE(static_cast<StubDanceBehavior*>(duck.GetDanceBehavior())->HasDanced() == true);
}

TEST_CASE("Flyable duck quacks after even flight") {
    auto flyBehavior = std::make_unique<StubFlyBehavior>();
    auto quackBehavior = std::make_unique<StubQuackBehavior>();

    ModelTestDuck duck(
        std::move(flyBehavior),
        std::move(quackBehavior),
        std::make_unique<StubDanceBehavior>()
    );

    duck.Fly();
    REQUIRE(duck.GetFlyBehavior()->GetFlightCount() == 1);
    REQUIRE(static_cast<StubQuackBehavior*>(duck.GetQuackBehavior())->HasQuacked() == false);

    duck.Fly();
    REQUIRE(duck.GetFlyBehavior()->GetFlightCount() == 2);
    REQUIRE(static_cast<StubQuackBehavior*>(duck.GetQuackBehavior())->HasQuacked() == true);
}

TEST_CASE("Not flyable duck doesn't quack after odd flight") {
    auto flyBehavior = std::make_unique<StubNoFlyBehavior>();
    auto quackBehavior = std::make_unique<StubQuackBehavior>();

    ModelTestDuck duck(
        std::move(flyBehavior),
        std::move(quackBehavior),
        std::make_unique<StubDanceBehavior>()
    );

    duck.Fly();

    REQUIRE(duck.GetFlyBehavior()->GetFlightCount() == 0);

    REQUIRE(static_cast<StubQuackBehavior*>(duck.GetQuackBehavior())->HasQuacked() == false);
}

TEST_CASE("Not flyable duck doesn't quack after even flight") {
    auto flyBehavior = std::make_unique<StubNoFlyBehavior>();
    auto quackBehavior = std::make_unique<StubQuackBehavior>();

    ModelTestDuck duck(
        std::move(flyBehavior),
        std::move(quackBehavior),
        std::make_unique<StubDanceBehavior>()
    );

    duck.Fly();
    duck.Fly();

    REQUIRE(duck.GetFlyBehavior()->GetFlightCount() == 0);

    REQUIRE(static_cast<StubQuackBehavior*>(duck.GetQuackBehavior())->HasQuacked() == false);
}
