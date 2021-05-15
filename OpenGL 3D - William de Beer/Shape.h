#ifndef __SHAPE_H__
#define __SHAPE_H__
// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   Shape.h
//  Description :   Shape class.
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // Library Includes 
#include <glew.h>
#include <freeglut.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
 // Local Includes
#include "Camera.h"
#include "ShaderLoader.h"
#include "InputHandle.h"
 // Implementation
struct Position
{
	float x;
	float y;
	float z;
};
class CShape
{
public:
	CShape();
	virtual ~CShape();

	virtual bool Initialise(glm::vec2 _pos, glm::vec2 _size, float _frames);
	virtual void Draw(GLuint _program, CCamera* _cam);
	virtual void Process(); 
	void SetTexture(std::string _tex);
protected:
	GLfloat* m_Vertices;
	GLuint* m_Indices;
	GLuint m_VAO;
	GLuint m_Texture;

	glm::vec3 ObjScale;
	glm::mat4 ScaleMat;

	const glm::vec3 WorldRotationAxis_X = glm::vec3(1.0f, 0.0f, 0.0f);
	const glm::vec3 WorldRotationAxis_Y = glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 WorldRotationAxis_Z = glm::vec3(0.0f, 0.0f, 1.0f);

	float ObjRotationAngle;
	glm::mat4 RotationMat;

	glm::vec3 ObjPosition;
	glm::mat4 TranslationMat;

	glm::mat4 ModelMat;

	unsigned int m_uiVertArraySize;
	unsigned int m_uiIndiArraySize;
	glm::vec2 Pos;
	glm::vec2 Size;
};

#endif
