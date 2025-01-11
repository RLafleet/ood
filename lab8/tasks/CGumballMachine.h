#ifndef CGUMBALLMACHINE_H
#define CGUMBALLMACHINE_H

#include <format>
#include "IGumballMachine.h"
#include "CHasQuarterState.h"
#include "CNoQuarterState.h"
#include "CSoldOutState.h"
#include "CSoldState.h"

namespace gumball_machine
{
	class CGumballMachine final : IGumballMachine
	{
	public:
		explicit CGumballMachine(const unsigned numBalls)
			: m_count(numBalls)
			, m_soldState(*this)
			, m_soldOutState(*this)
			, m_noQuarterState(*this)
			, m_hasQuarterState(*this)
			, m_state(&m_soldOutState)
		{
			if (m_count > 0)
			{
				m_state = &m_noQuarterState;
			}
		}

		void EjectQuarter() const
		{
			m_state->EjectQuarter();
		}

		void InsertQuarter() const
		{
			m_state->InsertQuarter();
		}

		void TurnCrank() const
		{
			m_state->TurnCrank();
			m_state->Dispense();
		}

		[[nodiscard]] std::string Info() const
		{
			return std::format(R"(
------------------------------
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016 (with state)
Machine number #206
------------------------------
Inventory: {} gumball{}
Machine is {}
------------------------------
)", m_count, m_count != 1 ? "s" : "", m_state->Info());
		}

	private:
		[[nodiscard]] unsigned GetBallCount() const override
		{
			return m_count;
		}

		void ReleaseBall() override
		{
			if (m_count != 0)
			{
				std::cout << "A gumball comes rolling out the slot...\n";
				--m_count;
			}
		}

		void SetSoldOutState() override
		{
			m_state = &m_soldOutState;
		}

		void SetNoQuarterState() override
		{
			m_state = &m_noQuarterState;
		}

		void SetSoldState() override
		{
			m_state = &m_soldState;
		}

		void SetHasQuarterState() override
		{
			m_state = &m_hasQuarterState;
		}

		unsigned m_count = 0;
		CSoldState m_soldState;
		CSoldOutState m_soldOutState;
		CNoQuarterState m_noQuarterState;
		CHasQuarterState m_hasQuarterState;
		IState* m_state;
	};
}

#endif //CGUMBALLMACHINE_H