#version 460 core

out vec4 fragColor;
in vec2 texCoords;
in vec4 textColor;

layout (binding = 0) uniform sampler2D tex0;

void main()
{
	//if(texture(tex0, texCoords).a < 0.1) discard;

	//vec4 color = texture(tex0, texCoords);
	//if(color.r == 0.0f) discard;
	//fragColor = vec4(color.r, 0.f, 0.f, 1.f);

	//options
	float thickness = 1.f;
	float bias = -0.15f;

	vec3 glowColor = vec3(0.9, 0.9, 1.0); // цвет свечени€
	float glowSize = 0.3;                // радиус свечени€
	float glowIntensity = 1.0;            // €ркость свечени€

	float outlineWidth = 0.3f;
	vec3 outlineColor = vec3(0.0, 0.0, 0.0);
	
	vec2 shadowOffset = vec2(0.003, 0.003); // смещение тени
	vec3 shadowColor = vec3(0.0, 0.0, 0.0);  // цвет тени
	float shadowAlpha = 0.5;                 // прозрачность тени

	float dist = texture(tex0, texCoords).r - 0.5f;
	dist *= 0.8f;
	float w = fwidth(dist);
	float fill = smoothstep(-w, w, dist);
	float outlineMask = smoothstep(-w - outlineWidth, w - outlineWidth, dist);

	// final color
	vec3 color = mix(outlineColor, textColor.rgb, fill);
	float alpha = max(fill, outlineMask);
	fragColor = vec4(color, alpha);

	/*
	// тень (смещЄнные координаты)
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
