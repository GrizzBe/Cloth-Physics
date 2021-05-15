#pragma once
// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   ShaderLoader.h
//  Description :   Handles rendering.
//  Author      :   William de Beer, Callan Moore 
//  Mail        :   William.Beer@mds.ac.nz
//
 // Library Includes 
#include <glew.h>
#include <freeglut.h>
#include <iostream>
#include <map>
 // Implementation
class ShaderLoader
{
public:	
	static ShaderLoader& GetInstance();
	static void RemoveInstance();
	GLuint GetProgram(const char* VertexShaderFilename, const char* FragmentShaderFilename);
	GLuint CreateProgram(const char* VertexShaderFilename, const char* FragmentShaderFilename);
	GLuint CreateVAO(GLfloat* _vertices, GLuint* _indices, unsigned int _uiVertArraySize, unsigned int _uiIndiArraySize);
	GLuint CreateTexture(const char* _textureFilename);
private:
	ShaderLoader(void);
	~ShaderLoader(void);
	GLuint CreateShader(GLenum shaderType, const char* shaderName);
	std::string ReadShaderFile(const char *filename);
	void PrintErrorDetails(bool isShader, GLuint id, const char* name);

	static ShaderLoader* sm_Instance;
	std::map<std::string, GLuint> m_Map;
};
