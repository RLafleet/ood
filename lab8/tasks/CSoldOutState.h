#ifndef CSOLDOUTSTATE_H
#define CSOLDOUTSTATE_H

#include <iostream>
#include "IState.h"
#include "IGumballMachine.h"

namespace gumball_machine
{
	class CSoldOutState final : public IState
	{
	public:
		explicit CSoldOutState(IGumballMachine& gumballMachine)
			: m_gumballMachine(gumballMachine)
		{
		}

		void InsertQuarter() override
		{
			std::cout << "You can't insert a quarter, the machine is sold out\n";
		}

		void EjectQuarter() override
		{
			std::cout << "You can't eject, you haven't inserted a quarter yet\n";
		}

		void TurnCrank() override
		{
			std::cout << "You turned but there's no gumballs\n";
		}

		void Dispense() override
		{
			std::cout << "No gumball dispensed\n";
		}

		[[nodiscard]] std::string ToString() const override
		{
			return "sold out";
		}

	private:
		IGumballMachine& m_gumballMachine;
	};
}

#endif //CSOLDOUTSTATE_H