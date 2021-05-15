#version 460 core

in vec3 FragPos;
in vec3 FragNormal;
in vec2 FragTexCoords;

out vec4 FinalColor;

uniform sampler2D ImageTexture;

uniform float ambientStr 		= 0.05f;
uniform vec3 ambientColor 		= vec3(1.0f, 0.7f, 0.2f);
uniform vec3 lightColor 		= vec3(1.0f, 0.8f, 0.8f);
uniform vec3 lightPos 			= vec3(0.0f, 6.0f, 0.0f);

uniform vec3 camPos;
uniform float lightSpecStr 		= 1.0f;
uniform float shininess 		= 32.0f;

uniform int rimExponent = 4;
uniform vec3 rimColor = vec3(1.0f, 0.5f, 0.2f);

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
	
	// Rim Lighting
	float rimFactor = 1.0 - dot(norm, negViewDir); // Calculate the rim factor.
	rimFactor = smoothstep(0.0, 1.0, rimFactor);
	rimFactor = pow(rimFactor, rimExponent);
	vec3 rim = rimFactor * rimColor * lightColor;
	
	FinalColor = vec4(ambient + diffuse + specular + rim, 1.0f) * texture(ImageTexture, FragTexCoords);
}