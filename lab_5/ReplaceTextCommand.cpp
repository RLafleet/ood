#include "ReplaceTextCommand.h"

void ReplaceTextCommand::DoExecute()
{
    if (!m_position.has_value())
    {
        throw std::invalid_argument("Position not null");
    }

    size_t pos = m_position.value();
    if (pos >= m_documentItems.size())
    {
        throw std::invalid_argument("Out of range by position");
    }

    auto documentItem = m_documentItems.at(pos);
    auto paragraph = documentItem.GetParagraph();
    if (paragraph != nullptr)
    {
        m_text = paragraph->GetText();
        paragraph->SetText(m_newText);
    }
}

void ReplaceTextCommand::DoUnexecute()
{
    if (!m_position.has_value())
    {
        throw std::invalid_argument("Position not null");
    }

    size_t pos = m_position.value();
    if (pos >= m_documentItems.size())
    {
        throw std::invalid_argument("Out of range by position");
    }

    auto documentItem = m_documentItems.at(pos);
    auto paragraph = documentItem.GetParagraph();

    if (paragraph != nullptr)
    {
        paragraph->SetText(m_text);
    }
}