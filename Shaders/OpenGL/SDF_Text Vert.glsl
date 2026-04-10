#version 460 core
layout (location = 0) in vec3 position;

out vec2 texCoords;
out vec4 textColor;
flat out uint texture_ID;

layout (std140, binding = 2) uniform UBO {
    mat4 projectionMatrix;
	mat4 viewMatrix;
};

struct UI_Instance {
	mat4 modelMatrix;
	vec2 UV[4];
	vec4 color;
	uint texture_id;
};

layout (std430, binding = 5) buffer UI_InstanceData {
	UI_Instance uiData[];
};

void main()
{
	mat4 model = uiData[gl_InstanceID].modelMatrix;
	gl_Position = projectionMatrix * viewMatrix * model * vec4(position, 1.0);
	texCoords = uiData[gl_InstanceID].UV[gl_VertexID % 4];
	textColor = uiData[gl_InstanceID].color;
	texture_ID = uiData[gl_InstanceID].texture_id;
}