#version 460 core

in vec3 FragPos;
in vec3 FragNormal;
in vec2 FragTexCoords;

out vec4 FinalColor;

uniform sampler2D ImageTexture;
uniform float ambientStr 		= 0.1f;
uniform vec3 ambientColor 		= vec3(1.0f, 1.0f, 1.0f);
uniform vec3 lightColor 		= vec3(1.0f, 1.0f, 1.0f);
uniform vec3 lightPos 			= vec3(-2.0f, 6.0f, 3.0f);

uniform vec3 camPos;
uniform float lightSpecStr 		= 1.0f;
uniform float shininess 		= 32.0f;


void main()
{
	// Ambient
	vec3 ambient = ambientStr * ambientColor;
	
	// Light Direction
	vec3 norm = normalize(FragNormal);
	vec3 lightDir = normalize(FragPos - lightPos);
	
	// Diffuse Coloring
	float diffuseStr = max(dot(norm, -lightDir), 0.0f);
	vec3 diffuse = diffuseStr * lightColor;
	
	// Specular Highlight
	vec3 negViewDir = normalize(camPos - FragPos);
	vec3 reflectDir = reflect(lightDir, norm);
	float spec = pow(max(dot(negViewDir, reflectDir), 0.0f), shininess);
	vec3 specular = lightSpecStr * spec * lightColor;
	
	FinalColor = vec4(ambient + diffuse + specular, 1.0f) * (texture(ImageTexture, FragTexCoords));
}