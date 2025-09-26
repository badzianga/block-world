#pragma once
#include <glm/vec3.hpp>

class Chunk;

class Generator {
public:
    virtual ~Generator() = default;

    virtual Chunk generate(glm::ivec3 chunkPos) = 0;
};

class DefaultGenerator final : public Generator {
    public:
    Chunk generate(glm::ivec3 chunkPos) override;
};
