#ifndef ISTATE_H
#define ISTATE_H
#include <string>

namespace gumball_machine
{
	struct IState
	{
		virtual void InsertQuarter() = 0;

		virtual void EjectQuarter() = 0;

		virtual void TurnCrank() = 0;

		virtual void Dispense() = 0;

		[[nodiscard]] virtual std::string ToString() const = 0;

		virtual ~IState() = default;
	};
}

#endif //ISTATE_H