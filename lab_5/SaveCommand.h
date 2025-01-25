#ifndef SAVECOMMAND_H
#define SAVECOMMAND_H

#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <filesystem>
#include <stdexcept>
#include "AbstractCommand.h"
#include "DocumentItem.h"
#include "ConstDocumentItem.h"

class SaveCommand : public AbstractCommand
{
public:
    static constexpr const char* name = "Save";

    SaveCommand(const std::vector<DocumentItem>& documentItems, std::string path, std::string  title)
        : m_documentItems(documentItems), m_path(std::move(path)), m_title(std::move(title)) {}

protected:
    void DoExecute() override;
    void DoUnexecute() override;

private:
    const std::string IMAGES_DIR = "iamges";
    const std::vector<DocumentItem>& m_documentItems;
    std::string m_path;
    std::string m_title;

    void ClearDirectory(const std::filesystem::path& dir);
    static std::string HtmlEncode(const std::string& text);
};


#endif //SAVECOMMAND_H