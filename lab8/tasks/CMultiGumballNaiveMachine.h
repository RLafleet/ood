#ifndef LAB8_GUMNAIVEBALLMACHINE_H
#define LAB8_GUMNAIVEBALLMACHINE_H

#pragma once

#include <format>
#include <iostream>

namespace multiNaive {
    class GumballMachine {
    public:
        enum class State {
            SoldOut,
            NoQuarter,
            HasQuarter,
            Sold,
        };

        GumballMachine(unsigned count)
            : m_count(count), m_state(count > 0 ? State::NoQuarter : State::SoldOut)
        {}


        void InsertQuarter() {
            using namespace std;
            if (m_quarter < MAX_QUARTER) 
            {
                switch (m_state) {
                case State::SoldOut:
                    cout << "You can't insert a quarter, the machine is sold out\n";
                    break;
                case State::NoQuarter:
                    cout << "You inserted a quarter\n";
                    m_quarter++;
                    if (m_quarter == 1)
                    {
                        m_state = State::HasQuarter;
                    }
                    break;
                case State::HasQuarter:
                    cout << "You inserted another quarter\n";
                    m_quarter++;
                    break;
                case State::Sold:
                    cout << "Please wait, we're already giving you a gumball\n";
                    break;
                }
            }
            else {
                cout << "You can't insert another quarter\n";
            }
        }

        void EjectQuarter() {
            using namespace std;
            switch (m_state) {
            case State::SoldOut:
                if (m_quarter > 0)
                {
                    cout << "Return all quarters\n";
                    m_quarter = 0;
                }
                else
                {
                    cout << "You can't eject, you haven't inserted a quarter yet\n";
                }
                break;
            case State::NoQuarter:
                cout << "You can't eject, you haven't inserted a quarter yet\n";
                break;
            case State::HasQuarter:
                cout << "Return all quarters\n";
                m_quarter = 0;
                m_state = State::NoQuarter;
                break;
            case State::Sold:
                std::cout << "Sorry you already turned the crank\n";
                break;
            }
        }

        void TurnCrank() {
            using namespace std;
            switch (m_state) {
            case State::SoldOut:
                cout << "You turned but there's no gumballs\n";
                break;
            case State::NoQuarter:
                cout << "You turned but there's no quarter\n";
                break;
            case State::HasQuarter:
                cout << "You turned...\n";
                Dispense();
                break;
            case State::Sold:
                cout << "Turning twice doesn't get you another gumball\n";
                break;
            }
        }

        void Refill(unsigned numBalls)
        {
            using namespace std;
            switch (m_state) {
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
                m_count = numBalls;
                std::cout << "Added gumball\n";
                break;
            case State::HasQuarter:
                m_count = numBalls;
                std::cout << "Added gumball\n";
                break;
            case State::Sold:
                std::cout << "Not added gumball when giving giving you a gumball\n";
                break;
            }

        }

        [[nodiscard]] std::string Info() const {
            std::string state = (m_state == State::SoldOut)
                ? "sold out"
                : (m_state == State::NoQuarter) ? "waiting for quarter"
                : (m_state == State::HasQuarter) ? "waiting for turn of crank"
                : "delivering a gumball";

       return std::format(R"(
------------------------------
Mighty Gumball, Inc.
C++-enabled Standing Gumball Model #2016 (with state)
Machine number #206
------------------------------
Inventory: {} gumball{}
Machine is {}
------------------------------
)", m_count, m_count != 1 ? "s" : "", state);
                        
        }

    private:
        void Dispense() {
            using namespace std;
            if (m_count > 0 && m_quarter > 0)
            {
                --m_count;
                --m_quarter;

                cout << "Your gumball...\n";

                if (m_count == 0) {
                    cout << "Oops, out of gumballs\n";
                    m_state = State::SoldOut;
                }
                else if (m_quarter == 0)
                {
                    m_state = State::NoQuarter;
                }
            }
            else if (m_count == 0)
            {
                cout << "No gumball dispensed\n";
            }
            else if (m_quarter == 0)
            {
                cout << "You need to pay first\n";
            }
        }

        unsigned m_count;
        unsigned m_quarter = 0;
        static constexpr unsigned MAX_QUARTER = 5;
        State m_state = State::SoldOut;
    };
}

#endif //LAB8_GUMNAIVEBALLMACHINE_H