#version 460 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoords;

out vec3 FragPos;
out vec3 FragNormal;
out vec2 FragTexCoords;
out vec4 FragPosLightSpace; 

uniform float Speed = 2.0f;
uniform float Amplitude = 1.0f;
uniform float Distance = 0.01f;

uniform mat4 PVM;
uniform mat4 ModelMat;
uniform float Time;
uniform mat4 LightVPMatrix; 

void main()
{
	
	//Position.x += sin(Time * Speed + Position.y * Amplitude) * Distance;
	
	vec4 WarpedPos = vec4(Position.x + sin(Time * Speed + Position.y * Amplitude) * Distance, 
	Position.y + sin(Time * Speed * 0.3f + Position.y * Amplitude) * Distance * 10.0f, 
	Position.z + sin(Time * 0.7f * Speed + Position.y * Amplitude) * Distance, 1.0f);
	
	gl_Position = PVM * WarpedPos;
	
	FragPos = vec3(ModelMat * WarpedPos);
	
	FragNormal = mat3(transpose(inverse(ModelMat))) * Normal;
	FragTexCoords = TexCoords;
	
	
	// Shadow
	FragPosLightSpace = LightVPMatrix * vec4(FragPos, 1.0f);
}