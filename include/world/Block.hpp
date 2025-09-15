#pragma once
#include <cstdint>

enum class BlockType : uint8_t {
    Air = 0,
    Dirt = 1,
    Grass = 2,
    Stone = 3,
};

enum class TextureType : uint8_t {
    Dirt = 0,
    Grass = 1,
    GrassSide = 2,
    Stone = 3,
};

struct Block {
    TextureType top;
    TextureType side;
    TextureType bottom;
};

static Block blocks[UINT8_MAX] = {
    {},  // air
    {TextureType::Dirt, TextureType::Dirt, TextureType::Dirt},  // dirt
    {TextureType::Grass, TextureType::GrassSide, TextureType::Dirt},  // grass
    {TextureType::Stone, TextureType::Stone, TextureType::Stone},  // stone
};
