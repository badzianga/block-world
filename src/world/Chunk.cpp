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

void Chunk::makeBlockMesh(Block block, float x, float y, float z, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) {
    // yup, this code is ugly, but will be completely refactored in the future

    constexpr float stride = 1.f / static_cast<float>(Config::Textures::inRow);

    const auto uSide = static_cast<float>(block.side % 16);
    const auto vSide = static_cast<float>(block.side >> 4);
    const auto uTop = static_cast<float>(block.top % 16);
    const auto vTop = static_cast<float>(block.top >> 4);
    const auto uBottom = static_cast<float>(block.bottom % 16);
    const auto vBottom = static_cast<float>(block.bottom >> 4);

    // front
    vertices.push_back({{-0.5f + x, -0.5f + y,  0.5f + z}, Normal::front, glm::vec2{uSide, vSide + 1} * stride});
    vertices.push_back({{ 0.5f + x, -0.5f + y,  0.5f + z}, Normal::front, glm::vec2{uSide + 1, vSide + 1} * stride});
    vertices.push_back({{ 0.5f + x,  0.5f + y,  0.5f + z}, Normal::front, glm::vec2{uSide + 1, vSide} * stride});
    vertices.push_back({{-0.5f + x,  0.5f + y,  0.5f + z}, Normal::front, glm::vec2{uSide, vSide} * stride});
    indices.push_back(vertices.size() - 4);
    indices.push_back(vertices.size() - 3);
    indices.push_back(vertices.size() - 2);
    indices.push_back(vertices.size() - 2);
    indices.push_back(vertices.size() - 1);
    indices.push_back(vertices.size() - 4);
    // back
    vertices.push_back({{-0.5f + x, -0.5f + y, -0.5f + z}, Normal::back, glm::vec2{uSide + 1, vSide + 1} * stride});
    vertices.push_back({{-0.5f + x,  0.5f + y, -0.5f + z}, Normal::back, glm::vec2{uSide + 1, vSide} * stride});
    vertices.push_back({{ 0.5f + x,  0.5f + y, -0.5f + z}, Normal::back, glm::vec2{uSide, vSide} * stride});
    vertices.push_back({{ 0.5f + x, -0.5f + y, -0.5f + z}, Normal::back, glm::vec2{uSide, vSide + 1} * stride});
    indices.push_back(vertices.size() - 4);
    indices.push_back(vertices.size() - 3);
    indices.push_back(vertices.size() - 2);
    indices.push_back(vertices.size() - 2);
    indices.push_back(vertices.size() - 1);
    indices.push_back(vertices.size() - 4);
    // left
    vertices.push_back({{-0.5f + x, -0.5f + y, -0.5f + z}, Normal::left, glm::vec2{uSide, vSide + 1} * stride});
    vertices.push_back({{-0.5f + x, -0.5f + y,  0.5f + z}, Normal::left, glm::vec2{uSide + 1, vSide + 1} * stride});
    vertices.push_back({{-0.5f + x,  0.5f + y,  0.5f + z}, Normal::left, glm::vec2{uSide + 1, vSide} * stride});
    vertices.push_back({{-0.5f + x,  0.5f + y, -0.5f + z}, Normal::left, glm::vec2{uSide, vSide} * stride});
    indices.push_back(vertices.size() - 4);
    indices.push_back(vertices.size() - 3);
    indices.push_back(vertices.size() - 2);
    indices.push_back(vertices.size() - 2);
    indices.push_back(vertices.size() - 1);
    indices.push_back(vertices.size() - 4);
    // right
    vertices.push_back({{ 0.5f + x, -0.5f + y, -0.5f + z}, Normal::right, glm::vec2{uSide + 1, vSide + 1} * stride});
    vertices.push_back({{ 0.5f + x,  0.5f + y, -0.5f + z}, Normal::right, glm::vec2{uSide + 1, vSide} * stride});
    vertices.push_back({{ 0.5f + x,  0.5f + y,  0.5f + z}, Normal::right, glm::vec2{uSide, vSide} * stride});
    vertices.push_back({{ 0.5f + x, -0.5f + y,  0.5f + z}, Normal::right, glm::vec2{uSide, vSide + 1} * stride});
    indices.push_back(vertices.size() - 4);
    indices.push_back(vertices.size() - 3);
    indices.push_back(vertices.size() - 2);
    indices.push_back(vertices.size() - 2);
    indices.push_back(vertices.size() - 1);
    indices.push_back(vertices.size() - 4);
    // top
    vertices.push_back({{-0.5f + x,  0.5f + y, -0.5f + z}, Normal::top, glm::vec2{uTop, vTop} * stride});
    vertices.push_back({{-0.5f + x,  0.5f + y,  0.5f + z}, Normal::top, glm::vec2{uTop, vTop + 1} * stride});
    vertices.push_back({{ 0.5f + x,  0.5f + y,  0.5f + z}, Normal::top, glm::vec2{uTop + 1, vTop + 1} * stride});
    vertices.push_back({{ 0.5f + x,  0.5f + y, -0.5f + z}, Normal::top, glm::vec2{uTop + 1, vTop} * stride});
    indices.push_back(vertices.size() - 4);
    indices.push_back(vertices.size() - 3);
    indices.push_back(vertices.size() - 2);
    indices.push_back(vertices.size() - 2);
    indices.push_back(vertices.size() - 1);
    indices.push_back(vertices.size() - 4);
    // bottom
    vertices.push_back({{-0.5f + x, -0.5f + y, -0.5f + z}, Normal::bottom, glm::vec2{uBottom, vBottom + 1} * stride});
    vertices.push_back({{ 0.5f + x, -0.5f + y, -0.5f + z}, Normal::bottom, glm::vec2{uBottom + 1, vBottom + 1} * stride});
    vertices.push_back({{ 0.5f + x, -0.5f + y,  0.5f + z}, Normal::bottom, glm::vec2{uBottom + 1, vBottom} * stride});
    vertices.push_back({{-0.5f + x, -0.5f + y,  0.5f + z}, Normal::bottom, glm::vec2{uBottom, vBottom} * stride});
    indices.push_back(vertices.size() - 4);
    indices.push_back(vertices.size() - 3);
    indices.push_back(vertices.size() - 2);
    indices.push_back(vertices.size() - 2);
    indices.push_back(vertices.size() - 1);
    indices.push_back(vertices.size() - 4);
}

void Chunk::buildMesh() {
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    for (int z = 0; z < Config::Rendering::chunkSize; ++z) {
        for (int y = 0; y < Config::Rendering::chunkSize; ++y) {
            for (int x = 0; x < Config::Rendering::chunkSize; ++x) {
                uint8_t type = m_blocks[x + Config::Rendering::chunkSize * y + Config::Rendering::chunkArea * z];

                if (type == 0) continue;

                makeBlockMesh(blocks[type], static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), vertices, indices);
            }
        }
    }

    p_mesh = std::make_unique<Mesh>(vertices, indices);

    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Chunk mesh built in " << elapsed_seconds.count() << " seconds\n";
}
