#include <cassert>
#include "History.h"
#include "ReplaceTextCommand.h"
#include "SetTitleCommand.h"
#include "ResizeImageCommand.h"

History::History()
= default;

History::~History()
= default;

bool History::CanUndo() const
{
    return m_nextCommandIndex != 0;
}

bool History::CanRedo() const
{
    return m_nextCommandIndex != m_commands.size();
}

void History::Undo()
{
    if (CanUndo())
    {
        m_commands[m_nextCommandIndex - 1]->Unexecute();
        --m_nextCommandIndex;
    }
}

void History::Redo()
{
    if (CanRedo())
    {
        m_commands[m_nextCommandIndex]->Execute();
        ++m_nextCommandIndex;
    }
}

//зависимость убрать
bool History::TryMergeCommand(ICommandPtr& command)
{
    if (m_nextCommandIndex > 0)
    {
        auto& lastCommand = m_commands[m_nextCommandIndex - 1];

        if (auto lastReplaceCommand = dynamic_cast<ReplaceTextCommand*>(lastCommand.get()))
        {
            if (auto newReplaceCommand = dynamic_cast<ReplaceTextCommand*>(command.get()))
            {
                if (lastReplaceCommand->IsSameParagraph(newReplaceCommand))
                {
                    lastReplaceCommand->Merge(newReplaceCommand);
                    return true;
                }
            }
        }

        if (auto lastResizeCommand = dynamic_cast<ResizeImageCommand*>(lastCommand.get()))
        {
            if (auto newResizeCommand = dynamic_cast<ResizeImageCommand*>(command.get()))
            {
                if (lastResizeCommand->IsSameImage(newResizeCommand))
                {
                    lastResizeCommand->Merge(newResizeCommand);
                    return true;
                }
            }
        }

        if (auto lastSetTitleCommand = dynamic_cast<SetTitleCommand*>(lastCommand.get()))
        {
            if (auto newSetTitleCommand = dynamic_cast<SetTitleCommand*>(command.get()))
            {
                if (lastSetTitleCommand->IsSameTitle(newSetTitleCommand))
                {
                    lastSetTitleCommand->Merge(newSetTitleCommand);
                    return true;
                }
            }
        }
    }
    return false;
}

void History::AddAndExecuteCommand(ICommandPtr&& command)
{
    if (TryMergeCommand(command))
    {
        return; 
    }

    if (m_nextCommandIndex < m_commands.size())
    {
        command->Execute();
        ++m_nextCommandIndex;
        m_commands.resize(m_nextCommandIndex);
        m_commands.back() = std::move(command);
    }
    else
    {
        assert(m_nextCommandIndex == m_commands.size());
        m_commands.emplace_back(nullptr);

        try
        {
            command->Execute();
            m_commands.back() = std::move(command);
            ++m_nextCommandIndex;
        }
        catch (...)
        {
            m_commands.pop_back();
            throw;
        }

    }
}