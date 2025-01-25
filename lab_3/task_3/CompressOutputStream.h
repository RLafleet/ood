#ifndef COMPRESSOUTPUTSTREAM_H
#define COMPRESSOUTPUTSTREAM_H

#include "IOutputDataStream.h"
#include <vector>
#include <iostream>
#include <memory>

class CompressOutputStream : public IOutputDataStream
{
public:
    CompressOutputStream(std::unique_ptr<IOutputDataStream> stream)
        : m_stream(std::move(stream)), m_buffer() {}

    void WriteByte(uint8_t data) override
    {
        m_buffer.push_back(data);

        if (m_buffer.size() >= 1024)
        {
            FlushBuffer();
        }
    }

    void WriteBlock(const void* srcData, std::streamsize size) override
    {
        const uint8_t* bytes = static_cast<const uint8_t*>(srcData);
        for (std::streamsize i = 0; i < size; ++i) {
            WriteByte(bytes[i]);
        }
    }

    void Close() override
    {
        FlushBuffer();
        m_stream->Close();
    }

private:
    void FlushBuffer()
    {
        if (m_buffer.empty()) return;

        size_t i = 0;
        while (i < m_buffer.size())
        {
            uint8_t current = m_buffer[i];
            size_t runLength = 1;

            while (i + runLength < m_buffer.size() && m_buffer[i + runLength] == current && runLength < 255)
            {
                ++runLength;
            }

            if (runLength >= 3 || current == 0xFF)
            {
                // разобраться в компрессировании. с двух байт посмотреть
                m_stream->WriteByte(0xFF);
                m_stream->WriteByte(current);
                m_stream->WriteByte(static_cast<uint8_t>(runLength));
            }
            else
            {
                for (size_t j = 0; j < runLength; ++j)
                {
                    m_stream->WriteByte(current);
                }
            }
            i += runLength;
        }
        m_buffer.clear();
    }

    std::unique_ptr<IOutputDataStream> m_stream;
    std::vector<uint8_t> m_buffer;
};

#endif // COMPRESSOUTPUTSTREAM_H