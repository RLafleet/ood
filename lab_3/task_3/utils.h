#pragma once
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <string>

std::vector<uint8_t> GenerateSubstitutionTable(uint32_t key) 
{
    std::vector<uint8_t> table(256);
    std::iota(table.begin(), table.end(), 0); 

    std::mt19937 rng(key);
    std::shuffle(table.begin(), table.end(), rng); 

    return table;
}

std::vector<uint8_t> GenerateReverseSubstitutionTable(const std::vector<uint8_t>& table) 
{
    std::vector<uint8_t> reverseTable(256);
    for (size_t i = 0; i < 256; ++i) 
    {
        reverseTable[table[i]] = static_cast<uint8_t>(i);
    }
    return reverseTable;
}
