#ifndef CSOLDOUTSTATEMULTI_H
#define CSOLDOUTSTATEMULTI_H

#include <iostream>
#include "IStateMulti.h"
#include "IMultiGumballMachine.h"

namespace multi_gumball_machine
{
	class CSoldOutState final : public IState
	{
	public:
		explicit CSoldOutState(IMultiGumballMachine& gumballMachine)
			: m_gumballMachine(gumballMachine)
		{
		}

		void InsertQuarter() override
		{
			std::cout << "You can't insert a quarter, the machine is sold out\n";
		}

		void EjectQuarter() override
		{
			if (m_gumballMachine.GetQuarterCount() > 0)
			{
				m_gumballMachine.ReturnAllQuarters();
			}
			else
			{
				std::cout << "You can't eject, you haven't inserted a quarter yet\n";
			}
		}

		void TurnCrank() override
		{
			std::cout << "You turned but there's no gumballs\n";
		}

		void Dispense() override
		{
			std::cout << "No gumball dispensed\n";
		}

		void Refill(const unsigned numBalls) override
		{
			std::cout << "Added gumball\n";
			m_gumballMachine.RefillBall(numBalls);
			if (m_gumballMachine.GetBallCount() > 0)
			{
				if (m_gumballMachine.GetQuarterCount() > 0)
				{
					m_gumballMachine.SetHasQuarterState();
				}
				else
				{
					m_gumballMachine.SetNoQuarterState();
				}
			}
		}

		[[nodiscard]] std::string Info() const override
		{
			return "sold out";
		}

	private:
		IMultiGumballMachine& m_gumballMachine;
	};
}

#endif //CSOLDOUTSTATE_H