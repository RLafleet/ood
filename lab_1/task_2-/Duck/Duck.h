#ifndef DUCK_H
#define DUCK_H

#include "Fly/IFlyBehavior.h"
#include "Dance/IDanceBehavior.h"
#include "Quack/IQuakBehavior.h"
#include "../IQuackSound.h"

#include <cassert>
#include <iostream>
#include <iostream>
#include <memory>
#include <vector>

class Duck
{
public:
	Duck(std::unique_ptr<IFlyBehavior>&& flyBehavior,
		std::unique_ptr<IDanceBehavior>&& danceBehavior,
		std::unique_ptr<IQuackBehavior>&& quackBehavior
	)
		: m_quackBehavior(std::move(quackBehavior))
	{
		assert(m_quackBehavior);
		SetFlyBehavior(std::move(flyBehavior));
		SetDanceBehavior(std::move(danceBehavior));
	}

	void Quack() const
	{
		m_quackBehavior->Quack();
	}

	void Swim()
	{
		std::cout << "I'm swimming" << std::endl;
	}

	void Fly()
	{
		OnFlightPerformed();
	}
		
	virtual void Dance()
	{
		m_danceBehavior->Dance();
	}

	void SetFlyBehavior(std::unique_ptr<IFlyBehavior>&& flyBehavior)
	{
		assert(flyBehavior);
		m_flyBehavior = std::move(flyBehavior);
	}

	void SetDanceBehavior(std::unique_ptr<IDanceBehavior>&& danceBehavior)
	{
		assert(danceBehavior);
		m_danceBehavior = std::move(danceBehavior);
	}

	IDanceBehavior* GetDanceBehavior() const {
		return m_danceBehavior.get();
	}

	IFlyBehavior* GetFlyBehavior() const
    {
        return m_flyBehavior.get();
    }

    IQuackBehavior* GetQuackBehavior() const
    {
        return m_quackBehavior.get();
    }

	bool AllowQuacking(const int flightAmount) const
	{
		return flightAmount % 2 == 0; // после четного
	}

	virtual void Display() const = 0;
	virtual ~Duck() = default;

private:
	std::unique_ptr<IFlyBehavior> m_flyBehavior;
	std::unique_ptr<IDanceBehavior> m_danceBehavior;
	std::unique_ptr<IQuackBehavior> m_quackBehavior;

	void OnFlightPerformed()
	{
		const auto nextFlightCount = m_flyBehavior->GetNextFlightCount();
		// Кряк перед полетом (если нужно)
		if (nextFlightCount != 0 && AllowQuacking(nextFlightCount))
		{
			std::cout << "I m happy ";
			m_quackBehavior->Quack();
		}

		m_flyBehavior->Fly();

		// Кряк после полета (если нужно)
		if (nextFlightCount != 0 && AllowQuacking(nextFlightCount))
		{
			std::cout << "I m happy ";
			m_quackBehavior->Quack();
		}
	}
};

#endif
