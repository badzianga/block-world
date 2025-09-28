#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include "core/Config.hpp"
#include "core/Input.hpp"
#include "rendering/Camera.hpp"

Camera::Camera(const Input& input) : m_input(input) {}

void Camera::update(float deltaTime) {
    processKeyboard(deltaTime);
    processMouse();
    updateCameraVectors();

    ImGui::Text("XYZ: %f / %f / %f", position.x, position.y, position.z);
    ImGui::Text("Block: %d / %d / %d",
                static_cast<int>(position.x),
                static_cast<int>(position.y),
                static_cast<int>(position.z)
    );
    ImGui::Text("Chunk: %d / %d / %d",
                static_cast<int>(std::floor(position.x / Config::Chunk::size)),
                static_cast<int>(std::floor(position.y / Config::Chunk::size)),
                static_cast<int>(std::floor(position.z / Config::Chunk::size))
    );
    ImGui::Text("Facing: %f / %f / %f", m_front.x, m_front.y, m_front.z);
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + m_front, {0.f, 1.f, 0.f});
}

glm::mat4 Camera::getProjectionMatrix() {
    return glm::perspective(
        Config::Graphics::fov,
        static_cast<float>(Config::Window::width) / static_cast<float>(Config::Window::height),
        Config::Graphics::nearPlane,
        Config::Graphics::farPlane
    );
}

void Camera::processKeyboard(float deltaTime) {
    float velocity = Config::Gameplay::movementSpeed * deltaTime;

    glm::vec3 flatFront = m_front;
    flatFront.y = 0.f;
    flatFront = glm::normalize(flatFront);

    glm::vec3 right = glm::normalize(glm::cross(flatFront, {0.f, 1.f, 0.f}));

    if (m_input.isKeyDown(GLFW_KEY_W)) {
        position += flatFront * velocity;
    }
    if (m_input.isKeyDown(GLFW_KEY_S)) {
        position -= flatFront * velocity;
    }
    if (m_input.isKeyDown(GLFW_KEY_A)) {
        position -= right * velocity;
    }
    if (m_input.isKeyDown(GLFW_KEY_D)) {
        position += right * velocity;
    }
    if (m_input.isKeyDown(GLFW_KEY_SPACE)) {
        position.y += velocity;
    }
    if (m_input.isKeyDown(GLFW_KEY_LEFT_SHIFT)) {
        position.y -= velocity;
    }
}

void Camera::processMouse() {
    glm::vec2 offset = m_input.getMouseDelta() * Config::Gameplay::mouseSensitivity;

    m_yaw += offset.x;
    m_pitch -= offset.y;

    m_pitch = glm::clamp(m_pitch, -89.9f, 89.9f);
}

void Camera::updateCameraVectors() {
    glm::vec3 front{
        std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch)),
        std::sin(glm::radians(m_pitch)),
        std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch))
    };
    m_front = glm::normalize(front);
}
