#ifndef PARAGRAPH_H
#define PARAGRAPH_H

#include "IParagraph.h"

class Paragraph : public IParagraph
{
public:
    explicit Paragraph(const std::string& text) : m_text(text) {}

    [[nodiscard]] std::string GetText() const override
    {
        return m_text;
    }

    void SetText(const std::string& text) override
    {
        m_text = text;
    }

private:
    std::string m_text;
};


#endif //PARAGRAPH_H