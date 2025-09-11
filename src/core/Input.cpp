#include <GLFW/glfw3.h>
#include "core/Input.hpp"
#include <iostream>

Input::Input(GLFWwindow* window) {
    glfwSetWindowUserPointer(window, this);

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int, int action, int) {
        auto input = static_cast<Input*>(glfwGetWindowUserPointer(window));

        if (action == GLFW_PRESS) {
            input->m_keysDown[key] = true;
            input->m_keysPressed[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            input->m_keysDown[key] = false;
        }
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* win, int button, int action, int) {
        auto input = static_cast<Input*>(glfwGetWindowUserPointer(win));

        if (action == GLFW_PRESS) {
            input->m_mouseDown[button] = true;
            input->m_mousePressed[button] = true;
        }
        else if (action == GLFW_RELEASE) {
            input->m_mouseDown[button] = false;
        }
    });
}

void Input::update() {
    m_keysPressed.clear();
    m_mousePressed.clear();
}

bool Input::isKeyDown(int key) const {
    auto it = m_keysDown.find(key);
    return it != m_keysDown.end() && it->second;
}

bool Input::isKeyPressed(int key) const {
    auto it = m_keysPressed.find(key);
    return it != m_keysPressed.end() && it->second;
}

bool Input::isMouseDown(int button) const {
    auto it = m_mouseDown.find(button);
    return it != m_mouseDown.end() && it->second;
}

bool Input::isMousePressed(int button) const {
    auto it = m_mousePressed.find(button);
    return it != m_mousePressed.end() && it->second;
}
