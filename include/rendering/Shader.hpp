#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <glm/fwd.hpp>

class Shader {
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    void use() const;

    void set(const char* name, bool value);
    void set(const char* name, int value);
    void set(const char* name, float value);
    void set(const char* name, float x, float y);
    void set(const char* name, float x, float y, float z);
    void set(const char* name, float x, float y, float z, float w);
    void set(const char* name, const glm::vec2& value);
    void set(const char* name, const glm::vec3& value);
    void set(const char* name, const glm::vec4& value);
    void set(const char* name, const glm::mat2& value);
    void set(const char* name, const glm::mat3& value);
    void set(const char* name, const glm::mat4& value);
private:
    uint32_t m_id = 0;
    std::unordered_map<const char*, int32_t> m_uniformLocationCache;

    int32_t getUniformLocation(const char* name);

    static std::string loadFile(const char* path);
    static void checkCompileErrors(uint32_t shader, const std::string& type);
    static void checkLinkErrors(uint32_t program);
};
