#pragma once
#include <unordered_map>

struct GLFWwindow;

class Input {
public:
    explicit Input(GLFWwindow* window);

    void update();

    bool isKeyDown(int key) const;
    bool isKeyPressed(int key) const;
    bool isMouseDown(int button) const;
    bool isMousePressed(int button) const;
private:
    std::unordered_map<int, bool> m_keysDown;
    std::unordered_map<int, bool> m_keysPressed;
    std::unordered_map<int, bool> m_mouseDown;
    std::unordered_map<int, bool> m_mousePressed;
};
