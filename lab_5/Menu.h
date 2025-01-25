#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <functional>

class Menu {
public:
    typedef std::function<void(std::istream& args)> Command;
    void AddItem(const std::string& shortcut, const std::string& description, const Command& command);

    void Run();

    void ShowInstructions()const;

    void Exit();

private:
    bool ExecuteCommand(const std::string& command);

    struct Item
    {
        Item(const std::string& shortcut, const std::string& description, const Command& command)
            : shortcut(shortcut)
            , description(description)
            , command(command)
        {}

        std::string shortcut;
        std::string description;
        Command command;
    };
    std::vector<Item> m_items;
    bool m_exit = false;
};


#endif //MENU_H