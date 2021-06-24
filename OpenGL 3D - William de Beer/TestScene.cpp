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

	m_HeightSlider = 0;
	m_WidthSlider = 0;
	m_HookSlider = 0;
	m_WindSlider = 0;
	m_fUpdateRate = 0;

	m_bUseWireframe = false;
	m_bActiveFan = true;
	m_bFanVisuals = false;
	m_bClothUntangle = false;
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
	if (m_Fan != nullptr)
	{
		delete m_Fan;
		m_Fan = 0;
	}
	if (m_FanVisuals != nullptr)
	{
		delete m_FanVisuals;
		m_FanVisuals = 0;
	}
	if (m_Pyramid != nullptr)
	{
		delete m_Pyramid;
		m_Pyramid = 0;
	}
	if (m_Capsule != nullptr)
	{
		delete m_Capsule;
		m_Capsule = 0;
	}
	if (m_Sphere != nullptr)
	{
		delete m_Sphere;
		m_Sphere = 0;
	}
	if (m_HeightSlider != nullptr)
	{
		delete m_HeightSlider;
		m_HeightSlider = 0;
	}
	if (m_WidthSlider != nullptr)
	{
		delete m_WidthSlider;
		m_WidthSlider = 0;
	}
	if (m_HookSlider != nullptr)
	{
		delete m_HookSlider;
		m_HookSlider = 0;
	}
	if (m_WindSlider != nullptr)
	{
		delete m_WindSlider;
		m_WindSlider = 0;
	}
	if (m_txtUpdateRate != nullptr)
	{
		delete m_txtUpdateRate;
		m_txtUpdateRate = 0;
	}
	std::vector<CText*>::iterator it = m_txtInstructions.begin();
	while (it != m_txtInstructions.end())
	{
		// Delete vector contents
		delete* it;
		it = m_txtInstructions.erase((it));
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
	
	// Create size slider
	m_WindAreaSlider = new Slider(glm::vec2(-0.95f, -0.5f), glm::vec2(0.02f, 0.02f), 1, 5, CUtilities::GetInstance().GetWindArea(), "Wind Size");
	m_WindSlider = new Slider(glm::vec2(-0.95f, -0.6f), glm::vec2(0.02f, 0.02f), 0, 200, CUtilities::GetInstance().GetWindSpeed(), "Wind Speed");
	m_WidthSlider = new Slider(glm::vec2(-0.95f, -0.7f), glm::vec2(0.02f, 0.02f), 2, 49, CUtilities::GetInstance().GetClothSize().x, "Cloth Width");
	m_HeightSlider = new Slider(glm::vec2(-0.95f, -0.8f), glm::vec2(0.02f, 0.02f), 2, 49, CUtilities::GetInstance().GetClothSize().y, "Cloth Height");

	// Create hook slider
	m_HookSlider = new Slider(glm::vec2(-0.95f, -0.9f), glm::vec2(0.02f, 0.02f), 1, 25, CUtilities::GetInstance().GetHookDensity(), "Hook Density");

	// Create cloth
	m_Cloth = new Cloth(CUtilities::GetInstance().GetClothSize().x, CUtilities::GetInstance().GetClothSize().y);

	// Create camera
	m_Cam = new CCamera(true);
	m_Cam->Initialise(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// Create fan
	m_Fan = new Cube();
	m_Fan->SetPosition(glm::vec3(0.0f, 0.0f, -5.0f));
	m_Fan->SetScale(glm::vec3(1.0f , 1.0f , 1.0f));
	m_Fan->SetTexture("Fan.png");

	// Create fan
	m_FanVisuals = new Cube();
	m_FanVisuals->SetPosition(glm::vec3(0.0f, 0.0f, -5.0f));
	m_FanVisuals->SetScale(glm::vec3(CUtilities::GetInstance().GetWindArea(), CUtilities::GetInstance().GetWindArea(), 50.0f));
	m_FanVisuals->SetTexture("Wind.png");

	// Create sphere
	m_Sphere = new Sphere();
	m_Sphere->SetPosition(glm::vec3(3.0f, 0.0f, -5.0f));
	m_Sphere->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
	m_Sphere->SetTexture("Map/earth.jpg");

	// Create capsule
	m_Capsule = new Capsule();
	m_Capsule->SetPosition(glm::vec3(6.0f, 0.0f, -5.0f));
	m_Capsule->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
	m_Capsule->SetTexture("Capsule.png");

	// Create pyramid
	m_Pyramid = new Pyramid();
	m_Pyramid->SetPosition(glm::vec3(-3.0f, 0.0f, -5.0f));
	m_Pyramid->SetScale(glm::vec3(2.0f, 2.0f, 2.0f));
	m_Pyramid->SetTexture("Pyramid.png");

	// Create ground
	m_Ground = new Quad();
	m_Ground->SetPosition(glm::vec3(0, -5.1, 0));
	m_Ground->SetScale(glm::vec3(20, 1, 20));
	m_Ground->SetReflectivity(1.0f);
	m_Ground->SetTexture("Map/Ground.jpg");

	// Update rate text
	std::string font = "Resources/Fonts/VERDANA.TTF";
	std::string text = std::to_string(m_fUpdateRate);
	glm::vec2 textPosition = glm::vec2(0.0f, CUtilities::GetInstance().GetResolution().y - 26.0f);

	m_txtUpdateRate = new CText(text, font, glm::vec2(24.0f, 24.0f), textPosition, glm::vec3(0.0f, 255.0f, 0.0f), glm::vec2(1.0f, 1.0f));

#pragma region Instructions
	// Instruction Text
	glm::vec2 startPos = glm::vec2(20.0f, CUtilities::GetInstance().GetResolution().y - 52.0f);
	float headerOffset = 10.0f;
	float spacing = 26.0f;
	float fontSize = 18.0f;
	
	text = "- Mouse Controls:";
	textPosition = glm::vec2(startPos.x - headerOffset, startPos.y - spacing * m_txtInstructions.size());
	CText* newText = new CText(text, font, glm::vec2(fontSize, fontSize), textPosition, glm::vec3(0.0f, 255.0f, 0.0f), glm::vec2(1.0f, 1.0f));
	m_txtInstructions.push_back(newText);

	text = "[M1] : Push Cloth";
	textPosition = glm::vec2(startPos.x, startPos.y - spacing * m_txtInstructions.size());
	newText = new CText(text, font, glm::vec2(fontSize, fontSize), textPosition, glm::vec3(0.0f, 255.0f, 0.0f), glm::vec2(1.0f, 1.0f));
	m_txtInstructions.push_back(newText);

	text = "[M1] + [G] : Grab Cloth";
	textPosition = glm::vec2(startPos.x, startPos.y - spacing * m_txtInstructions.size());
	newText = new CText(text, font, glm::vec2(fontSize, fontSize), textPosition, glm::vec3(0.0f, 255.0f, 0.0f), glm::vec2(1.0f, 1.0f));
	m_txtInstructions.push_back(newText);

	text = "[M1] + [T] : Tear Cloth";
	textPosition = glm::vec2(startPos.x, startPos.y - spacing * m_txtInstructions.size());
	newText = new CText(text, font, glm::vec2(fontSize, fontSize), textPosition, glm::vec3(0.0f, 255.0f, 0.0f), glm::vec2(1.0f, 1.0f));
	m_txtInstructions.push_back(newText);

	text = "[M1] + [I] : Ignite Cloth";
	textPosition = glm::vec2(startPos.x, startPos.y - spacing * m_txtInstructions.size());
	newText = new CText(text, font, glm::vec2(fontSize, fontSize), textPosition, glm::vec3(0.0f, 255.0f, 0.0f), glm::vec2(1.0f, 1.0f));
	m_txtInstructions.push_back(newText);

	text = "- Object Controls";
	textPosition = glm::vec2(startPos.x - headerOffset, startPos.y - spacing * m_txtInstructions.size());
	newText = new CText(text, font, glm::vec2(fontSize, fontSize), textPosition, glm::vec3(0.0f, 255.0f, 0.0f), glm::vec2(1.0f, 1.0f));
	m_txtInstructions.push_back(newText);

	text = "[WASDQE] : Move selected object";
	textPosition = glm::vec2(startPos.x, startPos.y - spacing * m_txtInstructions.size());
	newText = new CText(text, font, glm::vec2(fontSize, fontSize), textPosition, glm::vec3(0.0f, 255.0f, 0.0f), glm::vec2(1.0f, 1.0f));
	m_txtInstructions.push_back(newText);

	text = "[1] : Select fan/cube";
	textPosition = glm::vec2(startPos.x, startPos.y - spacing * m_txtInstructions.size());
	newText = new CText(text, font, glm::vec2(fontSize, fontSize), textPosition, glm::vec3(0.0f, 255.0f, 0.0f), glm::vec2(1.0f, 1.0f));
	m_txtInstructions.push_back(newText);

	text = "[2] : Select sphere";
	textPosition = glm::vec2(startPos.x, startPos.y - spacing * m_txtInstructions.size());
	newText = new CText(text, font, glm::vec2(fontSize, fontSize), textPosition, glm::vec3(0.0f, 255.0f, 0.0f), glm::vec2(1.0f, 1.0f));
	m_txtInstructions.push_back(newText);

	text = "[3] : Select pyramid";
	textPosition = glm::vec2(startPos.x, startPos.y - spacing * m_txtInstructions.size());
	newText = new CText(text, font, glm::vec2(fontSize, fontSize), textPosition, glm::vec3(0.0f, 255.0f, 0.0f), glm::vec2(1.0f, 1.0f));
	m_txtInstructions.push_back(newText);

	text = "[4] : Select capsule";
	textPosition = glm::vec2(startPos.x, startPos.y - spacing * m_txtInstructions.size());
	newText = new CText(text, font, glm::vec2(fontSize, fontSize), textPosition, glm::vec3(0.0f, 255.0f, 0.0f), glm::vec2(1.0f, 1.0f));
	m_txtInstructions.push_back(newText);

	text = "- Misc Controls";
	textPosition = glm::vec2(startPos.x - headerOffset, startPos.y - spacing * m_txtInstructions.size());
	newText = new CText(text, font, glm::vec2(fontSize, fontSize), textPosition, glm::vec3(0.0f, 255.0f, 0.0f), glm::vec2(1.0f, 1.0f));
	m_txtInstructions.push_back(newText);

	text = "[F] : Toggle fan";
	textPosition = glm::vec2(startPos.x, startPos.y - spacing * m_txtInstructions.size());
	newText = new CText(text, font, glm::vec2(fontSize, fontSize), textPosition, glm::vec3(0.0f, 255.0f, 0.0f), glm::vec2(1.0f, 1.0f));
	m_txtInstructions.push_back(newText);

	text = "[X] : Toggle fan visuals";
	textPosition = glm::vec2(startPos.x, startPos.y - spacing * m_txtInstructions.size());
	newText = new CText(text, font, glm::vec2(fontSize, fontSize), textPosition, glm::vec3(0.0f, 255.0f, 0.0f), glm::vec2(1.0f, 1.0f));
	m_txtInstructions.push_back(newText);

	text = "[U] : Toggle untangle";
	textPosition = glm::vec2(startPos.x, startPos.y - spacing * m_txtInstructions.size());
	newText = new CText(text, font, glm::vec2(fontSize, fontSize), textPosition, glm::vec3(0.0f, 255.0f, 0.0f), glm::vec2(1.0f, 1.0f));
	m_txtInstructions.push_back(newText);

	text = "[Y] : Toggle wireframe";
	textPosition = glm::vec2(startPos.x, startPos.y - spacing * m_txtInstructions.size());
	newText = new CText(text, font, glm::vec2(fontSize, fontSize), textPosition, glm::vec3(0.0f, 255.0f, 0.0f), glm::vec2(1.0f, 1.0f));
	m_txtInstructions.push_back(newText);

	text = "[V] : Drop cloth";
	textPosition = glm::vec2(startPos.x, startPos.y - spacing * m_txtInstructions.size());
	newText = new CText(text, font, glm::vec2(fontSize, fontSize), textPosition, glm::vec3(0.0f, 255.0f, 0.0f), glm::vec2(1.0f, 1.0f));
	m_txtInstructions.push_back(newText);

	text = "[.]/[,] : Move rings";
	textPosition = glm::vec2(startPos.x, startPos.y - spacing * m_txtInstructions.size());
	newText = new CText(text, font, glm::vec2(fontSize, fontSize), textPosition, glm::vec3(0.0f, 255.0f, 0.0f), glm::vec2(1.0f, 1.0f));
	m_txtInstructions.push_back(newText);

	text = "[R] : Restart Scene";
	textPosition = glm::vec2(startPos.x, startPos.y - spacing * m_txtInstructions.size());
	newText = new CText(text, font, glm::vec2(fontSize, fontSize), textPosition, glm::vec3(0.0f, 255.0f, 0.0f), glm::vec2(1.0f, 1.0f));
	m_txtInstructions.push_back(newText);

#pragma endregion

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
	m_Sphere->Render(Program_Object, m_Cam);
	m_Pyramid->Render(Program_Object, m_Cam);
	m_Capsule->Render(Program_Object, m_Cam);
	m_Fan->Render(Program_Object, m_Cam);

	if (m_bFanVisuals)
		m_FanVisuals->Render(Program_Object, m_Cam);

	glDisable(GL_DEPTH_TEST);
	m_HeightSlider->Render(Program_UI, m_Cam);
	m_WidthSlider->Render(Program_UI, m_Cam);
	m_HookSlider->Render(Program_UI, m_Cam);
	m_WindSlider->Render(Program_UI, m_Cam);
	m_WindAreaSlider->Render(Program_UI, m_Cam);
	m_txtUpdateRate->Render();
	for (auto i : m_txtInstructions)
	{
		i->Render();
	}
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

	float timeStep;
	if (m_bClothUntangle)
		timeStep = 1.0f / 30.0f;
	else
		timeStep = 1.0f / 60.0f;

	//timeStep = 0.0167f;
	if (m_fDeltaTime < timeStep)
	{
		return;
	}

	m_txtUpdateRate->SetText("Update Rate: " + std::to_string((int)std::ceilf(1.0f / m_fDeltaTime)));

	m_fDeltaTime = timeStep;

	m_fPreviousTimeStamp = m_fCurrentTime;

	switch (object)
	{
	case ControlledObject::FAN:
		ApplyMovement(m_Fan);
		break;
	case ControlledObject::SPHERE:
		ApplyMovement(m_Sphere);
		break;
	case ControlledObject::PYRAMID:
		ApplyMovement(m_Pyramid);
		break;
	case ControlledObject::CAPSULE:
		ApplyMovement(m_Capsule);
		break;
	default:
		break;
	}

	m_Fan->Update(m_fDeltaTime);
	m_FanVisuals->SetPosition(m_Fan->GetPosition());
	m_FanVisuals->SetScale(glm::vec3(CUtilities::GetInstance().GetWindArea(), CUtilities::GetInstance().GetWindArea(), 50.0f));

	m_FanVisuals->Update(m_fDeltaTime);

	m_Sphere->Update(m_fDeltaTime);
	m_Pyramid->Update(m_fDeltaTime);
	m_Capsule->Update(m_fDeltaTime);

	if (m_bActiveFan)
	{
		m_Cloth->ApplyWind(m_Fan->GetPosition());
	}

	m_Cloth->CalculateCollision(CollisionType::SPHERE, m_Sphere->GetPosition(), m_Sphere->GetScale());
	m_Cloth->CalculateCollision(CollisionType::CUBE, m_Fan->GetPosition(), m_Fan->GetScale());
	m_Cloth->CalculateCollision(CollisionType::PYRAMID, m_Pyramid->GetPosition(), m_Pyramid->GetScale());
	m_Cloth->CalculateCollision(CollisionType::CAPSULE, m_Capsule->GetPosition(), m_Capsule->GetScale());

	if (m_bClothUntangle)
		m_Cloth->Untangle();

	m_Cloth->Update(m_fDeltaTime, m_Cam);
	m_Cam->Process(0.0f, 0.0f, 0.0f);
	m_Ground->Update(m_fDeltaTime);

	CUtilities::GetInstance().SetClothSize(glm::vec2(m_WidthSlider->Update(m_fDeltaTime), m_HeightSlider->Update(m_fDeltaTime)));
	CUtilities::GetInstance().SetHookDensity(m_HookSlider->Update(m_fDeltaTime));
	CUtilities::GetInstance().SetWindSpeed(m_WindSlider->Update(m_fDeltaTime));
	CUtilities::GetInstance().SetWindArea(m_WindAreaSlider->Update(m_fDeltaTime));

	CAudioManager::GetInstance().Process();
	glutPostRedisplay();

	ProcessInput();
}

/***********************
* ProcessInput: Processes inputs.
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

	// Set fan to be controlled
	if (CInputHandle::GetInstance().GetKeyboardState('1') == InputState::Input_DownFirst)
	{
		CInputHandle::GetInstance().UpdateKeyboardState('1', InputState::Input_Down, 0, 0);
		object = ControlledObject::FAN;
	}
	// Set sphere to be controlled
	if (CInputHandle::GetInstance().GetKeyboardState('2') == InputState::Input_DownFirst)
	{
		CInputHandle::GetInstance().UpdateKeyboardState('2', InputState::Input_Down, 0, 0);
		object = ControlledObject::SPHERE;
	}
	// Set pyramid to be controlled
	if (CInputHandle::GetInstance().GetKeyboardState('3') == InputState::Input_DownFirst)
	{
		CInputHandle::GetInstance().UpdateKeyboardState('3', InputState::Input_Down, 0, 0);
		object = ControlledObject::PYRAMID;
	}
	// Set capsule to be controlled
	if (CInputHandle::GetInstance().GetKeyboardState('4') == InputState::Input_DownFirst)
	{
		CInputHandle::GetInstance().UpdateKeyboardState('4', InputState::Input_Down, 0, 0);
		object = ControlledObject::CAPSULE;
	}

	// Drop cloth
	if (CInputHandle::GetInstance().GetKeyboardState('v') == InputState::Input_DownFirst)
	{
		CInputHandle::GetInstance().UpdateKeyboardState('v', InputState::Input_Down, 0, 0);
		m_Cloth->DropCloth();
	}

	// Toggle fan
	if (CInputHandle::GetInstance().GetKeyboardState('f') == InputState::Input_DownFirst)
	{
		CInputHandle::GetInstance().UpdateKeyboardState('f', InputState::Input_Down, 0, 0);
		m_bActiveFan = !m_bActiveFan;
	}

	// Toggle fan visuals
	if (CInputHandle::GetInstance().GetKeyboardState('x') == InputState::Input_DownFirst)
	{
		CInputHandle::GetInstance().UpdateKeyboardState('x', InputState::Input_Down, 0, 0);
		m_bFanVisuals = !m_bFanVisuals;
	}

	// Toggle cloth untangle
	if (CInputHandle::GetInstance().GetKeyboardState('u') == InputState::Input_DownFirst)
	{
		CInputHandle::GetInstance().UpdateKeyboardState('u', InputState::Input_Down, 0, 0);
		m_bClothUntangle = !m_bClothUntangle;
	}

	Move();

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

void CTestScene::Move()
{
	m_Move = glm::vec3(0, 0, 0);
	float speed = 5.0f;

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
		m_Move.z += 1;
	}
	if (CInputHandle::GetInstance().GetKeyboardState('s') == InputState::Input_Down)
	{
		m_Move.z -= 1;
	}
	if (CInputHandle::GetInstance().GetKeyboardState('a') == InputState::Input_Down)
	{
		m_Move.x -= 1;
	}
	if (CInputHandle::GetInstance().GetKeyboardState('d') == InputState::Input_Down)
	{
		m_Move.x += 1;
	}
	if (CInputHandle::GetInstance().GetKeyboardState('q') == InputState::Input_Down)
	{
		m_Move.y += 1;
	}
	if (CInputHandle::GetInstance().GetKeyboardState('e') == InputState::Input_Down)
	{
		m_Move.y -= 1;
	}

	// If moving
	if (m_Move != glm::vec3(0, 0, 0))
	{
		glm::vec3 newMove = glm::normalize(m_Move);

		glm::vec3 camDirection = m_Cam->GetCamPos();
		camDirection.y = 0;
		camDirection = glm::normalize(camDirection);

		float angle = atanf(camDirection.z / camDirection.x);
		angle = -angle;

		glm::vec3 move = glm::vec3(0, 0, 0);
		move.x = newMove.x * sinf(angle) + newMove.z * cosf(angle);
		move.z = newMove.x * cosf(angle) - newMove.z * sinf(angle);

		if (camDirection.x >= 0)
		{
			move = -move;
		}

		move.y = newMove.y;

		// Apply movement
		m_Move = glm::normalize(move) * speed;
	}
}

void CTestScene::ApplyMovement(CObject* _object)
{
	_object->SetPosition(_object->GetPosition() + m_Move * m_fDeltaTime);
	if (_object->GetPosition().x > 10.0f)
	{
		_object->SetPosition(glm::vec3(10.0f, _object->GetPosition().y, _object->GetPosition().z));
	}
	if (_object->GetPosition().x < -10.0f)
	{
		_object->SetPosition(glm::vec3(-10.0f, _object->GetPosition().y, _object->GetPosition().z));
	}
	if (_object->GetPosition().z > 10.0f)
	{
		_object->SetPosition(glm::vec3(_object->GetPosition().x, _object->GetPosition().y, 10.0f));
	}
	if (_object->GetPosition().z < -10.0f)
	{
		_object->SetPosition(glm::vec3(_object->GetPosition().x, _object->GetPosition().y, -10.0f));
	}
}


