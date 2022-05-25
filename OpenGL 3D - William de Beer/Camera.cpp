// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   Camera.cpp
//  Description :   Camera object
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // This Include
#include "Camera.h"
 // Library Includes 
#include "InputHandle.h"
#include "Utilities.h"
#include <iostream>
#include <algorithm>
 // Implementation

CCamera::CCamera(bool _game)
{
	m_bIsGame = _game;
	TextureID = 0;
	PrevMousePosX = (int)CInputHandle::GetInstance().GetMouseX();
	PrevMousePosY = (int)CInputHandle::GetInstance().GetMouseY();
	Radius = 10.0f;
	CurrentOffset = 0;

	SensitivityX = 0.01f;
	SensitivityY = 0.05f;
}

CCamera::~CCamera()
{

}

/***********************
* Initialise: Initialises camera
* @author: William de Beer
* @parameter: position, look direction, up direction
* @return: Booleans
********************/
bool CCamera::Initialise(glm::vec3 _pos, glm::vec3 _lookdir, glm::vec3 _updir)
{
	CameraPos = _pos;
	CameraLookDir = _lookdir;
	CameraUpDir = _updir;

	ViewMat = glm::lookAt(CameraPos, CameraLookDir, CameraUpDir);
	ProjMat = glm::perspective(45.0f, CUtilities::GetInstance().GetScreenRatio(), 0.1f, 10000.0f);
	return true;
}

/***********************
* ProgramUpdate: Updates program
* @author: William de Beer
* @parameter: program
********************/
void CCamera::ProgramUpdate(GLuint _program)
{
	GLuint ViewLoc = glGetUniformLocation(_program, "ViewMat");
	glUniformMatrix4fv(ViewLoc, 1, GL_FALSE, glm::value_ptr(ViewMat));
}

/***********************
* Process: Processes/updates camera variables
* @author: William de Beer
********************/
void CCamera::Process(float _x, float _y, float _z)
{
	if (m_bIsGame) // Check if the camera is being used in the game or in the menu
	{
		float CurrentMousePosX = (float)CInputHandle::GetInstance().GetMouseX();
		float DeltaMousePosX = PrevMousePosX - CurrentMousePosX;
		PrevMousePosX = CurrentMousePosX;

		float CurrentMousePosY = (float)CInputHandle::GetInstance().GetMouseY();
		float DeltaMousePosY = PrevMousePosY - CurrentMousePosY;
		PrevMousePosY = CurrentMousePosY;


		if (CInputHandle::GetInstance().GetMouseButtonState(GLUT_RIGHT_BUTTON) == InputState::Input_Down)
		{
			CurrentOffset += DeltaMousePosX * SensitivityX;
			Radius += DeltaMousePosY * SensitivityY;
			if (Radius < 1.0f)
			{
				Radius = 1.0f;
			}
		}

		float CamX = sin(CurrentOffset) * Radius;
		float CamZ = cos(CurrentOffset) * Radius;

		CameraPos = glm::vec3(CamX, 3.0f, CamZ);
		ViewMat = glm::lookAt(CameraPos + CameraLookDir, CameraLookDir, CameraUpDir);

	}
	else
	{ // If camera is used in menu, orbiting camera to display cubemap.
		// Orbiting Camera
		float CurrentTime;
		CurrentTime = static_cast<float>(glutGet(GLUT_ELAPSED_TIME));
		CurrentTime *= 0.001f;
		CurrentTime = 0;
		// Calculate new camera position every update.
		float CamX = sin(CurrentTime * 0.1f) * Radius;
		float CamZ = cos(CurrentTime * 0.1f) * Radius;

		CameraPos = glm::vec3(CamX, 0.0f, CamZ);
		ViewMat = glm::lookAt(CameraPos + CameraLookDir, CameraLookDir, CameraUpDir);
	}
	UpdateMousePicking();
}

/***********************
* UpdateMousePicking: Updates the ray casting direction variable.
* @author: William de Beer
********************/
void CCamera::UpdateMousePicking()
{
	float screenWidth = CUtilities::GetInstance().GetResolution().x;
	float screenHeight = CUtilities::GetInstance().GetResolution().y;

	float MouseX = (2.0f * (float)CInputHandle::GetInstance().GetMouseX()) / screenWidth - 1.0f;
	float MouseY = 1.0f - (2.0f * (float)CInputHandle::GetInstance().GetMouseY()) / screenHeight;

	//screen pos
	glm::vec2 normalizedScreenPos = glm::vec2(MouseX, MouseY);

	//screenpos to Proj Space
	glm::vec4 clipCoords = glm::vec4(normalizedScreenPos.x,
		normalizedScreenPos.y, -1.0f, 1.0f);

	//Proj Space to eye space
	glm::mat4 invProjMat = glm::inverse(GetProjMat());
	glm::vec4 eyeCoords = invProjMat * clipCoords;
	eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

	//eyespace to world space
	glm::mat4 invViewMat = glm::inverse(GetViewMat());
	glm::vec4 rayWorld = invViewMat * eyeCoords;
	RayDirection = -glm::normalize(glm::vec3(rayWorld));

	//std::cout << RayDirection.x << ", " << RayDirection.y << ", " << RayDirection.z << std::endl;
}

/***********************
* CheckIntersection: Checks to see if a sphere is being intersected by the raycast.
* @author: William de Beer
* @parameter: Sphere postition, Sphere radius.
* @return: Boolean
********************/
bool CCamera::CheckIntersection(glm::vec3 _objPos, float _radius)
{
	glm::vec3 v = _objPos - CameraPos;

	float a = glm::dot(RayDirection, RayDirection);
	float b = 2 * glm::dot(v, RayDirection);
	float c = glm::dot(v, v) - _radius * _radius;
	float d = b * b - 4 * a * c;

	if (d > 0)
	{
		float x1 = (-b - sqrt(d)) / 2;
		float x2 = (-b + sqrt(d)) / 2;
		if (x1 >= 0 && x2 >= 0)
			return true; // intersects
		if (x1 < 0 && x2 >= 0)
			return true; // intersects
	}
	else if (d <= 0)
	{
		return false;// no intersection
	}
	return false;
}

/***********************
* CheckIntersection: Checks to see if a cube is being intersected by the raycast.
* @author: William de Beer
* @parameter: Cube postition, Size of cube.
* @return: Boolean
********************/
bool CCamera::CheckIntersection(glm::vec3 _objPos, glm::vec3 _size)
{
	float t1 = ((_objPos.x - _size.x / 2) - CameraPos.x) / RayDirection.x; // Min
	float t2 = ((_objPos.x + _size.x / 2) - CameraPos.x) / RayDirection.x; // Max
	float t3 = ((_objPos.y - _size.y / 2) - CameraPos.y) / RayDirection.y;
	float t4 = ((_objPos.y + _size.y / 2) - CameraPos.y) / RayDirection.y;
	float t5 = ((_objPos.z - _size.z / 2) - CameraPos.z) / RayDirection.z;
	float t6 = ((_objPos.z + _size.z / 2) - CameraPos.z) / RayDirection.z;

	float tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
	float tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

	if (tmax < 0)
	{
		return false;
	}
	if (tmax < tmin)
	{
		return false;
	}

	return true;
}

/***********************
* GetPVMatrix: Returns projection matrix * view matrix.
* @author: William de Beer
* @return: glm::mat4 4x4 matrix
********************/
glm::mat4 CCamera::GetPVMatrix()
{
	return ProjMat * ViewMat;
}

/***********************
* GetCamPos: Returns the position of the camera.
* @author: William de Beer
* @return: glm::vec3 position
********************/
glm::vec3 CCamera::GetCamPos()
{
	return CameraPos;
}

/***********************
* GetViewMat: Returns the view matrix
* @author: William de Beer
* @return: View Matrix
********************/
glm::mat4 CCamera::GetViewMat()
{
	return ViewMat;
}

/***********************
* GetProjMat: Returns the projections matrix
* @author: William de Beer
* @return: Projection Matrix
********************/
glm::mat4 CCamera::GetProjMat()
{
	return ProjMat;
}

/***********************
* SetCubeMapID: Set the stored texture ID of the cubemap object.
* @author: William de Beer
* @parameter: GLuint texture ID
********************/
void CCamera::SetCubeMapID(GLuint _id)
{
	TextureID = _id;
}

/***********************
* GetCubeMapID: Returns stored texture ID of the cubemap object.
* @author: William de Beer
* @return: GLuint texture ID
********************/
GLuint CCamera::GetCubeMapID()
{
	return TextureID;
}
