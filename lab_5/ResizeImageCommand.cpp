#include "ResizeImageCommand.h"

void ResizeImageCommand::DoExecute()
{
    if (!m_position.has_value())
    {
        throw std::invalid_argument("This command needs position to operate");
    }

    if (m_position.value() >= m_documentItems.size())
    {
        throw std::invalid_argument("Not existing position");
    }

    auto image = m_documentItems.at(m_position.value()).GetImage();
    if (image != nullptr)
    {
        m_height = image->GetHeight();
        m_width = image->GetWidth();
        image->Resize(m_newWidth, m_newHeight);
    }
    else
    {
        throw std::invalid_argument("There are no image in this position!");
    }
}

void ResizeImageCommand::DoUnexecute()
{
    if (!m_position.has_value())
    {
        throw std::invalid_argument("This command needs position to operate");
    }

    if (m_position.value() >= m_documentItems.size())
    {
        throw std::invalid_argument("Not existing position");
    }

    auto image = m_documentItems.at(m_position.value()).GetImage();

    if (image != nullptr)
    {
        image->Resize(m_width, m_height);
    }
    else
    {
        throw std::invalid_argument("There are no image in this position!");
    }
}