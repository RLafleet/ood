#include "InsertParagraphCommand.h"

void InsertParagraphCommand::DoExecute()
{
    auto paragraph = std::make_shared<Paragraph>(m_text);
    DocumentItem documentItem(paragraph);

    if (!m_position.has_value())
    {
        m_documentItems.push_back(documentItem);
    }
    else
    {
        size_t pos = m_position.value();

        if (pos >= m_documentItems.size())
        {
            throw std::invalid_argument("Out of range by position");
        }
        m_documentItems.insert(m_documentItems.begin() + static_cast<std::vector<DocumentItem>::iterator::difference_type>(pos), documentItem);
    }
}

void InsertParagraphCommand::DoUnexecute()
{
    if (m_documentItems.empty())
    {
        return;
    }

    if (!m_position.has_value())
    {
        m_documentItems.pop_back();
    }
    else
    {
        size_t pos = m_position.value();

        if (pos < m_documentItems.size())
        {
            m_documentItems.erase(m_documentItems.begin() + static_cast<std::vector<DocumentItem>::iterator::difference_type>(pos));
        }
    }
}