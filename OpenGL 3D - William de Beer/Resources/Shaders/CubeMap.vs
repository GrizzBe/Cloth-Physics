#version 460 core
layout (location = 0) in vec3 position;

out vec3 FragTexCoord;
uniform mat4 PVM;

void main()
{
	gl_Position = PVM * vec4(position, 1.0);
	FragTexCoord = position;
}