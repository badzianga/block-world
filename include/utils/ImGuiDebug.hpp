#pragma once

struct GLFWwindow;

class ImGuiDebug {
public:
    static void init(GLFWwindow* window);
    static void destroy();
    static void beginFrame();
    static void endFrame();
};
