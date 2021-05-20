// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   main.cpp
//  Description :   Initialises all the programs.
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // This Include
 // Library Includes 
#include <glew.h>
#include <freeglut.h>
#include <iostream>
#include <time.h>
 // Local Includes
#include "TestScene.h"
#include "SceneManager.h"
#include "ShaderLoader.h"
#include "Utilities.h"
 // Implementation

/***********************
* main: It is the main
* @author: William de Beer
* @parameter: argc, argv
* @return: int
********************/
int main(int argc, char** argv)
{
	srand((unsigned int)time(NULL));

	unsigned int ScreenWidth = CUtilities::GetInstance().GetResolution().x;
	unsigned int ScreenHeight = CUtilities::GetInstance().GetResolution().y;

	// Create display
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutSetOption(GLUT_MULTISAMPLE, 8);
	glEnable(GL_MULTISAMPLE);

	glutInitWindowPosition(50, 50);
	glutInitWindowSize(ScreenWidth, ScreenHeight);
	glutCreateWindow("Summative 1");

	// Initialise glew
	if (glewInit() != GLEW_OK)
	{
		std::cout << "oof" << std::endl;
	}

	// Set clear colour
	glClearColor(0.8f, 0.6f, 0.6f, 1.0f);
	glColor3f(0, 1, 0);

	// Culling
	glFrontFace(GL_CCW);
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Enable alpha
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Set current scene to new menu
	CSceneManager::SetCurrentScene(new CTestScene());

	// Set glut functions
	glutDisplayFunc(CSceneManager::Render);
	glutIdleFunc(CSceneManager::Update);
	glutCloseFunc(CSceneManager::ShutDown);

	glutKeyboardFunc(CSceneManager::KeyboardDown);
	glutKeyboardUpFunc(CSceneManager::KeyboardUp);

	glutMouseFunc(CSceneManager::MouseInteraction);
	glutPassiveMotionFunc(CSceneManager::MouseMotionTrackingPassive);
	glutMotionFunc(CSceneManager::MouseMotionTrackingInteracting);

	glutMainLoop();

	return 0;
}
