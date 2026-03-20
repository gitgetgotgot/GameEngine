#version 460 core

out vec4 fragColor;
in vec2 texUV;

layout (binding = 0) uniform sampler2D tex0;

void main()
{
	vec4 texColor = texture(tex0, texUV);
	if(texColor.a == 0.0) discard;
	fragColor = texColor;
}