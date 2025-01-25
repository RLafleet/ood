#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <vector>
#include "IDocument.h"
#include "History.h"
#include "InsertParagraphCommand.h"
#include "SetTitleCommand.h"
#include "ReplaceTextCommand.h"
#include "DeleteCommand.h"
#include "SaveCommand.h"
#include "DocumentItem.h"
#include "InsertImageCommand.h"
#include "ResizeImageCommand.h"
#include "DocumentItem.h"

class Document : public IDocument
{
public:
    void InsertParagraph(const std::string& text,
        std::optional<size_t> position = std::nullopt) override;

    void ReplaceText(const std::string& newText, std::optional<size_t> position) override;

    void InsertImage(const std::string& path, int width, int height,
        std::optional<size_t> position = std::nullopt) override;

    void ResizeImage(int width, int height, std::optional<size_t> position) override;

    [[nodiscard]] size_t GetItemsCount()const override;

    [[nodiscard]] ConstDocumentItem GetItem(size_t index)const override;

    DocumentItem GetItem(size_t index) override;

    void DeleteItem(size_t index) override;

    [[nodiscard]] std::string GetTitle()const override;

    void SetTitle(const std::string& title) override;

    [[nodiscard]] bool CanUndo()const override;

    void Undo() override;

    [[nodiscard]] bool CanRedo()const override;

    void Redo() override;

    void Save(const std::string& path)const override;

private:
    std::string m_title;
    History m_history;
    std::vector<DocumentItem> m_documentItems;
};


#endif //DOCUMENT_H