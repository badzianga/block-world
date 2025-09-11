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
        constexpr float farPlane = 100.f;
    }  // Graphics
}  // Config
