#include "rendering/Mesh.hpp"
#include "world/Chunk.hpp"

#include "utils/Normal.hpp"
#include "world/Block.hpp"

Chunk::Chunk() {
    generate();

    buildMesh();
}

void Chunk::draw() const {
    p_mesh->draw();
}

void Chunk::generate() {
    for (int z = 0; z < Config::Rendering::chunkSize; ++z) {
        for (int y = 0; y < Config::Rendering::chunkSize; ++y) {
            for (int x = 0; x < Config::Rendering::chunkSize; ++x) {
                int type;

                if (y < 10) type = 3;
                else if (y < 13) type = 1;
                else if (y < 14) type = 2;
                else type = 0;

                m_blocks[z * Config::Rendering::chunkArea + y * Config::Rendering::chunkSize + x] = type;
            }
        }
    }
}

void Chunk::makeBlockMesh(uint8_t type, float x, float y, float z, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) {
    // yup, this code is ugly, but will be completely refactored in the future

    constexpr float stride = 1.f / static_cast<float>(Config::Rendering::chunkSize);
    // front
    vertices.push_back({{-0.5f + x, -0.5f + y,  0.5f + z}, Normal::front, glm::vec2{(float)(blocks[type % 16].side), (float)(blocks[type / 16].side + 1)} * stride});
    vertices.push_back({{ 0.5f + x, -0.5f + y,  0.5f + z}, Normal::front, glm::vec2{(float)(blocks[type % 16].side + 1), (float)(blocks[type / 16].side + 1)} * stride});
    vertices.push_back({{ 0.5f + x,  0.5f + y,  0.5f + z}, Normal::front, glm::vec2{(float)(blocks[type % 16].side + 1), (float)(blocks[type / 16].side)} * stride});
    vertices.push_back({{-0.5f + x,  0.5f + y,  0.5f + z}, Normal::front, glm::vec2{(float)(blocks[type % 16].side), (float)(blocks[type / 16].side)} * stride});
    indices.push_back(vertices.size() - 4);
    indices.push_back(vertices.size() - 3);
    indices.push_back(vertices.size() - 2);
    indices.push_back(vertices.size() - 2);
    indices.push_back(vertices.size() - 1);
    indices.push_back(vertices.size() - 4);
    // back
    vertices.push_back({{-0.5f + x, -0.5f + y, -0.5f + z}, Normal::back, glm::vec2{(float)(blocks[type % 16].side + 1), (float)(blocks[type / 16].side + 1)} * stride});
    vertices.push_back({{-0.5f + x,  0.5f + y, -0.5f + z}, Normal::back, glm::vec2{(float)(blocks[type % 16].side + 1), (float)(blocks[type / 16].side)} * stride});
    vertices.push_back({{ 0.5f + x,  0.5f + y, -0.5f + z}, Normal::back, glm::vec2{(float)(blocks[type % 16].side), (float)(blocks[type / 16].side)} * stride});
    vertices.push_back({{ 0.5f + x, -0.5f + y, -0.5f + z}, Normal::back, glm::vec2{(float)(blocks[type % 16].side), (float)(blocks[type / 16].side + 1)} * stride});
    indices.push_back(vertices.size() - 4);
    indices.push_back(vertices.size() - 3);
    indices.push_back(vertices.size() - 2);
    indices.push_back(vertices.size() - 2);
    indices.push_back(vertices.size() - 1);
    indices.push_back(vertices.size() - 4);
    // left
    vertices.push_back({{-0.5f + x, -0.5f + y, -0.5f + z}, Normal::left, glm::vec2{(float)(blocks[type % 16].side), (float)(blocks[type / 16].side + 1)} * stride});
    vertices.push_back({{-0.5f + x, -0.5f + y,  0.5f + z}, Normal::left, glm::vec2{(float)(blocks[type % 16].side + 1), (float)(blocks[type / 16].side + 1)} * stride});
    vertices.push_back({{-0.5f + x,  0.5f + y,  0.5f + z}, Normal::left, glm::vec2{(float)(blocks[type % 16].side + 1), (float)(blocks[type / 16].side)} * stride});
    vertices.push_back({{-0.5f + x,  0.5f + y, -0.5f + z}, Normal::left, glm::vec2{(float)(blocks[type % 16].side), (float)(blocks[type / 16].side)} * stride});
    indices.push_back(vertices.size() - 4);
    indices.push_back(vertices.size() - 3);
    indices.push_back(vertices.size() - 2);
    indices.push_back(vertices.size() - 2);
    indices.push_back(vertices.size() - 1);
    indices.push_back(vertices.size() - 4);
    // right
    vertices.push_back({{ 0.5f + x, -0.5f + y, -0.5f + z}, Normal::right, glm::vec2{(float)(blocks[type % 16].side + 1), (float)(blocks[type / 16].side + 1)} * stride});
    vertices.push_back({{ 0.5f + x,  0.5f + y, -0.5f + z}, Normal::right, glm::vec2{(float)(blocks[type % 16].side + 1), (float)(blocks[type / 16].side)} * stride});
    vertices.push_back({{ 0.5f + x,  0.5f + y,  0.5f + z}, Normal::right, glm::vec2{(float)(blocks[type % 16].side), (float)(blocks[type / 16].side)} * stride});
    vertices.push_back({{ 0.5f + x, -0.5f + y,  0.5f + z}, Normal::right, glm::vec2{(float)(blocks[type % 16].side), (float)(blocks[type / 16].side + 1)} * stride});
    indices.push_back(vertices.size() - 4);
    indices.push_back(vertices.size() - 3);
    indices.push_back(vertices.size() - 2);
    indices.push_back(vertices.size() - 2);
    indices.push_back(vertices.size() - 1);
    indices.push_back(vertices.size() - 4);
    // top
    vertices.push_back({{-0.5f + x,  0.5f + y, -0.5f + z}, Normal::top, glm::vec2{(float)(blocks[type % 16].top), (float)(blocks[type / 16].top)} * stride});
    vertices.push_back({{-0.5f + x,  0.5f + y,  0.5f + z}, Normal::top, glm::vec2{(float)(blocks[type % 16].top), (float)(blocks[type / 16].top + 1)} * stride});
    vertices.push_back({{ 0.5f + x,  0.5f + y,  0.5f + z}, Normal::top, glm::vec2{(float)(blocks[type % 16].top + 1), (float)(blocks[type / 16].top + 1)} * stride});
    vertices.push_back({{ 0.5f + x,  0.5f + y, -0.5f + z}, Normal::top, glm::vec2{(float)(blocks[type % 16].top + 1), (float)(blocks[type / 16].top)} * stride});
    indices.push_back(vertices.size() - 4);
    indices.push_back(vertices.size() - 3);
    indices.push_back(vertices.size() - 2);
    indices.push_back(vertices.size() - 2);
    indices.push_back(vertices.size() - 1);
    indices.push_back(vertices.size() - 4);
    // bottom
    vertices.push_back({{-0.5f + x, -0.5f + y, -0.5f + z}, Normal::bottom, glm::vec2{(float)(blocks[type % 16].bottom), (float)(blocks[type / 16].bottom + 1)} * stride});
    vertices.push_back({{ 0.5f + x, -0.5f + y, -0.5f + z}, Normal::bottom, glm::vec2{(float)(blocks[type % 16].bottom + 1), (float)(blocks[type / 16].bottom + 1)} * stride});
    vertices.push_back({{ 0.5f + x, -0.5f + y,  0.5f + z}, Normal::bottom, glm::vec2{(float)(blocks[type % 16].bottom + 1), (float)(blocks[type / 16].bottom)} * stride});
    vertices.push_back({{-0.5f + x, -0.5f + y,  0.5f + z}, Normal::bottom, glm::vec2{(float)(blocks[type % 16].bottom), (float)(blocks[type / 16].bottom)} * stride});
    indices.push_back(vertices.size() - 4);
    indices.push_back(vertices.size() - 3);
    indices.push_back(vertices.size() - 2);
    indices.push_back(vertices.size() - 2);
    indices.push_back(vertices.size() - 1);
    indices.push_back(vertices.size() - 4);
}

void Chunk::buildMesh() {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    for (int z = 0; z < Config::Rendering::chunkSize; ++z) {
        for (int y = 0; y < Config::Rendering::chunkSize; ++y) {
            for (int x = 0; x < Config::Rendering::chunkSize; ++x) {
                uint8_t type = m_blocks[x + Config::Rendering::chunkSize * y + Config::Rendering::chunkArea * z];

                if (type == 0) continue;

                makeBlockMesh(type, static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), vertices, indices);
            }
        }
    }

    p_mesh = std::make_unique<Mesh>(vertices, indices);
}
