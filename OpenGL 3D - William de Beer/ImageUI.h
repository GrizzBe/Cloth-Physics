#ifndef __CIMAGEUI_H__
#define __CIMAGEUI_H__
// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   ImageUI.h
//  Description :   Class used to display image.
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // Local Includes 
#include "Shape.h"
 // Implementation 
class CImageUI : public CShape
{
public:
	CImageUI();
	virtual ~CImageUI();

	virtual bool Initialise(glm::vec2 _pos, glm::vec2 _size, float _frames);
	virtual void Draw(GLuint _program, CCamera* _cam);
	virtual void Process();
	void SetFrame(int _frame);
private:
	int m_iCurrentFrame;
	float m_fOffset;
	float m_fFrameWidth;
};

#endif // __CIMAGEUI_H__