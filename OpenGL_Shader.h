#pragma once
#include "Shader.h"
#include "glad/glad.h"

class OpenGL_Shader : public Shader {
public:
	OpenGL_Shader(ShaderDesc desc);
	~OpenGL_Shader();
	void bind() override;
private:
	GLuint id;
};
