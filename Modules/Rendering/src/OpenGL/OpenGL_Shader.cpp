#include <Rendering/OpenGL/OpenGL_Shader.h>
#include <iostream>

OpenGL_Shader::OpenGL_Shader(ShaderDesc desc) {
    std::string vertexContents = get_file_contents(desc.vertexEntry.c_str());
    std::string fragmentContents = get_file_contents(desc.fragmentEntry.c_str());

    const char* vertexSource = vertexContents.c_str();
    const char* fragmentSource = fragmentContents.c_str();

    auto compileShader = [&](GLenum type, const char* src) {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        GLint success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success) {
            GLint length = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

            std::string info(length, '\0');
            glGetShaderInfoLog(shader, length, nullptr, info.data());

            std::cerr << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT")
                << " SHADER ERROR:\n" << info << std::endl;
        }

        return shader;
        };

    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);

    GLint success = 0;
    glGetProgramiv(id, GL_LINK_STATUS, &success);

    if (!success) {
        GLint length = 0;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);

        std::string info(length, '\0');
        glGetProgramInfoLog(id, length, nullptr, info.data());

        std::cerr << "PROGRAM LINK ERROR:\n" << info << std::endl;
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
