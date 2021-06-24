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
	ClothSize = glm::vec2(17, 17);
	WindArea = 1;
	HookNo = 4;
	WindSpeed = 0;
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
	float distance = glm::dot(_normal, _planePosition);
	float val = glm::dot(_normal, _pointPosition) - distance;
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

bool CUtilities::AABB(glm::vec3 _pointPos, glm::vec3 _cubePos, glm::vec3 _cubeSize)
{
	return ((_pointPos.x >= (_cubePos.x - _cubeSize.x) && _pointPos.x <= (_cubePos.x + _cubeSize.x)) &&
		(_pointPos.y >= (_cubePos.y - _cubeSize.y) && _pointPos.y <= (_cubePos.y + _cubeSize.y)) &&
		(_pointPos.z >= (_cubePos.z - _cubeSize.z) && _pointPos.z <= (_cubePos.z + _cubeSize.z)));
}

glm::vec2 CUtilities::GetResolution()
{
	return m_vResolution;
}

float CUtilities::GetScreenRatio()
{
	return m_fScreenRatio;
}
