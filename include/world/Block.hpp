#pragma once
#include <cstdint>

// TODO: use enums to eliminate magic numbers

struct Block {
    uint8_t top;
    uint8_t side;
    uint8_t bottom;
};

static Block blocks[UINT8_MAX] = {
    {0, 0, 0},  // air
    {0, 0, 0},  // dirt
    {1, 2, 0},  // grass
    {3, 3, 3},  // stone
};
