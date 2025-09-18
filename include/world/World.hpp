#pragma once
#include <unordered_map>
#include <glm/vec3.hpp>
#include "world/Chunk.hpp"

class Shader;

struct ChunkCoordHash {
    std::size_t operator()(const glm::ivec3& c) const {
        return ((std::hash<int>()(c.x) ^ (std::hash<int>()(c.y) << 1)) >> 1) ^ (std::hash<int>()(c.z) << 1);
    }
};

class World {
public:
    World();

    void draw(Shader& shader) const;
    const Chunk& getChunk(glm::ivec3 chunkPos);
private:
    Chunk& makeChunk(glm::ivec3 chunkPos);

    std::unordered_map<glm::ivec3, std::unique_ptr<Chunk>, ChunkCoordHash> m_chunks;
};
