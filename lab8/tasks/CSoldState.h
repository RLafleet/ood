#ifndef CSOLDSTATE_H
#define CSOLDSTATE_H

#include <iostream>
#include "IState.h"
#include "IGumballMachine.h"

namespace gumball_machine
{
	class CSoldState final : public IState
	{
	public:
		explicit CSoldState(IGumballMachine& gumballMachine)
			: m_gumballMachine(gumballMachine)
		{
		}

		void InsertQuarter() override
		{
			std::cout << "Please wait, we're already giving you a gumball\n";
		}

		void EjectQuarter() override
		{
			std::cout << "Sorry you already turned the crank\n";
		}

		void TurnCrank() override
		{
			std::cout << "Turning twice doesn't get you another gumball\n";
		}

		void Dispense() override
		{
			m_gumballMachine.ReleaseBall();
			if (m_gumballMachine.GetBallCount() == 0)
			{
				std::cout << "Oops, out of gumballs\n";
				m_gumballMachine.SetSoldOutState();
			}
			else
			{
				m_gumballMachine.SetNoQuarterState();
			}
		}

		[[nodiscard]] std::string ToString() const override
		{
			return "delivering a gumball";
		}

	private:
		IGumballMachine& m_gumballMachine;
	};
}
#endif //CSOLDSTATE_H