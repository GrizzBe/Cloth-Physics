// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   ClothQuad.h
//  Description :   Quad object.
//  Author      :   William de Beer 
//  Mail        :   William.Beer@mds.ac.nz
// 
 // This Includes 
#include "ClothQuad.h"
ClothQuad::ClothQuad(int _width, int _height, ClothNode** _nodes)
{
	SetTexture("Picnic.jpg");
	SetShininess(32);

	m_ClothNodes = _nodes;
	m_Width = _width;
	m_Height = _height;

	CreateVertices();
	CreateIndices();
	CreateVAO();
}

ClothQuad::~ClothQuad()
{
	if (m_Vertices != nullptr)
	{
		delete[] m_Vertices;
		m_Vertices = 0;
	}
	if (m_Indices != nullptr)
	{
		delete[] m_Indices;
		m_Indices = 0;
	}
}

/***********************
* Render: Renders object
* @author: William de Beer
* @parameter: Program to use, pointer to camera
********************/
void ClothQuad::Render(GLuint _program, CCamera* _cam)
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

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint) * m_AllIndices.size() * 3, m_Indices);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * m_Width * m_Height * 8, m_Vertices);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, m_AllIndices.size() * 3, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glUseProgram(0);
}

/***********************
* Update: Updates object
* @author: William de Beer
* @parameter: Delta time
********************/
void ClothQuad::Update(float _dT)
{
	// Update vertices
	for (unsigned int i = 0; i < m_Width * m_Height * 8; i += 8)
	{
		m_Vertices[i + 0] = m_ClothNodes[i / 8]->GetPos().x;
		m_Vertices[i + 1] = m_ClothNodes[i / 8]->GetPos().y;
		m_Vertices[i + 2] = m_ClothNodes[i / 8]->GetPos().z;
		m_Vertices[i + 3] = m_ClothNodes[i / 8]->GetColor().x;
		m_Vertices[i + 4] = m_ClothNodes[i / 8]->GetColor().y;
		m_Vertices[i + 5] = m_ClothNodes[i / 8]->GetColor().z;
		m_Vertices[i + 6] = m_ClothNodes[i / 8]->GetUV().x;
		m_Vertices[i + 7] = m_ClothNodes[i / 8]->GetUV().y;
	}

	// Update indices
	for (unsigned int i = 0; i < m_AllIndices.size() * 3; i+=3)
	{
		m_Indices[i + 0] = m_AllIndices.at(i / 3).x;
		m_Indices[i + 1] = m_AllIndices.at(i / 3).y;
		m_Indices[i + 2] = m_AllIndices.at(i / 3).z;
	}
}

/***********************
* CreateVertices: Create vertices array
* @author: William de Beer
********************/
void ClothQuad::CreateVertices()
{
	m_Vertices = new GLfloat[m_Width * m_Height * 8];
}

/***********************
* CreateIndices: Create indices vector & array
* @author: William de Beer
********************/
void ClothQuad::CreateIndices()
{
	// Create vector of triangle vertices
	for (unsigned int i = 0; i < m_Width - 1; i++)
	{
		unsigned int row1 = i * (m_Height);
		unsigned int row2 = (i + 1) * (m_Height);
		for (unsigned int j = 0; j < m_Height - 1; j++)
		{
			m_AllIndices.push_back(glm::uvec3(row1 + j, row1 + j + 1, row2 + j + 1));
			m_AllIndices.push_back(glm::uvec3(row1 + j, row2 + j, row2 + j + 1));
		}
	}
	m_Indices = new GLuint[m_AllIndices.size() * 3];
}

/***********************
* CreateVAO: Create VAO for cloth
* @author: William de Beer
********************/
void ClothQuad::CreateVAO()
{
	// VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_AllIndices.size() * 3, m_Indices, GL_DYNAMIC_DRAW);

	// VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_Width * m_Height * 8, m_Vertices, GL_DYNAMIC_DRAW);

	// Vertex Attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
}

/***********************
* DestroySection: Destroy section of cloth containing index
* @author: William de Beer
********************/
void ClothQuad::DestroySection(int _pos)
{
	auto it = m_AllIndices.begin();
	while (it != m_AllIndices.end())
	{
		// Check if contains index
		if ((*it).x == _pos || (*it).y == _pos || (*it).z == _pos)
			it = m_AllIndices.erase(it); // Erase
		else
			it++;
	}
}
