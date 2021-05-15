#version 460 core

in vec3 FragColor;

uniform float CurrentTime;

out vec4 FinalColor;

void main()
{
	FinalColor = vec4(	FragColor.x * abs (sin(3 * CurrentTime)), 
						FragColor.y * abs (sin(5 * CurrentTime + 90)), 
						FragColor.z * abs (sin(7 * CurrentTime + 180)), 
						1.0f);
}