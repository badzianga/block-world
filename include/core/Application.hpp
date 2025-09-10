#pragma once

struct GLFWwindow;

class Application {
public:
    Application();
    ~Application();

    void run();
private:
    GLFWwindow* p_window = nullptr;
};
