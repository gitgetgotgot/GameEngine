#include "OpenGL_Shader.h"
#include <iostream>

OpenGL_Shader::OpenGL_Shader(ShaderDesc desc) {
	std::string vertexContents = get_file_contents(desc.vertexEntry.c_str());
	std::string fragmentContetns = get_file_contents(desc.fragmentEntry.c_str());
	const char* vertexSource = vertexContents.c_str();
	const char* fragmentSource = fragmentContetns.c_str();

	GLint compile;

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compile);
	if (!compile) {
		GLchar info[1024];
		glGetShaderInfoLog(vertexShader, 1024, nullptr, info);
		std::cerr << "VERTEX SHADER ERROR: " << info << std::endl;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compile);
	if (!compile) {
		GLchar info[1024];
		glGetShaderInfoLog(fragmentShader, 1024, nullptr, info);
		std::cerr << "FRAGMENT SHADER ERROR: " << info << std::endl;
	}

	this->id = glCreateProgram();
	glAttachShader(this->id, vertexShader);
	glAttachShader(this->id, fragmentShader);
	glLinkProgram(this->id);

	glGetProgramiv(id, GL_LINK_STATUS, &compile);
	if (!compile) {
		GLchar info[1024];
		glGetShaderInfoLog(id, 1024, nullptr, info);
		std::cerr << "SHADER PROGRAM ERROR: " << info << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

OpenGL_Shader::~OpenGL_Shader() {
	glDeleteProgram(id);
}

void OpenGL_Shader::bind() {
	glUseProgram(id);
}
