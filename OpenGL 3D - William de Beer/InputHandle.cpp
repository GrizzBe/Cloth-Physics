// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   InputHandle.cpp
//  Description :   Handles all inputs
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // This Include 
#include "InputHandle.h"
#include <iostream>
 // Implementation 
CInputHandle* CInputHandle::sm_Instance = 0;

CInputHandle::CInputHandle(void) 
{
	m_iMouseX = 0;
	m_iMouseY = 0;
}
CInputHandle::~CInputHandle(void) {}

/***********************
* GetInstance: Returns instance of InputHandle.
* @author: William de Beer
* @return: Return InputHandle
********************/
CInputHandle& CInputHandle::GetInstance()
{
	if (sm_Instance == 0)
	{
		sm_Instance = new CInputHandle();
	}
	return *sm_Instance;
}

/***********************
* RemoveInstance: Deletes current instance of InputHandle.
* @author: William de Beer
********************/
void CInputHandle::RemoveInstance()
{
	if (sm_Instance != 0)
	{
		delete sm_Instance;
		sm_Instance = 0;
	}
}

/***********************
* UpdateMousePoition: Updates the mouse position.
* @author: William de Beer
* @parameter: X and Y position
********************/
void CInputHandle::UpdateMousePosition(int X, int Y)
{
	m_iMouseX = X;
	m_iMouseY = Y;
}

/***********************
* UpdateKeyboardState: Updates a key's state.
* @author: William de Beer
* @parameter: Key, State, X and Y positions
* @return: Return values
********************/
void CInputHandle::UpdateKeyboardState(int Key, InputState State, int X, int Y)
{
	if (State == InputState::Input_Down && (KeyState[Key] != InputState::Input_Down && KeyState[Key] != InputState::Input_DownFirst))
	{
		KeyState[Key] = InputState::Input_DownFirst;
		return;
	}
	if (State == InputState::Input_Up && (KeyState[Key] != InputState::Input_Up && KeyState[Key] != InputState::Input_UpFirst))
	{
		KeyState[Key] = InputState::Input_UpFirst;
		return;
	}

	KeyState[Key] = State;
}

/***********************
* UpdateMouseState: Updates a mouse button's state.
* @author: William de Beer
* @parameter: Button, State, X and Y positions
* @return: Return values
********************/
void CInputHandle::UpdateMouseState(int Button, InputState State, int X, int Y)
{
	/*if (State == InputState::Input_Down && (MouseState[Button] != InputState::Input_Down))
	{
		MouseState[Button] = InputState::Input_DownFirst;
		return;
	}
	if (State == InputState::Input_Up && (MouseState[Button] != InputState::Input_Up))
	{
		MouseState[Button] = InputState::Input_UpFirst;
		return;
	}*/

	MouseState[Button] = State;
}

/***********************
* GetKeyboardState: Returns state of a key.
* @author: William de Beer
* @parameter: Key
* @return: Input state.
********************/
InputState CInputHandle::GetKeyboardState(int Key)
{
	return KeyState[Key];
}

/***********************
* GetMouseButtonState: Returns state of a mouse button.
* @author: William de Beer
* @parameter: Mouse button
* @return: Input state.
********************/
InputState CInputHandle::GetMouseButtonState(int Button)
{
	return MouseState[Button];
}

/***********************
* GetMouseX: Returns X position of mouse.
* @author: William de Beer
* @return: Integer
********************/
int CInputHandle::GetMouseX()
{
	return m_iMouseX;
}

/***********************
* GetMouseY: Returns Y position of mouse.
* @author: William de Beer
* @return: Integer
********************/
int CInputHandle::GetMouseY()
{
	return m_iMouseY;
}
