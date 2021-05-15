// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   Utilities.cpp
//  Description :   Contains utility information
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // This Include 
#include "Utilities.h"
 // Static Variables 
CUtilities* CUtilities::sm_Instance = 0;
 // Implementation 
CUtilities::CUtilities()
{
	m_vResolution = glm::vec2( 1200.0f , 800.0f );
	m_fScreenRatio = m_vResolution.x / m_vResolution.y;
}

CUtilities::~CUtilities()
{

}

CUtilities& CUtilities::GetInstance()
{
	if (sm_Instance == 0)
	{
		sm_Instance = new CUtilities();
	}
	return *sm_Instance;
}

void CUtilities::RemoveInstance()
{
	if (sm_Instance != 0)
	{
		delete sm_Instance;
		sm_Instance = 0;
	}
}

glm::vec2 CUtilities::GetResolution()
{
	return m_vResolution;
}

float CUtilities::GetScreenRatio()
{
	return m_fScreenRatio;
}
