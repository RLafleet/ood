#ifndef ISTATEMULTI_H
#define ISTATEMULTI_H

#include <string>

namespace multi_gumball_machine
{
	struct IState
	{
		virtual void InsertQuarter() = 0;

		virtual void EjectQuarter() = 0;

		virtual void TurnCrank() = 0;

		virtual void Dispense() = 0;

		virtual void Refill(unsigned numBalls) = 0;

		[[nodiscard]] virtual std::string Info() const = 0;

		virtual ~IState() = default;
	};
}

#endif //ISTATEMULTI_H