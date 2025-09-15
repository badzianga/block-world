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
    explicit Chunk(const std::array<uint8_t, Config::Rendering::chunkVolume>& blocks);

    void draw() const;
private:
    [[nodiscard]] bool isAir(const glm::ivec3& localPos) const;
    void buildMesh();

    std::array<uint8_t, Config::Rendering::chunkVolume> m_blocks{};
    std::unique_ptr<Mesh> p_mesh;
};
