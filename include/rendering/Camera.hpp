#pragma once
#include <memory>
#include <glm/mat4x4.hpp>

class Input;

class Camera {
public:
    explicit Camera(const std::shared_ptr<Input>& input);

    void update(float deltaTime);

    [[nodiscard]] glm::mat4 getViewMatrix() const;
    static glm::mat4 getProjectionMatrix();

    glm::vec3 position{0.f};
private:
    void processKeyboard(float deltaTime);
    void processMouse();
    void updateCameraVectors();

    std::shared_ptr<Input> p_input;

    float m_yaw = -90.f;
    float m_pitch = 0.f;

    glm::vec3 m_front{0.f, 0.f, -1.f};
};
