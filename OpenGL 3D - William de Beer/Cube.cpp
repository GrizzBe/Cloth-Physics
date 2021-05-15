// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   Cube.cpp
//  Description :   Cube object.
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // This Include 
#include "Cube.h" 
 // Implementation 
Cube::Cube()
{
	GLfloat Vertices_Cube[] = {
		////Position	
		//Front
		-0.5,	0.5,	0.5,	0.0f, 0.0f, 1.0f,		0.0f, 0.0f,
		-0.5,	-0.5,	0.5,	0.0f, 0.0f, 1.0f,		0.0f, 1.0f,
		0.5,	-0.5,	0.5,	0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
		0.5,	0.5,	0.5,	0.0f, 0.0f, 1.0f,		1.0f, 0.0f,
		//Back					
		-0.5,	0.5,	-0.5,	0.0f, 0.0f, -1.0f,		1.0f, 0.0f,
		-0.5,	-0.5,	-0.5,	0.0f, 0.0f, -1.0f,		1.0f, 1.0f,
		0.5,	-0.5,	-0.5,	0.0f, 0.0f, -1.0f,		0.0f, 1.0f,
		0.5,	0.5,	-0.5,	0.0f, 0.0f, -1.0f,		0.0f, 0.0f,
		// Left					
		-0.5,	0.5,	-0.5,	-1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		-0.5,	-0.5,	-0.5,	-1.0f, 0.0f, 0.0f,		0.0f, 1.0f,
		-0.5,	-0.5,	0.5,	-1.0f, 0.0f, 0.0f,		1.0f, 1.0f,
		-0.5,	0.5,	0.5,	-1.0f, 0.0f, 0.0f,		1.0f, 0.0f,
		// Right				
		0.5,	0.5,	0.5,	1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		0.5,	-0.5,	0.5,	1.0f, 0.0f, 0.0f,		0.0f, 1.0f,
		0.5,	-0.5,	-0.5,	1.0f, 0.0f, 0.0f,		1.0f, 1.0f,
		0.5,	0.5,	-0.5,	1.0f, 0.0f, 0.0f,		1.0f, 0.0f,
		// Top					
		-0.5,	0.5,	-0.5,	0.0f, 1.0f, 0.0f,		0.0f, 0.0f,
		-0.5,	0.5,	0.5,	0.0f, 1.0f, 0.0f,		0.0f, 1.0f,
		0.5,	0.5,	0.5,	0.0f, 1.0f, 0.0f,		1.0f, 1.0f,
		0.5,	0.5,	-0.5,	0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
		// Bottom				
		-0.5,	-0.5,	0.5,	0.0f, -1.0f, 0.0f,		0.0f, 0.0f,
		-0.5,	-0.5,	-0.5,	0.0f, -1.0f, 0.0f,		0.0f, 1.0f,
		0.5,	-0.5,	-0.5,	0.0f, -1.0f, 0.0f,		1.0f, 1.0f,
		0.5,	-0.5,	0.5,	0.0f, -1.0f, 0.0f,		1.0f, 0.0f,
	};

	GLuint Indices_Cube[] = {
		0,1,2, // Front
		0,2,3, // Front

		7,6,5, // Back
		7,5,4, // Back

		8,9,10, // Left
		8,10,11, // Left

		12,13,14, // Right
		12,14,15, // Right

		16,17,18, // Top
		16,18,19, // Top

		20,21,22, // Bottom
		20,22,23, // Bottom
	};

	unsigned int uiVertArraySize = sizeof(Vertices_Cube) / sizeof(GLfloat);
	unsigned int uiIndiArraySize = sizeof(Indices_Cube) / sizeof(GLuint);

	GLfloat* Vertices = new GLfloat[uiVertArraySize];
	GLuint* Indices = new GLuint[uiIndiArraySize];

	for (unsigned int i = 0; i < uiVertArraySize; i++)
	{
		Vertices[i] = Vertices_Cube[i];
	}

	for (unsigned int i = 0; i < uiIndiArraySize; i++)
	{
		Indices[i] = Indices_Cube[i];
	}

	GLuint EBO;
	GLuint VBO;

	// VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices_Cube), Indices, GL_STATIC_DRAW);

	// VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Cube), Vertices, GL_STATIC_DRAW);

	// Vertex Attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);


	IndiceCount = sizeof(Indices_Cube);
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

Cube::~Cube()
{
}

/***********************
* Render: Renders cube
* @author: William de Beer
* @parameter: Program to use, pointer to camera
********************/
void Cube::Render(GLuint _program, CCamera* _cam)
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
* Update: Updates Cube
* @author: William de Beer
* @parameter: Delta time
********************/
void Cube::Update(float _dT)
{
	CObject::Update(_dT);
}
