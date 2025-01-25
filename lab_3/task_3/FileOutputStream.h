#ifndef FILEOUTPUTSTREAM_H
#define FILEOUTPUTSTREAM_H

#include "IOutputDataStream.h"
#include <fstream>
#include <stdexcept>

class FileOutputStream : public IOutputDataStream
{
public:
    explicit FileOutputStream(const std::string& fileName)
        : m_file(fileName, std::ios::binary)
    {
        if (!m_file.is_open())
        {
            throw std::ios_base::failure("Failed to open output file.");
        }
    }

    void WriteByte(uint8_t data) override
    {
        char byte = static_cast<char>(data);
        if (!m_file.write(&byte, 1))
        {
            throw std::ios_base::failure("Failed to write byte.");
        }
    }

    void WriteBlock(const void* srcData, std::streamsize size) override
    {
        if (!m_file.write(static_cast<const char*>(srcData), size))
        {
            throw std::ios_base::failure("Failed to write block.");
        }
    }

    void Close() override
    {
        m_file.close();
    }

private:
    std::ofstream m_file;
};

#endif // FILEOUTPUTSTREAM_H