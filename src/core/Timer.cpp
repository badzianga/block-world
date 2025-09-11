#include <GLFW/glfw3.h>
#include "core/Timer.hpp"

void Timer::init() {
    m_lastFrameTime = static_cast<float>(glfwGetTime());
}

void Timer::update() {
    auto currentFrameTime = static_cast<float>(glfwGetTime());
    m_deltaTime = currentFrameTime - m_lastFrameTime;
    m_lastFrameTime = currentFrameTime;
}

float Timer::getDeltaTime() const {
    return m_deltaTime;
}

float Timer::getFPS() const {
    return 1.f / m_deltaTime;
}
