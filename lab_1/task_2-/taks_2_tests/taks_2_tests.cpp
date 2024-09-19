#define CATCH_CONFIG_MAIN
#include "../../../Catch2/catch.hpp"

#include "../DuckFunctions.h"
#include "../Duck/Duck.h"
#include "../Duck/Dance/DanceNoWay.h"
#include "../Duck/Fly/IFlyBehavior.h"
#include "../Duck/Quack/IQuakBehavior.h"
#include "../IQuackSound.h"
#include <memory>

// Заглушка для IFlyBehavior (летающая утка)
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

// Заглушка для IFlyBehavior (нелетающая утка)
class StubNoFlyBehavior : public IFlyBehavior {
public:
    void Fly() override {
        // Никакой логики, утка не летает
    }

    int GetFlightCount() const override {
        return 0;  // Всегда 0, так как утка не летает
    }

    int GetNextFlightCount() const override {
        return 0;  // Всегда 0, утка не будет летать
    }
};

// Заглушка для IQuackBehavior
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

// Заглушка для IDanceBehavior
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

class StubQuackSound : public IQuackSound {
public:
    bool AllowQuacking(int flightAmount) const override {
        return (flightAmount % 2 == 0);  
    }
};

class ModelTestDuck : public Duck {
public:
    ModelTestDuck(std::unique_ptr<IFlyBehavior>&& flyBehavior,
        std::unique_ptr<IQuackBehavior>&& quackBehavior,
        std::unique_ptr<IDanceBehavior>&& danceBehavior,
        std::unique_ptr<IQuackSound>&& quackSound)
        : Duck(std::move(flyBehavior), std::move(danceBehavior), std::move(quackBehavior), std::move(quackSound)) {}

    void Display() const override {
    }
};

TEST_CASE("Duck can dance") {
    auto danceBehavior = std::make_unique<StubDanceBehavior>();
    auto quackSound = std::make_unique<StubQuackSound>();

    ModelTestDuck duck(
        std::make_unique<StubFlyBehavior>(),
        std::make_unique<StubQuackBehavior>(),
        std::move(danceBehavior),
        std::make_unique<StubQuackSound>()
    );

    duck.Dance();

    REQUIRE(static_cast<StubDanceBehavior*>(duck.GetDanceBehavior())->HasDanced() == true);
}

TEST_CASE("Flyable duck quacks after even flight") {
    auto flyBehavior = std::make_unique<StubFlyBehavior>();
    auto quackBehavior = std::make_unique<StubQuackBehavior>();
    auto quackSound = std::make_unique<StubQuackSound>();

    ModelTestDuck duck(
        std::move(flyBehavior),
        std::move(quackBehavior),
        std::make_unique<StubDanceBehavior>(),
        std::move(quackSound)
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
    auto quackSound = std::make_unique<StubQuackSound>();

    ModelTestDuck duck(
        std::move(flyBehavior),
        std::move(quackBehavior),
        std::make_unique<StubDanceBehavior>(),
        std::move(quackSound)
    );

    duck.Fly();

    REQUIRE(duck.GetFlyBehavior()->GetFlightCount() == 0);

    REQUIRE(static_cast<StubQuackBehavior*>(duck.GetQuackBehavior())->HasQuacked() == false);
}

TEST_CASE("Not flyable duck doesn't quack after even flight") {
    auto flyBehavior = std::make_unique<StubNoFlyBehavior>();
    auto quackBehavior = std::make_unique<StubQuackBehavior>();
    auto quackSound = std::make_unique<StubQuackSound>();

    ModelTestDuck duck(
        std::move(flyBehavior),
        std::move(quackBehavior),
        std::make_unique<StubDanceBehavior>(),
        std::move(quackSound)
    );

    duck.Fly();
    duck.Fly();

    REQUIRE(duck.GetFlyBehavior()->GetFlightCount() == 0);

    REQUIRE(static_cast<StubQuackBehavior*>(duck.GetQuackBehavior())->HasQuacked() == false);
}
