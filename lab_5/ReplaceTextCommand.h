#ifndef REPLACETEXTCOMMAND_H
#define REPLACETEXTCOMMAND_H

#include <iostream>
#include <utility>
#include <vector>
#include <optional>
#include "AbstractCommand.h"
#include "DocumentItem.h"
#include "Paragraph.h"

class ReplaceTextCommand : public AbstractCommand
{
public:
    constexpr static const char* name = "ReplaceText";

    ReplaceTextCommand(std::vector<DocumentItem>& documentItems,
        const std::string& newText,
        std::optional<size_t> position)
        : m_documentItems(documentItems), m_newText(newText), m_position(position) {};

    bool IsSameParagraph(const ReplaceTextCommand* other) const
    {
        return m_position.has_value() && other->m_position.has_value() && (m_position == other->m_position);
    }

    void Merge(const ReplaceTextCommand* other)
    {
        m_newText = other->m_newText; 
    }
protected:
    void DoExecute() override;
    void DoUnexecute() override;

private:
    std::vector<DocumentItem>& m_documentItems;
    std::string m_text;
    std::string m_newText;
    std::optional<size_t> m_position;
};


#endif //REPLACETEXTCOMMAND_H