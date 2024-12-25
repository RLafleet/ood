#ifndef CMULTIGUMBALLMACHINENAIVE_H
#define CMULTIGUMBALLMACHINENAIVE_H

#include <format>
#include <iostream>

namespace multi_gumball_naive_machine
{
	class CMultiGumballMachine
	{
	public:
		enum class State
		{
			SoldOut,
			NoQuarter,
			HasQuarter,
			Sold,
		};

		explicit CMultiGumballMachine(const unsigned count)
			: m_count(count),
			m_state(count > 0 ? State::NoQuarter : State::SoldOut)
		{
		}

		void InsertQuarter()
		{
			if (m_quarter < MAX_QUARTER)
			{
				switch (m_state)
				{
				case State::SoldOut:
					std::cout << "You can't insert a quarter, the machine is sold out\n";
					break;
				case State::NoQuarter:
					std::cout << "You inserted a quarter\n";
					m_quarter++;
					m_state = State::HasQuarter;
					break;
				case State::HasQuarter:
					std::cout << "You inserted another quarter\n";
					m_quarter++;
					break;
				case State::Sold:
					std::cout << "Please wait, we're already giving you a gumball\n";
					break;
				}
			}
			else
			{
				std::cout << "You can't insert another quarter\n";
			}
		}

		void EjectQuarter()
		{
			switch (m_state)
			{
			case State::SoldOut:
				if (m_quarter > 0)
				{
					std::cout << "Return all quarters\n";
					m_quarter = 0;
				}
				else
				{
					std::cout << "You can't eject, you haven't inserted a quarter yet\n";
				}
				break;
			case State::NoQuarter:
				std::cout << "You can't eject, you haven't inserted a quarter yet\n";
				break;
			case State::HasQuarter:
				std::cout << "Return all quarters\n";
				m_quarter = 0;
				m_state = State::NoQuarter;
				break;
			case State::Sold:
				std::cout << "Sorry you already turned the crank\n";
				break;
			}
		}

		void TurnCrank()
		{
			switch (m_state)
			{
			case State::SoldOut:
				std::cout << "You turned but there's no gumballs\n";
				break;
			case State::NoQuarter:
				std::cout << "You turned but there's no quarter\n";
				break;
			case State::HasQuarter:
				std::cout << "You turned...\n";
				Dispense();
				break;
			case State::Sold:
				std::cout << "Turning twice doesn't get you another gumball\n";
				break;
			}
		}

		void Refill(const unsigned numBalls)
		{
			switch (m_state)
			{
			case State::SoldOut:
				m_count = numBalls;
				std::cout << "Added gumball\n";
				if (m_count > 0)
				{
					if (m_quarter > 0)
					{
						m_state = State::HasQuarter;
					}
					else
					{
						m_state = State::NoQuarter;
					}
				}
				break;
			case State::NoQuarter:
			case State::HasQuarter:
				m_count = numBalls;
				std::cout << "Added gumball\n";
				break;
			case State::Sold:
				std::cout << "Not added gumball when giving giving you a gumball\n";
				break;
			}
		}

		[[nodiscard]] std::string ToString() const
		{
			std::string state = (m_state == State::SoldOut)
				? "sold out"
				: (m_state == State::NoQuarter)
				? "waiting for quarter"
				: (m_state == State::HasQuarter)
				? "waiting for turn of crank"
				: "delivering a gumball";

			return std::format(R"(
				Mighty Gumball, Inc.
				C++-enabled Standing Gumball Model #2016
				Inventory: {} gumball{}
				Machine is {}
			)", m_count, m_count != 1 ? "s" : "", state);
		}

	private:
		void Dispense()
		{
			if (m_count > 0 && m_quarter > 0)
			{
				--m_count;
				--m_quarter;

				std::cout << "A gumball comes rolling out the slot...\n";

				if (m_count == 0)
				{
					std::cout << "Oops, out of gumballs\n";
					m_state = State::SoldOut;
				}
				else if (m_quarter == 0)
				{
					m_state = State::NoQuarter;
				}
			}
			else if (m_count == 0)
			{
				std::cout << "No gumball dispensed\n";
			}
			else if (m_quarter == 0)
			{
				std::cout << "You need to pay first\n";
			}
		}

		unsigned m_count;
		unsigned m_quarter = 0;
		static constexpr unsigned MAX_QUARTER = 5;
		State m_state = State::SoldOut;
	};
}

#endif //CMULTIGUMBALLMACHINENAIVE_H