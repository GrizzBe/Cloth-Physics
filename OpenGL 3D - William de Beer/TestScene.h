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
#include "Capsule.h"
#include "Cloth.h"
#include "Cube.h"
#include "CubeMap.h"
#include "ImageUI.h"
#include "InputHandle.h"
#include "Pyramid.h"
#include "Quad.h"
#include "Scene.h"
#include "Slider.h"
#include "Sphere.h"
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

	void Move();
	void ApplyMovement(CObject* _object);
private:

	enum class ControlledObject {
		FAN,
		SPHERE,
		PYRAMID,
		CAPSULE,
	};


	float m_fCurrentTime; 
	float m_fPreviousTimeStamp;
	float m_fDeltaTime;

	bool m_bUseWireframe;
	bool m_bActiveFan;
	bool m_bFanVisuals;
	bool m_bClothUntangle;

	GLuint Program_UIAnim;
	GLuint Program_UI;
	GLuint Program_Object;
	GLuint Program_Water;
	GLuint Program_StencilOutline;

	glm::vec3 m_Move;

	ControlledObject object;

	Cloth* m_Cloth;
	Capsule* m_Capsule;
	CCamera* m_Cam;
	Quad* m_Ground;
	Cube* m_Fan;
	Cube* m_FanVisuals;
	Pyramid* m_Pyramid;
	Sphere* m_Sphere;

	Slider* m_HeightSlider;
	Slider* m_WidthSlider;
	Slider* m_WindAreaSlider;
	Slider* m_HookSlider;
	Slider* m_WindSlider;

	std::vector<CText*> m_txtInstructions;

	CText* m_txtUpdateRate;
	float m_fUpdateRate;
};

#endif