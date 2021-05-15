#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__
// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   SceneManager.h
//  Description :   Manages scenes.
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // Local Includes 
#include "Scene.h"
 // Implementation 
class CSceneManager
{
public:
	static void SetCurrentScene(CScene* _scene);
	static void Render();
	static void Update();
	static void ShutDown();
	static void ProcessInput();
	static void KeyboardDown(unsigned char Key, int X, int Y);
	static void KeyboardUp(unsigned char Key, int X, int Y);
	static void MouseInteraction(int Button, int State, int X, int Y);
	static void MouseMotionTrackingPassive(int X, int Y);
	static void MouseMotionTrackingInteracting(int X, int Y);
protected:
	static CScene* m_CurrentScene;
};
#endif

