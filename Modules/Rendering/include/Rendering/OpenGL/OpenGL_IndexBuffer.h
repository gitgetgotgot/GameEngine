#pragma once
#include <Rendering/Interfaces/IndexBuffer.h>
#include <glad/glad.h>
//OpenGL EBO

class OpenGL_IndexBuffer : public IndexBuffer {
public:
	OpenGL_IndexBuffer();
	~OpenGL_IndexBuffer();
	void bind() override;
	void unbind() override;
	void set_data(const void* data, size_t data_size, int usage) override;
	void update_data(const void* data, size_t data_size, size_t offset = 0) override;
private:
	GLuint id;
};
