#version 460 core

in vec3 FragColor;
in vec2 FragTexCoords;

uniform sampler2D ImageTexture;

out vec4 FinalColor;

void main()
{
	FinalColor = mix(texture(ImageTexture, FragTexCoords), vec4(FragColor, 1.0f), 1.0f - FragColor.b);
}