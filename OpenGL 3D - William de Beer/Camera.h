#ifndef __CAMERA_H__
#define __CAMERA_H__
// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   Camera.h
//  Description :   Camera object
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
class CCamera
{
public:
	CCamera(bool _game);
	~CCamera();

	bool Initialise(glm::vec3 _pos, glm::vec3 _lookdir, glm::vec3 _updir);
	void ProgramUpdate(GLuint _program);
	void Process(float _x, float _y, float _z);

	void UpdateMousePicking();
	bool CheckIntersection(glm::vec3 _objPos, float _radius);
	bool CheckIntersection(glm::vec3 _objPos, glm::vec3 _size);

	glm::mat4 GetPVMatrix();

	glm::vec3 GetCamPos();
	glm::mat4 GetViewMat();
	glm::mat4 GetProjMat();

	void SetCubeMapID(GLuint _id);
	GLuint GetCubeMapID();

	void SetRadius(float _radius) { Radius = _radius; };
	float GetRadius() { return Radius; };
private:
	bool m_bIsGame;

	float PrevMousePosX;
	float PrevMousePosY;

	float SensitivityX; 
	float SensitivityY;

	float CurrentOffset;
	float Radius;

	glm::vec3 RayDirection;
	glm::vec3 CameraPos;
	glm::vec3 CameraLookDir;
	glm::vec3 CameraUpDir; 
	glm::mat4 ViewMat;
	glm::mat4 ProjMat;
	glm::mat4 ProjectionMat;
	GLuint TextureID;
};
#endif
