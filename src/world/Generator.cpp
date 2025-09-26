#include <iostream>
#include <glm/gtc/noise.hpp>
#include "world/Chunk.hpp"
#include "world/Generator.hpp"

Chunk DefaultGenerator::generate(glm::ivec3 chunkPos) {
    std::array<BlockType, Config::Chunk::volume> blocks{};

    const glm::ivec3 c = chunkPos * Config::Chunk::size;

    for (int z = 0; z < Config::Chunk::size; ++z) {
        const int worldZ = c.z + z;

        for (int x = 0; x < Config::Chunk::size; ++x) {
            const int worldX = c.x + x;

            BlockType type = BlockType::Air;

            const int worldHeight = static_cast<int>(glm::simplex(glm::vec2(worldX, worldZ) * 0.01f) * 32.f + 64.f);
            const int localHeight = std::min(worldHeight - c.y, Config::Chunk::size);

            for (int y = 0; y < localHeight; ++y) {
                const int worldY = c.y + y;
                if (worldY == worldHeight - 1) {
                    type = BlockType::Grass;
                }
                // making more types of blocks makes it slow, like 2,5x times slow
                else if (worldY >= worldHeight - 4) {
                    type = BlockType::Dirt;
                }
                else {
                    type = BlockType::Stone;
                }

                blocks[z * Config::Chunk::area + y * Config::Chunk::size + x] = type;
            }
        }
    }

    return Chunk(chunkPos, blocks);
}
