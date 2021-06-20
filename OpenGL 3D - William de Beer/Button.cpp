// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   Button.cpp
//  Description :   Button player can press
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // This Include 
#include "Button.h"

#include "Utilities.h"
 // Implementation 
CButton::CButton()
{
	m_fOffset = 0.0f;
	m_fFrameWidth = 0.5f;
}

CButton::~CButton()
{

}

/***********************
* Initialise: Initialises button object.
* @author: William de Beer
* @parameter: x, y and z position
********************/
bool CButton::Initialise(glm::vec2 _pos, glm::vec2 _size, float _frames)
{
	CShape::Initialise(_pos, _size, _frames);

	return true;
}

/***********************
* Draw: Renders quad
* @author: William de Beer
* @parameter: Program, view matrix and projection matrix
********************/
void CButton::Draw(GLuint _program, CCamera* _cam)
{
	// Draw button
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

/***********************
* Process: Updates quad variables
* @author: William de Beer
********************/
void CButton::Process()
{
	CShape::Process();

	float screenWidth = CUtilities::GetInstance().GetResolution().x;
	float screenHeight = CUtilities::GetInstance().GetResolution().y;

	float MouseX = (float)CInputHandle::GetInstance().GetMouseX() - screenWidth / 2;
	float MouseY = (float)CInputHandle::GetInstance().GetMouseY() - screenHeight / 2;



	float BoundryX = (CUtilities::GetInstance().GetResolution().x * Size.x) / 2;
	float BoundryY = (CUtilities::GetInstance().GetResolution().y * Size.y) / 2;

	if ((MouseX > (ObjPosition.x * screenWidth / 2) - BoundryX) &&
		(MouseX < (ObjPosition.x * screenWidth / 2) + BoundryX) &&
		(MouseY > (ObjPosition.y * -screenHeight / 2) - BoundryY) &&
		(MouseY < (ObjPosition.y * -screenHeight / 2) + BoundryY))
	{
		m_fOffset = m_fFrameWidth;
	}
	else
	{
		m_fOffset = 0;
	}
}

/***********************
* ActiveButton: Activate button function.
* @author: William de Beer
********************/
bool CButton::ActivateButton()
{
	float screenWidth = CUtilities::GetInstance().GetResolution().x;
	float screenHeight = CUtilities::GetInstance().GetResolution().y;

	// Get mouse position relative to center of the screen
	float MouseX = (float)CInputHandle::GetInstance().GetMouseX() - screenWidth / 2;
	float MouseY = (float)CInputHandle::GetInstance().GetMouseY() - screenHeight / 2;

	float BoundryX = (CUtilities::GetInstance().GetResolution().x * Size.x) / 2;
	float BoundryY = (CUtilities::GetInstance().GetResolution().y * Size.y) / 2;

	// Local bounds for button
	if ((MouseX > (ObjPosition.x * screenWidth / 2) - BoundryX) &&
		(MouseX < (ObjPosition.x * screenWidth / 2) + BoundryX) &&
		(MouseY > (ObjPosition.y * -screenHeight / 2) - BoundryY) &&
		(MouseY < (ObjPosition.y * -screenHeight / 2) + BoundryY))
	{
		if (CInputHandle::GetInstance().GetMouseButtonState(GLUT_LEFT_BUTTON) == InputState::Input_Down)
		{
			return true;
		}
	}
	return false;
}


