#version 460 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 _normal;
layout (location = 2) in vec2 _uv;

struct SubMeshInstanceData {
	mat4 modelMatrix;  // 64 bytes
	uint albedoID;     // 4 bytes
	uint normalID;     // 4 bytes
	uint roughnessID;  // 4 bytes
	uint metallicID;   // 4 bytes
};

layout (std140, binding = 1) uniform UBO {
    mat4 projectionMatrix;
	mat4 viewMatrix;
};

layout (std430, binding = 1) buffer MeshInstanceData {
	SubMeshInstanceData submeshesData[];
};

out vec2 uv;
out vec3 normal;
flat out uint albedo_ID;

void main()
{
	mat4 model = submeshesData[gl_InstanceID + gl_BaseInstance].modelMatrix;
	vec4 worldPos = model * vec4(position, 1.0);
	gl_Position = projectionMatrix * viewMatrix * worldPos;
	uv = _uv;
	//normal = _normal;
	normal = normalize(mat3(model) * _normal);
	albedo_ID = submeshesData[gl_InstanceID + gl_BaseInstance].albedoID;
}