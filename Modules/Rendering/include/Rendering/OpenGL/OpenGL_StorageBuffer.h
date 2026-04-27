#pragma once
#include <Rendering/Interfaces/StorageBuffer.h>
#include <glad/glad.h>
//OpenGL SSBO

class OpenGL_StorageBuffer : public StorageBuffer {
public:
    OpenGL_StorageBuffer();
    ~OpenGL_StorageBuffer();
    void setData(const void* data, size_t size) override;
    void updateData(const void* data, size_t size, size_t offset = 0) override;
    void bind(uint32_t slot) override;
private:
    GLuint id;
};