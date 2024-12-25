#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H
#include <memory>

#include "CMenu.h"
#include "CMultiGumballMachine.h"

template<typename GumballMachineType>
class CommandHandler
{
public:
	explicit CommandHandler(std::unique_ptr<GumballMachineType>&& gumballMachine) : m_multiGumballMachine(
		std::move(gumballMachine))
	{
		m_menu.AddItem(
			"EjectQuarter",
			"Usage: EjectQuarter",
			[&](std::istringstream& params) { m_multiGumballMachine->EjectQuarter(); }
		);

		m_menu.AddItem(
			"InsertQuarter",
			"Usage: InsertQuarter",
			[&](std::istringstream& params) { m_multiGumballMachine->InsertQuarter(); }
		);

		m_menu.AddItem(
			"TurnCrank",
			"Usage: TurnCrank",
			[&](std::istringstream& params) { m_multiGumballMachine->TurnCrank(); }
		);

		m_menu.AddItem(
			"ToString",
			"Usage: ToString",
			[&](std::istringstream& params) {
				std::cout << m_multiGumballMachine->ToString() << std::endl;
			}
		);

		m_menu.AddItem(
			"Refill",
			"Usage: Refill <balls count>",
			[&](std::istringstream& params) {
				unsigned balls;
				if (!(params >> balls))
				{
					std::cout << "Usage: Refill <balls count>" << std::endl;
					return;
				}
				m_multiGumballMachine->Refill(balls);
			}
		);

		m_menu.AddItem(
			"Help",
			"Usage: Help. Shows the available commands.",
			[&](std::istringstream& params) { Help(); }
		);

		m_menu.AddItem(
			"Exit",
			"Usage: Exit. Exits the program.",
			[&](std::istringstream& params) { Exit(); }
		);
	}

	void Help()
	{
		m_menu.ShowInstructions();
	}

	void Exit()
	{
		m_menu.Exit();
	}

	void Run()
	{
		m_menu.Run();
	}

private:
	CMenu m_menu{};
	std::unique_ptr<GumballMachineType> m_multiGumballMachine;
};

#endif //COMMANDHANDLER_H