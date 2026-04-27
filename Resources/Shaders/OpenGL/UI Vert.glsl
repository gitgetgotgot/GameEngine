#version 460 core
layout (location = 0) in vec3 position;

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

layout (std430, binding = 2) buffer UI_InstanceData {
	UI_Instance uiData[];
};

out vec2 texUV;
out vec4 _color;
flat out uint texture_ID;

void main()
{
	mat4 model = uiData[gl_InstanceID].modelMatrix;
	gl_Position = projectionMatrix * viewMatrix * model * vec4(position, 1.0);
	texUV = uiData[gl_InstanceID].UV[gl_VertexID % 4];
	_color = uiData[gl_InstanceID].color;
	texture_ID = uiData[gl_InstanceID].texture_id;
}