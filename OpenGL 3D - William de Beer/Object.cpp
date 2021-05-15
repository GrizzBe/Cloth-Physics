// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   Object.cpp
//  Description :   Object class containing shader properties.
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // This Include
#include "Object.h"
 // Implementation
CObject::CObject()
{
	albedo = glm::vec3(1.0f,1.0f,1.0f); 
	reflectivity = 0.5f;
	shininess = 32.0f;
	lightSpecStr = 1.0f;

	ObjPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	ObjScale = glm::vec3(1.0f, 1.0f, 1.0f);
	ObjRotationAngle = glm::vec3(0.0f, 0.0f, 0.0f);

	ScaleMat = glm::scale(glm::mat4(), ObjScale);
	// Add rotation matrices
	RotationMat = glm::rotate(glm::mat4(), glm::radians(ObjRotationAngle.x), WorldRotationAxis_X) *
		glm::rotate(glm::mat4(), glm::radians(ObjRotationAngle.y), WorldRotationAxis_Y) *
		glm::rotate(glm::mat4(), glm::radians(ObjRotationAngle.z), WorldRotationAxis_Z);

	TranslationMat = glm::translate(glm::mat4(), ObjPosition);

	CalculateModelMatrix();

	m_ShadowMap = 0;

	VAO = 0;
	Texture = 0;

	IndiceCount = 0;
	DrawType = 0;
}

CObject::~CObject()
{
}

/***********************
* Render: Renders object
* @author: William de Beer
* @parameter: Program to use, pointer to camera
********************/
void CObject::Render(GLuint _program, CCamera* _cam)
{
	glm::mat4 pvmMat = _cam->GetProjMat() * _cam->GetViewMat() * ModelMat;

	if (m_ShadowMap != nullptr)
	{
		GLint vpLoc = glGetUniformLocation(m_ShadowMap->GetProgram(), "LightVPMatrix");
		glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(LightVPMat));

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, GL_FRAMEBUFFER);
		glUniform1i(glGetUniformLocation(_program, "ShadowMap"), 0);
	}


	GLuint PVM = glGetUniformLocation(_program, "PVM");
	glUniformMatrix4fv(PVM, 1, GL_FALSE, glm::value_ptr(pvmMat));

	GLuint ModelLoc = glGetUniformLocation(_program, "ModelMat");
	glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(ModelMat));

	GLuint Albedo = glGetUniformLocation(_program, "albedo");
	glUniform3fv(Albedo, 1, glm::value_ptr(albedo));

	GLuint Reflectivity = glGetUniformLocation(_program, "reflectivity");
	glUniform1f(Reflectivity, reflectivity);

	GLuint Shininess = glGetUniformLocation(_program, "shininess");
	glUniform1f(Shininess, shininess);

	GLuint LightSpecStr = glGetUniformLocation(_program, "lightSpecStr");
	glUniform1f(LightSpecStr, lightSpecStr);
}

/***********************
* Update: Updates object
* @author: William de Beer
* @parameter: Delta time
********************/
void CObject::Update(float _dT)
{
	ScaleMat = glm::scale(glm::mat4(), ObjScale);
	// Add rotation matrices
	RotationMat = glm::rotate(glm::mat4(), glm::radians(ObjRotationAngle.x), WorldRotationAxis_X) *
		glm::rotate(glm::mat4(), glm::radians(ObjRotationAngle.y), WorldRotationAxis_Y) *
		glm::rotate(glm::mat4(), glm::radians(ObjRotationAngle.z), WorldRotationAxis_Z);

	TranslationMat = glm::translate(glm::mat4(), ObjPosition);

	CalculateModelMatrix();
}

void CObject::ShadowPass(ShadowMap* _shadowmap)
{
	glUseProgram(_shadowmap->GetProgram());
	glm::mat4 lightViewMatrix = glm::lookAt(_shadowmap->GetLightSource()->GetPos(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 lightVPMatrix = _shadowmap->GetCamera()->GetProjMat() * lightViewMatrix;
	LightVPMat = lightVPMatrix;

	GLint vpLoc = glGetUniformLocation(_shadowmap->GetProgram(), "LightVPMatrix");
	glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(lightVPMatrix));

	// model matrix of the current object
	GLint modelLoc = glGetUniformLocation(_shadowmap->GetProgram(), "ModelMat");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(ModelMat));

	// draw the current object
	glBindVertexArray(VAO);
	glDrawElements(DrawType, IndiceCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

/***********************
* GetPosition: Returns object position
* @author: William de Beer
* @return: Vec3
********************/
glm::vec3 CObject::GetPosition()
{
	return ObjPosition;
}

/***********************
* GetRotation: Returns object rotation
* @author: William de Beer
* @return: Vec3
********************/
glm::vec3 CObject::GetRotation()
{
	return ObjRotationAngle;
}

/***********************
* GetScale: Returns object scale
* @author: William de Beer
* @return: Vec3
********************/
glm::vec3 CObject::GetScale()
{
	return ObjScale;
}

/***********************
* CalculateModelMatrix: Calculates the model matrix from other matrices
* @author: William de Beer
********************/
void CObject::CalculateModelMatrix()
{
	ModelMat = TranslationMat * RotationMat * ScaleMat;
}

/***********************
* SetPosition: Sets object position
* @author: William de Beer
* @parameter: Vec3
********************/
void CObject::SetPosition(glm::vec3 _pos)
{
	ObjPosition = _pos;
	TranslationMat = glm::translate(glm::mat4(), ObjPosition);
	CalculateModelMatrix();
}

/***********************
* SetRotation: Sets object rotation
* @author: William de Beer
* @parameter: Vec3
********************/
void CObject::SetRotation(glm::vec3 _rot)
{
	ObjRotationAngle = _rot;
	RotationMat = glm::rotate(glm::mat4(), glm::radians(ObjRotationAngle.x), WorldRotationAxis_X) *
		glm::rotate(glm::mat4(), glm::radians(ObjRotationAngle.y), WorldRotationAxis_Y) *
		glm::rotate(glm::mat4(), glm::radians(ObjRotationAngle.z), WorldRotationAxis_Z);
	CalculateModelMatrix();
}

/***********************
* SetScale: Sets object scale
* @author: William de Beer
* @parameter: Vec3
********************/
void CObject::SetScale(glm::vec3 _scale)
{
	ObjScale = _scale;
	ScaleMat = glm::scale(glm::mat4(), ObjScale);
	CalculateModelMatrix();
}

/***********************
* SetAlbedo: Sets object albedo
* @author: William de Beer
* @parameter: Vec3
********************/
void CObject::SetAlbedo(glm::vec3 _albedo)
{
	albedo = _albedo;
}

/***********************
* SetPosition: Sets object reflectivity
* @author: William de Beer
* @parameter: Float
********************/
void CObject::SetReflectivity(float _reflectivity)
{
	reflectivity = _reflectivity;
}

/***********************
* SetPosition: Sets object shininess
* @author: William de Beer
* @parameter: Float
********************/
void CObject::SetShininess(float _shininess)
{
	shininess = _shininess;
}

/***********************
* SetPosition: Sets object light specular strength
* @author: William de Beer
* @parameter: Float
********************/
void CObject::SetLightSpecularStrength(float _lightSpecStr)
{
	lightSpecStr = _lightSpecStr;
}

/***********************
* SetPosition: Sets object texture
* @author: William de Beer
* @parameter: Texture file location
********************/
void CObject::SetTexture(std::string _tex)
{
	Texture = ShaderLoader::GetInstance().CreateTexture(_tex.c_str());
}