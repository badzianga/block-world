#include <iostream>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "core/Application.hpp"
#include "core/Config.hpp"
#include "core/Input.hpp"
#include "core/Timer.hpp"
#include "rendering/Camera.hpp"
#include "rendering/Shader.hpp"
#include "rendering/Texture.hpp"
#include "utils/ImGuiDebug.hpp"
#include "world/World.hpp"

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
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    std::cout << "OpenGL API " << reinterpret_cast<const char*>(glGetString(GL_VERSION)) << "\n"
              << "Using Device: " << reinterpret_cast<const char*>(glGetString(GL_VENDOR))
              << " - " << reinterpret_cast<const char*>(glGetString(GL_RENDERER)) << "\n";

    p_input = std::make_shared<Input>(p_window);
    p_timer = std::make_unique<Timer>();
    p_camera = std::make_unique<Camera>(p_input);

    ImGuiDebug::init(p_window);
}

Application::~Application() {
    ImGuiDebug::destroy();

    glfwDestroyWindow(p_window);
    glfwTerminate();
}

void Application::run() {
    p_timer->init();

    World world;
    Shader shader{"../res/shaders/cube.vert", "../res/shaders/cube.frag"};
    Texture texture{"../res/textures/terrain.png"};

    shader.use();
    texture.use();

    glClearColor(0.47f, 0.655f, 1.f, 1.f);

    glfwSetInputMode(p_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    shader.set("u_projection", Camera::getProjectionMatrix());

    float timeSinceLastChunkUpdate = 0.f;

    while (!glfwWindowShouldClose(p_window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGuiDebug::beginFrame();

        if (p_input->isKeyPressed(GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(p_window, GLFW_TRUE);
        }

        p_timer->update();
        p_camera->update(p_timer->getDeltaTime());
        p_input->update();

        timeSinceLastChunkUpdate += p_timer->getDeltaTime();
        if (timeSinceLastChunkUpdate > 5.f) {
            world.generateChunksAroundPosition(p_camera->position);
            timeSinceLastChunkUpdate = 0.f;
        }

        shader.set("u_view", p_camera->getViewMatrix());

        world.draw(shader);

        ImGuiDebug::endFrame();

        glfwSwapBuffers(p_window);
        glfwPollEvents();
    }
}
