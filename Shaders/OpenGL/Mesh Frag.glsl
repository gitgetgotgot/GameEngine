#version 460 core
#extension GL_ARB_bindless_texture : enable
#extension GL_ARB_gpu_shader_int64 : enable

out vec4 fragColor;
in vec2 uv;
in vec3 normal;
flat in uint material_ID;

struct MaterialData {
	uint materialID;
	uint albedoID;
	uint normalID;
	uint roughnessID;
	uint metallicID;
};

layout (std430, binding = 3) buffer TextureHandles {
	uint64_t texHandles[];
};

layout (std430, binding = 4) buffer MaterialsData {
	MaterialData materials_data[];
};

//light test
uniform vec3 lightDir = normalize(vec3(-1.0, -1.0, -1.0));

void main()
{
	MaterialData mat = materials_data[material_ID];
	vec4 texColor = texture(sampler2D(texHandles[mat.albedoID]), uv);
	if(texColor.a == 0.0) discard;

	vec3 N = normalize(normal);
	vec3 L = normalize(-lightDir);
	float diff = max(max(dot(N, L), 0.0), 0.1);
	fragColor = vec4(texColor.rgb * diff, texColor.a);
}