#ifndef HISTORY_H
#define HISTORY_H

#include "ICommand_fwd.h"
#include "ICommand.h"
#include <deque>

class History
{
public:
    History();
    ~History();

    History(const History&) = delete;
    History& operator=(const History&) = delete;

    History(History&&) noexcept = default;
    History& operator=(History&&) noexcept = default;

    [[nodiscard]] bool CanUndo()const;
    void Undo();
    [[nodiscard]] bool CanRedo()const;
    void Redo();
    void AddAndExecuteCommand(ICommandPtr&& command);
private:
    bool TryMergeCommand(ICommandPtr& command);
        
    std::deque<ICommandPtr> m_commands;
    size_t m_nextCommandIndex = 0;
};

#endif //HISTORY_H