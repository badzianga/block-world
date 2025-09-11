#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "rendering/Shader.hpp"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::string vertCode = loadFile(vertexPath);
    std::string fragCode = loadFile(fragmentPath);
    const char* vShaderCode = vertCode.c_str();
    const char* fShaderCode = fragCode.c_str();

    uint32_t vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "Vertex");

    uint32_t fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "Fragment");

    m_id = glCreateProgram();
    glAttachShader(m_id, vertex);
    glAttachShader(m_id, fragment);
    glLinkProgram(m_id);
    checkLinkErrors(m_id);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    if (m_id) glDeleteProgram(m_id);
}

void Shader::use() const {
    glUseProgram(m_id);
}

void Shader::set(const char* name, bool value) {
    glUniform1i(getUniformLocation(name), static_cast<int>(value));
}

void Shader::set(const char* name, int value) {
    glUniform1i(getUniformLocation(name), value);
}

void Shader::set(const char* name, float value) {
    glUniform1f(getUniformLocation(name), value);
}

void Shader::set(const char* name, float x, float y) {
    glUniform2f(getUniformLocation(name), x, y);
}

void Shader::set(const char* name, float x, float y, float z) {
    glUniform3f(getUniformLocation(name), x, y, z);
}

void Shader::set(const char* name, float x, float y, float z, float w) {
    glUniform4f(getUniformLocation(name), x, y, z, w);
}

void Shader::set(const char* name, const glm::vec2& value) {
    glUniform2fv(getUniformLocation(name), 1, &value[0]);
}

void Shader::set(const char* name, const glm::vec3& value) {
    glUniform3fv(getUniformLocation(name), 1, &value[0]);
}

void Shader::set(const char* name, const glm::vec4& value) {
    glUniform4fv(getUniformLocation(name), 1, &value[0]);
}

void Shader::set(const char* name, const glm::mat2& value) {
    glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

void Shader::set(const char* name, const glm::mat3& value) {
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

void Shader::set(const char* name, const glm::mat4& value) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

int32_t Shader::getUniformLocation(const char* name) {
    auto it = m_uniformLocationCache.find(name);
    if (it != m_uniformLocationCache.end()) {
        return it->second;
    }
    int32_t location = glGetUniformLocation(m_id, name);
    m_uniformLocationCache[name] = location;
    return location;
}

std::string Shader::loadFile(const char* path) {
    std::ifstream file;
    file.open(path);
    if (!file.is_open()) {
        std::cerr << "Failed to read shader: " << path << '\n';
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();
    file.close();
    return ss.str();
}

void Shader::checkCompileErrors(uint32_t shader, const std::string& type) {
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
        std::cerr << type << " shader compilation error: " << infoLog << '\n';
    }
}

void Shader::checkLinkErrors(uint32_t program) {
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(program, 1024, nullptr, infoLog);
        std::cerr << "Shader program linkage error: " << infoLog << '\n';
    }
}
