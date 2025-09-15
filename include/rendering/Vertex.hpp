#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "utils/Face.hpp"
#include "world/Block.hpp"

struct Vertex {
    glm::vec3 position;
    Face face;
    TextureType textureIndex;
};
