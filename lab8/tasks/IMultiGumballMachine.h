#ifndef IMULTIGUMBALLMACHINE_H
#define IMULTIGUMBALLMACHINE_H

namespace multi_gumball_machine
{
	struct IMultiGumballMachine
	{
		virtual void ReleaseBall() = 0;

		[[nodiscard]] virtual unsigned GetBallCount() const = 0;

		[[nodiscard]] virtual unsigned GetQuarterCount() const = 0;

		[[nodiscard]] virtual unsigned GetMaxQuarterCount() const = 0;

		virtual void AddQuarter() = 0;

		virtual void RefillBall(unsigned numBalls) = 0;

		virtual void ReturnAllQuarters() = 0;

		virtual void SetSoldOutState() = 0;

		virtual void SetNoQuarterState() = 0;

		virtual void SetSoldState() = 0;

		virtual void SetHasQuarterState() = 0;

		virtual ~IMultiGumballMachine() = default;
	};
}

#endif //IMULTIGUMBALLMACHINE_H