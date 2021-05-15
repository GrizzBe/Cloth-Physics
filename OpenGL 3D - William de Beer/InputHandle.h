#ifndef __INPUTHANDLE_H__
#define __INPUTHANDLE_H__
// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   InputHandle.h
//  Description :   Handles all inputs.
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // Implementation 
enum class InputState
{
	Input_Up,
	Input_UpFirst,
	Input_Down,
	Input_DownFirst,
};

class CInputHandle
{
public:
	static CInputHandle& GetInstance();
	static void RemoveInstance();

	void UpdateMousePosition(int X, int Y);
	void UpdateMouseState(int Button, InputState State, int X, int Y);
	void UpdateKeyboardState(int Key, InputState State, int X, int Y);

	InputState GetKeyboardState(int Key);
	InputState GetMouseButtonState(int Button);
	int GetMouseX();
	int GetMouseY();

private:
	CInputHandle(void);
	~CInputHandle(void);
	int m_iMouseX;
	int m_iMouseY;
	InputState KeyState[225] = { InputState::Input_Up };
	InputState MouseState[3] = { InputState::Input_Up };

	static CInputHandle* sm_Instance;
};

#endif