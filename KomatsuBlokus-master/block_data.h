// block_data.h
#pragma once
#include <array>
#include <unordered_map>
#include <string>

struct BlockData
{
    std::array<std::array<int, 5>, 5> shape;
    std::array<std::array<int, 7>, 7> influence;
    int score;
};

extern const std::unordered_map<std::string, BlockData> block_table;
