#pragma once
#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "utils/Face.hpp"

struct Vertex {
    glm::vec3 position;
    Face face;
    uint8_t textureIndex;
};
