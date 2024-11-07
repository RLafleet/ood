#include "Menu.h"
#include <sstream>

void Menu::AddItem(const std::string& shortcut, const std::string& description, const Command& command)
{
    m_items.emplace_back(shortcut, description, command);
}

void Menu::Run()
{
    ShowInstructions();

    std::string command;
    while ((std::cout << ">")
        && getline(std::cin, command)
        && !command.empty()
        && ExecuteCommand(command))
    {
    }
}

void Menu::ShowInstructions() const
{
    std::cout << "Commands list:\n";
    for (auto& item : m_items)
    {
        std::cout << "  " << item.shortcut << ": " << item.description << "\n";
    }
}

void Menu::Exit()
{
    m_exit = true;
}

bool Menu::ExecuteCommand(const std::string& command)
{
    std::istringstream iss(command);
    std::string name;
    iss >> name;

    m_exit = false;
    auto it = std::find_if(m_items.begin(), m_items.end(), [&](const Item& item) {
        return item.shortcut == name;
        });
    if (it != m_items.end())
    {
        it->command(iss);
    }
    else
    {
        std::cout << "Unknown command\n";
    }
    return !m_exit;
}