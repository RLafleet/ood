#ifndef MALLARDDUCK_H
#define MALLARDDUCK_H

#include "Duck.h"
#include "Fly/FlyWithWings.h"
#include "Dance/DanceWaltzBehavior.h"
#include "Quack/QuackBehavior.h"
#include "../EvenFlyQuackSound.h"


class MallardDuck : public Duck
{
public:
	MallardDuck()
		: Duck(
			std::make_unique<FlyWithWings>(), 
			std::make_unique<WaltzDance>(),
			std::make_unique<QuackBehavior>(),
			std::make_unique<EvenFlightsQuacking>()
			)
	{
	}

	void Display() const override
	{
		std::cout << "I'm mallard duck" << std::endl;
	}
};

#endif