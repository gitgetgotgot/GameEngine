#pragma once
#include <Rendering/Interfaces/VertexBuffer.h>
#include <glad/glad.h>
//OpenGL VBO

class OpenGL_VertexBuffer : public VertexBuffer {
public:
	OpenGL_VertexBuffer();
	~OpenGL_VertexBuffer();
	void bind() override;
	void unbind() override;
	void set_data(const void* data, size_t data_size, int usage) override;
	void update_data(const void* data, size_t data_size, size_t offset = 0) override;
private:
	GLuint id;
};