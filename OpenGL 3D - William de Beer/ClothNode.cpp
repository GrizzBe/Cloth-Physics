#include "ClothNode.h"
#include <iostream>

ClothNode::ClothNode(glm::vec3 _pos, glm::vec2 _uv)
{
	m_UV = _uv;
    m_Position = _pos;
    m_PreviousPos = _pos;
    m_Acceleration = glm::vec3(0, 0, 0);
    m_Color = glm::vec3(1.0f, 1.0f, 1.0f);
    m_Damping = 10.0f;
    m_Mass = 1.0f;
    m_Static = false;
    m_Connected = true;
    m_OnFire = false;
    m_ToBeDestroyed = false;
    m_FireLevel = 0.0f;

    m_fHeatResistance = rand() % 20 + 80;

    m_Top = nullptr;
    m_Left = nullptr;
    m_Bottom = nullptr;
    m_Right = nullptr;
    m_TL = nullptr;
    m_TR = nullptr;
    m_BL = nullptr;
    m_BR = nullptr;
}

ClothNode::~ClothNode()
{
}

void ClothNode::Update(float _dT)
{
    if (m_Static)
    {
        m_Acceleration = glm::vec3(0, 0, 0);
        return;
    }
    
    float magAcceleration = glm::distance(glm::vec3(0, 0, 0), m_Acceleration);
    if (magAcceleration > m_MaxAcceleration)
    {
        m_Acceleration = glm::normalize(m_Acceleration) * m_MaxAcceleration;
    }

    // Get velocity
    glm::vec3 velocity = m_Position - m_PreviousPos;
    float magVelocity = glm::distance(glm::vec3(0, 0, 0), velocity);
    if (magVelocity > m_MaxSpeed)
    {
        velocity = glm::normalize(velocity) * m_MaxSpeed;
    }

    // Apply damping
    m_Acceleration -= m_Acceleration * _dT * m_Damping / m_Mass;
    //m_Acceleration -= velocity * m_Damping / m_Mass;


    // Apply physics to position
    glm::vec3 newPos = m_Position + velocity + 0.5f * m_Acceleration * powf(_dT, 2);
    m_PreviousPos = m_Position;

    // Floor
    if (newPos.y < -5.0f)
        newPos.y = -5.0f;
    if (newPos.x < -10.0f)
        newPos.x = -10.0f;
    if (newPos.x > 10.0f)
        newPos.x = 10.0f;
    if (newPos.z < -10.0f)
        newPos.z = -10.0f;
    if (newPos.z > 10.0f)
        newPos.z = 10.0f;
    m_Position = newPos;

   

    CalculateFire(_dT);
    m_Color = glm::vec3(1.0f, 1.0f - 0.5f * (m_FireLevel / 100.0f), 1.0f - (m_FireLevel / 100.0f));
}

void ClothNode::ApplyForce(glm::vec3 _force)
{
    m_Acceleration += _force / m_Mass;
}

void ClothNode::ApplyConstraint(ClothNode* _other, float _spacing)
{
    if (_other == nullptr)
        return;

    glm::vec3 delta = m_Position - _other->GetPos();
    float deltaLength = glm::distance(m_Position, _other->GetPos());
    if (deltaLength > _spacing * m_BreakingDistance)
    {
        m_ToBeDestroyed = true;
    }

    float difference = ((_spacing * m_RestingDistance) - deltaLength) / deltaLength;

    float Im1 = 1 / m_Mass;
    float Im2 = 1 / _other->GetMass();

    float thisMult = (_other->GetStatic()) ? 1.0f : 0.5f;
    float otherMult = (m_Static) ? 1.0f : 0.5f;

    if (!m_Static)
        m_Position += thisMult * (delta * (Im1 / (Im1 + Im2)) * m_Stiffness * difference);

    if (!_other->GetStatic())
        _other->SetPos(_other->GetPos() - otherMult * (delta * (Im2 / (Im1 + Im2)) * m_Stiffness * difference));
}

void ClothNode::CalculateFire(float _dT)
{
    if (!m_OnFire)
        return;

    m_FireLevel += _dT * 10;
    if (m_FireLevel > 50)
    {
        if (m_Bottom != nullptr)
            m_Bottom->CatchFire();
    }
    else if (m_FireLevel > 40)
    {
        if (m_BL != nullptr)
            m_BL->CatchFire();
        if (m_BR != nullptr)
            m_BR->CatchFire();
    }
    else if (m_FireLevel > 30)
    {
        if (m_Left != nullptr)
            m_Left->CatchFire();
        if (m_Right != nullptr)
            m_Right->CatchFire();
    }
    else if (m_FireLevel > 20)
    {
        if (m_TL != nullptr)
            m_TL->CatchFire();
        if (m_TR != nullptr)
            m_TR->CatchFire();
    }
    else if (m_FireLevel > 10)
    {
        if (m_Top != nullptr)
            m_Top->CatchFire();
    }

    if (m_FireLevel >= m_fHeatResistance)
    {
        m_ToBeDestroyed = true;
    }
}

bool ClothNode::IsEdge()
{
	return (m_Top == nullptr || m_Left == nullptr || m_Bottom == nullptr || m_Right == nullptr || m_BR == nullptr || m_TR == nullptr || m_BL == nullptr || m_TL == nullptr);
}

ClothNode* ClothNode::GetConnection(Side _side)
{
    switch (_side)
    {
    case Side::TOP:
        return m_Top;
    case Side::LEFT:
        return m_Left;
    case Side::BOTTOM:
        return m_Bottom;
    case Side::RIGHT:
        return m_Right;
	case Side::TR:
		return m_TR;
	case Side::BR:
		return m_BR;
	case Side::TL:
		return m_TL;
	case Side::BL:
		return m_BL;
    default:
        return nullptr;
    }
}

void ClothNode::SetConnection(Side _side, ClothNode* _node)
{
    switch (_side)
    {
    case Side::TOP:
        m_Top = _node;
        return;
    case Side::LEFT:
        m_Left = _node;
        return;
    case Side::BOTTOM:
        m_Bottom = _node;
        return;
    case Side::RIGHT:
        m_Right = _node;
        return;
	case Side::TR:
		m_TR = _node;
		return;
	case Side::BR:
		m_BR = _node;
		return;
	case Side::TL:
		m_TL = _node;
		return;
	case Side::BL:
		m_BL = _node;
		return;
    default:
        return;
    }
}

void ClothNode::ClearConnections()
{
	if (GetConnection(Side::TOP) != nullptr)
		GetConnection(Side::TOP)->SetConnection(Side::BOTTOM, nullptr);

	if (GetConnection(Side::RIGHT) != nullptr)
		GetConnection(Side::RIGHT)->SetConnection(Side::LEFT, nullptr);

	if (GetConnection(Side::BOTTOM) != nullptr)
		GetConnection(Side::BOTTOM)->SetConnection(Side::TOP, nullptr);

	if (GetConnection(Side::LEFT) != nullptr)
		GetConnection(Side::LEFT)->SetConnection(Side::RIGHT, nullptr);

	if (GetConnection(Side::TR) != nullptr)
		GetConnection(Side::TR)->SetConnection(Side::BL, nullptr);

	if (GetConnection(Side::BR) != nullptr)
		GetConnection(Side::BR)->SetConnection(Side::TL, nullptr);

	if (GetConnection(Side::TL) != nullptr)
		GetConnection(Side::TL)->SetConnection(Side::BR, nullptr);

	if (GetConnection(Side::BL) != nullptr)
		GetConnection(Side::BL)->SetConnection(Side::TR, nullptr);

	SetConnection(Side::TOP, nullptr);
	SetConnection(Side::RIGHT, nullptr);
	SetConnection(Side::BOTTOM, nullptr);
	SetConnection(Side::LEFT, nullptr);
	SetConnection(Side::TR, nullptr);
	SetConnection(Side::BR, nullptr);
	SetConnection(Side::TL, nullptr);
	SetConnection(Side::BL, nullptr);

    m_Connected = false;
}

void ClothNode::RenderConnectionLines(CCamera* _camera, Side _side)
{
    /*if (_side == Side::TOP || _side == Side::RIGHT || _side == Side::BOTTOM || _side == Side::LEFT)
    {
        glColor3f(1, 0, 0);
    }
    else
    {
    }*/

    if (m_OnFire)
        glColor3f(m_Color.x, m_Color.y, m_Color.z);
    else
        glColor3f(1, 1, 1);


    ClothNode* node = GetConnection(_side);
    if (node != nullptr)
    {
        // Convert to 3d space
        glm::vec4 pos3D = _camera->GetPVMatrix() * glm::vec4(GetPos(), 1.0f);
        glm::vec3 position = glm::vec3(pos3D.x, pos3D.y, pos3D.z) / pos3D.w;

        // Vertex 1
        glVertex3f(position.x,
            position.y,
            position.z);

        // Convert to 3d space
        pos3D = _camera->GetPVMatrix() * glm::vec4(node->GetPos(), 1.0f);
        position = glm::vec3(pos3D.x, pos3D.y, pos3D.z) / pos3D.w;

        // Vertex 2
        glVertex3f(position.x,
            position.y,
            position.z);
    }
}
