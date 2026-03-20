#include "OpenGL_VertexArray.h"

OpenGL_VertexArray::OpenGL_VertexArray() {
	glGenVertexArrays(1, &id);
}

void OpenGL_VertexArray::bind() {
	glBindVertexArray(id);
}

void OpenGL_VertexArray::unbind() {
	glBindVertexArray(0);
}

void OpenGL_VertexArray::add_attribute(VertexAttribute attrib, std::unique_ptr<VertexBuffer>& vb) {
	vb->bind();
	glVertexAttribPointer(attrib.index, attrib.size, GL_FLOAT, GL_FALSE, attrib.stride, (const void*)attrib.offset);
	glEnableVertexAttribArray(attrib.index);
	vb->unbind();
}

OpenGL_VertexArray::~OpenGL_VertexArray() {
	glDeleteVertexArrays(1, &id);
}
