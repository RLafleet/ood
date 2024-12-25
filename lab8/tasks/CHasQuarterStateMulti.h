#ifndef CHASQUARTERSTATEMULTI_H
#define CHASQUARTERSTATEMULTI_H

#include <iostream>
#include "IStateMulti.h"
#include "IMultiGumballMachine.h"

namespace multi_gumball_machine
{
	class CHasQuarterState final : public IState
	{
	public:
		explicit CHasQuarterState(IMultiGumballMachine& gumballMachine)
			: m_gumballMachine(gumballMachine)
		{
		}

		void InsertQuarter() override
		{
			if (m_gumballMachine.GetQuarterCount() < m_gumballMachine.GetMaxQuarterCount())
			{
				m_gumballMachine.AddQuarter();
				std::cout << "You inserted another quarter\n";
			}
			else
			{
				std::cout << "You can't insert another quarter\n";
			}
		}

		void EjectQuarter() override
		{
			m_gumballMachine.ReturnAllQuarters();
			m_gumballMachine.SetNoQuarterState();
		}

		void TurnCrank() override
		{
			if (m_gumballMachine.GetQuarterCount() > 0)
			{
				std::cout << "You turned...\n";
				m_gumballMachine.SetSoldState();
			}
			else
			{
				std::cout << "No quarters left.\n";
				m_gumballMachine.SetNoQuarterState();
			}
		}

		void Dispense() override
		{
			std::cout << "No gumball dispensed\n";
		}

		void Refill(const unsigned numBalls) override
		{
			std::cout << "Added gumball\n";
			m_gumballMachine.RefillBall(numBalls);
		}

		[[nodiscard]] std::string ToString() const override
		{
			return "waiting for turn of crank";
		}

	private:
		IMultiGumballMachine& m_gumballMachine;
	};
}

#endif //CHASQUARTERSTATEMULTI_H