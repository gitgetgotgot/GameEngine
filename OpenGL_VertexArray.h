#pragma once
#include "glad/glad.h"
#include "VertexArray.h"
//OpenGL VAO

class OpenGL_VertexArray : public VertexArray {
public:
	OpenGL_VertexArray();
	~OpenGL_VertexArray();
	void bind() override;
	void unbind() override;
	void add_attribute(VertexAttribute attrib, std::unique_ptr<VertexBuffer>& vb) override;
private:
	GLuint id;
};