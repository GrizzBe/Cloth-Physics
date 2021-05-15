// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   Shape.cpp
//  Description :   Shape class
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // This Include
#include "Shape.h"
 // Library Includes
#include "Utilities.h"
 // Implementation
CShape::CShape()
{
	ObjRotationAngle = 0;
	m_Vertices = 0;
	m_Indices = 0;
	m_Texture = 0;
	m_uiIndiArraySize = 0;
	m_uiVertArraySize = 0;
	Pos.x = 0;
	Pos.y = 0;

	m_VAO = 0;
}

CShape::~CShape()
{
	delete [] m_Vertices;
	m_Vertices = 0;

	delete[] m_Indices;
	m_Indices = 0;
}

bool CShape::Initialise(glm::vec2 _pos, glm::vec2 _size, float _frames)
{
	Pos = _pos;

	float frameWidth = 1.0f / _frames;
	float screenRatio = CUtilities::GetInstance().GetScreenRatio();

	Size = glm::vec2((_size.x), (_size.y * (screenRatio)));

	glm::vec2 halfSize = glm::vec2((_size.x), (_size.y * (screenRatio)));

	GLfloat Vertices_Quad[] = {
		// Position					// Colour
		Size.x,	  Size.y,	 0.0f,		1.0f,	0.0f,	1.0f,		frameWidth, 0.0f,		// Top Right
		-Size.x,  Size.y,	 0.0f,		1.0f,	1.0f,	0.0f,		0.0f, 0.0f,		// Top Left
		-Size.x, -Size.y,	 0.0f,		1.0f,	0.0f,	1.0f,		0.0f, 1.0,		// Bottom Left
		Size.x,	 -Size.y,	 0.0f,		0.0f,	1.0f,	1.0f,		frameWidth, 1.0,		// Bottom Right
	};


	GLuint Indices_Quad[] = {
		0, 1, 2,
		0, 2, 3,
	};


	// Get size
	m_uiVertArraySize = sizeof(Vertices_Quad) / sizeof(GLfloat);
	m_uiIndiArraySize = sizeof(Indices_Quad) / sizeof(GLuint);

	m_Vertices = new GLfloat[m_uiVertArraySize];
	m_Indices = new GLuint[m_uiIndiArraySize];

	for (unsigned int i = 0; i < m_uiVertArraySize; i++)
	{
		m_Vertices[i] = Vertices_Quad[i];
	}

	for (unsigned int i = 0; i < m_uiIndiArraySize; i++)
	{
		m_Indices[i] = Indices_Quad[i];
	}

	ObjPosition = glm::vec3(Pos, 0);
	ObjScale = glm::vec3(1.0f, 1.0f, 1.0f);

	// Create VAO
	m_VAO = ShaderLoader::GetInstance().CreateVAO(m_Vertices, m_Indices, sizeof(Vertices_Quad), sizeof(Indices_Quad));

	return true;
}

void CShape::Draw(GLuint _program, CCamera* _cam)
{
	glUseProgram(_program);

	GLuint ModelLoc = glGetUniformLocation(_program, "ModelMat");
	glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(ModelMat));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
	glUniform1i(glGetUniformLocation(_program, "ImageTexture"), 0);

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_uiIndiArraySize, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glUseProgram(0);
}

void CShape::Process()
{
	ScaleMat = glm::scale(glm::mat4(), ObjScale);

	// Transformation of object
	RotationMat = glm::rotate(glm::mat4(), glm::radians(ObjRotationAngle), WorldRotationAxis_Z);
	TranslationMat = glm::translate(glm::mat4(), ObjPosition);

	// Set model matrix
	ModelMat = TranslationMat * RotationMat * ScaleMat;
}

void CShape::SetTexture(std::string _tex)
{
	m_Texture = ShaderLoader::GetInstance().CreateTexture(_tex.c_str());
}
