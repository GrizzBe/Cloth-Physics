// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   TestScene.cpp
//  Description :   Scene used to test.
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // Local Includes
#include "AudioManager.h"
#include "LightManager.h"
#include "ShaderLoader.h"
#include "TestScene.h"
#include "Utilities.h"
 // Implementation
CTestScene::CTestScene()
{
	m_fCurrentTime = (float)glutGet(GLUT_ELAPSED_TIME);
	m_fPreviousTimeStamp = (float)glutGet(GLUT_ELAPSED_TIME);
	m_fDeltaTime = 0;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	Program_UIAnim = 0;
	Program_UI = 0;
	Program_Object = 0;
	Program_Water = 0;
	Program_StencilOutline = 0;

	m_Ground = 0;
	m_Cam = 0;
	m_Cloth = 0;
	m_Slider = 0;

	m_bUseWireframe = false;
	m_bActiveFan = false;
}

CTestScene::~CTestScene()
{
	if (m_Cloth != nullptr)
	{
		delete m_Cloth;
		m_Cloth = 0;
	}
	if (m_Ground != nullptr)
	{
		delete m_Ground;
		m_Ground = 0;
	}
	if (m_Cam != nullptr)
	{
		delete m_Cam;
		m_Cam = 0;
	}
	if (m_Slider != nullptr)
	{
		delete m_Slider;
		m_Slider = 0;
	}
	CLightManager::RemoveInstance();
}

/***********************
* Initialise: Initialise game.
* @author: William de Beer
********************/
void CTestScene::Initialise()
{
	// Create programs
	Program_UIAnim = ShaderLoader::GetInstance().CreateProgram("WorldSpaceAnim.vs", "Texture.fs");
	Program_UI = ShaderLoader::GetInstance().CreateProgram("WorldSpace.vs", "Texture.fs");
	Program_Water = ShaderLoader::GetInstance().CreateProgram("WobbleSpace.vs", "LightingEffectsFog.fs");
	Program_Object = ShaderLoader::GetInstance().CreateProgram("NormalSpace.vs", "LightingEffects.fs");
	Program_StencilOutline = ShaderLoader::GetInstance().CreateProgram("NormalSpace.vs", "StencilOutline.fs");

	// Create cloth
	m_Cloth = new Cloth(CUtilities::GetInstance().GetClothSize().x, CUtilities::GetInstance().GetClothSize().y);

	// Create camera
	m_Cam = new CCamera(true);
	m_Cam->Initialise(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// Create fan
	m_Fan = new Cube();
	m_Fan->SetPosition(glm::vec3(0.0f, -5.1, -5.0f));
	m_Fan->SetScale(glm::vec3(1, 1, 1));
	m_Fan->SetTexture("Fan.png");

	// Create ground
	m_Ground = new Quad();
	m_Ground->SetPosition(glm::vec3(0, -10.1, 0));
	m_Ground->SetScale(glm::vec3(20, 1, 20));
	m_Ground->SetReflectivity(1.0f);
	m_Ground->SetTexture("Map/Ground.jpg");

	// Create size slider
	m_Slider = new Slider(glm::vec2(), glm::vec2(0.02f, 0.02f), 2, 49, CUtilities::GetInstance().GetClothSize().x);

	// Create light
	CLightSource* newLight = new CLightSource(glm::vec3(0.0f, 0.0f, 5.0f));
	newLight->SetColor(glm::vec3(1.0f, 0.9f, 0.9f));
	CLightManager::GetInstance().AddLight(newLight);

	CLightManager::GetInstance().SetRimColor(glm::vec3(1.0f, 0.9f, 0.9f));

	Update();
}

/***********************
* Render: Renders all objects.
* @author: William de Beer
********************/
void CTestScene::Render()
{
	GLbitfield BufferCopy = GL_COLOR_BUFFER_BIT;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//*******//
	m_Ground->Render(Program_Object, m_Cam);
	m_Cloth->Render(m_Cam);
	m_Fan->Render(Program_Object, m_Cam);

	glDisable(GL_DEPTH_TEST);
	m_Slider->Render(Program_UI, m_Cam);
	glEnable(GL_DEPTH_TEST);
	//*******//
	glUseProgram(0);

	glutSwapBuffers();
}

/***********************
* Update: Updates objects.
* @author: William de Beer
********************/
void CTestScene::Update()
{
	// Get delta time
	m_fCurrentTime = (float)glutGet(GLUT_ELAPSED_TIME);
	m_fDeltaTime = (m_fCurrentTime - m_fPreviousTimeStamp) * 0.001f;
	if (m_fDeltaTime < 0.016)
	{
		return;
	}
	m_fDeltaTime = 0.016;

	m_fPreviousTimeStamp = m_fCurrentTime;

	m_Fan->SetPosition(m_Fan->GetPosition() + m_FanMove * m_fDeltaTime);
	m_Fan->Update(m_fDeltaTime);

	if (m_bActiveFan)
	{
		m_Cloth->ApplyWind(m_Fan->GetPosition());
	}
	m_Cloth->Update(m_fDeltaTime, m_Cam);
	m_Cam->Process(0.0f, 0.0f, 0.0f);
	m_Ground->Update(m_fDeltaTime);
	m_Slider->Update(m_fDeltaTime);

	CAudioManager::GetInstance().Process();
	glutPostRedisplay();

	ProcessInput();
}

/***********************
* ProcessInput: It doesn't do much really.
* @author: William de Beer
********************/
void CTestScene::ProcessInput()
{
	// Wireframe toggle
	if (CInputHandle::GetInstance().GetKeyboardState('y') == InputState::Input_DownFirst)
	{
		CInputHandle::GetInstance().UpdateKeyboardState('y', InputState::Input_Down, 0, 0);
		m_bUseWireframe = !m_bUseWireframe;
		if (m_bUseWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Use wireframe 
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	// Drop cloth
	if (CInputHandle::GetInstance().GetKeyboardState('v') == InputState::Input_DownFirst)
	{
		CInputHandle::GetInstance().UpdateKeyboardState('v', InputState::Input_Down, 0, 0);
		m_Cloth->DropCloth();
	}

	// Drop cloth
	if (CInputHandle::GetInstance().GetKeyboardState('f') == InputState::Input_DownFirst)
	{
		CInputHandle::GetInstance().UpdateKeyboardState('f', InputState::Input_Down, 0, 0);
		m_bActiveFan = !m_bActiveFan;
	}

	FanMove();

	// Restart Scene
	if (CInputHandle::GetInstance().GetKeyboardState('r') == InputState::Input_DownFirst)
	{
		CInputHandle::GetInstance().UpdateKeyboardState('r', InputState::Input_Down, 0, 0);
		CSceneManager::SetCurrentScene(new CTestScene());
	}

}

/***********************
* KeyboardDown: Set keyboard state as down.
* @author: William de Beer
* @parameter: Key, X and Y position
********************/
void CTestScene::KeyboardDown(unsigned char Key, int X, int Y)
{
	CInputHandle::GetInstance().UpdateKeyboardState(Key, InputState::Input_Down, X, Y);
}

/***********************
* KeyboardUp: Set keyboard state as up.
* @author: William de Beer
* @parameter: Key, X and Y position
********************/
void CTestScene::KeyboardUp(unsigned char Key, int X, int Y)
{
	CInputHandle::GetInstance().UpdateKeyboardState(Key, InputState::Input_Up, X, Y);
}

/***********************
* MouseInteraction: Set mouse buttons.
* @author: William de Beer
* @parameter: Mouse button, state, X and Y position
********************/
void CTestScene::MouseInteraction(int Button, int State, int X, int Y)
{
	if (Button >= 3)
	{
		return;
	}
	CInputHandle::GetInstance().UpdateMouseState(Button, (State == GLUT_DOWN) ? InputState::Input_Down : InputState::Input_Up, X, Y);
}

/***********************
* MouseMotionTrackingPassive: Update position while passive.
* @author: William de Beer
* @parameter: X and Y position
********************/
void CTestScene::MouseMotionTrackingPassive(int X, int Y)
{
	CInputHandle::GetInstance().UpdateMousePosition(X, Y);
}

/***********************
* MouseMotionTrackingPassive: Update position while passive.
* @author: William de Beer
* @parameter: X and Y position
********************/
void CTestScene::MouseMotionTrackingInteracting(int X, int Y)
{
	CInputHandle::GetInstance().UpdateMousePosition(X, Y);
}

void CTestScene::FanMove()
{
	m_FanMove = glm::vec3(0, 0, 0);
	float speed = 10.0f;

	// Update button presses.
	if (CInputHandle::GetInstance().GetKeyboardState('w') == InputState::Input_DownFirst)
	{
		CInputHandle::GetInstance().UpdateKeyboardState('w', InputState::Input_Down, 0, 0);
	}
	if (CInputHandle::GetInstance().GetKeyboardState('a') == InputState::Input_DownFirst)
	{
		CInputHandle::GetInstance().UpdateKeyboardState('a', InputState::Input_Down, 0, 0);
	}
	if (CInputHandle::GetInstance().GetKeyboardState('s') == InputState::Input_DownFirst)
	{
		CInputHandle::GetInstance().UpdateKeyboardState('s', InputState::Input_Down, 0, 0);
	}
	if (CInputHandle::GetInstance().GetKeyboardState('d') == InputState::Input_DownFirst)
	{
		CInputHandle::GetInstance().UpdateKeyboardState('d', InputState::Input_Down, 0, 0);
	}
	if (CInputHandle::GetInstance().GetKeyboardState('q') == InputState::Input_DownFirst)
	{
		CInputHandle::GetInstance().UpdateKeyboardState('q', InputState::Input_Down, 0, 0);
	}
	if (CInputHandle::GetInstance().GetKeyboardState('e') == InputState::Input_DownFirst)
	{
		CInputHandle::GetInstance().UpdateKeyboardState('e', InputState::Input_Down, 0, 0);
	}


	if (CInputHandle::GetInstance().GetKeyboardState('w') == InputState::Input_Down)
	{
		m_FanMove.z += 1;
	}
	if (CInputHandle::GetInstance().GetKeyboardState('s') == InputState::Input_Down)
	{
		m_FanMove.z -= 1;
	}
	if (CInputHandle::GetInstance().GetKeyboardState('a') == InputState::Input_Down)
	{
		m_FanMove.x -= 1;
	}
	if (CInputHandle::GetInstance().GetKeyboardState('d') == InputState::Input_Down)
	{
		m_FanMove.x += 1;
	}
	if (CInputHandle::GetInstance().GetKeyboardState('q') == InputState::Input_Down)
	{
		m_FanMove.y += 1;
	}
	if (CInputHandle::GetInstance().GetKeyboardState('e') == InputState::Input_Down)
	{
		m_FanMove.y -= 1;
	}

	// If moving
	if (m_FanMove != glm::vec3(0, 0, 0))
	{
		glm::vec3 newMove = glm::normalize(m_FanMove);

		glm::vec3 camDirection = m_Cam->GetCamPos() - m_Fan->GetPosition();
		camDirection.y = 0;
		camDirection = glm::normalize(camDirection);

		float angle = atanf(camDirection.z / camDirection.x);
		angle = -angle;

		glm::vec3 move = glm::vec3(0, 0, 0);
		move.x = newMove.x * sinf(angle) + newMove.z * cosf(angle);
		move.y = newMove.y;
		move.z = newMove.x * cosf(angle) - newMove.z * sinf(angle);

		if (camDirection.x >= 0)
		{
			move = -move;
		}

		// Apply movement
		m_FanMove = glm::normalize(move) * speed;
	}
}


