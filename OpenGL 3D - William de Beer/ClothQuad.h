#ifndef __CLOTH_QUAD_H__
#define __CLOTH_QUAD_H__

// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   ClothQuad.h
//  Description :   Quad object.
//  Author      :   William de Beer 
//  Mail        :   William.Beer@mds.ac.nz
// 
 // Library Includes 
#include <glew.h>
#include <freeglut.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
 // Local Includes 
#include "CubeMap.h"
#include "Camera.h"
#include "ClothNode.h"
#include "Object.h"
#include "LightManager.h"
#include "ShaderLoader.h"
 // Implementation 
class ClothQuad : public CObject
{
public:
	ClothQuad(int _width, int _height, ClothNode** _nodes);
	virtual ~ClothQuad();

	virtual void Render(GLuint _program, CCamera* _cam);
	virtual void Update(float _dT);
	void CreateVertices();
	void CreateIndices();
	void CreateVAO();
	void DestroySection(int _pos);
private:
	GLuint EBO; // Indices
	GLuint VBO; // Vertices

	ClothNode** m_ClothNodes;

	int m_Width;
	int m_Height;

	GLfloat* m_Vertices;
	GLuint* m_Indices;

	std::vector<glm::uvec3> m_AllIndices;

};
#endif 
