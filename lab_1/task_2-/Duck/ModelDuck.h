#ifndef MODELDUCK_H
#define MODELDUCK_H

#include "Duck.h"
#include "Fly/FlyNoWay.h"
#include "Dance/DanceNoWay.h"
#include "Quack/QuackBehavior.h"
#include "../OddFlyQuackSound.h"

class ModelDuck : public Duck
{
public:
	ModelDuck()
		: Duck(
			std::make_unique<FlyNoWay>(), 
			std::make_unique<DanceNoWay>(), 
			std::make_unique<QuackBehavior>()
		)
	{
	}

	void Display() const override
	{
		std::cout << "I'm model duck" << std::endl;
	}

	void Dance() override {}
};

#endif
