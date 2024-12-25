#define CATCH_CONFIG_MAIN
#include "../../Catch2/catch.hpp"
#include <sstream>
#include "../tasks/CGumballMachine.h"

using namespace gumball_machine;

// Функция для получения строки состояния
std::string GetExpectedMachineStringState(unsigned count, const std::string& stateString)
{
    return std::format(R"(
                Mighty Gumball, Inc.
                C++-enabled Standing Gumball Model #2016 (with state)
                Inventory: {} gumball{}
                Machine is {}
            )", count, count != 1 ? "s" : "", stateString);
}

// Вспомогательная функция для захвата вывода
std::string CaptureStdOut(std::function<void()> func)
{
    std::ostringstream output;
    std::streambuf* oldCout = std::cout.rdbuf(output.rdbuf());
    func();
    std::cout.rdbuf(oldCout);
    return output.str();
}

TEST_CASE("Initialization with positive count")
{
    const CGumballMachine machine(5);
    REQUIRE(machine.ToString() == GetExpectedMachineStringState(5, "waiting for quarter"));
}

TEST_CASE("Initialization with zero count")
{
    const CGumballMachine machine(0);
    REQUIRE(machine.ToString() == GetExpectedMachineStringState(0, "sold out"));
}

TEST_CASE("Eject quarter without inserting")
{
    const CGumballMachine machine(5);

    auto output = CaptureStdOut([&]() { machine.EjectQuarter(); });

    REQUIRE(output == "You haven't inserted a quarter\n");
    REQUIRE(machine.ToString() == GetExpectedMachineStringState(5, "waiting for quarter"));
}

TEST_CASE("Insert quarter")
{
    const CGumballMachine machine(5);

    auto output = CaptureStdOut([&]() { machine.InsertQuarter(); });

    REQUIRE(output == "You inserted a quarter\n");
    REQUIRE(machine.ToString() == GetExpectedMachineStringState(5, "waiting for turn of crank"));
}

TEST_CASE("Turn crank without quarter")
{
    const CGumballMachine machine(5);

    auto output = CaptureStdOut([&]() { machine.TurnCrank(); });

    REQUIRE(output == "You turned but there's no quarter\nYou need to pay first\n");
    REQUIRE(machine.ToString() == GetExpectedMachineStringState(5, "waiting for quarter"));
}

TEST_CASE("Release ball after inserting quarter and turning crank")
{
    CGumballMachine machine(5);

    auto output = CaptureStdOut([&]() {
        machine.InsertQuarter();
        machine.TurnCrank();
        });

    REQUIRE(output == "You inserted a quarter\nYou turned...\nA gumball comes rolling out the slot...\n");
    REQUIRE(machine.ToString() == GetExpectedMachineStringState(4, "waiting for quarter"));
}

TEST_CASE("Release ball with zero gumballs")
{
    CGumballMachine machine(0);

    auto output = CaptureStdOut([&]() {
        machine.InsertQuarter();
        machine.TurnCrank();
        });

    REQUIRE(output == "You can't insert a quarter, the machine is sold out\nYou turned but there's no gumballs\nNo gumball dispensed\n");
    REQUIRE(machine.ToString() == GetExpectedMachineStringState(0, "sold out"));
}

TEST_CASE("Set sold-out state after last gumball dispensed")
{   
    CGumballMachine machine(1);

    auto output = CaptureStdOut([&]() {
        machine.InsertQuarter();
        machine.TurnCrank();
        });

    REQUIRE(output == "You inserted a quarter\nYou turned...\nA gumball comes rolling out the slot...\nOops, out of gumballs\n");
    REQUIRE(machine.ToString() == GetExpectedMachineStringState(0, "sold out"));
}
