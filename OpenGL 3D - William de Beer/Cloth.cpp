#include "Cloth.h"

Cloth::Cloth(int _width, int _height)
{
	m_Spacing = 0.5f;
	m_Gravity = 5.0f;

	m_Width = _width;
	m_Height = _height;

	m_Nodes = new ClothNode * [m_Width * m_Height];

	for (int i = 0; i < m_Width; i++)
	{
		for (int j = 0; j < m_Height; j++)
		{
			m_Nodes[i * m_Height + j] = new ClothNode(glm::vec3(i * m_Spacing - (m_Spacing * m_Width - 0.5f) / 2.0f, 2.0f + -j * m_Spacing, 0));
			if (j == 0)
				m_Nodes[i * m_Height + j]->SetStatic(true);
		}
	}

	for (int i = 0; i < m_Width; i++)
	{
		for (int j = 0; j < m_Height; j++)
		{
			if (j - 1 >= 0)
				m_Nodes[i * m_Height + j]->SetConnection(Side::TOP, m_Nodes[i * m_Height + (j - 1)]);

			if (i - 1 >= 0)
				m_Nodes[i * m_Height + j]->SetConnection(Side::LEFT, m_Nodes[(i - 1) * m_Height + j]);

			if (j + 1 < m_Height)
				m_Nodes[i * m_Height + j]->SetConnection(Side::BOTTOM, m_Nodes[i * m_Height + (j + 1)]);

			if (i + 1 < m_Width)
				m_Nodes[i * m_Height + j]->SetConnection(Side::RIGHT, m_Nodes[(i + 1) * m_Height + j]);
		}
	}
}

Cloth::~Cloth()
{
	for (int i = 0; i < m_Width; i++)
	{
		for (int j = 0; j < m_Height; j++)
		{
			if (m_Nodes[i * m_Height + j] != nullptr)
			{
				delete m_Nodes[i * m_Height + j];
				m_Nodes[i * m_Height + j] = 0;
			}
		}
	}

	delete[] m_Nodes;
	m_Nodes = 0;
}

void Cloth::Render(CCamera* _camera)
{
	glBegin(GL_LINES);
	for (int i = 0; i < m_Width; i++)
	{
		for (int j = 0; j < m_Height; j++)
		{
			ClothNode* currentNode = m_Nodes[i * m_Height + j];
			ClothNode* bottomNode = currentNode->GetConnection(Side::BOTTOM);

			if (bottomNode != nullptr) // Check if node exists
			{
				// Convert to 3d space
				glm::vec4 pos3D = _camera->GetPVMatrix() * glm::vec4(currentNode->GetPos(), 1.0f);
				glm::vec3 position = glm::vec3(pos3D.x, pos3D.y, pos3D.z) / pos3D.w;

				// Vertex 1
				glVertex3f(position.x,
					position.y,
					position.z);

				// Convert to 3d space
				pos3D = _camera->GetPVMatrix() * glm::vec4(bottomNode->GetPos(), 1.0f);
				position = glm::vec3(pos3D.x, pos3D.y, pos3D.z) / pos3D.w;

				// Vertex 2
				glVertex3f(position.x,
					position.y,
					position.z);
			}
			
			ClothNode* rightNode = currentNode->GetConnection(Side::RIGHT);

			if (rightNode != nullptr)
			{
				glm::vec4 pos3D = _camera->GetPVMatrix() * glm::vec4(currentNode->GetPos(), 1.0f);
				glm::vec3 position = glm::vec3(pos3D.x, pos3D.y, pos3D.z) / pos3D.w;

				glVertex3f(position.x,
					position.y,
					position.z);

				pos3D = _camera->GetPVMatrix() * glm::vec4(rightNode->GetPos(), 1.0f);
				position = glm::vec3(pos3D.x, pos3D.y, pos3D.z) / pos3D.w;

				glVertex3f(position.x,
					position.y,
					position.z);
			}
		}
	}
	glEnd();
}

void Cloth::Update(float _dT)
{
	for (int i = 0; i < m_Width; i++)
	{
		for (int j = 0; j < m_Height; j++)
		{
			if (m_Nodes[i * m_Height + j] != nullptr)
			{
				// Apply gravity
				glm::vec3 gravityForce(0, -m_Gravity * m_Nodes[i * m_Height + j]->GetMass(), 0);
				m_Nodes[i * m_Height + j]->ApplyForce(gravityForce);

				m_Nodes[i * m_Height + j]->Update(_dT);
			}
		}
	}

	for (int i = 0; i < m_Width; i++)
	{
		for (int j = 0; j < m_Height; j++)
		{
			if (m_Nodes[i * m_Height + j] != nullptr)
			{
				m_Nodes[i * m_Height + j]->ApplyConstraint(m_Nodes[i * m_Height + j]->GetConnection(Side::TOP));
				m_Nodes[i * m_Height + j]->ApplyConstraint(m_Nodes[i * m_Height + j]->GetConnection(Side::RIGHT));
			}
		}
	}
}
