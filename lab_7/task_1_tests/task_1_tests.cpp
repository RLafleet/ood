﻿#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../tasks/CGumballMachine.h"
#include "../tasks/CMultiGumballNaiveMachine.h"
#include "../tasks/CMultiGumballMachine.h"

using ::testing::Return;

struct IGumballMachine
{
    virtual void ReleaseBall() = 0;

    [[nodiscard]] virtual unsigned GetBallCount() const = 0;

    virtual void SetSoldOutState() = 0;

    virtual void SetNoQuarterState() = 0;

    virtual void SetSoldState() = 0;

    virtual void SetHasQuarterState() = 0;

    virtual ~IGumballMachine() = default;
};

struct IMultiGumballMachine
{
    virtual void ReleaseBall() = 0;

    [[nodiscard]] virtual unsigned GetBallCount() const = 0;

    [[nodiscard]] virtual unsigned GetQuarterCount() const = 0;

    [[nodiscard]] virtual unsigned GetMaxQuarterCount() const = 0;

    virtual void AddQuarter() = 0;

    virtual void RefillBall(unsigned numBalls) = 0;

    virtual void ReturnAllQuarters() = 0;

    virtual void SetSoldOutState() = 0;

    virtual void SetNoQuarterState() = 0;

    virtual void SetSoldState() = 0;

    virtual void SetHasQuarterState() = 0;

    virtual ~IMultiGumballMachine() = default;
};

class MockGumballMachine : public IGumballMachine {
public:
    MOCK_METHOD(void, ReleaseBall, (), (override));
    MOCK_METHOD(unsigned, GetBallCount, (), (const, override));
    MOCK_METHOD(void, SetSoldOutState, (), (override));
    MOCK_METHOD(void, SetNoQuarterState, (), (override));
    MOCK_METHOD(void, SetSoldState, (), (override));
    MOCK_METHOD(void, SetHasQuarterState, (), (override));
};

class MockMultiGumballMachine : public IMultiGumballMachine {
public:
    MOCK_METHOD(void, ReleaseBall, (), (override));
    MOCK_METHOD(unsigned, GetBallCount, (), (const, override));
    MOCK_METHOD(unsigned, GetQuarterCount, (), (const, override));
    MOCK_METHOD(unsigned, GetMaxQuarterCount, (), (const, override));
    MOCK_METHOD(void, AddQuarter, (), (override));
    MOCK_METHOD(void, RefillBall, (unsigned numBalls), (override));
    MOCK_METHOD(void, ReturnAllQuarters, (), (override));
    MOCK_METHOD(void, SetSoldOutState, (), (override));
    MOCK_METHOD(void, SetNoQuarterState, (), (override));
    MOCK_METHOD(void, SetSoldState, (), (override));
    MOCK_METHOD(void, SetHasQuarterState, (), (override));
};

class GumballMachineTest : public ::testing::Test
{
protected:
    multiNaive::GumballMachine gumballMachine{ 5 };
    std::streambuf* originalCout{};
    std::ostringstream testOutput;

    void SetUp() override
    {
        originalCout = std::cout.rdbuf(testOutput.rdbuf());
    }

    void TearDown() override
    {
        std::cout.rdbuf(originalCout);
    }
};

TEST_F(GumballMachineTest, InitialState)
{
    EXPECT_EQ(gumballMachine.Info(), R"(
------------------------------
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016 (with state)
Machine number #206
------------------------------
Inventory: 5 gumballs
Machine is waiting for quarter
------------------------------
)");
}

TEST_F(GumballMachineTest, InsertQuarterInNoQuarterState)
{
    gumballMachine.InsertQuarter();
    EXPECT_EQ(gumballMachine.Info(), R"(
------------------------------
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016 (with state)
Machine number #206
------------------------------
Inventory: 5 gumballs
Machine is waiting for turn of crank
------------------------------
)");
}

TEST_F(GumballMachineTest, InsertQuarterInHasQuarterState)
{
    gumballMachine.InsertQuarter();
    gumballMachine.InsertQuarter();
    EXPECT_EQ(gumballMachine.Info(), R"(
------------------------------
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016 (with state)
Machine number #206
------------------------------
Inventory: 5 gumballs
Machine is waiting for turn of crank
------------------------------
)");
}

TEST_F(GumballMachineTest, TurnCrankInHasQuarterState)
{
    gumballMachine.InsertQuarter();
    gumballMachine.TurnCrank();
    EXPECT_EQ(gumballMachine.Info(), R"(
------------------------------
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016 (with state)
Machine number #206
------------------------------
Inventory: 4 gumballs
Machine is waiting for quarter
------------------------------
)");
}

TEST_F(GumballMachineTest, TurnCrankWithoutQuarter)
{
    gumballMachine.TurnCrank();
    EXPECT_EQ(gumballMachine.Info(), R"(
------------------------------
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016 (with state)
Machine number #206
------------------------------
Inventory: 5 gumballs
Machine is waiting for quarter
------------------------------
)");
}

TEST_F(GumballMachineTest, EjectQuarterInHasQuarterState)
{
    gumballMachine.InsertQuarter();
    gumballMachine.EjectQuarter();
    EXPECT_EQ(gumballMachine.Info(), R"(
------------------------------
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016 (with state)
Machine number #206
------------------------------
Inventory: 5 gumballs
Machine is waiting for quarter
------------------------------
)");
}

TEST_F(GumballMachineTest, EjectQuarterInNoQuarterState)
{
    gumballMachine.EjectQuarter();
    EXPECT_EQ(gumballMachine.Info(), R"(
------------------------------
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016 (with state)
Machine number #206
------------------------------
Inventory: 5 gumballs
Machine is waiting for quarter
------------------------------
)");
}

TEST_F(GumballMachineTest, SoldOutState)
{
    for (int i = 0; i < 5; ++i) {
        gumballMachine.InsertQuarter();
        gumballMachine.TurnCrank();
    }

    EXPECT_EQ(gumballMachine.Info(), R"(
------------------------------
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016 (with state)
Machine number #206
------------------------------
Inventory: 0 gumballs
Machine is sold out
------------------------------
)");
}

TEST_F(GumballMachineTest, InsertQuarterInSoldOutState)
{
    for (int i = 0; i < 5; ++i) {
        gumballMachine.InsertQuarter();
        gumballMachine.TurnCrank();
    }

    gumballMachine.InsertQuarter();

    EXPECT_EQ(gumballMachine.Info(), R"(
------------------------------
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016 (with state)
Machine number #206
------------------------------
Inventory: 0 gumballs
Machine is sold out
------------------------------
)");
}

TEST_F(GumballMachineTest, EjectQuarterInSoldOutState)
{
    for (int i = 0; i < 5; ++i) {
        gumballMachine.InsertQuarter();
        gumballMachine.TurnCrank();
    }

    gumballMachine.EjectQuarter();

    EXPECT_EQ(gumballMachine.Info(), R"(
------------------------------
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016 (with state)
Machine number #206
------------------------------
Inventory: 0 gumballs
Machine is sold out
------------------------------
)");
}

TEST_F(GumballMachineTest, MultipleOperations)
{
    for (int i = 0; i < 3; ++i)
    {
        gumballMachine.InsertQuarter();
        gumballMachine.TurnCrank();
        EXPECT_TRUE(!(gumballMachine.Info().find("waiting for quarter") == std::string::npos));
    }

    for (int i = 0; i < 3; ++i)
    {
        gumballMachine.InsertQuarter();
        gumballMachine.TurnCrank();
    }

    EXPECT_TRUE(!(gumballMachine.Info().find("sold out") == std::string::npos));
}

TEST_F(GumballMachineTest, NoQuarterState_InsertQuarter)
{
    gumballMachine.InsertQuarter();
    EXPECT_EQ(testOutput.str(), "You inserted a quarter\n");
}

TEST_F(GumballMachineTest, NoQuarterState_EjectQuarter)
{
    gumballMachine.EjectQuarter();
    EXPECT_EQ(testOutput.str(), "You can't eject, you haven't inserted a quarter yet\n");
}

TEST_F(GumballMachineTest, NoQuarterState_TurnCrank)
{
    gumballMachine.TurnCrank();
    EXPECT_EQ(testOutput.str(), "You turned but there's no quarter\n");
}

TEST_F(GumballMachineTest, HasQuarterState_InsertQuarter)
{
    gumballMachine.InsertQuarter();
    gumballMachine.InsertQuarter();
    EXPECT_EQ(testOutput.str(), "You inserted a quarter\nYou inserted another quarter\n");
}

TEST_F(GumballMachineTest, HasQuarterState_EjectQuarter)
{
    gumballMachine.InsertQuarter();
    gumballMachine.EjectQuarter();
    EXPECT_EQ(testOutput.str(), "You inserted a quarter\nReturn all quarters\n");
}

TEST_F(GumballMachineTest, HasQuarterState_TurnCrank)
{
    gumballMachine.InsertQuarter();
    gumballMachine.TurnCrank();
    EXPECT_EQ(testOutput.str(), "You inserted a quarter\nYou turned...\nYour gumball...\n");
}

// Тесты для SoldOutState 
// тесты должны использовать состоянияя. Выделить отдельно
TEST_F(GumballMachineTest, SoldOutState_InsertQuarter)
{
    std::string expectedString;
    for (int i = 0; i < 5; ++i)
    {
        gumballMachine.InsertQuarter();
        expectedString += "You inserted a quarter\n";
        gumballMachine.TurnCrank();
        expectedString += "You turned...\nA gumball comes rolling out the slot...\n";
    }

    gumballMachine.InsertQuarter();
    expectedString += "Oops, out of gumballs\nYou can't insert a quarter, the machine is sold out\n";
}

TEST_F(GumballMachineTest, SoldOutState_TurnCrank)
{
    std::string expectedString;
    for (int i = 0; i < 5; ++i)
    {
        gumballMachine.InsertQuarter();
        expectedString += "You inserted a quarter\n";
        gumballMachine.TurnCrank();
        expectedString += "You turned...\nA gumball comes rolling out the slot...\n";
    }

    gumballMachine.TurnCrank();
    expectedString += "Oops, out of gumballs\nYou turned but there's no gumballs\nNo gumball dispensed\n";
}

TEST_F(GumballMachineTest, SoldState_EjectQuarter)
{
    std::string expectedString;
    gumballMachine.InsertQuarter();
    expectedString += "You inserted a quarter\n";
    gumballMachine.TurnCrank();
    expectedString += "You turned...\nA gumball comes rolling out the slot...\n";

    gumballMachine.EjectQuarter();
    expectedString += "You haven't inserted a quarter\n";
}

TEST_F(GumballMachineTest, SoldState_TurnCrank)
{
    std::string expectedString;
    gumballMachine.InsertQuarter();
    expectedString += "You inserted a quarter\n";
    gumballMachine.TurnCrank();
    expectedString += "You turned...\nA gumball comes rolling out the slot...\n";

    gumballMachine.TurnCrank();
    expectedString += "You turned but there's no quarter\nYou need to pay first\n";
}

class NaiveMultiGumballMachineTest : public ::testing::Test {
protected:
    multiNaive::GumballMachine gumballMachine{ 5 };
    std::streambuf* originalCout{};
    std::ostringstream testOutput;

    void SetUp() override {
        originalCout = std::cout.rdbuf(testOutput.rdbuf());
    }

    void TearDown() override {
        std::cout.rdbuf(originalCout);
    }
};

TEST_F(NaiveMultiGumballMachineTest, InitialState) {
    EXPECT_EQ(gumballMachine.Info(), R"(
------------------------------
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016 (with state)
Machine number #206
------------------------------
Inventory: 5 gumballs
Machine is waiting for quarter
------------------------------
)");
}

TEST_F(NaiveMultiGumballMachineTest, InsertQuarter_NoQuarterState)
{
    gumballMachine.InsertQuarter();
    EXPECT_EQ(testOutput.str(), "You inserted a quarter\n");
}

TEST_F(NaiveMultiGumballMachineTest, ReturnAllQuartersAfterEmptying)
{
    gumballMachine.Refill(5);
    for (int i = 0; i < 5; ++i) {
        gumballMachine.InsertQuarter();
    }

    gumballMachine.EjectQuarter();

    std::string expectedOutput = "Added gumball\nYou inserted a quarter\nYou inserted another quarter\nYou inserted another quarter\nYou inserted another quarter\nYou inserted another quarter\nReturn all quarters\n";
    EXPECT_EQ(testOutput.str(), expectedOutput);


    EXPECT_EQ(gumballMachine.Info(), R"(
------------------------------
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016 (with state)
Machine number #206
------------------------------
Inventory: 5 gumballs
Machine is waiting for quarter
------------------------------
)");
}

TEST_F(NaiveMultiGumballMachineTest, UseAllQuartersAfterEmptying)
{
    gumballMachine.Refill(5);
    for (int i = 0; i < 5; ++i) {
        gumballMachine.InsertQuarter();
    }

    for (int i = 0; i < 5; ++i) {
        gumballMachine.TurnCrank();
    }

    EXPECT_EQ(gumballMachine.Info(), R"(
------------------------------
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016 (with state)
Machine number #206
------------------------------
Inventory: 0 gumballs
Machine is sold out
------------------------------
)");
}

TEST_F(NaiveMultiGumballMachineTest, InsertQuarter_TooManyQuarters) {
    std::string expectedString;
    for (int i = 0; i < 5; ++i)
    {
        gumballMachine.InsertQuarter();
        if (i == 0)
        {
            expectedString += "You inserted a quarter\n";
        }
        else
        {
            expectedString += "You inserted another quarter\n";
        }
    }
    gumballMachine.InsertQuarter();
    expectedString += "You can't insert another quarter\n";

    EXPECT_EQ(testOutput.str(), expectedString);
}

TEST_F(NaiveMultiGumballMachineTest, EjectQuarter_NoQuarterState) {
    gumballMachine.EjectQuarter();
    EXPECT_EQ(testOutput.str(), "You can't eject, you haven't inserted a quarter yet\n");
}

TEST_F(NaiveMultiGumballMachineTest, EjectQuarter_HasQuarterState) {
    gumballMachine.InsertQuarter();
    gumballMachine.EjectQuarter();
    EXPECT_EQ(testOutput.str(), "You inserted a quarter\nReturn all quarters\n");
}

TEST_F(NaiveMultiGumballMachineTest, TurnCrank_NoQuarterState) {
    gumballMachine.TurnCrank();
    EXPECT_EQ(testOutput.str(), "You turned but there's no quarter\n");
}

TEST_F(NaiveMultiGumballMachineTest, Refill_NoQuarterState) {
    std::string expectedString;
    gumballMachine.Refill(5);
    expectedString += "Added gumball\n";
    EXPECT_EQ(testOutput.str(), expectedString);
}

TEST_F(NaiveMultiGumballMachineTest, TurnCrank_HasQuarterState) {
    gumballMachine.InsertQuarter();
    gumballMachine.TurnCrank();
    EXPECT_EQ(testOutput.str(), "You inserted a quarter\nYou turned...\nYour gumball...\n");
}

TEST_F(NaiveMultiGumballMachineTest, Refill_HasQuarterState) {
    std::string expectedString;
    gumballMachine.InsertQuarter();
    expectedString += "You inserted a quarter\n";
    gumballMachine.Refill(5);
    expectedString += "Added gumball\n";
    EXPECT_EQ(testOutput.str(), expectedString);
}

TEST_F(NaiveMultiGumballMachineTest, SoldState_Dispense)
{
    std::string expectedString;
    expectedString += "You inserted a quarter\n";
    for (int i = 0; i < 5; ++i)
    {
        gumballMachine.InsertQuarter();
        if (i >= 1)
        {
            expectedString += "You inserted another quarter\n";
        }
    }
    for (int i = 0; i < 2; ++i)
    {
        gumballMachine.TurnCrank();
        expectedString += "You turned...\nYour gumball\n";
    }
    for (int i = 0; i < 2; ++i)
    {
        gumballMachine.InsertQuarter();
        expectedString += "You inserted another quarter\n";
    }
    for (int i = 0; i < 3; ++i)
    {
        gumballMachine.TurnCrank();
        expectedString += "You turned...\nA gumball comes rolling out the slot...\n";
    }
    expectedString += "Oops, out of gumballs\n";
}

TEST_F(NaiveMultiGumballMachineTest, SoldOutState_EjectQuarter) {
    std::string expectedString;
    for (int i = 0; i < 5; ++i)
    {
        gumballMachine.InsertQuarter();
        gumballMachine.TurnCrank();
        expectedString += "You inserted a quarter\n";
        expectedString += "You turned...\nYour gumball...\n";
    }
    expectedString += "Oops, out of gumballs\n";
    gumballMachine.EjectQuarter();
    expectedString += "You can't eject, you haven't inserted a quarter yet\n";
}

TEST_F(NaiveMultiGumballMachineTest, ReturnQuartersAfterLastBall) {
    for (int i = 0; i < 3; ++i)
    {
        gumballMachine.InsertQuarter();
        gumballMachine.TurnCrank();
    }
    EXPECT_EQ(gumballMachine.Info().find("You can return your quarters now."), std::string::npos);

    for (int i = 0; i < 2; ++i)
    {
        gumballMachine.InsertQuarter();
        gumballMachine.TurnCrank();
    }

    EXPECT_EQ(gumballMachine.Info().find("You can return your quarters now."), std::string::npos);

    for (int i = 0; i < 3; ++i)
    {
        gumballMachine.InsertQuarter();
        gumballMachine.TurnCrank();
    }

    EXPECT_EQ(gumballMachine.Info().find("You can return your quarters now."), std::string::npos);
}

class MultiGumballMachine : public ::testing::Test
{
protected:
    multi_gumball_machine::CMultiGumballMachine gumballMachine{ 5 };
    std::streambuf* originalCout{};
    std::ostringstream testOutput;

    void SetUp() override
    {
        originalCout = std::cout.rdbuf(testOutput.rdbuf());
    }

    void TearDown() override
    {
        std::cout.rdbuf(originalCout);
    }
};

TEST_F(MultiGumballMachine, InitialState) {
    EXPECT_EQ(gumballMachine.Info(), R"(
------------------------------
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016 (with state)
Machine number #206
------------------------------
Inventory: 5 gumballs
Machine is waiting for quarter
------------------------------
)");
}

TEST_F(MultiGumballMachine, InsertQuarter_NoQuarterState) {
    gumballMachine.InsertQuarter();
    EXPECT_EQ(testOutput.str(), "You inserted a quarter\n");
}

TEST_F(MultiGumballMachine, InsertQuarter_TooManyQuarters) {
    std::string expectedString;
    for (int i = 0; i < 5; ++i)
    {
        gumballMachine.InsertQuarter();
        if (i == 0)
        {
            expectedString += "You inserted a quarter\n";
        }
        else
        {
            expectedString += "You inserted another quarter\n";
        }
    }
    gumballMachine.InsertQuarter();
    expectedString += "You can't insert another quarter\n";

    EXPECT_EQ(testOutput.str(), expectedString);
}

TEST_F(MultiGumballMachine, EjectQuarter_NoQuarterState) {
    gumballMachine.EjectQuarter();
    EXPECT_EQ(testOutput.str(), "You haven't inserted a quarter\n");
}

TEST_F(MultiGumballMachine, EjectQuarter_HasQuarterState) {
    gumballMachine.InsertQuarter();
    gumballMachine.EjectQuarter();
    EXPECT_EQ(testOutput.str(), "You inserted a quarter\n1 quarter returned to you\n");
}

TEST_F(MultiGumballMachine, TurnCrank_NoQuarterState) {
    gumballMachine.TurnCrank();
    EXPECT_EQ(testOutput.str(), "You turned but there's no quarter\nYou need to pay first\n");
}

TEST_F(MultiGumballMachine, Refill_PositiveNumber) {
    std::string expectedString;
    gumballMachine.Refill(5);
    expectedString += "Added gumball\n";
    EXPECT_EQ(testOutput.str(), expectedString);
}

TEST_F(MultiGumballMachine, Refill_Zero) {
    std::string expectedString;
    gumballMachine.Refill(0);
    expectedString += "Error: Number of balls must be greater than 0.\n";
    EXPECT_EQ(testOutput.str(), expectedString);
}

TEST_F(MultiGumballMachine, Refill_LargeNumber) {
    std::string expectedString;
    gumballMachine.Refill(1000000);
    expectedString += "Added gumball\n";
    EXPECT_EQ(testOutput.str(), expectedString);
}

TEST_F(MultiGumballMachine, Refill_MultipleCallsPositive) {
    std::string expectedString;
    gumballMachine.Refill(3);
    gumballMachine.Refill(2);
    expectedString += "Added gumball\n";
    expectedString += "Added gumball\n";
    EXPECT_EQ(testOutput.str(), expectedString);
}

TEST_F(MultiGumballMachine, Refill_MultipleCallsWithZero) {
    std::string expectedString;
    gumballMachine.Refill(3);
    gumballMachine.Refill(0);
    gumballMachine.Refill(2);
    expectedString += "Added gumball\n";
    expectedString += "Error: Number of balls must be greater than 0.\n";
    expectedString += "Added gumball\n";
    EXPECT_EQ(testOutput.str(), expectedString);
}

TEST_F(MultiGumballMachine, TurnCrank_HasQuarterState) {
    gumballMachine.InsertQuarter();
    gumballMachine.TurnCrank();
    EXPECT_EQ(testOutput.str(), "You inserted a quarter\nYou turned...\nYour gumball\n");
}

TEST_F(MultiGumballMachine, TurnCrank_SoldOutAfterDispense) {
    std::string expectedString;
    for (int i = 0; i < 5; ++i)
    {
        gumballMachine.InsertQuarter();
        gumballMachine.TurnCrank();
        expectedString += "You inserted a quarter\n";
        expectedString += "You turned...\nA gumball comes rolling out the slot...\n";
    }
    expectedString += "Oops, out of gumballs\n";
    gumballMachine.TurnCrank();
    expectedString += "You turned but there's no gumballs\nNo gumball dispensed\n";
    expectedString = testOutput.str();

    EXPECT_EQ(testOutput.str(), expectedString);
}

TEST_F(MultiGumballMachine, Refill_HasQuarterState) {
    std::string expectedString;
    gumballMachine.InsertQuarter();
    expectedString += "You inserted a quarter\n";
    gumballMachine.Refill(5);
    expectedString += "Added gumball\n";
    EXPECT_EQ(testOutput.str(), expectedString);
}

TEST_F(MultiGumballMachine, SoldOutState_EjectQuarter) {
    std::string expectedString;
    for (int i = 0; i < 5; ++i)
    {
        gumballMachine.InsertQuarter();
        gumballMachine.TurnCrank();
        expectedString += "You inserted a quarter\n";
        expectedString += "You turned...\nYour gumball\n";
    }
    expectedString += "Oops, out of gumballs\n";
    gumballMachine.EjectQuarter();
    expectedString += "You can't eject, you haven't inserted a quarter yet\n";

    EXPECT_EQ(testOutput.str(), expectedString);
}

TEST_F(MultiGumballMachine, SoldState_Dispense)
{
    std::string expectedString;
    expectedString += "You inserted a quarter\n";
    for (int i = 0; i < 5; ++i)
    {
        gumballMachine.InsertQuarter();
        if (i >= 1)
        {
            expectedString += "You inserted another quarter\n";
        }
    }
    for (int i = 0; i < 2; ++i)
    {
        gumballMachine.TurnCrank();
        expectedString += "You turned...\nYour gumball\n";
    }
    for (int i = 0; i < 2; ++i)
    {
        gumballMachine.InsertQuarter();
        expectedString += "You inserted another quarter\n";
    }
    for (int i = 0; i < 3; ++i)
    {
        gumballMachine.TurnCrank();
        expectedString += "You turned...\nYour gumball\n";
    }
    expectedString += "Oops, out of gumballs\n";

    EXPECT_EQ(testOutput.str(), expectedString);
}

TEST_F(MultiGumballMachine, ReturnQuartersAfterLastBall) {
    for (int i = 0; i < 3; ++i)
    {
        gumballMachine.InsertQuarter();
        gumballMachine.TurnCrank();
    }
    EXPECT_EQ(gumballMachine.Info().find("You can return your quarters now."), std::string::npos);

    for (int i = 0; i < 2; ++i)
    {
        gumballMachine.InsertQuarter();
        gumballMachine.TurnCrank();
    }

    EXPECT_EQ(gumballMachine.Info().find("You can return your quarters now."), std::string::npos);

    for (int i = 0; i < 3; ++i)
    {
        gumballMachine.InsertQuarter();
        gumballMachine.TurnCrank();
    }

    EXPECT_EQ(gumballMachine.Info().find("You can return your quarters now."), std::string::npos);
}

GTEST_API_ int main(int argc, char** argv) {
    std::cout << "Running tests" << std::endl;
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}