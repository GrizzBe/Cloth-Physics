#version 460 core

in vec3 FragPos;
in vec3 FragNormal;
in vec2 FragTexCoords;

out vec4 FinalColor;

uniform sampler2D ImageTexture;
uniform float ambientStr 		;
uniform vec3 ambientColor 		;
//uniform vec3 lightColor 		;
//uniform vec3 lightPos 			;

uniform vec3 albedo = vec3(1.0f,1.0f,1.0f);
uniform float reflectivity = 0.6f;

uniform vec3 camPos;
uniform float lightSpecStr 		= 1.0f;
uniform float shininess 		= 32.0f;

uniform int rimExponent = 2;
uniform vec3 rimColor = vec3(1.0f, 0.3f, 0.2f);

uniform samplerCube skybox;

struct LightSource
{
	vec3 lightColor;
	vec3 lightPos;
	float lightSpecStr;
};

uniform LightSource lights[5];

void main()
{
	// Reflect
	vec3 norm = normalize(FragNormal);
	vec3 viewDir = normalize(FragPos - camPos);
	vec3 reflectDir = reflect(viewDir, norm);

	vec3 diffuse = vec3(0,0,0);
	vec3 specular = vec3(0,0,0);

	// Ambient
	vec3 ambient = ambientStr * ambientColor;
	
	// Negative View Direction
	vec3 negViewDir = normalize(camPos - FragPos);
	
	// Rim Lighting
	float rimFactor = 1.0 - dot(norm, negViewDir); // Calculate the rim factor.
	rimFactor = smoothstep(0.0, 1.0, rimFactor);
	rimFactor = pow(rimFactor, rimExponent);
	vec3 rim = rimFactor * rimColor;
	vec3 outRim;
	
	for (int i = 0; i < 5; i++)
	{
			// Light Direction
			vec3 lightDir = normalize(FragPos - lights[i].lightPos);
			
			// Diffuse Coloring
			float diffuseStr = max(dot(norm, -lightDir), 0.0f);
			diffuse += diffuseStr * lights[i].lightColor;
			
			// Specular Highlight
			vec3 reflectDirSpec = reflect(lightDir, norm);
			float spec = pow(max(dot(negViewDir, reflectDirSpec), 0.0f), shininess);
			specular += lights[i].lightSpecStr * spec * lights[i].lightColor;
			

			// Add color to rim lighting
			outRim += rim * lights[i].lightColor; 
	}
	
	FinalColor = vec4(ambient + diffuse + specular + rim, 1.0f) * (mix(vec4(texture(skybox, reflectDir).rgb, 1.0f), vec4(albedo, 1.0f), reflectivity));
	//texture(ImageTexture, FragTexCoords)
}


