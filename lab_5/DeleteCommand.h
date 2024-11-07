#ifndef DELETEITEMCOMMAND_H
#define DELETEITEMCOMMAND_H

#include <iostream>
#include <optional>
#include <vector>
#include "AbstractCommand.h"
#include "DocumentItem.h"

class DeleteItemCommand : public AbstractCommand
{
public:
    constexpr static const char* name = "DeleteItem";

    DeleteItemCommand(std::vector<DocumentItem>& documentItems, size_t position)
        : m_documentItems(documentItems), m_position(position)
    {}

protected:
    void DoExecute() override;
    void DoUnexecute() override;

private:
    std::vector<DocumentItem>& m_documentItems;
    size_t m_position;
    std::unique_ptr<DocumentItem> m_oldDocumentItem = nullptr;
};

#endif //DELETEITEMCOMMAND_H