#version 460 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 TexCoords;

uniform mat4 LightVPMatrix;
uniform mat4 ModelMat;

void main()
{
	gl_Position = LightVPMatrix * ModelMat * vec4(Position, 1.0);
}