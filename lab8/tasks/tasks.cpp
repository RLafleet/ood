#include <memory>
#include "CommandHandler.h"
#include "CMultiGumballMachine.h"

int main()
{
	auto machine = std::make_unique<multi_gumball_machine::CMultiGumballMachine>(0);
	CommandHandler handler(std::move(machine));
	handler.Run();

	auto machine1 = std::make_unique<multi_gumball_machine::CMultiGumballMachine>(1);
	multi_gumball_machine::IMultiGumballMachine& m = *machine;
	m.ReleaseBall();

	return 0;
}