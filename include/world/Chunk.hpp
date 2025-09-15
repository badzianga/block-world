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
    static void makeBlockMesh(Block block, float x, float y, float z, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
    void buildMesh();

    std::array<uint8_t, Config::Rendering::chunkVolume> m_blocks{};
    std::unique_ptr<Mesh> p_mesh;
};
