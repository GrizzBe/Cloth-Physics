#include "ClothNode.h"
#include <iostream>

ClothNode::ClothNode(glm::vec3 _pos, glm::vec2 _uv)
{
	m_UV = _uv;
    m_Position = _pos;
    m_PreviousPos = _pos;
    m_Acceleration = glm::vec3(0, 0, 0);
    m_Damping = 10.0;
    m_Mass = 0.05f;
    m_Static = false;

    m_Top = nullptr;
    m_Left = nullptr;
    m_Bottom = nullptr;
    m_Right = nullptr;
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

    // Get velocity
    glm::vec3 velocity = m_Position - m_PreviousPos;

    // Apply damping
    m_Acceleration -= velocity * m_Damping / m_Mass;

    // Apply physics to position
    glm::vec3 newPos = m_Position + velocity + 0.5f * m_Acceleration * powf(_dT, 2);
    m_PreviousPos = m_Position;

    if (newPos.y < -10.0f)
        newPos.y = -10.0f;
    m_Position = newPos;

    m_Acceleration = glm::vec3(0, 0, 0);
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
    float difference = (_spacing - deltaLength) / deltaLength;

    float Im1 = 1 / m_Mass;
    float Im2 = 1 / _other->GetMass();

    float thisMult = (_other->GetStatic()) ? 1.0f : 0.5f;
    float otherMult = (m_Static) ? 1.0f : 0.5f;

    if (!m_Static)
        m_Position += thisMult * (delta * (Im1 / (Im1 + Im2)) * m_Stiffness * difference);

    if (!_other->GetStatic())
        _other->SetPos(_other->GetPos() - otherMult * (delta * (Im2 / (Im1 + Im2)) * m_Stiffness * difference));
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
	/*if (GetConnection(Side::TOP) != nullptr)
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
		GetConnection(Side::BL)->SetConnection(Side::TR, nullptr);*/

	SetConnection(Side::TOP, nullptr);
	SetConnection(Side::RIGHT, nullptr);
	SetConnection(Side::BOTTOM, nullptr);
	SetConnection(Side::LEFT, nullptr);
	SetConnection(Side::TR, nullptr);
	SetConnection(Side::BR, nullptr);
	SetConnection(Side::TL, nullptr);
	SetConnection(Side::BL, nullptr);
}
