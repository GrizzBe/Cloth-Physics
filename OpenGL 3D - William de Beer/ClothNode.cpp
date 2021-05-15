#include "ClothNode.h"
#include <iostream>

ClothNode::ClothNode(glm::vec3 _pos)
{
    m_Position = _pos;
    m_PreviousPos = _pos;
    m_Damping = 100.0;
    m_Mass = 1.0f;
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
        return;

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

void ClothNode::ApplyConstraint(ClothNode* _other)
{
    if (_other == nullptr)
        return;

    glm::vec3 delta = m_Position - _other->GetPos();
    float deltaLength = glm::distance(m_Position, _other->GetPos());
    float difference = (m_RestingDistance - deltaLength) / deltaLength;

    float Im1 = 1 / m_Mass;
    float Im2 = 1 / _other->GetMass();

    float thisMult = (_other->GetStatic()) ? 1.0f : 0.5f;
    float otherMult = (m_Static) ? 1.0f : 0.5f;

    if (!m_Static)
        m_Position += thisMult * (delta * (Im1 / (Im1 + Im2)) * m_Stiffness * difference);

    if (!_other->GetStatic())
        _other->SetPos(_other->GetPos() - otherMult * (delta * (Im2 / (Im1 + Im2)) * m_Stiffness * difference));
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
    default:
        return;
    }
}
