#include "Document.h"

void Document::InsertParagraph(const std::string& text, std::optional<size_t> position)
{
    m_history.AddAndExecuteCommand(std::make_unique<InsertParagraphCommand>(m_documentItems, text, position));
}

void Document::InsertImage(const std::string& path, int width, int height, std::optional<size_t> position)
{
    m_history.AddAndExecuteCommand(
        std::make_unique<InsertImageCommand>(
            m_documentItems,
            path,
            width,
            height,
            position
        )
    );
}

size_t Document::GetItemsCount() const
{
    return m_documentItems.size();
}

ConstDocumentItem Document::GetItem(size_t index) const
{
    return m_documentItems.at(index);
}

DocumentItem Document::GetItem(size_t index)
{
    return m_documentItems.at(index);
}

void Document::DeleteItem(size_t index)
{
    m_history.AddAndExecuteCommand(std::make_unique<DeleteItemCommand>(m_documentItems, index));
}

std::string Document::GetTitle() const
{
    return m_title;
}

void Document::SetTitle(const std::string& title)
{
    m_history.AddAndExecuteCommand(std::make_unique<SetTitleCommand>(m_title, title));
}

bool Document::CanUndo() const
{
    return m_history.CanUndo();
}

void Document::Undo()
{
    m_history.Undo();
}

bool Document::CanRedo() const
{
    return m_history.CanRedo();
}

void Document::Redo()
{
    m_history.Redo();
}

void Document::Save(const std::string& path) const
{
    std::make_unique<SaveCommand>(m_documentItems, path, m_title)->Execute();
}

void Document::ReplaceText(const std::string& newText, std::optional<size_t> position)
{
    m_history.AddAndExecuteCommand(std::make_unique<ReplaceTextCommand>(m_documentItems, newText, position));
}

void Document::ResizeImage(int width, int height, std::optional<size_t> position)
{
    m_history.AddAndExecuteCommand(std::make_unique<ResizeImageCommand>(m_documentItems, width, height, position));
}