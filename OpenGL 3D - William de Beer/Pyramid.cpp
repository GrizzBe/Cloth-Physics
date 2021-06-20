// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   Pyramid.cpp
//  Description :   Pyramid object.
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // This Include 
#include "Pyramid.h" 
 // Implementation 
Pyramid::Pyramid()
{
	GLfloat Vertices_Pyramid[] = {
		////Position	
		// Bottom				
		-0.5,	-0.25,	0.5,	0.0f, -1.0f, 0.0f,		0.0f, 0.0f,
		-0.5,	-0.25,	-0.5,	0.0f, -1.0f, 0.0f,		0.0f, 1.0f,
		0.5,	-0.25,	-0.5,	0.0f, -1.0f, 0.0f,		1.0f, 1.0f,
		0.5,	-0.25,	0.5,	0.0f, -1.0f, 0.0f,		1.0f, 0.0f,
		
		// Side 1
		-0.5,	-0.25,	-0.5,	0.0f, 0.5f, -0.5f,		1.0f, 1.0f,
		0.5,	-0.25,	-0.5,	0.0f, 0.5f, -0.5f,		0.0f, 1.0f,

		// Side 2
		0.5,	-0.25,	-0.5,	0.5f, 0.5f, 0.0f,		1.0f, 1.0f,
		0.5,	-0.25,	0.5,	0.5f, 0.5f, 0.0f,		0.0f, 1.0f,

		// Side 3
		0.5,	-0.25,	0.5,	0.0f, 0.5f, 0.5f,		1.0f, 1.0f,
		-0.5,	-0.25,	0.5,	0.0f, 0.5f, 0.5f,		0.0f, 1.0f,

		// Side 4
		-0.5,	-0.25,	0.5,	-0.5f, 0.5f, 0.0f,		1.0f, 1.0f,
		-0.5,	-0.25,	-0.5,	-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,

		// Top
		0.0,	0.25,	0.0,	0.0f, 1.0f, 0.0f,		0.5f, 0.0f,
	};

	GLuint Indices_Pyramid[] = {
		1, 0, 3, // Base
		1, 3, 2, // Base

		4, 12, 5,   // Side 1
		6, 12, 7,   // Side 2
		8, 12, 9,   // Side 3
		10, 12, 11, // Side 4
	};

	unsigned int uiVertArraySize = sizeof(Vertices_Pyramid) / sizeof(GLfloat);
	unsigned int uiIndiArraySize = sizeof(Indices_Pyramid) / sizeof(GLuint);

	GLfloat* Vertices = new GLfloat[uiVertArraySize];
	GLuint* Indices = new GLuint[uiIndiArraySize];

	for (unsigned int i = 0; i < uiVertArraySize; i++)
	{
		Vertices[i] = Vertices_Pyramid[i];
	}

	for (unsigned int i = 0; i < uiIndiArraySize; i++)
	{
		Indices[i] = Indices_Pyramid[i];
	}

	VAO = ShaderLoader::GetInstance().CreateVAO(Vertices, Indices, sizeof(Vertices_Pyramid), sizeof(Indices_Pyramid));


	IndiceCount = sizeof(Indices_Pyramid);
	DrawType = GL_TRIANGLES;

	// Clean up memory 
	delete[] Vertices;
	Vertices = 0;

	delete[] Indices;
	Indices = 0;

	ObjPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	ObjScale = glm::vec3(1.0f, 1.0f, 1.0f);
	ObjRotationAngle = glm::vec3(0.0f, 0.0f, 0.0f);
}

Pyramid::~Pyramid()
{
}

/***********************
* Render: Renders Pyramid
* @author: William de Beer
* @parameter: Program to use, pointer to camera
********************/
void Pyramid::Render(GLuint _program, CCamera* _cam)
{
	glUseProgram(_program);

	float currTime = (float)glutGet(GLUT_ELAPSED_TIME) / 1000;
	GLuint Time = glGetUniformLocation(_program, "Time");
	glUniform1f(Time, currTime);

	CLightManager::GetInstance().RenderLighting(_program);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glUniform1i(glGetUniformLocation(_program, "ImageTexture"), 0);

	GLuint CamPos = glGetUniformLocation(_program, "camPos");
	glUniform3fv(CamPos, 1, glm::value_ptr(_cam->GetCamPos()));

	glActiveTexture(GL_TEXTURE1);
	glUniform1i(glGetUniformLocation(_program, "skybox"), 1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cam->GetCubeMapID());

	CObject::Render(_program, _cam);

	glBindVertexArray(VAO);
	glDrawElements(DrawType, IndiceCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

/***********************
* Update: Updates Pyramid
* @author: William de Beer
* @parameter: Delta time
********************/
void Pyramid::Update(float _dT)
{
	CObject::Update(_dT);
}
