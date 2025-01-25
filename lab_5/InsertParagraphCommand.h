#ifndef INSERTPARAGRAPHCOMMAND_H
#define INSERTPARAGRAPHCOMMAND_H

#include <iostream>
#include <utility>
#include <vector>
#include <optional>
#include "AbstractCommand.h"
#include "DocumentItem.h"
#include "Paragraph.h"

class InsertParagraphCommand : public AbstractCommand
{
public:
    constexpr static const char* name = "InsertParagraph";

    InsertParagraphCommand(std::vector<DocumentItem>& documentItems,
        const std::string& text,
        std::optional<size_t> position)
        : m_documentItems(documentItems), m_text(text), m_position(position) {};
protected:
    void DoExecute() override;
    void DoUnexecute() override;

private:
    std::vector<DocumentItem>& m_documentItems;
    std::string m_text;
    std::optional<size_t> m_position;
};


#endif //INSERTPARAGRAPHCOMMAND_H