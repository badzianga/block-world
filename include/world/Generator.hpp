#pragma once
#include <glm/vec3.hpp>

class Chunk;

class Generator {
public:
    static Chunk generate(glm::ivec3 chunkPos);
    static Chunk generateTerrain(glm::ivec3 chunkPos);
};
