#version 460 core
in vec3 FragNormal;
in vec3 FragPos;
in vec2 FragTexCoord;

out vec4 FinalColor;

uniform vec3 camPos;
uniform sampler2D tex;
uniform samplerCube skybox;

void main()
{
	vec3 norm = normalize(FragNormal);
	vec3 viewDir = normalize(FragPos - camPos);
	vec3 reflectDir = reflect(viewDir, norm);
	
	FinalColor = vec4(texture(skybox, reflectDir).rgb, 1.0f);
}