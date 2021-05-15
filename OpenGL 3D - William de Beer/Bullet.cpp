// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2020 Media Design School 
// 
//  File Name   :   Bullet.cpp
//  Description :   Projectile created by player that will destroy enemies.
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // This Include 
#include "Bullet.h"
 // Implementation 
CBullet::CBullet(float _horizpos, float _height, bool _lookright)
{
	float rad = 10.0f;

	m_fSpeed = 2.0f;

	// Initialise bullet at position and direction of player.
	m_HorizPos = _horizpos;
	m_bLookRight = _lookright;
	ObjPosition = glm::vec3(sin(m_HorizPos) * rad, _height, cos(m_HorizPos) * rad);

	m_bDestroyInstance = false;
	m_fAliveTimer = 0;
}

CBullet::~CBullet()
{
}

/***********************
* Initialise: Initialise bullet object.
* @author: William de Beer
* @parameter: Model Pointer.
********************/
void CBullet::Initialise(Model* _model)
{
	m_Model = _model;
	
	// Set default values of object
	ObjScale = glm::vec3(0.2f, 0.2f, 0.2f);
	ObjRotationAngle = glm::vec3(90.0f, 0.0f, 0.0f);

	ScaleMat = glm::scale(glm::mat4(), ObjScale);
	// Add rotation matrices
	RotationMat = glm::rotate(glm::mat4(), glm::radians(ObjRotationAngle.z), WorldRotationAxis_Z) *
		glm::rotate(glm::mat4(), glm::radians(ObjRotationAngle.y), WorldRotationAxis_Y) *
		glm::rotate(glm::mat4(), glm::radians(ObjRotationAngle.x), WorldRotationAxis_X);


	TranslationMat = glm::translate(glm::mat4(), ObjPosition);
}
void CBullet::Initialise(std::string _path, CCamera* _camera)
{
	m_Model = new Model(_path, _camera, ShaderLoader::GetInstance().CreateProgram("NormalSpace.vs", "LightingEffects.fs"));

	ObjScale = glm::vec3(0.2f, 0.2f, 0.2f);
	ObjRotationAngle = glm::vec3(0.0f, 0.0f, 0.0f);

	ScaleMat = glm::scale(glm::mat4(), ObjScale);
	RotationMat = glm::rotate(glm::mat4(), glm::radians(ObjRotationAngle.x), WorldRotationAxis_X) +
		glm::rotate(glm::mat4(), glm::radians(ObjRotationAngle.y), WorldRotationAxis_Y) +
		glm::rotate(glm::mat4(), glm::radians(ObjRotationAngle.z), WorldRotationAxis_Z);

	TranslationMat = glm::translate(glm::mat4(), ObjPosition);
}

/***********************
* Render: Renders model
* @author: William de Beer
********************/
void CBullet::Render()
{
	// Update matrices
	ScaleMat = glm::scale(glm::mat4(), ObjScale);
	RotationMat = glm::rotate(glm::mat4(), glm::radians(ObjRotationAngle.z), WorldRotationAxis_Z) *
		glm::rotate(glm::mat4(), glm::radians(ObjRotationAngle.y), WorldRotationAxis_Y) *
		glm::rotate(glm::mat4(), glm::radians(ObjRotationAngle.x), WorldRotationAxis_X);

	TranslationMat = glm::translate(glm::mat4(), ObjPosition);

	// Call model render
	m_Model->Render(TranslationMat, RotationMat, ScaleMat);
}

/***********************
* Update: Updates bullet
* @author: William de Beer
* @parameter: Delta time
********************/
void CBullet::Update(float dT)
{
	// Set radius of path
	float rad = 10.0f;

	// Set bullet to be deleted after one second.
	m_fAliveTimer += dT;
	if (m_fAliveTimer >= 1.0f)
	{
		m_bDestroyInstance = true;
	}

	// Update the position of the object
	if (m_bLookRight)
	{
		m_HorizPos -= dT * m_fSpeed;
	}
	else
	{
		m_HorizPos += dT * m_fSpeed;
	}
	// Calculate position of object around perimeter of circle
	ObjPosition = glm::vec3(sin(m_HorizPos) * rad, ObjPosition.y, cos(m_HorizPos) * rad);

	// Set object rotation.
	if (ObjPosition.x != 0)
	{
		ObjRotationAngle.y = atan((float)-ObjPosition.z / (float)ObjPosition.x) * 180.0f / 3.14f - 90.0f;
	}
	if (ObjPosition.x > 0)
	{
		ObjRotationAngle.y += 180;
	}
	if (m_bLookRight)
	{
		ObjRotationAngle.y += 180;
	}

}

/***********************
* GetDestroyInstance: Returns value indicating whether bullet is to be destroyed.
* @author: William de Beer
********************/
bool CBullet::GetDestroyInstance()
{
	return m_bDestroyInstance;
}

