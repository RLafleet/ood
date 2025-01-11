#ifndef CNOQUARTERSTATE_H
#define CNOQUARTERSTATE_H

#include <iostream>
#include "IState.h"
#include "IGumballMachine.h"

namespace gumball_machine
{
	class CNoQuarterState final : public IState
	{
	public:
		explicit CNoQuarterState(IGumballMachine& gumballMachine)
			: m_gumballMachine(gumballMachine)
		{
		}

		void InsertQuarter() override
		{
			std::cout << "You inserted a quarter\n";
			m_gumballMachine.SetHasQuarterState();
		}

		void EjectQuarter() override
		{
			std::cout << "You haven't inserted a quarter\n";
		}

		void TurnCrank() override
		{
			std::cout << "You turned but there's no quarter\n";
		}

		void Dispense() override
		{
			std::cout << "You need to pay first\n";
		}

		[[nodiscard]] std::string Info() const override
		{
			return "waiting for quarter";
		}

	private:
		IGumballMachine& m_gumballMachine;
	};
}
#endif //CNOQUARTERSTATE_H