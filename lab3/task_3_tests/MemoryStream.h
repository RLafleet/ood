#include "../task_3/CompressOutputStream.h"
#include "../task_3/DecompressInputStream.h"

class MemoryOutputStream : public IOutputDataStream
{
public:
    void WriteByte(uint8_t data) override
    {
        m_data.push_back(data);
    }

    void WriteBlock(const void* srcData, std::streamsize size) override
    {
        const uint8_t* bytes = static_cast<const uint8_t*>(srcData);
        m_data.insert(m_data.end(), bytes, bytes + size);
    }

    void Close() override {}

    const std::vector<uint8_t>& GetData() const
    {
        return m_data;
    }

private:
    std::vector<uint8_t> m_data;
};

class MemoryInputStream : public IInputDataStream
{
public:
    MemoryInputStream(const std::vector<uint8_t>& data)
        : m_data(data), m_position(0) {}

    uint8_t ReadByte() override
    {
        if (m_position >= m_data.size())
        {
            throw std::runtime_error("End of stream");
        }
        return m_data[m_position++];
    }

    std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override
    {
        std::streamsize bytesToRead = std::min(size, static_cast<std::streamsize>(m_data.size() - m_position));
        std::memcpy(dstBuffer, m_data.data() + m_position, bytesToRead);
        m_position += bytesToRead;
        return bytesToRead;
    }

    bool IsEOF() const override
    {
        return m_position >= m_data.size();
    }

private:
    std::vector<uint8_t> m_data;
    size_t m_position;
};
