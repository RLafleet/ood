#include "InsertImageCommand.h"

void InsertImageCommand::DoExecute()
{
    Image image(m_path, m_width, m_height);
    DocumentItem documentItem(std::make_shared<Image>(image));

    if (!m_position.has_value())
    {
        m_documentItems.push_back(documentItem);
        CopyImage();
        return;
    }

    if (m_position.value() > m_documentItems.size())
    {
        throw std::invalid_argument("Not existing position");
    }

    CopyImage();
    m_documentItems.insert(m_documentItems.begin() + static_cast<std::vector<DocumentItem>::iterator::difference_type>(m_position.value()), documentItem);
}

void InsertImageCommand::DoUnexecute()
{
    if (m_documentItems.empty())
    {
        return;
    }

    if (!m_position.has_value())
    {
        m_documentItems.pop_back();
        return;
    }

    if (m_position.value() < m_documentItems.size())
    {
        m_documentItems.erase(m_documentItems.begin() + static_cast<std::vector<DocumentItem>::iterator::difference_type>(m_position.value()));
    }
}

void InsertImageCommand::CopyImage()
{
    std::filesystem::path imagesDir = TMP_DIR_PATH;

    if (!std::filesystem::exists(imagesDir))
    {
        std::filesystem::create_directory(imagesDir);
    }

    std::filesystem::path imagePath = std::filesystem::path(m_path);
    std::string imageFileName = imagePath.filename().string();

    if (std::filesystem::exists(imagePath))
    {
        try
        {
            std::filesystem::path target = imagesDir / imageFileName;
            if (std::filesystem::exists(target))
            {
                return;
            }

            std::filesystem::copy(imagePath, target);
            std::cout << "Image copied to: " << (imagesDir / imageFileName).string() << std::endl;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error during copy operation: " << e.what() << std::endl;
        }
    }
    else
    {
        throw std::invalid_argument("Error: Image does not exist at path: " + imagePath.string());
    }
}