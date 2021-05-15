// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   SceneManager.cpp
//  Description :   Manages scenes.
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // This Include 
#include "AudioManager.h"
 // Local Include
#include "InputHandle.h"
#include "LightManager.h"
#include "SceneManager.h"
#include "ShaderLoader.h"
#include "Utilities.h"
 // Static Variables 
CScene* CSceneManager::m_CurrentScene = 0;
 // Implementation 

/***********************
* SetCurrentScene: Sets the current scene.
* @author: William de Beer
* @parameter: Scene
********************/
void CSceneManager::SetCurrentScene(CScene* _scene)
{
	// Delete old scene.
	if (m_CurrentScene != nullptr)
	{
		delete m_CurrentScene;
		m_CurrentScene = 0;
	}
	// Save new scene.
	m_CurrentScene = _scene;
	m_CurrentScene->Initialise();
}

/***********************
* All functions below call corresponding scene functions
* @author: William de Beer
********************/

/***********************
* Render: Calls scene Render function.
* @author: William de Beer
********************/
void CSceneManager::Render()
{
	if (m_CurrentScene != nullptr)
	{
		m_CurrentScene->Render();
	}
}

/***********************
* Update: Calls scene Update function.
* @author: William de Beer
********************/
void CSceneManager::Update()
{
	if (m_CurrentScene != nullptr)
	{
		m_CurrentScene->Update();
	}
}

/***********************
* ShutDown: Calls scene ShutDown function.
* @author: William de Beer
********************/
void CSceneManager::ShutDown()
{
	if (m_CurrentScene != nullptr)
	{
		delete m_CurrentScene;
		m_CurrentScene = 0;
	}

	CInputHandle::RemoveInstance();
	CAudioManager::RemoveInstance();
	CLightManager::RemoveInstance();
	ShaderLoader::RemoveInstance();
	CUtilities::RemoveInstance();
}

/***********************
* ProcessInput: Calls scene ProcessInput function.
* @author: William de Beer
********************/
void CSceneManager::ProcessInput()
{
	if (m_CurrentScene != nullptr)
	{
		m_CurrentScene->ProcessInput();
	}

}
/***********************
* KeyboardDown: Calls scene KeyboardDown function.
* @author: William de Beer
* @parameter: Key, X and Y
********************/
void CSceneManager::KeyboardDown(unsigned char Key, int X, int Y)
{
	if (m_CurrentScene != nullptr)
	{
		m_CurrentScene->KeyboardDown(Key, X, Y);
	}
}

/***********************
* KeyboardUp: Calls scene KeyboardUp function.
* @author: William de Beer
* @parameter: Key, X and Y
********************/
void CSceneManager::KeyboardUp(unsigned char Key, int X, int Y)
{
	if (m_CurrentScene != nullptr)
	{
		m_CurrentScene->KeyboardUp(Key, X, Y);
	}
}

/***********************
* MouseInteraction: Calls scene MouseInteraction function.
* @author: William de Beer
* @parameter: Mouse Button, State, X and Y
********************/
void CSceneManager::MouseInteraction(int Button, int State, int X, int Y)
{
	if (m_CurrentScene != nullptr)
	{
		m_CurrentScene->MouseInteraction(Button, State, X, Y);
	}
}

/***********************
* MouseMotionTrackingPassive: Calls scene MouseMotionTrackingPassive function.
* @author: William de Beer
* @parameter: X and Y
********************/
void CSceneManager::MouseMotionTrackingPassive(int X, int Y)
{
	if (m_CurrentScene != nullptr)
	{
		m_CurrentScene->MouseMotionTrackingPassive(X, Y);
	}
}

/***********************
* MouseMotionTrackingInteracting: Calls scene !!! function.
* @author: William de Beer
* @parameter: X and Y
********************/
void CSceneManager::MouseMotionTrackingInteracting(int X, int Y)
{
	if (m_CurrentScene != nullptr)
	{
		m_CurrentScene->MouseMotionTrackingInteracting(X, Y);
	}
}
