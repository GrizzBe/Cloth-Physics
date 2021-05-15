#ifndef __LIGHTMANAGER_H__
#define __LIGHTMANAGER_H__
// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   LightManager.h
//  Description :   Light Manager singleton
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // Library Includes 
#include "LightSource.h"
#include <vector>
 // Implementation
class CLightManager
{
public:
	~CLightManager();

	static CLightManager& GetInstance();
	static void RemoveInstance();

	void SetAmbientStrength(float _str);
	void SetAmbientColor(glm::vec3 _col);

	void SetRimExponent(int _exp);
	void SetRimColor(glm::vec3 _col);

	void AddLight(CLightSource* _light);
	void RenderLighting(GLuint _program);

private:
	CLightManager();

	static CLightManager* sm_Instance;

	float m_fAmbientStrength;
	glm::vec3 m_AmbientColor;

	int m_iRimExponent;
	glm::vec3 m_RimColor;

	CLightSource* m_Light[5];
	int m_iCurrentIndex;
};

#endif