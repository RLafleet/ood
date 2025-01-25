#ifndef INSERTIMAGECOMMAND_H
#define INSERTIMAGECOMMAND_H

#include <iostream>
#include <vector>
#include <optional>
#include <filesystem>
#include <fstream>
#include "AbstractCommand.h"
#include "DocumentItem.h"
#include "Image.h"

class InsertImageCommand : public AbstractCommand
{
public:
    static constexpr const char* name = "InsertImage";

    InsertImageCommand(
        std::vector<DocumentItem>& documentItems,
        const std::string& path,
        int width,
        int height,
        std::optional<size_t> position
    ) : m_documentItems(documentItems), m_path(path), m_width(width), m_height(height), m_position(position)
    {}

protected:
    void DoExecute() override;
    void DoUnexecute() override;

private:
    std::string TMP_DIR_PATH = "images";
    std::vector<DocumentItem>& m_documentItems;
    std::string m_path;
    int m_width;
    int m_height;
    std::optional<size_t> m_position;

    void CopyImage();
};


#endif //INSERTIMAGECOMMAND_H