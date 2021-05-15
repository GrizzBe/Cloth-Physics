#version 460 core
in vec3 FragTexCoord;

out vec4 color;
uniform samplerCube cubeSampler;

void main()
{
	color = texture(cubeSampler, FragTexCoord);
}