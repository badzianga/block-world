#include <iostream>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "core/Application.hpp"
#include "core/Input.hpp"

Application::Application() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        std::exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    p_window = glfwCreateWindow(800, 600, "MineCppraft", nullptr, nullptr);
    if (!p_window) {
        std::cerr << "Failed to open GLFW window\n";
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(p_window);
    glfwSetWindowUserPointer(p_window, this);

    p_input = std::make_unique<Input>(p_window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        glfwDestroyWindow(p_window);
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    std::cout << "OpenGL API " << reinterpret_cast<const char*>(glGetString(GL_VERSION)) << "\n"
              << "Using Device: " << reinterpret_cast<const char*>(glGetString(GL_VENDOR))
              << " - " << reinterpret_cast<const char*>(glGetString(GL_RENDERER)) << "\n";
}

Application::~Application() {
    glfwDestroyWindow(p_window);
    glfwTerminate();
}

void Application::run() {
    while (!glfwWindowShouldClose(p_window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        if (p_input->isKeyPressed(GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(p_window, GLFW_TRUE);
        }

        p_input->update();

        glfwSwapBuffers(p_window);
        glfwPollEvents();
    }
}
