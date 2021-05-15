#ifndef __LIGHTSOURCE_H__
#define __LIGHTSOURCE_H__
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
 // Library Includes 
#include <glew.h>
#include <freeglut.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
 // Implementation
class CLightSource
{
public:
	CLightSource(glm::vec3 _pos);
	~CLightSource();

	void SetPos(glm::vec3 _pos);
	glm::vec3 GetPos();
	void SetColor(glm::vec3 _color);
	glm::vec3 GetColor();
	void SetSpecStrength(float _strength);
	float GetSpecStrength();
private:
	glm::vec3 Pos;
	glm::vec3 Color;
	float SpecStrength;
};

#endif