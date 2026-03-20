#pragma once
#include "glad/glad.h"
#include "UniformBuffer.h"
//OpenGL UBO

class OpenGL_UniformBuffer : public UniformBuffer {
public:
    OpenGL_UniformBuffer();
    ~OpenGL_UniformBuffer();
    void setData(const void* data, size_t size) override;
    void updateData(const void* data, size_t size) override;
    void bind(uint32_t slot) override;
private:
    GLuint id;
};