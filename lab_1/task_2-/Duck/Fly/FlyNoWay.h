#ifndef FLYNOWAY_H
#define FLYNOWAY_H

#include "IFlyBehavior.h"

class FlyNoWay final : public IFlyBehavior
{
public:
	void Fly() override
	{
	}

	int GetFlightCount() const override
	{
		return 0;
	}

	int GetNextFlightCount() const override
	{
		return 0;
	}
};

#endif // FLYNOWAY_H