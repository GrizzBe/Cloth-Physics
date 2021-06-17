#ifndef __BUTTON_H__
#define __BUTTON_H__
// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   Button.h
//  Description :   Button that can be pressed.
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // Local Includes
#include "SceneManager.h"
#include "Shape.h"
#include "Text.h"
 // Implementation 
class CButton : public CShape
{
public:
	CButton();
	virtual ~CButton();
	virtual bool Initialise(glm::vec2 _pos, glm::vec2 _size, float _frames);
	virtual void Draw(GLuint _program, CCamera* _cam);
	virtual void Process();

	bool ActivateButton();
	
private:
	float m_fOffset;
	float m_fFrameWidth;
};

#endif