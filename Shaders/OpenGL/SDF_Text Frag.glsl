#version 460 core
#extension GL_ARB_bindless_texture : enable
#extension GL_ARB_gpu_shader_int64 : enable

out vec4 fragColor;
in vec2 texCoords;
in vec4 textColor;
flat in uint texture_ID;

layout (std430, binding = 3) buffer TextureHandles {
	uint64_t texHandles[];
};

void main()
{
	//if(texture(tex0, texCoords).a < 0.1) discard;

	//vec4 color = texture(tex0, texCoords);
	//if(color.r == 0.0f) discard;
	//fragColor = vec4(color.r, 0.f, 0.f, 1.f);

	//options
	float thickness = 1.f;
	float bias = -0.15f;

	vec3 glowColor = vec3(0.9, 0.9, 1.0); // цвет свечения
	float glowSize = 0.3;                // радиус свечения
	float glowIntensity = 1.0;            // яркость свечения

	float outlineWidth = 0.2f;
	vec3 outlineColor = vec3(0.0, 0.0, 0.0);            //outline color
	
	vec2 shadowOffset = vec2(0.003, 0.003); // смещение тени
	vec3 shadowColor = vec3(0.0, 0.0, 0.0);  // цвет тени
	float shadowAlpha = 0.5;                 // прозрачность тени

	sampler2D tex0 = sampler2D(texHandles[texture_ID]);

	float dist = texture(tex0, texCoords).r - 0.5f;
	float w = fwidth(dist);
	float alpha = smoothstep(-w * thickness, w * thickness, dist + bias);

	
	// контур: проверяем диапазон вокруг границы
	float outline = 1.0 - smoothstep(outlineWidth - w, outlineWidth + w, abs(dist + bias));
	// итоговый цвет
	vec3 finalColor = mix(outlineColor, textColor.rgb, alpha);
	fragColor = vec4(finalColor, max(alpha, outline));
	//fragColor = vec4(textColor.rgb, alpha);

	/*

	// тень (смещённые координаты)
    float sdfShadow = texture(tex0, texCoords + shadowOffset).r;
    float distShadow = sdfShadow - 0.5;
    float wShadow = fwidth(distShadow);
    float alphaShadow = smoothstep(-wShadow, wShadow, distShadow);

	// итоговый цвет: сначала тень, потом текст
    vec3 color = mix(shadowColor, textColor.rgb, alpha);
    float finalAlpha = max(alpha, alphaShadow * shadowAlpha);

    fragColor = vec4(color, finalAlpha);
	*/
}
