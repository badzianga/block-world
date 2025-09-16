#include "world/Generator.hpp"
#include "world/World.hpp"

World::World() {
    for (int z = -8; z < 8; ++z) {
        for (int y = 0; y < 7; ++y) {
            for (int x = -8; x < 8; ++x) {
                m_chunks[{x, y, z}] = std::make_unique<Chunk>(Generator::generateTerrain({x, y, z}));
            }
        }
    }
}

void World::draw(Shader& shader) const {
    for (auto& chunk : m_chunks) {
        chunk.second->draw(shader);
    }
}
