#ifndef SETTITLECOMMAND_H
#define SETTITLECOMMAND_H

#include <iostream>
#include <utility>
#include "AbstractCommand.h"

class SetTitleCommand : public AbstractCommand
{
public:
    static constexpr const char* name = "SetTitle";

    SetTitleCommand(std::string& title, const std::string& newTitle) : m_title(title), m_newTitle(newTitle) {}

    bool IsSameTitle(const SetTitleCommand* other) const
    {
        return m_newTitle == other->m_newTitle; 
    }

    void Merge(const SetTitleCommand* other)
    {
        m_newTitle = other->m_newTitle; 
    }

protected:
    void DoExecute() override;
    void DoUnexecute() override;

private:
    std::string& m_title;
    std::string m_newTitle;
};


#endif //SETTITLECOMMAND_H