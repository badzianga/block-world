#include <chrono>
#include <iostream>
#include "world/Generator.hpp"
#include "world/World.hpp"

World* World::p_currentWorld = nullptr;

World::World() {
    p_currentWorld = this;
    std::cout << "Starting world generation...\n";
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

    for (int z = -2; z < 2; ++z) {
        for (int y = 0; y < 6; ++y) {
            for (int x = -2; x < 2; ++x) {
            auto chunk = std::make_unique<Chunk>(Generator::generateTerrain({x, y, z}));
            if (!chunk->hasMesh()) {
                chunk->buildMesh();
            }
            m_chunks[{x, y, z}] = std::move(chunk);
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

const Chunk& World::getChunk(glm::ivec3 chunkPos) {
    const auto it = m_chunks.find(chunkPos);
    if (it == m_chunks.end()) {
        return makeChunk(chunkPos);
    }
    return *it->second;
}

World& World::getRef() {
    return *p_currentWorld;
}

Chunk& World::makeChunk(glm::ivec3 chunkPos) {
    auto [it, inserted] = m_chunks.try_emplace(chunkPos, std::make_unique<Chunk>(Generator::generateTerrain(chunkPos)));
    if (!inserted) {
        throw std::runtime_error("Chunk already exists, but shouldn't");
    }
    return *it->second;
}
