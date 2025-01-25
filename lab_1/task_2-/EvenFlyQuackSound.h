#ifndef QUACKSOUNDEVENFLIGNT_H
#define QUACKSOUNDEVENFLIGNT_H
#include "IQuackSound.h"

class EvenFlightsQuacking final : public IQuackSound
{
public:
	bool AllowQuacking(const int flightAmount) const override
	{
		return flightAmount % 2 == 0;
	}
};

#endif //QUACKSOUNDEVENFLIGNT_H