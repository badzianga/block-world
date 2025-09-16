#pragma once

namespace Config {
    namespace Window {
        constexpr int width = 800;
        constexpr int height = 600;
        constexpr auto title = "BlockWorld";
    }  // Window

    namespace Graphics {
        constexpr bool vsync = true;
        constexpr float fov = 45.f;
        constexpr float nearPlane = 0.1f;
        constexpr float farPlane = 1000.f;
    }  // Graphics

    namespace Gameplay {
        constexpr float movementSpeed = 20.f;
        constexpr float mouseSensitivity = 0.25f;
    }  // Gameplay

    namespace Chunk {
        constexpr int size = 16;
        constexpr int area = size * size;
        constexpr int volume = area * size;
    }  // Rendering
}  // Config
