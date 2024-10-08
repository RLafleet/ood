#ifndef REDHEADDUCK_H
#define REDHEADDUCK_H

#include "Duck.h"
#include "Fly/FlyWithWings.h"
#include "Dance/DanceMinuetBehavior.h"
#include "Quack/QuackBehavior.h"
#include "../OddFlyQuackSound.h"

#include <memory>

class RedheadDuck : public Duck
{
public:
	RedheadDuck()
		: Duck(
			std::make_unique<FlyWithWings>(), 
			std::make_unique<MinuetDance>(),
			std::make_unique<QuackBehavior>()
		)
	{
	}

	void Display() const override
	{
		std::cout << "I'm redhead duck" << std::endl;
	}
};

#endif
