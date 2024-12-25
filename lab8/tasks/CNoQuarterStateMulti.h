#ifndef CNOQUARTERSTATEMULTI_H
#define CNOQUARTERSTATEMULTI_H

#include <iostream>
#include "IStateMulti.h"
#include "IMultiGumballMachine.h"

namespace multi_gumball_machine
{
	class CNoQuarterState final : public IState
	{
	public:
		explicit CNoQuarterState(IMultiGumballMachine& gumballMachine)
			: m_gumballMachine(gumballMachine)
		{
		}

		void InsertQuarter() override
		{
			std::cout << "You inserted a quarter\n";
			m_gumballMachine.AddQuarter();
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

		void Refill(const unsigned numBalls) override
		{
			std::cout << "Added gumball\n";
			m_gumballMachine.RefillBall(numBalls);
		}

		[[nodiscard]] std::string ToString() const override
		{
			return "waiting for quarter";
		}

	private:
		IMultiGumballMachine& m_gumballMachine;
	};
}

#endif //CNOQUARTERSTATEMULTI_H