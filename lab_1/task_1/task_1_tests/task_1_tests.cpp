#define CATCH_CONFIG_MAIN
#include "../../../Catch2/catch.hpp"

#include "../DuckFunctions.h"
#include "../Duck/MallardDuck.h"
#include "../Duck/RedheadDuck.h"
#include "../Duck/ModelDuck.h"
#include "../Duck/RubberDuck.h"
#include "../Duck/Dance/IDanceBehavior.h"

const std::string danceNoWayOutput = "";
const std::string danceMinuetOutput = "I'm dancing the Minuet!\n";
const std::string danceWaltzOutput = "I'm dancing the Waltz!\n";

class MockDanceBehavior : public IDanceBehavior {
public:
    std::string output;

    MockDanceBehavior(const std::string& expectedOutput)
        : output(""), expectedOutput(expectedOutput) {}

    void Dance() override {
        output = expectedOutput;
    }

    const std::string& GetOutput() const {
        return output;
    }

private:
    std::string expectedOutput;
};

void assertDance(Duck& duck, const std::string& expectedOutput) {
    auto mockDance = std::make_unique<MockDanceBehavior>(expectedOutput);
    auto* mockDancePtr = mockDance.get(); 

    duck.SetDanceBehavior(std::move(mockDance));
    duck.Dance();


    REQUIRE(mockDancePtr->GetOutput() == expectedOutput);
}

TEST_CASE("Duck Dance Tests", "[dance]") {
    SECTION("Mallard Duck") {
        MallardDuck duck;
        assertDance(duck, danceWaltzOutput);
    }

    SECTION("Model Duck") {
        ModelDuck duck;
        assertDance(duck, danceNoWayOutput);
    }

    SECTION("Redhead Duck") {
        RedheadDuck duck;
        assertDance(duck, danceMinuetOutput);
    }

    SECTION("Rubber Duck") {
        RubberDuck duck;
        assertDance(duck, danceNoWayOutput);
    }
}