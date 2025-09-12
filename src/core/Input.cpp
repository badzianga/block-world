#include <GLFW/glfw3.h>
#include "core/Input.hpp"
#include <iostream>

Input::Input(GLFWwindow* window) : p_window(window) {
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

    glfwSetCursorPosCallback(window, [](GLFWwindow* win, double xpos, double ypos) {
        auto input = static_cast<Input*>(glfwGetWindowUserPointer(win));

        input->m_mousePosition.x = static_cast<float>(xpos);
        input->m_mousePosition.y = static_cast<float>(ypos);

        if (input->m_firstMouseInput) {
            input->m_prevMousePosition = input->m_mousePosition;
            input->m_firstMouseInput = false;
        }
    });
}

void Input::update() {
    m_keysPressed.clear();
    m_mousePressed.clear();

    m_mouseDelta = m_mousePosition - m_prevMousePosition;
    m_prevMousePosition = m_mousePosition;
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

glm::vec2 Input::getMousePosition() const {
    return m_mousePosition;
}

glm::vec2 Input::getMouseDelta() const {
    return m_mouseDelta;
}
