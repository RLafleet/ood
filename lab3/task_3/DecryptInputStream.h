#ifndef DECRYPTINPUTSTREAM_H
#define DECRYPTINPUTSTREAM_H

#include "IInputDataStream.h"
#include <memory>
#include <vector>

class DecryptInputStream : public IInputDataStream
{
public:
    DecryptInputStream(std::unique_ptr<IInputDataStream> stream, uint32_t key)
        : m_stream(std::move(stream)), m_reverseTable(GenerateReverseSubstitutionTable(GenerateSubstitutionTable(key))) {}

    uint8_t ReadByte() override
    {
        uint8_t data = m_stream->ReadByte();
        return m_reverseTable[data];
    }

    std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override
    {
        std::streamsize bytesRead = m_stream->ReadBlock(dstBuffer, size);
        uint8_t* bytes = static_cast<uint8_t*>(dstBuffer);
        for (std::streamsize i = 0; i < bytesRead; ++i)
        {
            bytes[i] = m_reverseTable[bytes[i]];
        }
        return bytesRead;
    }

    bool IsEOF() const override
    {
        return m_stream->IsEOF();
    }

private:
    std::unique_ptr<IInputDataStream> m_stream;
    std::vector<uint8_t> m_reverseTable;
};

#endif // DECRYPTINPUTSTREAM_H