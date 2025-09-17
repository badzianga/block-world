#include <chrono>
#include <iostream>
#include "world/Generator.hpp"
#include "world/World.hpp"

World::World() {
    std::cout << "Starting world generation...\n";
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

    for (int z = -2; z < 2; ++z) {
        for (int y = 0; y < 6; ++y) {
            for (int x = -2; x < 2; ++x) {
                m_chunks[{x, y, z}] = std::make_unique<Chunk>(Generator::generateTerrain({x, y, z}));
            }
        }
    }

    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Chunks generated in " << elapsed_seconds.count() << " seconds\n";
}

void World::draw(Shader& shader) const {
    for (auto& chunk : m_chunks) {
        chunk.second->draw(shader);
    }
}
