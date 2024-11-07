#ifndef CONSTDOCUMENTITEM_H
#define CONSTDOCUMENTITEM_H

#include <iostream>
#include <memory>
#include "IImage.h"
#include "IParagraph.h"

class ConstDocumentItem
{
public:
    explicit ConstDocumentItem(std::shared_ptr<const IImage> image) : m_image(std::move(image)) {}

    explicit ConstDocumentItem(std::shared_ptr<const IParagraph> paragraph) : m_paragraph(std::move(paragraph)) {}

    [[nodiscard]] std::shared_ptr<const IImage> GetImage() const
    {
        return m_image;
    }

    [[nodiscard]] std::shared_ptr<const IParagraph> GetParagraph() const
    {
        return m_paragraph;
    }

    virtual ~ConstDocumentItem() = default;

private:
    std::shared_ptr<const IImage> m_image;
    std::shared_ptr<const IParagraph> m_paragraph;
};

#endif //CONSTDOCUMENTITEM_H