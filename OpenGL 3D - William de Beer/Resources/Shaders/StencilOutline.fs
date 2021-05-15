#version 460 core

in vec3 FragPos;
in vec3 FragNormal;
in vec2 FragTexCoords;

out vec4 FinalColor;

uniform vec3 camPos;

uniform vec3 albedo = vec3(1.0f,0.0f,0.0f);
uniform vec4 vFogColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);

void main()
{
	// Fog
	float d = distance(FragPos.xyz, camPos);
	float lerp =(d - 5.0f)/10.0f;
	lerp = clamp(lerp,0.0f,1.0f);
	
	FinalColor = mix(vec4(albedo, 1.0f), vFogColor, lerp);
}


