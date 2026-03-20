#version 460 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 UV;
layout (location = 3) in float materialID;

layout (std140, binding = 1) uniform UBO {
    mat4 projectionMatrix;
	mat4 viewMatrix;
};

struct MeshInstance {
	mat4 modelMatrix;
};

layout (std430, binding = 1) buffer MeshInstanceData {
	MeshInstance meshesData[];
};

out vec2 texUV;

void main()
{
	mat4 model = meshesData[gl_InstanceID].modelMatrix;
	gl_Position = projectionMatrix * viewMatrix * model * vec4(position, 1.0);
	texUV = UV;
	//normal
	//meterialID
}