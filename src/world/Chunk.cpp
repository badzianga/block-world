#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "rendering/Mesh.hpp"
#include "rendering/Shader.hpp"
#include "utils/Face.hpp"
#include "world/Chunk.hpp"
#include "world/Block.hpp"
#include "world/World.hpp"

Chunk::Chunk(glm::ivec3 position, const std::array<BlockType, Config::Chunk::volume>& blocks)
    : m_position(position), m_model(1.f), m_blocks(blocks) {
    m_model = glm::translate(m_model, static_cast<glm::vec3>(position * Config::Chunk::size));
    m_isEmpty = !std::ranges::any_of(blocks, [](const BlockType type) -> bool {
        return type != BlockType::Air;
    });
}

bool Chunk::hasMesh() const {
    return p_mesh != nullptr;
}

void Chunk::buildMesh() {
    m_built = true;
    if (m_isEmpty) {
        return;
    }
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    const std::array<std::reference_wrapper<const Chunk>, 6> neighborChunks = {
        World::getRef().getChunk({m_position.x - 1, m_position.y,       m_position.z      }),
        World::getRef().getChunk({m_position.x + 1, m_position.y,       m_position.z      }),
        World::getRef().getChunk({m_position.x,       m_position.y - 1, m_position.z      }),
        World::getRef().getChunk({m_position.x,       m_position.y + 1, m_position.z      }),
        World::getRef().getChunk({m_position.x,       m_position.y,       m_position.z + 1}),
        World::getRef().getChunk({m_position.x,       m_position.y,       m_position.z - 1}),
    };

    auto addFace = [&](const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4) -> void {
        vertices.insert(vertices.end(), {v1, v2, v3, v4});
        indices.insert(indices.end(), {
            static_cast<uint32_t>(vertices.size() - 4), static_cast<uint32_t>(vertices.size() - 3),
            static_cast<uint32_t>(vertices.size() - 2), static_cast<uint32_t>(vertices.size() - 2),
            static_cast<uint32_t>(vertices.size() - 1), static_cast<uint32_t>(vertices.size() - 4)
        });
    };

    for (int z = 0; z < Config::Chunk::size; ++z) {
        for (int y = 0; y < Config::Chunk::size; ++y) {
            for (int x = 0; x < Config::Chunk::size; ++x) {
                const BlockType type = m_blocks[x + Config::Chunk::size * y + Config::Chunk::area * z];

                if (type == BlockType::Air) continue;

                const auto [topTex, sideTex, bottomTex] = blocks[static_cast<uint8_t>(type)];

                const auto fX = static_cast<float>(x);
                const auto fY = static_cast<float>(y);
                const auto fZ = static_cast<float>(z);

                // left face
                if (isAir({x - 1, y, z}, neighborChunks)) {
                    Vertex v1{{-0.5f + fX, -0.5f + fY, -0.5f + fZ}, Face::Left, sideTex};
                    Vertex v2{{-0.5f + fX, -0.5f + fY,  0.5f + fZ}, Face::Left, sideTex};
                    Vertex v3{{-0.5f + fX,  0.5f + fY,  0.5f + fZ}, Face::Left, sideTex};
                    Vertex v4{{-0.5f + fX,  0.5f + fY, -0.5f + fZ}, Face::Left, sideTex};
                    addFace(v1, v2, v3, v4);
                }
                // right face
                if (isAir({x + 1, y, z}, neighborChunks)) {
                    Vertex v1{{ 0.5f + fX, -0.5f + fY, -0.5f + fZ}, Face::Right, sideTex};
                    Vertex v2{{ 0.5f + fX,  0.5f + fY, -0.5f + fZ}, Face::Right, sideTex};
                    Vertex v3{{ 0.5f + fX,  0.5f + fY,  0.5f + fZ}, Face::Right, sideTex};
                    Vertex v4{{ 0.5f + fX, -0.5f + fY,  0.5f + fZ}, Face::Right, sideTex};
                    addFace(v1, v2, v3, v4);
                }
                // bottom face
                if (isAir({x, y - 1, z}, neighborChunks)) {
                    Vertex v1{{-0.5f + fX, -0.5f + fY, -0.5f + fZ}, Face::Bottom, bottomTex};
                    Vertex v2{{ 0.5f + fX, -0.5f + fY, -0.5f + fZ}, Face::Bottom, bottomTex};
                    Vertex v3{{ 0.5f + fX, -0.5f + fY,  0.5f + fZ}, Face::Bottom, bottomTex};
                    Vertex v4{{-0.5f + fX, -0.5f + fY,  0.5f + fZ}, Face::Bottom, bottomTex};
                    addFace(v1, v2, v3, v4);
                }
                // top face
                if (isAir({x, y + 1, z}, neighborChunks)) {
                    Vertex v1{{ 0.5f + fX,  0.5f + fY,  0.5f + fZ}, Face::Top, topTex};
                    Vertex v2{{ 0.5f + fX,  0.5f + fY, -0.5f + fZ}, Face::Top, topTex};
                    Vertex v3{{-0.5f + fX,  0.5f + fY, -0.5f + fZ}, Face::Top, topTex};
                    Vertex v4{{-0.5f + fX,  0.5f + fY,  0.5f + fZ}, Face::Top, topTex};
                    addFace(v1, v2, v3, v4);
                }
                // front face
                if (isAir({x, y, z + 1}, neighborChunks)) {
                    Vertex v1{{-0.5f + fX, -0.5f + fY,  0.5f + fZ}, Face::Front, sideTex};
                    Vertex v2{{ 0.5f + fX, -0.5f + fY,  0.5f + fZ}, Face::Front, sideTex};
                    Vertex v3{{ 0.5f + fX,  0.5f + fY,  0.5f + fZ}, Face::Front, sideTex};
                    Vertex v4{{-0.5f + fX,  0.5f + fY,  0.5f + fZ}, Face::Front, sideTex};
                    addFace(v1, v2, v3, v4);
                }
                // back face
                if (isAir({x, y, z - 1}, neighborChunks)) {
                    Vertex v1{{-0.5f + fX, -0.5f + fY, -0.5f + fZ}, Face::Back, sideTex};
                    Vertex v2{{-0.5f + fX,  0.5f + fY, -0.5f + fZ}, Face::Back, sideTex};
                    Vertex v3{{ 0.5f + fX,  0.5f + fY, -0.5f + fZ}, Face::Back, sideTex};
                    Vertex v4{{ 0.5f + fX, -0.5f + fY, -0.5f + fZ}, Face::Back, sideTex};
                    addFace(v1, v2, v3, v4);
                }
            }
        }
    }

    p_mesh = std::make_unique<Mesh>(vertices, indices);
}

void Chunk::draw(Shader& shader) const {
    if (m_built && !m_isEmpty) {
        shader.set("u_model", m_model);
        p_mesh->draw();
    }
}

bool Chunk::isAir(const glm::ivec3& localPos, const std::array<std::reference_wrapper<const Chunk>, 6>& neighborChunks) const {
    static const auto airAt = [](const Chunk& chunk, const glm::ivec3& position) -> bool {
        return chunk.m_blocks[position.x + position.y * Config::Chunk::size + position.z * Config::Chunk::area] == BlockType::Air;
    };

    if (localPos.x < 0) {
        return airAt(neighborChunks[0].get(), {Config::Chunk::size - 1, localPos.y, localPos.z});
    }
    if (localPos.x >= Config::Chunk::size) {
        return airAt(neighborChunks[1].get(), {0, localPos.y, localPos.z});
    }
    if (localPos.y < 0) {
        return airAt(neighborChunks[2].get(), {localPos.x, Config::Chunk::size - 1, localPos.z});
    }
    if (localPos.y >= Config::Chunk::size) {
        return airAt(neighborChunks[3].get(), {localPos.x, 0, localPos.z});
    }
    if (localPos.z < 0) {
        return airAt(neighborChunks[5].get(), {localPos.x, localPos.y, Config::Chunk::size - 1});
    }
    if (localPos.z >= Config::Chunk::size) {
        return airAt(neighborChunks[4].get(), {localPos.x, localPos.y, 0});
    }
    return airAt(*this, localPos);
}
