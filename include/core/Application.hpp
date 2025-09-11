#pragma once
#include <memory>

struct GLFWwindow;

class Input;
class Timer;

class Application {
public:
    Application();
    ~Application();

    void run();
private:
    GLFWwindow* p_window = nullptr;
    std::unique_ptr<Timer> p_timer;
    std::unique_ptr<Input> p_input;
};
