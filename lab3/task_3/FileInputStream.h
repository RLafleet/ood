#ifndef FILEINPUTSTREAM_H
#define FILEINPUTSTREAM_H

#include "IInputDataStream.h"
#include <fstream>
#include <stdexcept>

class FileInputStream : public IInputDataStream
{
public:
	explicit FileInputStream(const std::string& fileName)
		: m_file(fileName, std::ios::binary)
	{
		if (!m_file.is_open())
		{
			throw std::ios_base::failure("Failed to open input file.");
		}
	}

	bool IsEOF() const override
	{
		return m_file.eof() || m_file.rdbuf()->sgetc() == EOF;
	}

	uint8_t ReadByte() override
	{
		char byte;
	
		if (!m_file.read(&byte, 1))
		{
			throw std::ios_base::failure("Failed to read byte.");
		}
		return static_cast<uint8_t>(byte);
	}

	std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) override
	{
		m_file.read(static_cast<char*>(dstBuffer), size);
		if (m_file.bad())
		{
			throw std::ios_base::failure("Failed to read block.");
		}
		return m_file.gcount();
	}

private:
	std::ifstream m_file;
};

#endif // FILEINPUTDATASTREAM_H