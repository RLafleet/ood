#ifndef ENCRYPTOUTPUTSTREAM_H
#define ENCRYPTOUTPUTSTREAM_H

#include "IOutputDataStream.h"

class EncryptOutputStream : public IOutputDataStream 
{
public:
	EncryptOutputStream(std::unique_ptr<IOutputDataStream> stream, uint32_t key)
		: m_stream(std::move(stream)), m_table(GenerateSubstitutionTable(key))
	{
	}

    void WriteByte(uint8_t data) override
    {
        uint8_t encryptedData = m_table[data];
        m_stream->WriteByte(encryptedData);
    }

    void WriteBlock(const void* srcData, std::streamsize size) override
    {
        const uint8_t* bytes = static_cast<const uint8_t*>(srcData);
        std::vector<uint8_t> encryptedBlock(size);
        for (std::streamsize i = 0; i < size; ++i)
        {
            encryptedBlock[i] = m_table[bytes[i]];
        }
        m_stream->WriteBlock(encryptedBlock.data(), size);
    }

    void Close() override
    {
        m_stream->Close();
    }

private:
	std::unique_ptr<IOutputDataStream> m_stream;
	std::vector<uint8_t> m_table;
}; 

#endif // ENCRYPTOUTPUTSTREAM_H