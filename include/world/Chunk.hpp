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

    void draw(Shader& shader) const;
private:
    [[nodiscard]] bool isAir(const glm::ivec3& localPos) const;
    void buildMesh();

    glm::ivec3 m_position;
    glm::mat4 m_model;
    std::array<BlockType, Config::Chunk::volume> m_blocks{};
    std::unique_ptr<Mesh> p_mesh;
};
