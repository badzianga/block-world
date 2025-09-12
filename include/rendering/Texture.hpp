#pragma once
#include <cstdint>

class Texture {
public:
    explicit Texture(const char* filePath, bool flipY=false);
    ~Texture();

    void use() const;
private:
    uint32_t m_id = 0;
};
