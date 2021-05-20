#include "Cloth.h"

Cloth::Cloth(int _width, int _height)
{
	m_Program = ShaderLoader::GetInstance().CreateProgram("NormalSpace.vs", "LightingEffects.fs");

	PrevMousePosX = 0;
	PrevMousePosY = 0;

	m_Spacing = 0.2f;
	m_Gravity = 2.0f * 9.81f;

	m_Width = _width;
	m_Height = _height;

	m_Nodes = new ClothNode * [m_Width * m_Height];

	for (int i = 0; i < m_Width; i++)
	{
		for (int j = 0; j < m_Height; j++)
		{
			glm::vec2 uv = glm::vec2((i + 1) / m_Width, (j + 1) / m_Height);
			m_Nodes[i * m_Height + j] = new ClothNode(glm::vec3(i * m_Spacing - (m_Spacing * m_Width - 0.5f) / 2.0f, 2.0f + -j * m_Spacing, 0), uv);
			if (j == 0 && i % 8 == 0)
			{
				m_Nodes[i * m_Height + j]->SetStatic(true);
				m_vRings.push_back(m_Nodes[i * m_Height + j]);
			}
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

	m_Quad = new ClothQuad(m_Width, m_Height, m_Nodes);
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

	if (m_Quad != nullptr)
	{
		delete m_Quad;
		m_Quad = 0;
	}
}

void Cloth::Render(CCamera* _camera)
{
	//glBegin(GL_LINES);
	//for (int i = 0; i < m_Width; i++)
	//{
	//	for (int j = 0; j < m_Height; j++)
	//	{
	//		ClothNode* currentNode = m_Nodes[i * m_Height + j];
	//		ClothNode* topNode = currentNode->GetConnection(Side::TOP);

	//		if (topNode != nullptr) // Check if node exists
	//		{
	//			// Convert to 3d space
	//			glm::vec4 pos3D = _camera->GetPVMatrix() * glm::vec4(currentNode->GetPos(), 1.0f);
	//			glm::vec3 position = glm::vec3(pos3D.x, pos3D.y, pos3D.z) / pos3D.w;

	//			// Vertex 1
	//			glVertex3f(position.x,
	//				position.y,
	//				position.z);

	//			// Convert to 3d space
	//			pos3D = _camera->GetPVMatrix() * glm::vec4(topNode->GetPos(), 1.0f);
	//			position = glm::vec3(pos3D.x, pos3D.y, pos3D.z) / pos3D.w;

	//			// Vertex 2
	//			glVertex3f(position.x,
	//				position.y,
	//				position.z);
	//		}
	//		
	//		ClothNode* rightNode = currentNode->GetConnection(Side::RIGHT);

	//		if (rightNode != nullptr)
	//		{
	//			glm::vec4 pos3D = _camera->GetPVMatrix() * glm::vec4(currentNode->GetPos(), 1.0f);
	//			glm::vec3 position = glm::vec3(pos3D.x, pos3D.y, pos3D.z) / pos3D.w;

	//			glVertex3f(position.x,
	//				position.y,
	//				position.z);

	//			pos3D = _camera->GetPVMatrix() * glm::vec4(rightNode->GetPos(), 1.0f);
	//			position = glm::vec3(pos3D.x, pos3D.y, pos3D.z) / pos3D.w;

	//			glVertex3f(position.x,
	//				position.y,
	//				position.z);
	//		}
	//	}
	//}
	//glEnd();

	m_Quad->Render(m_Program, _camera);
}

void Cloth::Update(float _dT, CCamera* _camera)
{/*
	float CurrentMousePosX = CInputHandle::GetInstance().GetMouseX();
	float DeltaMousePosX = PrevMousePosX - CurrentMousePosX;
	PrevMousePosX = CurrentMousePosX;

	float CurrentMousePosY = CInputHandle::GetInstance().GetMouseY();
	float DeltaMousePosY = PrevMousePosY - CurrentMousePosY;
	PrevMousePosY = CurrentMousePosY;*/

	if (CInputHandle::GetInstance().GetMouseButtonState(GLUT_LEFT_BUTTON) == InputState::Input_Down)
	{
		// Apply external force
		for (int i = 0; i < m_Width; i++)
		{
			for (int j = 0; j < m_Height; j++)
			{
				if (m_Nodes[i * m_Height + j] != nullptr)
				{
					if (_camera->CheckIntersection(m_Nodes[i * m_Height + j]->GetPos(), m_Spacing))
					{
						if (CInputHandle::GetInstance().GetKeyboardState('t') == InputState::Input_Down)
						{
							m_Nodes[i * m_Height + j]->SetConnection(Side::TOP, nullptr);
							m_Nodes[i * m_Height + j]->SetConnection(Side::RIGHT, nullptr);
						}
						else
						{
							glm::vec3 externalForce(_camera->GetRayDirection() * 100.0f);
							m_Nodes[i * m_Height + j]->ApplyForce(externalForce);
						}
					}
				}
			}
		}
	}

	if (CInputHandle::GetInstance().GetKeyboardState(',') == InputState::Input_Down)
	{
		for (auto it : m_vRings)
		{
			glm::vec3 oldPos = it->GetPos();
			it->SetPos(glm::vec3(oldPos.x * (1 - _dT), oldPos.y, oldPos.z));
		}
	}
	if (CInputHandle::GetInstance().GetKeyboardState('.') == InputState::Input_Down)
	{
		for (auto it : m_vRings)
		{
			glm::vec3 oldPos = it->GetPos();
			it->SetPos(glm::vec3(oldPos.x * (1 + _dT), oldPos.y, oldPos.z));
		}
	}

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
	m_Quad->Update(_dT);
}

void Cloth::DropCloth()
{
	for (auto it : m_vRings)
	{
		it->SetStatic(false);
	}
}
