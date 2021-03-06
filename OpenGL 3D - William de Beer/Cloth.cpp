// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   Cloth.h
//  Description :   Cloth object.
//  Author      :   William de Beer 
//  Mail        :   William.Beer@mds.ac.nz
// 
 // This Includes 
#include "Cloth.h"
 // Local Includes 
#include "Utilities.h"

Cloth::Cloth(int _width, int _height)
{
	// Creat program
	m_Program = ShaderLoader::GetInstance().CreateProgram("ColorSpace.vs", "Cloth.fs");

	PrevMousePosX = 0;
	PrevMousePosY = 0;

	m_Spacing = 0.1f;
	m_Gravity = 0.1f * 9.81f;

	m_Width = _width;
	m_Height = _height;

	m_Nodes = new ClothNode * [m_Width * m_Height];

	for (int j = 0; j < m_Height; j++)
	{
		for (int i = 0; i < m_Width; i++)
		{
			// Get UV coords
			glm::vec2 uv((i + 1) / (float)m_Width, (j + 1) / (float)m_Height);
			// Create node

			m_Nodes[i * m_Height + j] = new ClothNode(glm::vec3(i * m_Spacing - (m_Spacing * m_Width - 0.5f) / 2.0f, 2.0f + -j * m_Spacing, 0), uv);

			if (j == 0 && i % CUtilities::GetInstance().GetHookDensity() == 0) // Set node to be hook/ring
			{
				m_Nodes[i * m_Height + j]->SetStatic(true);
				m_vRings.push_back(m_Nodes[i * m_Height + j]);
			}
			if (i == m_Width - 1 && j == m_Height - 1) // Slightly offset one node for not terrible physics
			{
				m_Nodes[i * m_Height + j]->SetPos(m_Nodes[i * m_Height + j]->GetPos() + glm::vec3(0.05, 0, 0.05));
			}
		}
	}

	// Set connections
	for (int j = 0; j < m_Height; j++)
	{
		for (int i = 0; i < m_Width; i++)
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

	// Create visuals
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

/***********************
* Render: Renders cloth
* @author: William de Beer
* @parameter: Pointer to camera
********************/
void Cloth::Render(CCamera* _camera)
{
	m_Quad->Render(m_Program, _camera);

	glBegin(GL_LINES);
	for (int i = 0; i < m_Width; i++)
	{
		for (int j = 0; j < m_Height; j++)
		{
			ClothNode* currentNode = m_Nodes[i * m_Height + j];
			// Check if the node is an edge node.
			if (!currentNode->GetConnectionStatus() || (!currentNode->IsEdge()))
				continue;

			// Render all line connections
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

/***********************
* Update: Updates cloth
* @author: William de Beer
* @parameter: Delta time, pointer to a camera
********************/
void Cloth::Update(float _dT, CCamera* _camera)
{
	// Check if user is holding left click
	if (CInputHandle::GetInstance().GetMouseButtonState(GLUT_LEFT_BUTTON) == InputState::Input_Down || 
		CInputHandle::GetInstance().GetMouseButtonState(GLUT_LEFT_BUTTON) == InputState::Input_DownFirst)
	{
		bool endLoop = false;
		
		for (int j = 0; j < m_Height; j++)
		{
			for (int i = 0; i < m_Width; i++)
			{
				if (m_Nodes[i * m_Height + j] != nullptr)
				{
					if (!m_Nodes[i * m_Height + j]->GetConnectionStatus()) // Check if node is connected
						continue;

					if (_camera->CheckIntersection(m_Nodes[i * m_Height + j]->GetPos(), m_Spacing)) // Raycast intersection
					{
						if (CInputHandle::GetInstance().GetKeyboardState('t') == InputState::Input_Down) // Tearing
						{
							TearCloth(i, j);

							endLoop = true;
							break;
						}
						else if(CInputHandle::GetInstance().GetKeyboardState('i') == InputState::Input_Down) // Igniting
						{
							IgniteCloth(i, j);

							endLoop = true;
							break;
						}
						else if (CInputHandle::GetInstance().GetKeyboardState('g') == InputState::Input_Down) // Grabbing
						{
							if (!grab.grabbing) // Apply node information
							{
								grab.grabbing = true;
								grab.i = i;
								grab.j = j;
								grab.distance = glm::distance(_camera->GetCamPos(), m_Nodes[i * m_Height + j]->GetPos());
							}

							endLoop = true;
							break;
						}
						else // Push
						{
							glm::vec3 externalForce(-_camera->GetRayDirection() * 5000.0f);
							m_Nodes[i * m_Height + j]->ApplyForce(externalForce);
						}
					}
				}
				if (endLoop)
					break;
			}
		}
	}


	// Continuation of grabbing
	if (CInputHandle::GetInstance().GetMouseButtonState(GLUT_LEFT_BUTTON) == InputState::Input_Down && 
		CInputHandle::GetInstance().GetKeyboardState('g') == InputState::Input_Down && grab.grabbing)
	{
		ClothNode* node = m_Nodes[grab.i * m_Height + grab.j];
		node->SetPos(_camera->GetCamPos() - _camera->GetRayDirection() * grab.distance);
	}
	if (CInputHandle::GetInstance().GetKeyboardState('g') == InputState::Input_UpFirst || 
		CInputHandle::GetInstance().GetMouseButtonState(GLUT_LEFT_BUTTON) == InputState::Input_Up)
	{
		grab.grabbing = false;
	}

	// Move cloth rings
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

		if (inRange) // Stop cloth from moving too far out
		{
			for (auto it : m_vRings)
			{
				glm::vec3 oldPos = it->GetPos();
				it->SetPos(glm::vec3(oldPos.x * (1 + _dT), oldPos.y, oldPos.z));
			}
		}
	}

	for (int j = 0; j < m_Height; j++)
	{
		for (int i = 0; i < m_Width; i++)
		{
			if (m_Nodes[i * m_Height + j] != nullptr)
			{
				if (!m_Nodes[i * m_Height + j]->GetConnectionStatus())
					continue;

				// Apply gravity
				glm::vec3 gravityForce(0, -m_Gravity * m_Nodes[i * m_Height + j]->GetMass(), 0);
				m_Nodes[i * m_Height + j]->ApplyForce(gravityForce);

				// Update node
				m_Nodes[i * m_Height + j]->Update(_dT);

				// Apply constraints to node
				m_Nodes[i * m_Height + j]->ApplyConstraint(m_Nodes[i * m_Height + j]->GetConnection(Side::TOP), m_Spacing);
				m_Nodes[i * m_Height + j]->ApplyConstraint(m_Nodes[i * m_Height + j]->GetConnection(Side::RIGHT), m_Spacing);
				m_Nodes[i * m_Height + j]->ApplyConstraint(m_Nodes[i * m_Height + j]->GetConnection(Side::TR), sqrtf(powf(m_Spacing, 2) * 2.0f));
				m_Nodes[i * m_Height + j]->ApplyConstraint(m_Nodes[i * m_Height + j]->GetConnection(Side::BR), sqrtf(powf(m_Spacing, 2) * 2.0f));
			
				// Check if it is to be destroyed and destroy it.
				if (m_Nodes[i * m_Height + j]->GetToBeDestroyed())
				{
					if (m_Nodes[i * m_Height + j]->GetConnectionStatus())
						TearCloth(i, j);
				}
			}
		}
	}
	// Update the mesh 
	m_Quad->Update(_dT);
}

/***********************
* DropCloth: Releases cloth hooks
* @author: William de Beer
********************/
void Cloth::DropCloth()
{
	for (auto it : m_vRings)
	{
		it->SetStatic(false);
	}
}

/***********************
* CalculateCollision: Allows the cloth to collider other objects
* @author: William de Beer
* @parameter: Type of object, object position, object size.
********************/
void Cloth::CalculateCollision(CollisionType _type, glm::vec3 _pos, glm::vec3 _scale)
{
	glm::vec3 size;
	float radius;
	float height;
	float collisionBonus = 0.025f;
	switch (_type)
	{
	case CollisionType::CUBE: // CUBE
		size = (_scale / 2.0f) + collisionBonus;
		for (int j = 0; j < m_Height; j++)
		{
			for (int i = 0; i < m_Width; i++)
			{
				if (m_Nodes[i * m_Height + j] != nullptr && !m_Nodes[i * m_Height + j]->GetStatic()) // Check if it is ring node
				{
					if (CUtilities::AABB(m_Nodes[i * m_Height + j]->GetPos(), _pos, size)) // AABB collision
					{
						int closestSide = -1;
						float furthestDistanceToCenter = 0.0f;
						// Find closest side of the cube to the point
						if ((m_Nodes[i * m_Height + j]->GetPos().x >= (_pos.x - size.x) && m_Nodes[i * m_Height + j]->GetPos().x <= (_pos.x + size.x)))
						{
							float distanceToCenter = abs(m_Nodes[i * m_Height + j]->GetPos().x - _pos.x);
							if (distanceToCenter > furthestDistanceToCenter)
							{
								furthestDistanceToCenter = distanceToCenter;
								closestSide = 1;
							}
						}
						if ((m_Nodes[i * m_Height + j]->GetPos().y >= (_pos.y - size.y) && m_Nodes[i * m_Height + j]->GetPos().y <= (_pos.y + size.y)))
						{
							float distanceToCenter = abs(m_Nodes[i * m_Height + j]->GetPos().y - _pos.y);
							if (distanceToCenter > furthestDistanceToCenter)
							{
								furthestDistanceToCenter = distanceToCenter;
								closestSide = 2;
							}
						}
						if ((m_Nodes[i * m_Height + j]->GetPos().z >= (_pos.z - size.z) && m_Nodes[i * m_Height + j]->GetPos().z <= (_pos.z + size.z)))
						{
							float distanceToCenter = abs(m_Nodes[i * m_Height + j]->GetPos().z - _pos.z);
							if (distanceToCenter > furthestDistanceToCenter)
							{
								furthestDistanceToCenter = distanceToCenter;
								closestSide = 3;
							}
						}

						// Set position based on closest side
						switch (closestSide)
						{
						// X
						case 1: 
							if (m_Nodes[i * m_Height + j]->GetPos().x < _pos.x)
							{
								m_Nodes[i * m_Height + j]->SetPos(glm::vec3(_pos.x - size.x,
									m_Nodes[i * m_Height + j]->GetPos().y,
									m_Nodes[i * m_Height + j]->GetPos().z));
							}
							else
							{
								m_Nodes[i * m_Height + j]->SetPos(glm::vec3(_pos.x + size.x,
									m_Nodes[i * m_Height + j]->GetPos().y,
									m_Nodes[i * m_Height + j]->GetPos().z));
							}
							break;
						// Y
						case 2:
							if (m_Nodes[i * m_Height + j]->GetPos().y < _pos.y)
							{
								m_Nodes[i * m_Height + j]->SetPos(glm::vec3(m_Nodes[i * m_Height + j]->GetPos().x,
									_pos.y - size.y,
									m_Nodes[i * m_Height + j]->GetPos().z));
							}
							else
							{
								m_Nodes[i * m_Height + j]->SetPos(glm::vec3(m_Nodes[i * m_Height + j]->GetPos().x,
									_pos.y + size.y,
									m_Nodes[i * m_Height + j]->GetPos().z));
							}
							break;
						// Z
						case 3:
							if (m_Nodes[i * m_Height + j]->GetPos().z < _pos.z)
							{
								m_Nodes[i * m_Height + j]->SetPos(glm::vec3(m_Nodes[i * m_Height + j]->GetPos().x,
									m_Nodes[i * m_Height + j]->GetPos().y,
									_pos.z - size.z));
							}
							else
							{
								m_Nodes[i * m_Height + j]->SetPos(glm::vec3(m_Nodes[i * m_Height + j]->GetPos().x,
									m_Nodes[i * m_Height + j]->GetPos().y,
									_pos.z + size.z));
							}
							break;
						default:
							break;
						}
					}
				}
			}
		}
		break;
	case CollisionType::SPHERE: // SPHERE
		radius = _scale.x + collisionBonus;
		for (int j = 0; j < m_Height; j++)
		{
			for (int i = 0; i < m_Width; i++)
			{
				if (m_Nodes[i * m_Height + j] != nullptr && !m_Nodes[i * m_Height + j]->GetStatic()) // Check if node is static
				{
					float distance = glm::distance(_pos, m_Nodes[i * m_Height + j]->GetPos());
					if (distance <= (radius)) // Check if in radius
					{
						glm::vec3 direction = glm::normalize(m_Nodes[i * m_Height + j]->GetPos() - _pos);
						m_Nodes[i * m_Height + j]->SetPos(_pos + direction * radius);
					}
				}
			}
		}
		break;
	case CollisionType::CLOTHNODE: // CLOTHNODE
		radius = _scale.x; 
		for (int j = 0; j < m_Height; j++)
		{
			for (int i = 0; i < m_Width; i++)
			{
				if (m_Nodes[i * m_Height + j] != nullptr && !m_Nodes[i * m_Height + j]->GetStatic())
				{
					float distance = glm::distance(_pos, m_Nodes[i * m_Height + j]->GetPos());
					if (distance != 0)
					{
						if (distance <= (radius))
						{
							glm::vec3 direction = glm::normalize(m_Nodes[i * m_Height + j]->GetPos() - _pos);
							m_Nodes[i * m_Height + j]->SetPos(_pos + direction * radius);
						}
					}
				}
			}
		}
		break;
	case CollisionType::PYRAMID: // PYRAMID
		size = _scale + collisionBonus;
		for (int j = 0; j < m_Height; j++)
		{
			for (int i = 0; i < m_Width; i++)
			{
				if (m_Nodes[i * m_Height + j] != nullptr && !m_Nodes[i * m_Height + j]->GetStatic()) // Check if node is static
				{
					// Create information for each plane of object
					CUtilities::PlaneInfo plane[5];
					for (int k = 0; k < 4; k++)
					{
						plane[k].index = k;
						plane[k].position = _pos + glm::vec3(0.0f, 0.25f, 0.0f) * size.y; // Set position
					}
					plane[4].position = _pos - glm::vec3(0.0f, 0.25f, 0.0f) * size.y * 1.5f;

					// Set normals
					plane[0].normal = glm::normalize(glm::vec3(glm::vec3(0.0f, 0.5f, -0.5f)));
					plane[1].normal = glm::normalize(glm::vec3(glm::vec3(0.5f, 0.5f, 0.0f)));
					plane[2].normal = glm::normalize(glm::vec3(glm::vec3(0.0f, 0.5f, 0.5f)));
					plane[3].normal = glm::normalize(glm::vec3(glm::vec3(-0.5f, 0.5f, 0.0f)));
					plane[4].normal = glm::normalize(glm::vec3(glm::vec3(0.0f, -1.0f, 0.0f)));

					// Check if object is behind all planes
					if (CUtilities::PlanePointCollision(plane[0].normal, plane[0].position, m_Nodes[i * m_Height + j]->GetPos()) &&
						CUtilities::PlanePointCollision(plane[1].normal, plane[1].position, m_Nodes[i * m_Height + j]->GetPos()) &&
						CUtilities::PlanePointCollision(plane[2].normal, plane[2].position, m_Nodes[i * m_Height + j]->GetPos()) &&
						CUtilities::PlanePointCollision(plane[3].normal, plane[3].position, m_Nodes[i * m_Height + j]->GetPos()) &&
						CUtilities::PlanePointCollision(plane[4].normal, plane[4].position, m_Nodes[i * m_Height + j]->GetPos()))
					{
						int closestSide = -1;
						float closestDistance = 100.0f;
						
						// Find closest plane
						for (int k = 0; k < 5; k++)
						{
							if (CUtilities::PlanePointCollision(plane[k].normal, plane[k].position, m_Nodes[i * m_Height + j]->GetPos()))
							{
								float distanceToSide = abs(CUtilities::PlanePointValue(plane[k].normal, plane[k].position, m_Nodes[i * m_Height + j]->GetPos()));
								if (distanceToSide < closestDistance)
								{
									closestDistance = distanceToSide;
									closestSide = k;
								}
							}
						}
						// Set new node location
						m_Nodes[i * m_Height + j]->SetPos(m_Nodes[i * m_Height + j]->GetPos() + plane[closestSide].normal * (closestDistance));

					}
				}
			}
		}
		break;
	case CollisionType::CAPSULE: // CAPSULE
		radius = _scale.x + collisionBonus;
		height = _scale.y;
		for (int j = 0; j < m_Height; j++)
		{
			for (int i = 0; i < m_Width; i++)
			{
				if (m_Nodes[i * m_Height + j] != nullptr && !m_Nodes[i * m_Height + j]->GetStatic()) // Check if node is static
				{
					// Store positions for each end of capsule
					glm::vec3 capsuleEnd[2];
					capsuleEnd[0] = _pos - (glm::vec3(0, 1, 0) * height * 0.5f);
					capsuleEnd[1] = _pos + (glm::vec3(0, 1, 0) * height * 0.5f);

					float endDist = glm::distance(capsuleEnd[0], capsuleEnd[1]);

					glm::vec3 point = m_Nodes[i * m_Height + j]->GetPos();
					
					// Get closest point in line
					float s = glm::dot(capsuleEnd[1] - capsuleEnd[0], point - capsuleEnd[0]) / (height * 0.5f);
					s = glm::clamp(s, 0.0f, 1.0f);
					glm::vec3 p = capsuleEnd[0] + s * (capsuleEnd[1] - capsuleEnd[0]);

					// Check distance
					float distance = glm::distance(point, p);
					if (distance <= (radius))
					{
						glm::vec3 direction = glm::normalize(point - p);
						m_Nodes[i * m_Height + j]->SetPos(p + direction * radius);
					}
				}
			}
		}
		break;
	default:
		break;
	}
}

/***********************
* Untangle: Calls collision between all nodes
* @author: William de Beer
********************/
void Cloth::Untangle()
{
	for (int j = 0; j < m_Height; j++)
	{
		for (int i = 0; i < m_Width; i++)
		{
			if (m_Nodes[i * m_Height + j] != nullptr)
			{
				CalculateCollision(CollisionType::CLOTHNODE, m_Nodes[i * m_Height + j]->GetPos(), glm::vec3(1.0f, 1.0f, 1.0f) * m_Spacing * 0.9f);
			}
		}
	}
}

/***********************
* ApplyWind: Apply wind to all nodes in an area
* @author: William de Beer
* @parameter: Position of the fan 
********************/
void Cloth::ApplyWind(glm::vec3 _windOrigin)
{
	for (int j = 0; j < m_Height; j++)
	{
		for (int i = 0; i < m_Width; i++)
		{
			if (m_Nodes[i * m_Height + j] != nullptr)
			{
				// Chehck
				if (!CUtilities::AABB(m_Nodes[i * m_Height + j]->GetPos(), _windOrigin, 
					glm::vec3(CUtilities::GetInstance().GetWindArea() / 2.0f, CUtilities::GetInstance().GetWindArea() / 2.0f, 50.0f)))
					continue;

				// Apply gravity
				glm::vec3 force = m_Nodes[i * m_Height + j]->GetPos() - _windOrigin;
				m_Nodes[i * m_Height + j]->ApplyForce(glm::normalize(force) * 0.1f * (float)CUtilities::GetInstance().GetWindSpeed());
			}
		}
	}
}

/***********************
* TearCloth: Destroys the targeted node
* @author: William de Beer
* @parameter: Node index
********************/
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

/***********************
* IgniteCloth: Ignites targeted node
* @author: William de Beer
* @parameter: Node index
********************/
void Cloth::IgniteCloth(int _i, int _j)
{
	m_Nodes[_i * m_Height + _j]->CatchFire();
	m_Nodes[_i * m_Height + _j]->SetFireLevel(50.0f);
}

