#include <iostream>
#include <cstdlib>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "core/Application.hpp"
#include "core/Config.hpp"
#include "core/Input.hpp"
#include "core/Timer.hpp"
#include "rendering/Mesh.hpp"
#include "rendering/Shader.hpp"
#include "rendering/Vertex.hpp"

Application::Application() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        std::exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    p_window = glfwCreateWindow(
        Config::Window::width, Config::Window::height,
        Config::Window::title,
        nullptr, nullptr
    );
    if (!p_window) {
        std::cerr << "Failed to open GLFW window\n";
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(p_window);
    glfwSwapInterval(Config::Graphics::vsync);
    glfwSetWindowUserPointer(p_window, this);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        glfwDestroyWindow(p_window);
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    glEnable(GL_DEPTH_TEST);

    std::cout << "OpenGL API " << reinterpret_cast<const char*>(glGetString(GL_VERSION)) << "\n"
              << "Using Device: " << reinterpret_cast<const char*>(glGetString(GL_VENDOR))
              << " - " << reinterpret_cast<const char*>(glGetString(GL_RENDERER)) << "\n";

    p_input = std::make_unique<Input>(p_window);
    p_timer = std::make_unique<Timer>();
}

Application::~Application() {
    glfwDestroyWindow(p_window);
    glfwTerminate();
}

void Application::run() {
    p_timer->init();

    Shader shader{"../res/shaders/cube.vert", "../res/shaders/cube.frag"};

    std::vector<Vertex> vertices = {
        // front
        {{-0.5f, -0.5f,  0.5f}},
        {{ 0.5f, -0.5f,  0.5f}},
        {{ 0.5f,  0.5f,  0.5f}},
        {{-0.5f,  0.5f,  0.5f}},
        // back
        {{-0.5f, -0.5f, -0.5f}},
        {{-0.5f,  0.5f, -0.5f}},
        {{ 0.5f,  0.5f, -0.5f}},
        {{ 0.5f, -0.5f, -0.5f}},
        // left
        {{-0.5f, -0.5f, -0.5f}},
        {{-0.5f, -0.5f,  0.5f}},
        {{-0.5f,  0.5f,  0.5f}},
        {{-0.5f,  0.5f, -0.5f}},
        // right
        {{ 0.5f, -0.5f, -0.5f}},
        {{ 0.5f,  0.5f, -0.5f}},
        {{ 0.5f,  0.5f,  0.5f}},
        {{ 0.5f, -0.5f,  0.5f}},
        // top
        {{-0.5f,  0.5f, -0.5f}},
        {{-0.5f,  0.5f,  0.5f}},
        {{ 0.5f,  0.5f,  0.5f}},
        {{ 0.5f,  0.5f, -0.5f}},
        // bottom
        {{-0.5f, -0.5f, -0.5f}},
        {{ 0.5f, -0.5f, -0.5f}},
        {{ 0.5f, -0.5f,  0.5f}},
        {{-0.5f, -0.5f,  0.5f}},
    };

    std::vector<uint32_t> indices = {
         0,  1,  2,  2,  3,  0,        // front
         4,  5,  6,  6,  7,  4,        // back
         8,  9, 10, 10, 11,  8,        // left
        12, 13, 14, 14, 15, 12,        // right
        16, 17, 18, 18, 19, 16,        // top
        20, 21, 22, 22, 23, 20,        // bottom
    };

    Mesh cubeMesh(vertices, indices);

    shader.use();

    glm::mat4 proj = glm::perspective(
        glm::radians(Config::Graphics::fov),
        static_cast<float>(Config::Window::width)/static_cast<float>(Config::Window::height),
        Config::Graphics::nearPlane,
        Config::Graphics::farPlane
    );

    glClearColor(0.1f, 0.15f, 0.2f, 1.f);

    while (!glfwWindowShouldClose(p_window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (p_input->isKeyPressed(GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(p_window, GLFW_TRUE);
        }

        p_input->update();
        p_timer->update();

        glm::vec3 camPos{2.f, 1.5f, 2.f};
        glm::mat4 view = glm::lookAt(camPos, {0.f ,0.f ,0.f}, {0.f, 1.f ,0.f});
        glm::mat4 model(1.f);

        shader.set("u_model", model);
        shader.set("u_view", view);
        shader.set("u_projection", proj);

        cubeMesh.draw();

        glfwSwapBuffers(p_window);
        glfwPollEvents();
    }
}
