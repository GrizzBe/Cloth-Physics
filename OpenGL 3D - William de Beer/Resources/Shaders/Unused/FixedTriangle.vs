#version 460 core

void main()
{
	const vec3 vertices[] = {	vec3(	 0.0f,	0.5f,  0.0f),
								vec3(	-0.5f, -0.5f,  0.0f),
								vec3(	 0.5f, -0.5f,  0.0f)};
								
	gl_Position = vec4(vertices[gl_VertexID], 1.0f);
}