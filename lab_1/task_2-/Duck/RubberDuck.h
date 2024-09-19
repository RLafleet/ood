#ifndef RUBBERDUCK_H
#define RUBBERDUCK_H

#include "Duck.h"
#include "Fly/FlyNoWay.h"
#include "Dance/DanceNoWay.h"
#include "Quack/SqueakBehavior.h"
#include <iostream>
#include "../OddFlyQuackSound.h"

class RubberDuck : public Duck
{
public:
	RubberDuck()
		: Duck(
			std::make_unique<FlyNoWay>(), 
			std::make_unique<DanceNoWay>(), 
			std::make_unique<SqueakBehavior>(),
			std::make_unique<OddFlightsQuacking>()
		)
	{
	}

	void Display() const override
	{
		std::cout << "I'm rubber duck" << std::endl;
	}

	void Dance() override {}
};

#endif
