#ifndef DUCK_H
#define DUCK_H

#include "Fly/IFlyBehavior.h"
#include "Dance/IDanceBehavior.h"
#include "Quack/IQuakBehavior.h"

#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

class Duck
{
public:
	Duck(std::unique_ptr<IFlyBehavior>&& flyBehavior,
		std::unique_ptr<IDanceBehavior>&& danceBehavior,
		std::unique_ptr<IQuackBehavior>&& quackBehavior)
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
		m_flyBehavior->Fly();
	}
		

	// зачем virtual
	virtual void Dance()
	{
		m_danceBehavior->Dance();
	}

	void SetFlyBehavior(std::unique_ptr<IFlyBehavior>&& flyBehavior)
	{
		assert(flyBehavior);
		m_flyBehavior = std::move(flyBehavior);
	}

	// так неправильно
	// зачем вводим интерфейсы
	// уметь объяснить в каком интерфейсе можем делать константным, а в каком нет. И кто принимает решения о том, какие методы интерфейса должны быть констатными, а какие нет
	void SetDanceBehavior(std::unique_ptr<IDanceBehavior>&& danceBehavior)
	{
		assert(danceBehavior);
		m_danceBehavior = std::move(danceBehavior);
	}

	IDanceBehavior* GetDanceBehavior() const {
		return m_danceBehavior.get();
	}

	virtual void Display() const = 0;
	virtual ~Duck() = default;

private:
	std::unique_ptr<IFlyBehavior> m_flyBehavior;
	std::unique_ptr<IDanceBehavior> m_danceBehavior;
	std::unique_ptr<IQuackBehavior> m_quackBehavior;
};

#endif
