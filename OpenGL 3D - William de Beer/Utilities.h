#ifndef __UTIL_H__
#define __UTIL_H__
// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   Utilities.h
//  Description :   Contains utility information
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

class CUtilities
{
public:
	~CUtilities();

	static CUtilities& GetInstance();
	static void RemoveInstance();

	glm::vec2 GetResolution();
	float GetScreenRatio();

private:
	CUtilities();
	static CUtilities* sm_Instance;

	glm::vec2 m_vResolution;
	float m_fScreenRatio;
};

#endif // __UTILITIES_H__