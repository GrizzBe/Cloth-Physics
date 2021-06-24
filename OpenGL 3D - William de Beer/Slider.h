#ifndef __SLIDER_H__
#define __SLIDER_H__
// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   Slider.h
//  Description :   Slider object.
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // Local Includes
#include "Button.h"
#include "Text.h"

class Slider
{
public:
	Slider(glm::vec2 _pos, glm::vec2 _size, int _minValue, int _maxValue, int _startValue, std::string _label);
	~Slider();

	int Update(float _dT);
	void Render(GLuint _program, CCamera* _cam);
private:
	CButton* m_Knob;
	CButton* m_Backing;
	int m_iCurrentValue;
	glm::vec2 m_CentrePoint;
	CText* m_Label;
	CText* m_Text;
	int m_MinValue;
	int m_MaxValue;
};

#endif //__SLIDER_H__