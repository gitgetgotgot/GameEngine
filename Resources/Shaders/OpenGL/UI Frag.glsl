#version 460 core
#extension GL_ARB_bindless_texture : enable
#extension GL_ARB_gpu_shader_int64 : enable

out vec4 fragColor;
in vec2 texUV;
in vec4 _color;
flat in uint texture_ID;

layout (std430, binding = 3) buffer TextureHandles {
	uint64_t texHandles[];
};

void main()
{
	vec4 texColor = texture(sampler2D(texHandles[texture_ID]), texUV);
	if(texColor.a == 0.0) discard;
	fragColor = texColor * _color;
}