#pragma once
#include <glm/vec3.hpp>

namespace Normal {
    constexpr glm::vec3 front  = {0.f, 0.f, -1.f};
    constexpr glm::vec3 back   = {0.f, 0.f, 1.f};
    constexpr glm::vec3 left   = {-1.f, 0.f, 0.f};
    constexpr glm::vec3 right  = {1.f, 0.f, 0.f};
    constexpr glm::vec3 top    = {0.f, 1.f, 0.f};
    constexpr glm::vec3 bottom = {0.f, -1.f, 0.f};
}  // Normal
