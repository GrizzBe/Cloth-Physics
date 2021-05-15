#ifndef __OBJECT_H__
#define __OBJECT_H__
// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   Object.h
//  Description :   Object class containing shader properties.
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // Library Includes
#include "Camera.h"
#include <glew.h>
#include <freeglut.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <string>
#include "ShaderLoader.h"
#include "ShadowMap.h"
 // Implementation
class CObject
{
public:
	CObject();
	virtual ~CObject();


	virtual void Render(GLuint _program, CCamera* _cam);
	virtual void Update(float _dT);
	virtual void ShadowPass(ShadowMap* _shadowmap);

	virtual glm::vec3 GetPosition();
	virtual glm::vec3 GetRotation();
	virtual glm::vec3 GetScale();

	virtual void CalculateModelMatrix();

	virtual void SetPosition(glm::vec3 _pos);
	virtual void SetRotation(glm::vec3 _rot);
	virtual void SetScale(glm::vec3 _scale);

	virtual void SetAlbedo(glm::vec3 _albedo);
	virtual void SetReflectivity(float _reflectivity);
	virtual void SetShininess(float _shininess);
	virtual void SetLightSpecularStrength(float _lightSpecStr);

	virtual void SetTexture(std::string _tex);
	virtual void SetShadowMap(ShadowMap* _shadowmap) { m_ShadowMap = _shadowmap; };
protected:
	GLuint VAO;
	GLuint Texture;

	const glm::vec3 WorldRotationAxis_X = glm::vec3(1.0f, 0.0f, 0.0f);
	const glm::vec3 WorldRotationAxis_Y = glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 WorldRotationAxis_Z = glm::vec3(0.0f, 0.0f, 1.0f);

	glm::vec3 ObjScale;
	glm::mat4 ScaleMat;

	glm::vec3 ObjRotationAngle;
	glm::mat4 RotationMat;

	glm::vec3 ObjPosition;
	glm::mat4 TranslationMat;

	glm::mat4 ModelMat;

	GLuint IndiceCount;
	int DrawType;

	glm::vec3 albedo;
	float reflectivity;
	float shininess;
	float lightSpecStr;

	ShadowMap* m_ShadowMap;
	glm::mat4 LightVPMat;
};

#endif