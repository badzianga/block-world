#include <chrono>
#include <iostream>
#include <glm/gtc/noise.hpp>
#include "world/Chunk.hpp"
#include "world/Generator.hpp"

Chunk Generator::generate() {
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

    return Chunk(blocks);
}

Chunk Generator::generateTerrain() {
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

    std::array<BlockType, Config::Chunk::volume> blocks{};

    for (int z = 0; z < Config::Chunk::size; ++z) {
        for (int x = 0; x < Config::Chunk::size; ++x) {
            BlockType type = BlockType::Air;

            auto localHeight = static_cast<int>(glm::simplex(glm::vec2(x, z) * 0.01f) * 4 + 8);

            for (int y = 0; y < localHeight; ++y) {
                if (y < localHeight - 4) {
                    type = BlockType::Stone;
                }
                else if (y < localHeight - 1) {
                    type = BlockType::Dirt;
                }
                else {
                    type = BlockType::Grass;
                }

                blocks[z * Config::Chunk::area + y * Config::Chunk::size + x] = type;
            }
        }
    }

    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Chunk generated in " << elapsed_seconds.count() << " seconds\n";

    return Chunk(blocks);
}
