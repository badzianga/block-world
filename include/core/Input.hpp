#pragma once
#include <unordered_map>
#include <glm/vec2.hpp>

struct GLFWwindow;

class Input {
public:
    explicit Input(GLFWwindow* window);

    void update();

    bool isKeyDown(int key) const;
    bool isKeyPressed(int key) const;
    bool isMouseDown(int button) const;
    bool isMousePressed(int button) const;
    glm::vec2 getMousePosition() const;
    glm::vec2 getMouseDelta() const;
private:
    GLFWwindow* p_window = nullptr;

    std::unordered_map<int, bool> m_keysDown;
    std::unordered_map<int, bool> m_keysPressed;
    std::unordered_map<int, bool> m_mouseDown;
    std::unordered_map<int, bool> m_mousePressed;

    glm::vec2 m_mousePosition{0.f};
    glm::vec2 m_prevMousePosition{0.f};
    glm::vec2 m_mouseDelta{0.f};

    bool m_firstMouseInput = true;
};
