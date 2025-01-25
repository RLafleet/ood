#ifndef IINPUTDATASTREAM_H
#define IINPUTDATASTREAM_H

#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <string>
#include "utils.h"

class IInputDataStream
{
public:
    virtual uint8_t ReadByte() = 0;
    virtual std::streamsize ReadBlock(void* dstBuffer, std::streamsize size) = 0; 
    virtual bool IsEOF() const = 0; 
    virtual ~IInputDataStream() = default;
};
#endif // IINPUTDATASTREAM_H
