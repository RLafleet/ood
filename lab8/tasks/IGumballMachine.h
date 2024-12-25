#ifndef IGUMBALLMACHINE_H
#define IGUMBALLMACHINE_H

namespace gumball_machine
{
	struct IGumballMachine
	{
		virtual void ReleaseBall() = 0;

		[[nodiscard]] virtual unsigned GetBallCount() const = 0;

		virtual void SetSoldOutState() = 0;

		virtual void SetNoQuarterState() = 0;

		virtual void SetSoldState() = 0;

		virtual void SetHasQuarterState() = 0;

		virtual ~IGumballMachine() = default;
	};
}

#endif //IGUMBALLMACHINE_H