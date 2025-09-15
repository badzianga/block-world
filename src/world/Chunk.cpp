#include <chrono>
#include <iostream>
#include "rendering/Mesh.hpp"
#include "utils/Normal.hpp"
#include "world/Chunk.hpp"
#include "world/Block.hpp"

Chunk::Chunk(const std::array<uint8_t, Config::Rendering::chunkVolume>& blocks) : m_blocks(blocks) {
    buildMesh();
}

void Chunk::draw() const {
    p_mesh->draw();
}

bool Chunk::isAir(const glm::ivec3& localPos) const {
    if (localPos.x < 0 || localPos.x >= Config::Rendering::chunkSize ||
        localPos.y < 0 || localPos.y >= Config::Rendering::chunkSize ||
        localPos.z < 0 || localPos.z >= Config::Rendering::chunkSize)
    {
        return true;
    }
    return m_blocks[localPos.x + localPos.y * Config::Rendering::chunkSize + localPos.z * Config::Rendering::chunkArea] == 0;
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

    for (int z = 0; z < Config::Rendering::chunkSize; ++z) {
        for (int y = 0; y < Config::Rendering::chunkSize; ++y) {
            for (int x = 0; x < Config::Rendering::chunkSize; ++x) {
                const uint8_t type = m_blocks[x + Config::Rendering::chunkSize * y + Config::Rendering::chunkArea * z];
                const auto [topTex, sideTex, bottomTex] = blocks[type];

                if (type == 0) continue;

                constexpr float stride = 1.f / Config::Textures::inRow;

                const auto uSide = static_cast<float>(sideTex % 16);
                const auto vSide = static_cast<float>(sideTex >> 4);
                const auto uTop = static_cast<float>(topTex % 16);
                const auto vTop = static_cast<float>(topTex >> 4);
                const auto uBottom = static_cast<float>(bottomTex % 16);
                const auto vBottom = static_cast<float>(bottomTex >> 4);

                const auto fX = static_cast<float>(x);
                const auto fY = static_cast<float>(y);
                const auto fZ = static_cast<float>(z);

                // left face
                if (isAir({x - 1, y, z})) {
                    Vertex v1{{-0.5f + fX, -0.5f + fY, -0.5f + fZ}, Normal::Left, glm::vec2{uSide, vSide + 1} * stride};
                    Vertex v2{{-0.5f + fX, -0.5f + fY,  0.5f + fZ}, Normal::Left, glm::vec2{uSide + 1, vSide + 1} * stride};
                    Vertex v3{{-0.5f + fX,  0.5f + fY,  0.5f + fZ}, Normal::Left, glm::vec2{uSide + 1, vSide} * stride};
                    Vertex v4{{-0.5f + fX,  0.5f + fY, -0.5f + fZ}, Normal::Left, glm::vec2{uSide, vSide} * stride};
                    addFace(v1, v2, v3, v4);
                }
                // right face
                if (isAir({x + 1, y, z})) {
                    Vertex v1{{ 0.5f + fX, -0.5f + fY, -0.5f + fZ}, Normal::Right, glm::vec2{uSide + 1, vSide + 1} * stride};
                    Vertex v2{{ 0.5f + fX,  0.5f + fY, -0.5f + fZ}, Normal::Right, glm::vec2{uSide + 1, vSide} * stride};
                    Vertex v3{{ 0.5f + fX,  0.5f + fY,  0.5f + fZ}, Normal::Right, glm::vec2{uSide, vSide} * stride};
                    Vertex v4{{ 0.5f + fX, -0.5f + fY,  0.5f + fZ}, Normal::Right, glm::vec2{uSide, vSide + 1} * stride};
                    addFace(v1, v2, v3, v4);
                }
                // top face
                if (isAir({x, y + 1, z})) {
                    Vertex v1{{-0.5f + fX,  0.5f + fY, -0.5f + fZ}, Normal::Top, glm::vec2{uTop, vTop} * stride};
                    Vertex v2{{-0.5f + fX,  0.5f + fY,  0.5f + fZ}, Normal::Top, glm::vec2{uTop, vTop + 1} * stride};
                    Vertex v3{{ 0.5f + fX,  0.5f + fY,  0.5f + fZ}, Normal::Top, glm::vec2{uTop + 1, vTop + 1} * stride};
                    Vertex v4{{ 0.5f + fX,  0.5f + fY, -0.5f + fZ}, Normal::Top, glm::vec2{uTop + 1, vTop} * stride};
                    addFace(v1, v2, v3, v4);
                }
                // bottom face
                if (isAir({x, y - 1, z})) {
                    Vertex v1{{-0.5f + fX, -0.5f + fY, -0.5f + fZ}, Normal::Bottom, glm::vec2{uBottom, vBottom + 1} * stride};
                    Vertex v2{{ 0.5f + fX, -0.5f + fY, -0.5f + fZ}, Normal::Bottom, glm::vec2{uBottom + 1, vBottom + 1} * stride};
                    Vertex v3{{ 0.5f + fX, -0.5f + fY,  0.5f + fZ}, Normal::Bottom, glm::vec2{uBottom + 1, vBottom} * stride};
                    Vertex v4{{-0.5f + fX, -0.5f + fY,  0.5f + fZ}, Normal::Bottom, glm::vec2{uBottom, vBottom} * stride};
                    addFace(v1, v2, v3, v4);

                }
                // front face
                if (isAir({x, y, z + 1})) {
                    Vertex v1{{-0.5f + fX, -0.5f + fY,  0.5f + fZ}, Normal::Front, glm::vec2{uSide, vSide + 1} * stride};
                    Vertex v2{{ 0.5f + fX, -0.5f + fY,  0.5f + fZ}, Normal::Front, glm::vec2{uSide + 1, vSide + 1} * stride};
                    Vertex v3{{ 0.5f + fX,  0.5f + fY,  0.5f + fZ}, Normal::Front, glm::vec2{uSide + 1, vSide} * stride};
                    Vertex v4{{-0.5f + fX,  0.5f + fY,  0.5f + fZ}, Normal::Front, glm::vec2{uSide, vSide} * stride};
                    addFace(v1, v2, v3, v4);
                }
                // back face
                if (isAir({x, y, z - 1})) {
                    Vertex v1{{-0.5f + fX, -0.5f + fY, -0.5f + fZ}, Normal::Back, glm::vec2{uSide + 1, vSide + 1} * stride};
                    Vertex v2{{-0.5f + fX,  0.5f + fY, -0.5f + fZ}, Normal::Back, glm::vec2{uSide + 1, vSide} * stride};
                    Vertex v3{{ 0.5f + fX,  0.5f + fY, -0.5f + fZ}, Normal::Back, glm::vec2{uSide, vSide} * stride};
                    Vertex v4{{ 0.5f + fX, -0.5f + fY, -0.5f + fZ}, Normal::Back, glm::vec2{uSide, vSide + 1} * stride};
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
