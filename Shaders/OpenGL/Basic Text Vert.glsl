#version 460 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec4 color;

out vec2 texCoords;
out vec4 textColor;

layout(std140, binding = 2) uniform UBO {
    mat4 projectionMatrix;
	mat4 viewMatrix;
};

void main()
{
	gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.0);
	texCoords = tex;
	textColor = color;
}