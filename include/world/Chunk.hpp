#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include "core/Config.hpp"
#include "rendering/Mesh.hpp"
#include "rendering/Vertex.hpp"

struct Block;
class Mesh;

class Chunk {
public:
    explicit Chunk(const std::array<BlockType, Config::Chunk::volume>& blocks);

    void draw() const;
private:
    [[nodiscard]] bool isAir(const glm::ivec3& localPos) const;
    void buildMesh();

    std::array<BlockType, Config::Chunk::volume> m_blocks{};
    std::unique_ptr<Mesh> p_mesh;
};
