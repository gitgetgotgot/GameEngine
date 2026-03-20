#version 460 core
layout (location = 0) in vec3 position;

layout (std140, binding = 0) uniform UBO {
    mat4 projectionMatrix;
	mat4 viewMatrix;
};

struct SpriteInstance {
	mat4 modelMatrix;
	vec2 UV[4];
};

layout (std430, binding = 0) buffer SpriteInstanceData {
	SpriteInstance spritesData[];
};

out vec2 texUV;

void main()
{
	mat4 model = spritesData[gl_InstanceID].modelMatrix;
	gl_Position = projectionMatrix * viewMatrix * model * vec4(position, 1.0);
	texUV = spritesData[gl_InstanceID].UV[gl_VertexID % 4];
}