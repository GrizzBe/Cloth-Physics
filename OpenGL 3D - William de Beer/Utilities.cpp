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
	ClothSize = glm::vec2(19, 19);
	HookNo = 8;
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

float CUtilities::PlanePointValue(glm::vec3 _normal, glm::vec3 _planePosition, glm::vec3 _pointPosition)
{
	float distance = DotProduct(_normal, _planePosition);
	float val = DotProduct(_normal, _pointPosition) - distance;
	return val;
}

bool CUtilities::PlanePointCollision(glm::vec3 _normal, glm::vec3 _planePosition, glm::vec3 _pointPosition)
{
	float val = PlanePointValue(_normal, _planePosition, _pointPosition);
	if (val > 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

float CUtilities::DotProduct(glm::vec3 _LHS, glm::vec3 _RHS)
{
	float result = (_LHS.x * _RHS.x) + (_LHS.y * _RHS.y) + (_LHS.z * _RHS.z);
	return result;
}

glm::vec2 CUtilities::GetResolution()
{
	return m_vResolution;
}

float CUtilities::GetScreenRatio()
{
	return m_fScreenRatio;
}
