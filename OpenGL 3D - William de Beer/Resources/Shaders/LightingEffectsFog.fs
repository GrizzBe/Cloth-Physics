#version 460 core

in vec3 FragPos;
in vec3 FragNormal;
in vec2 FragTexCoords;
in vec4 FragPosLightSpace;

out vec4 FinalColor;

uniform sampler2D ImageTexture;
uniform sampler2D ShadowMap;

uniform float ambientStr 		= 0.05f;
uniform vec3 ambientColor 		= vec3(1.0f, 0.7f, 0.2f);
uniform vec3 lightColor 		= vec3(1.0f, 0.8f, 0.8f);
uniform vec3 lightPos 			= vec3(0.0f, 6.0f, 0.0f);

uniform vec3 camPos;
uniform float lightSpecStr 		= 1.0f;
uniform float shininess 		= 32.0f;

uniform int rimExponent = 4;
uniform vec3 rimColor = vec3(1.0f, 0.5f, 0.2f);

struct LightSource
{
	vec3 lightColor;
	vec3 lightPos;
	float lightSpecStr;
};

uniform LightSource lights[5];

uniform vec4 vFogColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);

float ShadowCalculation(vec4 fragPosLightSpace)
{
	// To get NDC [-1, 1] from screenspace
	vec3 ndcSpace = fragPosLightSpace.xyz / fragPosLightSpace.w;
	
	// Convert to Tex Coord Space [0,1]
	vec3 texCoordSpace = 0.5f * ndcSpace + 0.5f;
	
	float currentDepth = texCoordSpace.z ;
	
	float closestDepth = texture(ShadowMap, texCoordSpace.xy).r;
	
	float shadow = closestDepth < currentDepth ? 0.0 : 1.0;
	return shadow;
}

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
	
	// Lighting
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
			
	}
	
	// Fog
	float d = distance(FragPos.xyz, camPos);
	float lerp =(d - 5.0f)/10.0f;
	lerp = clamp(lerp,0.0f,1.0f);
	
	// Shadow
	//float shadow = ShadowCalculation(FragPosLightSpace);
	// To get NDC [-1, 1] from screenspace
	vec3 ndcSpace = FragPosLightSpace.xyz / FragPosLightSpace.w;
	
	// Convert to Tex Coord Space [0,1]
	vec3 texCoordSpace = 0.5f * ndcSpace + 0.5f;
	
	float currentDepth = texCoordSpace.z ;
	
	float closestDepth = texture(ShadowMap, texCoordSpace.xy).r;
	
	float shadow = closestDepth < currentDepth ? 0.0 : 1.0;
	
	// Final 
	FinalColor = mix(vec4(ambient + (shadow) * (diffuse + specular + rim), 1.0f) * texture(ImageTexture, FragTexCoords),vFogColor, lerp);
}
