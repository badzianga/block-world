#include <chrono>
#include <iostream>
#include "rendering/Mesh.hpp"
#include "utils/Face.hpp"
#include "world/Chunk.hpp"
#include "world/Block.hpp"

Chunk::Chunk(const std::array<BlockType, Config::Chunk::volume>& blocks) : m_blocks(blocks) {
    buildMesh();
}

void Chunk::draw() const {
    p_mesh->draw();
}

bool Chunk::isAir(const glm::ivec3& localPos) const {
    if (localPos.x < 0 || localPos.x >= Config::Chunk::size ||
        localPos.y < 0 || localPos.y >= Config::Chunk::size ||
        localPos.z < 0 || localPos.z >= Config::Chunk::size)
    {
        return true;
    }
    return m_blocks[localPos.x + localPos.y * Config::Chunk::size + localPos.z * Config::Chunk::area] == BlockType::Air;
}

void Chunk::buildMesh() {
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    auto addFace = [&](const Vertex& v1, const Vertex& v2, const Vertex& v3, const Vertex& v4) -> void {
        // interesting thing is, using reserve before insert here makes mesh building 10x slower (~0.0006s -> ~0.006s)
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
                if (isAir({x - 1, y, z})) {
                    Vertex v1{{-0.5f + fX, -0.5f + fY, -0.5f + fZ}, Face::Left, sideTex};
                    Vertex v2{{-0.5f + fX, -0.5f + fY,  0.5f + fZ}, Face::Left, sideTex};
                    Vertex v3{{-0.5f + fX,  0.5f + fY,  0.5f + fZ}, Face::Left, sideTex};
                    Vertex v4{{-0.5f + fX,  0.5f + fY, -0.5f + fZ}, Face::Left, sideTex};
                    addFace(v1, v2, v3, v4);
                }
                // right face
                if (isAir({x + 1, y, z})) {
                    Vertex v1{{ 0.5f + fX, -0.5f + fY, -0.5f + fZ}, Face::Right, sideTex};
                    Vertex v2{{ 0.5f + fX,  0.5f + fY, -0.5f + fZ}, Face::Right, sideTex};
                    Vertex v3{{ 0.5f + fX,  0.5f + fY,  0.5f + fZ}, Face::Right, sideTex};
                    Vertex v4{{ 0.5f + fX, -0.5f + fY,  0.5f + fZ}, Face::Right, sideTex};
                    addFace(v1, v2, v3, v4);
                }
                // bottom face
                if (isAir({x, y - 1, z})) {
                    Vertex v1{{-0.5f + fX, -0.5f + fY, -0.5f + fZ}, Face::Bottom, bottomTex};
                    Vertex v2{{ 0.5f + fX, -0.5f + fY, -0.5f + fZ}, Face::Bottom, bottomTex};
                    Vertex v3{{ 0.5f + fX, -0.5f + fY,  0.5f + fZ}, Face::Bottom, bottomTex};
                    Vertex v4{{-0.5f + fX, -0.5f + fY,  0.5f + fZ}, Face::Bottom, bottomTex};
                    addFace(v1, v2, v3, v4);
                }
                // top face
                if (isAir({x, y + 1, z})) {
                    Vertex v1{{ 0.5f + fX,  0.5f + fY,  0.5f + fZ}, Face::Top, topTex};
                    Vertex v2{{ 0.5f + fX,  0.5f + fY, -0.5f + fZ}, Face::Top, topTex};
                    Vertex v3{{-0.5f + fX,  0.5f + fY, -0.5f + fZ}, Face::Top, topTex};
                    Vertex v4{{-0.5f + fX,  0.5f + fY,  0.5f + fZ}, Face::Top, topTex};
                    addFace(v1, v2, v3, v4);
                }
                // front face
                if (isAir({x, y, z + 1})) {
                    Vertex v1{{-0.5f + fX, -0.5f + fY,  0.5f + fZ}, Face::Front, sideTex};
                    Vertex v2{{ 0.5f + fX, -0.5f + fY,  0.5f + fZ}, Face::Front, sideTex};
                    Vertex v3{{ 0.5f + fX,  0.5f + fY,  0.5f + fZ}, Face::Front, sideTex};
                    Vertex v4{{-0.5f + fX,  0.5f + fY,  0.5f + fZ}, Face::Front, sideTex};
                    addFace(v1, v2, v3, v4);
                }
                // back face
                if (isAir({x, y, z - 1})) {
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

    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Chunk mesh built in " << elapsed_seconds.count() << " seconds\n";
}
