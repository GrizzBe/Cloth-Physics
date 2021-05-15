// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   LightManager.h
//  Description :   Light Manager singleton
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // This Include
#include "LightManager.h"
 // Library Includes 
#include "string"
 // Static Variables
CLightManager* CLightManager::sm_Instance = 0;
 // Implementation
CLightManager::CLightManager()
{
	m_iCurrentIndex = 0;
	for (int i = 0; i < 5; i++)
	{
		m_Light[i] = 0;
	}
	m_fAmbientStrength = 0.1f;
	m_AmbientColor = glm::vec3(1.0f, 0.8f, 0.8f);
	m_iRimExponent = 2;
	m_RimColor = glm::vec3(1.0f, 0.8f, 0.8f);
}

CLightManager::~CLightManager()
{
	for (int i = 0; i < 5; i++)
	{
		if (m_Light[i] != nullptr)
		{
			delete m_Light[i];
			m_Light[i] = 0;
		}
	}
}

CLightManager& CLightManager::GetInstance()
{
	if (sm_Instance == 0)
	{
		sm_Instance = new CLightManager();
	}
	return *sm_Instance;
}

void CLightManager::RemoveInstance()
{
	if (sm_Instance != 0)
	{
		delete sm_Instance;
		sm_Instance = 0;
	}
}

void CLightManager::SetAmbientStrength(float _str)
{
	m_fAmbientStrength = _str;
}

void CLightManager::SetAmbientColor(glm::vec3 _col)
{
	m_AmbientColor = _col;
}

void CLightManager::SetRimExponent(int _exp)
{
	m_iRimExponent = _exp;
}

void CLightManager::SetRimColor(glm::vec3 _col)
{
	m_RimColor = _col;
}

void CLightManager::AddLight(CLightSource* _light)
{
	if (m_iCurrentIndex < 5)
	{
		m_Light[m_iCurrentIndex] = _light;
		m_iCurrentIndex++;
	}
}

void CLightManager::RenderLighting(GLuint _program)
{
	GLuint ambientStrength = glGetUniformLocation(_program, "ambientStr");
	glUniform1f(ambientStrength, m_fAmbientStrength);

	GLuint ambientColor = glGetUniformLocation(_program, "ambientColor");
	glUniform3fv(ambientColor, 1, glm::value_ptr(m_AmbientColor));

	GLuint rimExponent = glGetUniformLocation(_program, "rimExponent");
	glUniform1f(rimExponent, m_fAmbientStrength);

	GLuint rimColor = glGetUniformLocation(_program, "rimColor");
	glUniform3fv(rimColor, 1, glm::value_ptr(m_RimColor));

	for (int i = 0; i < 5; i++)
	{
		if (m_Light[i] != nullptr)
		{
			std::string lightPrefix = "lights[" + std::to_string(i);
			GLuint lightColor = glGetUniformLocation(_program, (lightPrefix + "].lightColor").c_str());
			glUniform3fv(lightColor, 1, glm::value_ptr(m_Light[i]->GetColor()));

			GLuint lightPos = glGetUniformLocation(_program, (lightPrefix + "].lightPos").c_str());
			glUniform3fv(lightPos, 1, glm::value_ptr(m_Light[i]->GetPos()));

			GLuint lightSpecStr = glGetUniformLocation(_program, (lightPrefix + "].lightSpecStr").c_str());
			glUniform1f(lightSpecStr, m_Light[i]->GetSpecStrength());

		}
	}
}
