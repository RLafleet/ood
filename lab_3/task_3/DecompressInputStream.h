#ifndef DECOMPRESSINPUTSTREAM_H
#define DECOMPRESSINPUTSTREAM_H

#include "IInputDataStream.h"
#include <vector>
#include <iostream>
#include <memory>
#include <algorithm>

class DecompressInputStream : public IInputDataStream
{
public:
    DecompressInputStream(std::unique_ptr<IInputDataStream> stream)
        : m_stream(std::move(stream)), m_buffer(), m_bufferPos(0) {}

    uint8_t ReadByte() override
    {
        if (m_bufferPos >= m_buffer.size())
        {
            FillBuffer();
        }
        if (m_bufferPos < m_buffer.size())
        {
            return m_buffer[m_bufferPos++];
        }
        throw std::runtime_error("End of stream reached");
    }

    std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override
    {
        uint8_t* bytes = static_cast<uint8_t*>(dstBuffer);
        std::streamsize bytesRead = 0;
        while (bytesRead < size) {
            if (m_bufferPos >= m_buffer.size())
            {
                if (!FillBuffer())
                {
                    break;
                }
            }
            std::streamsize remaining = std::min(size - bytesRead, static_cast<std::streamsize>(m_buffer.size() - m_bufferPos));
            std::copy(m_buffer.begin() + m_bufferPos, m_buffer.begin() + m_bufferPos + remaining, bytes + bytesRead);
            m_bufferPos += remaining;
            bytesRead += remaining;
        }
        return bytesRead;
    }

    bool IsEOF() const override
    {
        return m_stream->IsEOF() && m_bufferPos >= m_buffer.size();
    }

private:
    bool FillBuffer()
    {
        m_buffer.clear();
        m_bufferPos = 0;

        while (m_buffer.size() < 1024 && !m_stream->IsEOF())
        {
            uint8_t byte = m_stream->ReadByte();
            if (byte == 0xFF)
            {
                if (m_stream->IsEOF())
                {
                    break;
                }
                uint8_t value = m_stream->ReadByte();
                if (m_stream->IsEOF())
                {
                    break;
                }

                uint8_t count = m_stream->ReadByte();
                m_buffer.insert(m_buffer.end(), count, value);
            }
            else
            {
                m_buffer.push_back(byte);
            }
        }
        return !m_buffer.empty();
    }

    std::unique_ptr<IInputDataStream> m_stream;
    std::vector<uint8_t> m_buffer;
    size_t m_bufferPos;
};

#endif // DECOMPRESSINPUTSTREAM_H