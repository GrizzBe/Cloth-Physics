// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   Capsule.cpp
//  Description :   Capsule object.
//  Author      :   William de Beer, Callan Moore 
//  Mail        :   William.Beer@mds.ac.nz
// 
 // Local Includes 
#include "ShaderLoader.h"
#include "LightManager.h"
 // This Include 
#include "Capsule.h"
 // Implementation 
Capsule::Capsule()
{
	float radius = 1.0f;

	const int sections = 20;
	const int vertexAttrib = 8;
	const int indexPerQuad = 6;

	double phi = 0;
	double theta = 0;

	// Create vertices as pointers
	GLfloat* vertices = new GLfloat[(sections) * (sections)*vertexAttrib];
	int offset = 0;
	for (int i = 0; i < sections; i++)
	{
		theta = 0;

		for (int j = 0; j < sections; j++)
		{
			float x = cosf((float)phi) * sinf((float)theta);
			float y = cosf((float)theta);
			float z = sinf((float)phi) * sinf((float)theta);

			if (y < 0)
				y -= 0.5f;
			else
				y += 0.5f;


			vertices[offset++] = x * radius;
			vertices[offset++] = y * radius;
			vertices[offset++] = z * radius;

			vertices[offset++] = x;
			vertices[offset++] = y;
			vertices[offset++] = z;

			vertices[offset++] = (float)i / (sections - 1);
			vertices[offset++] = (float)j / (sections - 1);

			theta += (M_PI / (double)(sections - 1));
		}

		phi += (2 * M_PI) / (double)(sections - 1);
	}

	// Create indices as pointers
	GLuint* indices = new GLuint[(sections) * (sections)*indexPerQuad];
	offset = 0;
	for (int i = 0; i < sections; i++)
	{
		for (int j = 0; j < sections; j++)
		{
			indices[offset++] = (((i + 1) % sections) * sections) + ((j + 1) % sections);
			indices[offset++] = (i * sections) + (j);
			indices[offset++] = (((i + 1) % sections) * sections) + (j);

			indices[offset++] = (i * sections) + ((j + 1) % sections);
			indices[offset++] = (i * sections) + (j);
			indices[offset++] = (((i + 1) % sections) * sections) + ((j + 1) % sections);
		}
	}

	GLuint VBO, EBO;

	// Create size variables
	size_t verticesSize = (sections) * (sections)*vertexAttrib * sizeof(GLfloat);
	size_t indicesSize = (sections) * (sections)*indexPerQuad * sizeof(GLuint);

	// Create VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	IndiceCount = (sections) * (sections)*indexPerQuad;
	DrawType = GL_TRIANGLES;

	// Initialise transformations
	ObjPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	ObjScale = glm::vec3(1.0f, 1.0f, 1.0f);
	ObjRotationAngle = glm::vec3(0.0f, 0.0f, 0.0f);

	// Clean up memory 
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;
}

Capsule::~Capsule()
{
}

/***********************
* Render: Renders Capsule
* @author: William de Beer
* @parameter: Program to use, pointer to camera
********************/
void Capsule::Render(GLuint _program, CCamera* _cam)
{
	glUseProgram(_program);

	CObject::Render(_program, _cam);

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

	glBindVertexArray(VAO);
	glDrawElements(DrawType, IndiceCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);


	glBegin(GL_LINES);
	glm::vec3 capsuleEnd[2];
	capsuleEnd[0] = ObjPosition - glm::vec3(0, 1, 0) * 0.5f;
	capsuleEnd[1] = ObjPosition + glm::vec3(0, 1, 0) * 0.5f;

	glColor3f(0, 1, 0);

	glm::vec4 pos3D = _cam->GetPVMatrix() * glm::vec4(capsuleEnd[0], 1.0f);
	glm::vec3 position = glm::vec3(pos3D.x, pos3D.y, pos3D.z) / pos3D.w;

	glDisable(GL_DEPTH_TEST);
	// Vertex 1
	glVertex3f(position.x,
		position.y,
		position.z);


	pos3D = _cam->GetPVMatrix() * glm::vec4(capsuleEnd[1], 1.0f);
	position = glm::vec3(pos3D.x, pos3D.y, pos3D.z) / pos3D.w;

	// Vertex 2
	glVertex3f(position.x,
		position.y,
		position.z);
	glEnd();
	glEnable(GL_DEPTH_TEST);
}

/***********************
* Update: Updates Capsule
* @author: William de Beer
* @parameter: Delta time
********************/
void Capsule::Update(float _dT)
{
	CObject::Update(_dT);
}
