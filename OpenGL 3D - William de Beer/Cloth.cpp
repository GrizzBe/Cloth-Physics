#include "Cloth.h"

Cloth::Cloth(int _width, int _height)
{
	m_Program = ShaderLoader::GetInstance().CreateProgram("ColorSpace.vs", "Cloth.fs");

	PrevMousePosX = 0;
	PrevMousePosY = 0;

	m_Spacing = 0.1f;
	m_Gravity = 0.1f * 9.81f;

	m_Width = _width;
	m_Height = _height;

	m_Nodes = new ClothNode * [m_Width * m_Height];

	for (int i = 0; i < m_Width; i++)
	{
		for (int j = 0; j < m_Height; j++)
		{
			glm::vec2 uv((i + 1) / (float)m_Width, (j + 1) / (float)m_Height);
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
		
			if (i + 1 < m_Width && j - 1 >= 0)
				m_Nodes[i * m_Height + j]->SetConnection(Side::TR, m_Nodes[(i + 1) * m_Height + j - 1]);

			if (i + 1 < m_Width && j + 1 < m_Height)
				m_Nodes[i * m_Height + j]->SetConnection(Side::BR, m_Nodes[(i + 1) * m_Height + j + 1]);
	
			if (i - 1 >= 0 && j - 1 >= 0)
				m_Nodes[i * m_Height + j]->SetConnection(Side::TL, m_Nodes[(i - 1) * m_Height + j - 1]);

			if (i - 1 >= 0 && j + 1 < m_Height)
				m_Nodes[i * m_Height + j]->SetConnection(Side::BL, m_Nodes[(i - 1) * m_Height + j + 1]);
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
	m_Quad->Render(m_Program, _camera);

	glBegin(GL_LINES);
	for (int i = 0; i < m_Width; i++)
	{
		for (int j = 0; j < m_Height; j++)
		{
			ClothNode* currentNode = m_Nodes[i * m_Height + j];
			if (!currentNode->GetConnectionStatus() || (!currentNode->IsEdge()))
				continue;

			currentNode->RenderConnectionLines(_camera, Side::TOP);
			currentNode->RenderConnectionLines(_camera, Side::RIGHT);
			currentNode->RenderConnectionLines(_camera, Side::BOTTOM);
			currentNode->RenderConnectionLines(_camera, Side::LEFT);
			currentNode->RenderConnectionLines(_camera, Side::TR);
			currentNode->RenderConnectionLines(_camera, Side::BR);
			currentNode->RenderConnectionLines(_camera, Side::TL);
			currentNode->RenderConnectionLines(_camera, Side::BL);
		}
	}
	glEnd();

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
		bool endLoop = false;

		// Apply external force
		for (int i = 0; i < m_Width; i++)
		{
			for (int j = 0; j < m_Height; j++)
			{
				if (m_Nodes[i * m_Height + j] != nullptr)
				{
					if (!m_Nodes[i * m_Height + j]->GetConnectionStatus())
						continue;

					if (_camera->CheckIntersection(m_Nodes[i * m_Height + j]->GetPos(), m_Spacing))
					{
						if (CInputHandle::GetInstance().GetKeyboardState('t') == InputState::Input_Down)
						{
							TearCloth(i, j);

							endLoop = true;
							break;
						}
						else if(CInputHandle::GetInstance().GetKeyboardState('i') == InputState::Input_Down)
						{
							IgniteCloth(i, j);

							endLoop = true;
							break;
						}
						else if (CInputHandle::GetInstance().GetKeyboardState('g') == InputState::Input_Down)
						{
							if (!grab.grabbing)
							{
								grab.grabbing = true;
								grab.i = i;
								grab.j = j;
								grab.distance = glm::distance(_camera->GetCamPos(), m_Nodes[i * m_Height + j]->GetPos());
							}

							endLoop = true;
							break;
						}
						else
						{
							glm::vec3 externalForce(_camera->GetRayDirection() * 500.0f);
							m_Nodes[i * m_Height + j]->ApplyForce(externalForce);
						}
					}
				}
				if (endLoop)
					break;
			}
		}
	}

	if (CInputHandle::GetInstance().GetMouseButtonState(GLUT_LEFT_BUTTON) == InputState::Input_Down && 
		CInputHandle::GetInstance().GetKeyboardState('g') == InputState::Input_Down && grab.grabbing)
	{
		ClothNode* node = m_Nodes[grab.i * m_Height + grab.j];
		node->SetPos(_camera->GetCamPos() + _camera->GetRayDirection() * grab.distance);
	}
	if (CInputHandle::GetInstance().GetKeyboardState('g') == InputState::Input_UpFirst || 
		CInputHandle::GetInstance().GetMouseButtonState(GLUT_LEFT_BUTTON) == InputState::Input_Up)
	{
		grab.grabbing = false;
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
		bool inRange = true;
		for (auto it : m_vRings)
		{
			if (abs(it->GetPos().x) > 10)
			{
				inRange = false;
				break;
			}
		}

		if (inRange)
		{
			for (auto it : m_vRings)
			{
				glm::vec3 oldPos = it->GetPos();
				it->SetPos(glm::vec3(oldPos.x * (1 + _dT), oldPos.y, oldPos.z));
			}
		}
	}

	for (int i = 0; i < m_Width; i++)
	{
		for (int j = 0; j < m_Height; j++)
		{
			if (m_Nodes[i * m_Height + j] != nullptr)
			{
				if (!m_Nodes[i * m_Height + j]->GetConnectionStatus())
					continue;

				// Apply gravity
				glm::vec3 gravityForce(0, -m_Gravity * m_Nodes[i * m_Height + j]->GetMass(), 0);
				m_Nodes[i * m_Height + j]->ApplyForce(gravityForce);

				m_Nodes[i * m_Height + j]->Update(_dT);

				m_Nodes[i * m_Height + j]->ApplyConstraint(m_Nodes[i * m_Height + j]->GetConnection(Side::TOP), m_Spacing);
				m_Nodes[i * m_Height + j]->ApplyConstraint(m_Nodes[i * m_Height + j]->GetConnection(Side::RIGHT), m_Spacing);
				m_Nodes[i * m_Height + j]->ApplyConstraint(m_Nodes[i * m_Height + j]->GetConnection(Side::TR), sqrtf(pow(m_Spacing, 2) * 2.0f));
				m_Nodes[i * m_Height + j]->ApplyConstraint(m_Nodes[i * m_Height + j]->GetConnection(Side::BR), sqrtf(pow(m_Spacing, 2) * 2.0f));
			
				if (m_Nodes[i * m_Height + j]->GetToBeDestroyed())
				{
					if (m_Nodes[i * m_Height + j]->GetConnectionStatus())
						TearCloth(i, j);
				}
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

void Cloth::ApplyWind(glm::vec3 _windOrigin)
{
	for (int i = 0; i < m_Width; i++)
	{
		for (int j = 0; j < m_Height; j++)
		{
			if (m_Nodes[i * m_Height + j] != nullptr)
			{
				// Apply gravity
				glm::vec3 force = m_Nodes[i * m_Height + j]->GetPos() - _windOrigin;
				m_Nodes[i * m_Height + j]->ApplyForce(glm::normalize(force) * 1.0f);
			}
		}
	}
}

void Cloth::TearCloth(int _i, int _j)
{
	if (_j - 1 >= 0 && m_Nodes[_i * m_Height + _j]->GetConnection(Side::TOP) != nullptr)
		m_Quad->DestroySection(_i * m_Height + (_j - 1));

	if (_i + 1 < m_Width && m_Nodes[_i * m_Height + _j]->GetConnection(Side::RIGHT) != nullptr)
		m_Quad->DestroySection((_i + 1) * m_Height + _j);

	if (_j + 1 >= 0 && m_Nodes[_i * m_Height + _j]->GetConnection(Side::BOTTOM) != nullptr)
		m_Quad->DestroySection(_i * m_Height + (_j + 1));

	if (_i - 1 < m_Width && m_Nodes[_i * m_Height + _j]->GetConnection(Side::LEFT) != nullptr)
		m_Quad->DestroySection((_i - 1) * m_Height + _j);

	if (_i + 1 < m_Width && _j - 1 >= 0 && m_Nodes[_i * m_Height + _j]->GetConnection(Side::TR) != nullptr)
		m_Quad->DestroySection((_i + 1) * m_Height + _j - 1);

	if (_i + 1 < m_Width && _j + 1 < m_Height && m_Nodes[_i * m_Height + _j]->GetConnection(Side::BR) != nullptr)
		m_Quad->DestroySection((_i + 1) * m_Height + _j + 1);

	if (_i - 1 < m_Width && _j - 1 >= 0 && m_Nodes[_i * m_Height + _j]->GetConnection(Side::TL) != nullptr)
		m_Quad->DestroySection((_i - 1) * m_Height + _j - 1);

	if (_i - 1 < m_Width && _j + 1 < m_Height && m_Nodes[_i * m_Height + _j]->GetConnection(Side::BL) != nullptr)
		m_Quad->DestroySection((_i - 1) * m_Height + _j + 1);


	m_Nodes[_i * m_Height + _j]->ClearConnections();
}

void Cloth::IgniteCloth(int _i, int _j)
{
	m_Nodes[_i * m_Height + _j]->CatchFire();
	m_Nodes[_i * m_Height + _j]->SetFireLevel(50.0f);
}
