#pragma once

class Timer {
public:
    void init();
    void update();

    [[nodiscard]] float getDeltaTime() const;
    [[nodiscard]] float getFPS() const;
private:
    float m_lastFrameTime = 0.f;
    float m_deltaTime = 0.f;
};
