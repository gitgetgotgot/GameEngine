#version 460 core

out vec4 fragColor;
in vec2 texCoords;

layout (binding = 0) uniform sampler2D tex0;

void main()
{
	if(texture(tex0, texCoords).a < 0.1) discard;

	fragColor = texture(tex0, texCoords);

	//vec3 texColor = fragColor.xyz;
	//vec3 result = texColor * global_light + texColor * outLight.xyz * outLight.w;
	//result = clamp(result, 0.0, 1.0);

	//fragColor = vec4(result, 1.0);

	//fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}