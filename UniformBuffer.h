#pragma once
#include <cstdint>
#include "glm/glm.hpp"

struct BasicShaderUBData {
    glm::mat4 projection, view;
};

class UniformBuffer {
public:
    virtual void setData(const void* data, size_t size) = 0;
    virtual void updateData(const void* data, size_t size) = 0;
    virtual void bind(uint32_t slot) = 0;
    virtual ~UniformBuffer() {}
};