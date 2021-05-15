#version 460 core

in vec3 FragColor;
in vec2 FragTexCoords;

uniform float CurrentTime;
uniform sampler2D ImageTexture;
uniform sampler2D ImageTexture2;

out vec4 FinalColor;

void main()
{
	FinalColor = mix(texture(ImageTexture, FragTexCoords),texture(ImageTexture2, FragTexCoords), (sin(CurrentTime) + 1)/2);
}