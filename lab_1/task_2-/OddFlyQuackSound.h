#ifndef QUACKSOUNDODDFLIGNT_H
#define QUACKSOUNDODDFLIGNT_H
#include "IQuackSound.h"

class OddFlightsQuacking final : public IQuackSound
{
public:
	bool AllowQuacking(const int flightAmount) const override
	{
		return flightAmount % 2 != 0;
	}
};

#endif //QUACKSOUNDODDFLIGNT_H