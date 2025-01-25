#ifndef RESIZEIMAGECOMMAND_H
#define RESIZEIMAGECOMMAND_H

#include <iostream>
#include <vector>
#include <optional>
#include <filesystem>
#include <fstream>
#include "AbstractCommand.h"
#include "DocumentItem.h"
#include "Image.h"

class ResizeImageCommand : public AbstractCommand
{
public:
    static constexpr const char* name = "ResizeImage";

    ResizeImageCommand(std::vector<DocumentItem>& documentItems,
        int newWidth, int newHeight,
        std::optional<size_t> position)
        : m_documentItems(documentItems), m_newHeight(newHeight), m_newWidth(newWidth), m_position(position) {};

    bool IsSameImage(const ResizeImageCommand* other) const
    {
        return m_position.has_value() && other->m_position.has_value() && (m_position == other->m_position);
    }

    void Merge(const ResizeImageCommand* other)
    {
        m_newWidth = other->m_newWidth;
        m_newHeight = other->m_newHeight;
    }

protected:
    void DoExecute() override;
    void DoUnexecute() override;

private:
    std::vector<DocumentItem>& m_documentItems;
    int m_height = 0;
    int m_width = 0;
    int m_newHeight;
    int m_newWidth;
    std::optional<size_t> m_position;
};


#endif //RESIZEIMAGECOMMAND_H