#ifndef __TESTSCENE_H__
#define __TESTSCENE_H__
// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   TestScene.h
//  Description :   Scene used to test.
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // Local Includes 
#include "AudioManager.h"
#include "Button.h"
#include "Camera.h"
#include "Cloth.h"
#include "CubeMap.h"
#include "ImageUI.h"
#include "InputHandle.h"
#include "Quad.h"
#include "Scene.h"
 // Implementation 
class CTestScene : public CScene
{
public:
	CTestScene();
	virtual ~CTestScene();
	virtual void Initialise();
	virtual void Render();
	virtual void Update();
	virtual void ProcessInput();
	virtual void KeyboardDown(unsigned char Key, int X, int Y);
	virtual void KeyboardUp(unsigned char Key, int X, int Y);
	virtual void MouseInteraction(int Button, int State, int X, int Y);
	virtual void MouseMotionTrackingPassive(int X, int Y);
	virtual void MouseMotionTrackingInteracting(int X, int Y);

private:

	float m_fCurrentTime; 
	float m_fPreviousTimeStamp;

	GLuint Program_UIAnim;
	GLuint Program_UI;
	GLuint Program_Object;
	GLuint Program_Water;
	GLuint Program_StencilOutline;

	Cloth* m_Cloth;
	CCamera* m_Cam;
	Quad* m_Ground;
};

#endif