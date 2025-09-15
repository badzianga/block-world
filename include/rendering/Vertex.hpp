#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "utils/Normal.hpp"

struct Vertex {
    glm::vec3 position;
    Normal normal;
    glm::vec2 uv;
};
