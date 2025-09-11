#pragma once
#include <memory>

struct GLFWwindow;

class Input;

class Application {
public:
    Application();
    ~Application();

    void run();
private:
    GLFWwindow* p_window = nullptr;
    std::unique_ptr<Input> p_input;
};
