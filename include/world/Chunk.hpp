#pragma once
#include <array>
#include <memory>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "core/Config.hpp"
#include "rendering/Mesh.hpp"
#include "rendering/Vertex.hpp"

struct Block;
class Mesh;
class Shader;

class Chunk {
public:
    explicit Chunk(glm::ivec3 position, const std::array<BlockType, Config::Chunk::volume>& blocks);

    [[nodiscard]] bool hasMesh() const;
    void buildMesh();
    void draw(Shader& shader) const;
private:
    [[nodiscard]] bool isAir(const glm::ivec3& localPos) const;

    glm::mat4 m_model;
    std::array<BlockType, Config::Chunk::volume> m_blocks{};
    bool m_built = false;
    bool m_isEmpty;
    std::unique_ptr<Mesh> p_mesh;
};
