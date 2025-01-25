#include "DeleteCommand.h"

void DeleteItemCommand::DoExecute()
{
    if (m_documentItems.empty())
    {
        return;
    }

    if (m_position >= m_documentItems.size())
    {
        throw std::invalid_argument("Out of range by position");
    }

    m_oldDocumentItem = std::make_unique<DocumentItem>(m_documentItems.at(m_position));

    m_documentItems.erase(m_documentItems.begin() + static_cast<std::vector<DocumentItem>::iterator::difference_type>(m_position));
}

void DeleteItemCommand::DoUnexecute()
{
    if (m_oldDocumentItem == nullptr)
    {
        return;
    }

    m_documentItems.insert(
        m_documentItems.begin() + static_cast<std::vector<DocumentItem>::iterator::difference_type>(m_position),
        *m_oldDocumentItem);

    m_oldDocumentItem = nullptr;
}