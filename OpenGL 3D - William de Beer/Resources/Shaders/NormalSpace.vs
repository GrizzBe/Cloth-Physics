#version 460 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoords;

out vec3 FragPos;
out vec3 FragNormal;
out vec2 FragTexCoords;
out vec4 FragPosLightSpace; 

uniform mat4 PVM;
uniform mat4 ModelMat;
uniform mat4 LightVPMatrix; 

void main()
{
	gl_Position = PVM * vec4(Position, 1.0f);
	FragPos = vec3(ModelMat * vec4(Position, 1.0f));
	FragNormal = mat3(transpose(inverse(ModelMat))) * Normal;
	FragTexCoords = TexCoords;
	
	// Shadow
	FragPosLightSpace = LightVPMatrix * vec4(FragPos, 1.0f);
}