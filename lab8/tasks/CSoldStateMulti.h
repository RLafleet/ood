#ifndef CSOLDSTATEMULTI_H
#define CSOLDSTATEMULTI_H

#include <iostream>
#include "IStateMulti.h"
#include "IMultiGumballMachine.h"

namespace multi_gumball_machine
{
	class CSoldState final : public IState
	{
	public:
		explicit CSoldState(IMultiGumballMachine& gumballMachine)
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
			else if (m_gumballMachine.GetQuarterCount() > 0)
			{
				m_gumballMachine.SetHasQuarterState();
			}
			else
			{
				m_gumballMachine.SetNoQuarterState();
			}
		}

		void Refill(unsigned numBalls) override
		{
			std::cout << "Cannot refill machine when giving you a gumball\n";
		}

		[[nodiscard]] std::string Info() const override
		{
			return "delivering a gumball";
		}

	private:
		IMultiGumballMachine& m_gumballMachine;
	};
}

#endif //CSOLDSTATEMULTI_H