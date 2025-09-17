#include <chrono>
#include <iostream>
#include <glm/gtc/noise.hpp>
#include "world/Chunk.hpp"
#include "world/Generator.hpp"

Chunk Generator::generate(glm::ivec3 chunkPos) {
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

    std::array<BlockType, Config::Chunk::volume> blocks{};

    for (int z = 0; z < Config::Chunk::size; ++z) {
        for (int y = 0; y < Config::Chunk::size; ++y) {
            for (int x = 0; x < Config::Chunk::size; ++x) {
                BlockType type;

                if (y == Config::Chunk::size - 1) {
                    type = BlockType::Air;
                }
                else if (y > Config::Chunk::size - 3) {
                    type = BlockType::Grass;
                }
                else if (y > Config::Chunk::size * 3 / 5) {
                    type = BlockType::Dirt;
                }
                else {
                    type = BlockType::Stone;
                }

                blocks[z * Config::Chunk::area + y * Config::Chunk::size + x] = type;
            }
        }
    }

    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Chunk generated in " << elapsed_seconds.count() << " seconds\n";

    return Chunk(chunkPos, blocks);
}

Chunk Generator::generateTerrain(glm::ivec3 chunkPos) {
    std::array<BlockType, Config::Chunk::volume> blocks{};

    const glm::ivec3 c = chunkPos * Config::Chunk::size;

    for (int z = 0; z < Config::Chunk::size; ++z) {
        const int worldZ = c.z + z;

        for (int x = 0; x < Config::Chunk::size; ++x) {
            const int worldX = c.x + x;

            BlockType type = BlockType::Air;

            const auto worldHeight = static_cast<int>(glm::simplex(glm::vec2(worldX, worldZ) * 0.01f) * 32 + 64);
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
    // TODO: use `edited` to decide during if chunk should build its mesh
    return Chunk(chunkPos, blocks);
}
