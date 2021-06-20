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
#include <wincrypt.h>
 // Implementation 


class CUtilities
{
public:
	~CUtilities();

	static CUtilities& GetInstance();
	static void RemoveInstance();

	static float PlanePointValue(glm::vec3 _normal, glm::vec3 _planePosition, glm::vec3 _pointPosition);
	static bool PlanePointCollision(glm::vec3 _normal, glm::vec3 _planePosition, glm::vec3 _pointPosition);
	static float DotProduct(glm::vec3 _LHS, glm::vec3 _RHS);

	glm::vec2 GetResolution();
	float GetScreenRatio();


	glm::vec2 GetClothSize() { return ClothSize; };
	void SetClothSize(glm::vec2 _size) { ClothSize = _size; };

	int GetHookDensity() { return HookNo; };
	void SetHookDensity(int _hookNo) { HookNo = _hookNo; };

	struct PlaneInfo {
		int index;
		glm::vec3 normal;
		glm::vec3 position;
	};

private:
	CUtilities();
	static CUtilities* sm_Instance;

	glm::vec2 m_vResolution;
	float m_fScreenRatio;

	glm::vec2 ClothSize;
	int HookNo;
};

#endif // __UTILITIES_H__