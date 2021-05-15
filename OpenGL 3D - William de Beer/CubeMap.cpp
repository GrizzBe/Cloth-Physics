// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   CubeMap.cpp
//  Description :   Object that holds the skybox.
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // Library Includes 
#include <stb_image.h>
 // Local Includes 
#include "ShaderLoader.h"
 // This Include 
#include "CubeMap.h"
 // Implementation 
CCubeMap::CCubeMap(CCamera* _cam, std::vector<std::string> _textures)
{
	m_Cam = _cam;

	GLfloat Vertices_Cube[] = {
		////Position	
		//Front
		-0.5,	0.5,	0.5,
		-0.5,	-0.5,	0.5,
		0.5,	-0.5,	0.5,
		0.5,	0.5,	0.5,
		//Back
		-0.5,	0.5,	-0.5,
		-0.5,	-0.5,	-0.5,
		0.5,	-0.5,	-0.5,
		0.5,	0.5,	-0.5,
		// Left
		-0.5,	0.5,	-0.5,
		-0.5,	-0.5,	-0.5,
		-0.5,	-0.5,	0.5,
		-0.5,	0.5,	0.5,
		// Right
		0.5,	0.5,	0.5,
		0.5,	-0.5,	0.5,
		0.5,	-0.5,	-0.5,
		0.5,	0.5,	-0.5,
		// Top
		-0.5,	0.5,	-0.5,
		-0.5,	0.5,	0.5,
		0.5,	0.5,	0.5,
		0.5,	0.5,	-0.5,
		// Bottom
		-0.5,	-0.5,	0.5,
		-0.5,	-0.5,	-0.5,
		0.5,	-0.5,	-0.5,
		0.5,	-0.5,	0.5,
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


	// Create cubemap program
	Program_Cubemap = ShaderLoader::GetInstance().CreateProgram("CubeMap.vs", "CubeMap.fs");
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Clean up memory 
	delete[] Vertices;
	Vertices = 0;

	delete[] Indices;
	Indices = 0;

	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);

	// Textures
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	int ImageHeight;
	int ImageWidth;
	int ImageComponents;
	unsigned char* ImageData;

	for (GLuint i = 0; i < 6; i ++)
	{

		// Form file path
		std::string vString = "Resources/Textures/CubeMap/Fire/";
		vString.append(_textures[i]);

		ImageData = stbi_load(vString.c_str(),
			&ImageWidth, &ImageHeight, &ImageComponents, 0);

		if (ImageData)
		{
			GLint LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, LoadedComponents, ImageWidth, ImageHeight, 0,
				LoadedComponents, GL_UNSIGNED_BYTE, ImageData);

		}
		else
		{
			std::cout << "Image not load" << std::endl;
		}
		stbi_image_free(ImageData);
	}

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

CCubeMap::~CCubeMap()
{
}

/***********************
* Update: Updates cubemap
* @author: William de Beer
* @parameter: Delta time
********************/
void CCubeMap::Update()
{
	glm::mat4 model = glm::scale(glm::mat4(), glm::vec3(2000.0f, 2000.0f, 2000.0f));
	PVM = m_Cam->GetProjMat() * m_Cam->GetViewMat() * model;
}

/***********************
* Render: Renders cubemap
* @author: William de Beer
********************/
void CCubeMap::Render()
{
	glFrontFace(GL_CW); // Reverse winding order
	glUseProgram(Program_Cubemap);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);
	glUniform1i(glGetUniformLocation(Program_Cubemap, "cubeSampler"), 0);
	glUniformMatrix4fv(glGetUniformLocation(Program_Cubemap, "PVM"), 1, GL_FALSE, glm::value_ptr(PVM));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
	glFrontFace(GL_CCW); // Set winding order to normal again
}

/***********************
* GetTextureID: Returns texture ID of the cubemap object.
* @author: William de Beer
* @return: GLuint texture ID
********************/
GLuint CCubeMap::GetTextureID()
{
	return TextureID;
}
