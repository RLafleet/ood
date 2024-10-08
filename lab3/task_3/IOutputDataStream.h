#ifndef IOUTPUTDATASTREAM_H
#define IOUTPUTDATASTREAM_H

#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <string>
#include <iostream>
#include "utils.h"

class IOutputDataStream 
{
public:
    virtual void WriteByte(uint8_t data) = 0; 
    virtual void WriteBlock(const void* srcData, std::streamsize size) = 0; 
    virtual void Close() = 0; 
    virtual ~IOutputDataStream() = default;
};

#endif // IOUTPUTDATASTREAM_H