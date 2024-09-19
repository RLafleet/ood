#ifndef FLYWITHWINGS_H
#define FLYWITHWINGS_H

#include "IFlyBehavior.h"
#include <iostream>

class FlyWithWings final : public IFlyBehavior
{
public:
	void Fly() override
	{
		m_flyAmmount = GetNextFlightCount();
		std::cout << "I'm flying with wings!! It's my " << m_flyAmmount << " time." << std::endl;
	}

	int GetFlightCount() const override
	{
		return m_flyAmmount;
	}

	int GetNextFlightCount() const override
	{
		return m_flyAmmount + 1;
	}

private:
	int m_flyAmmount = 0;
};

#endif // FLYWITHWINGS_H