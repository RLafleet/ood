﻿#ifndef CMULTIGUMBALLMACHINE_H
#define CMULTIGUMBALLMACHINE_H

#include <format>
#include "IMultiGumballMachine.h"
#include "CHasQuarterStateMulti.h"
#include "CNoQuarterStateMulti.h"
#include "CSoldOutStateMulti.h"
#include "CSoldStateMulti.h"

namespace multi_gumball_machine
{
	// Чем различаются наследования в c++
	// как сделать так, чтобы наследование могло быть приватным в языках не имеющих private
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

		void Refill(const unsigned numBalls) const
		{
			if (numBalls > 0)
			{
				m_state->Refill(numBalls);
			}
			else
			{
				std::cout << "Error: Number of balls must be greater than 0." << std::endl;
			}
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
				std::cout << "Your gumball\n";
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
			// refill 0, state машины другой и тестами покрыть
			m_count = numBalls;
		}

		void ReturnAllQuarters() override
		{
			std::cout << m_quarter << " quarter returned to you\n";

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