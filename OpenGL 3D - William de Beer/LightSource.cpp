// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   LightSource.h
//  Description :   Light Source object
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // This Include
#include "LightSource.h"
 // Library Includes 

CLightSource::CLightSource(glm::vec3 _pos)
{
	Pos = _pos;
	Color = glm::vec3(1.0f, 0.8f, 0.8f);
	SpecStrength = 1.0f;
}

CLightSource::~CLightSource()
{

}

void CLightSource::SetPos(glm::vec3 _pos)
{
	Pos = _pos;
}

glm::vec3 CLightSource::GetPos()
{
	return Pos;
}

void CLightSource::SetColor(glm::vec3 _color)
{
	Color = _color;
}

glm::vec3 CLightSource::GetColor()
{
	return Color;
}

void CLightSource::SetSpecStrength(float _strength)
{
	SpecStrength = _strength;
}

float CLightSource::GetSpecStrength()
{
	return SpecStrength;
}
