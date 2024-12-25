#ifndef CMULTIGUMBALLMACHINE_H
#define CMULTIGUMBALLMACHINE_H

#include <format>
#include "IMultiGumballMachine.h"
#include "CHasQuarterStateMulti.h"
#include "CNoQuarterStateMulti.h"
#include "CSoldOutStateMulti.h"
#include "CSoldStateMulti.h"

namespace multi_gumball_machine
{
	class CMultiGumballMachine final : IMultiGumballMachine
	{
	public:
		explicit CMultiGumballMachine(const unsigned numBalls)
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

		[[nodiscard]] std::string ToString() const
		{
			return std::format(R"(
				Mighty Gumball, Inc.
				C++-enabled Standing Gumball Model #2016 (with state)
				Inventory: {} gumball{}
				Machine is {}
			)", m_count, m_count != 1 ? "s" : "", m_state->ToString());
		}

		void Refill(const unsigned numBalls) const
		{
			m_state->Refill(numBalls);
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
				--m_quarter;
			}
		}

		[[nodiscard]] unsigned GetQuarterCount() const override
		{
			return m_quarter;
		}

		[[nodiscard]] unsigned GetMaxQuarterCount() const override
		{
			return MAX_QUARTER;
		}

		void AddQuarter() override
		{
			++m_quarter;
		}

		void RefillBall(const unsigned numBalls) override
		{
			m_count = numBalls;
		}

		void ReturnAllQuarters() override
		{
			std::cout << "Quarter returned\n";

			m_quarter = 0;
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

		static constexpr unsigned MAX_QUARTER = 5;
		unsigned m_count = 0;
		unsigned m_quarter = 0;
		CSoldState m_soldState{ *this };
		CSoldOutState m_soldOutState{ *this };
		CNoQuarterState m_noQuarterState{ *this };
		CHasQuarterState m_hasQuarterState{ *this };
		IState* m_state{};
	};
}

#endif //CMULTIGUMBALLMACHINE_H