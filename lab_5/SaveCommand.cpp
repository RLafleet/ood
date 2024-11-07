#include "SaveCommand.h"

void SaveCommand::DoExecute()
{
    std::filesystem::path imagesDir = "images";

    ClearDirectory(imagesDir);

    std::ofstream outFile(m_path);
    if (!outFile.is_open()) {
        throw std::runtime_error("Unable to open file for writing");
    }

    outFile << "<!DOCTYPE html>\n<html>\n<head>\n";
    outFile << "<title>" << HtmlEncode(m_title) << "</title>\n";
    outFile << "</head>\n<body>\n";

    for (const ConstDocumentItem& item : m_documentItems)
    {
        auto paragraph = item.GetParagraph();
        if (paragraph != nullptr)
        {
            outFile << "<p>" << HtmlEncode(paragraph->GetText()) << "</p>\n";
        }

        auto image = item.GetImage();
        if (image != nullptr)
        {
            std::string imagePath = image->GetPath();
            std::string imageFileName = std::filesystem::path(imagePath).filename().string();
            std::string destinationPath = (imagesDir / imageFileName).string();

            std::filesystem::copy(imagePath, destinationPath,
                std::filesystem::copy_options::overwrite_existing);

            int imageWidth = image->GetWidth();
            int imageHeight = image->GetHeight();

            outFile << "<img src=\"" << HtmlEncode("images/" + imageFileName)
                << "\" alt=\"" << HtmlEncode(imageFileName)
                << "\" width=\"" << imageWidth
                << "\" height=\"" << imageHeight
                << "\" />\n";
        }
    }

    outFile << "</body>\n</html>";

    outFile.close();
}

void SaveCommand::DoUnexecute()
{

}

void SaveCommand::ClearDirectory(const std::filesystem::path& dir)
{
    if (std::filesystem::exists(dir))
    {
        for (const auto& entry : std::filesystem::directory_iterator(dir))
        {
            std::filesystem::remove(entry.path());
        }
    }
    else
    {
        std::filesystem::create_directory(dir);
    }
}

std::string SaveCommand::HtmlEncode(const std::string& text)
{
    std::string encoded;
    for (char c : text)
    {
        switch (c)
        {
        case '<': encoded += "&lt;"; break;
        case '>': encoded += "&gt;"; break;
        case '"': encoded += "&quot;"; break;
        case '\'': encoded += "&apos;"; break;
        case '&': encoded += "&amp;"; break;
        default: encoded += c; break;
        }
    }
    return encoded;
}