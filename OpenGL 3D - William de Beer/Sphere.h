#pragma once
// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   Sphere.h
//  Description :   Sphere object, squashed to use as ground object.
//  Author      :   William de Beer, Callan Moore 
//  Mail        :   William.Beer@mds.ac.nz
// 
 // Library Includes 
#include <glew.h>
#include <freeglut.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
 // Local Includes 
#include "CubeMap.h"
#include "Camera.h"
#include "Object.h"
 // Implementation 
class Sphere : public CObject
{
public:
	Sphere();
	virtual ~Sphere();

	virtual void Render(GLuint _program, CCamera* _cam);
	virtual void Update(float _dT);

};

