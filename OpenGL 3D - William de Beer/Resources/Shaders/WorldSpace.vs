#version 460 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Color;
layout (location = 2) in vec2 TexCoords;

out vec3 FragColor;
out vec2 FragTexCoords;

uniform mat4 ModelMat;
uniform float Offset;

void main()
{
	gl_Position = ModelMat * vec4(Position, 1.0f);
	FragColor = Color;
	FragTexCoords = TexCoords;
}