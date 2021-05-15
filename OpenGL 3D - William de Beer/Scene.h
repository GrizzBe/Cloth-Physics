#ifndef __SCENE_H__
#define __SCENE_H__
// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   Scene.h
//  Description :   Pure virtual scene header.
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // Implementation 
class CScene
{
public:
	CScene()
	{

	}
	virtual ~CScene()
	{

	}

	virtual void Initialise() = 0;
	virtual void Render() = 0;
	virtual void Update() = 0;
	virtual void ProcessInput() = 0;
	virtual void KeyboardDown(unsigned char Key, int X, int Y) = 0;
	virtual void KeyboardUp(unsigned char Key, int X, int Y) = 0;
	virtual void MouseInteraction(int Button, int State, int X, int Y) = 0;
	virtual void MouseMotionTrackingPassive(int X, int Y) = 0;
	virtual void MouseMotionTrackingInteracting(int X, int Y) = 0;
};


#endif
