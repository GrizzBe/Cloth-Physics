#ifndef __PYRAMID_H__
#define __PYRAMID_H__
// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   Pyramid.h
//  Description :   Pyramid object.
//  Author      :   William de Beer 
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
#include "LightManager.h"
#include "ShaderLoader.h"
 // Implementation 
#include "Object.h"
class Pyramid :
    public CObject
{
public:
	Pyramid();
	virtual ~Pyramid();

	virtual void Render(GLuint _program, CCamera* _cam);
	virtual void Update(float _dT);
};
#endif
