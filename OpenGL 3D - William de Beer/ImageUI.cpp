// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   ImageUI.cpp
//  Description :   Class used to display image.
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // This Include 
#include "ImageUI.h"
 // Implementation 

CImageUI::CImageUI()
{
	m_iCurrentFrame = 0;
	m_fOffset = 0;
	m_fFrameWidth = 0;
}

CImageUI::~CImageUI()
{
}

bool CImageUI::Initialise(glm::vec2 _pos, glm::vec2 _size, float _frames)
{
	CShape::Initialise(_pos, _size, _frames);

	m_fFrameWidth = 1.0f / _frames;

	return false;
}

void CImageUI::Draw(GLuint _program, CCamera* _cam)
{
	glUseProgram(_program);

	GLuint ModelLoc = glGetUniformLocation(_program, "ModelMat");
	glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(ModelMat));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
	glUniform1i(glGetUniformLocation(_program, "ImageTexture"), 0);

	glUniform1f(glGetUniformLocation(_program, "Offset"), m_fOffset);

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_uiIndiArraySize, GL_UNSIGNED_INT, 0);


	glBindVertexArray(0);
	glUseProgram(0);
}

void CImageUI::Process()
{
	CShape::Process();
	m_fOffset = m_iCurrentFrame * m_fFrameWidth; // Set image offset
}

void CImageUI::SetFrame(int _frame)
{
	m_iCurrentFrame = _frame;
}
