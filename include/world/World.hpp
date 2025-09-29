#pragma once
#include <unordered_map>
#include <glm/vec3.hpp>
#include "world/Chunk.hpp"
#include "world/Generator.hpp"

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
    Chunk& getChunk(glm::ivec3 chunkPos);
    void generateChunksAroundPosition(const glm::ivec3& chunkPos);
    [[nodiscard]] float getPositionHeight(float x, float z) const;

    static World& getRef();
private:
    static World* p_currentWorld;

    Chunk& makeChunk(glm::ivec3 chunkPos);

    std::unique_ptr<Generator> p_generator;
    std::unordered_map<glm::ivec3, std::unique_ptr<Chunk>, ChunkCoordHash> m_chunks;
    std::unique_ptr<Chunk> p_emptyChunk;
};
