#ifndef __CLOTH_NODE_H__
#define __CLOTH_NODE_H__

#include <glew.h>
#include <freeglut.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

enum class Side {
	TOP,
	LEFT,
	BOTTOM,
	RIGHT,
};

class ClothNode
{
public:
	ClothNode(glm::vec3 _pos, glm::vec2 _uv);
	~ClothNode();
	void Update(float _dT);

	void ApplyForce(glm::vec3 _force);
	void ApplyConstraint(ClothNode* _other);

	void SetStatic(bool _static) { m_Static = _static; };
	bool GetStatic() { return m_Static; };

	void SetPos(glm::vec3 _pos) { m_Position = _pos; };
	glm::vec3 GetPos() { return m_Position; };
	glm::vec2 GetUV() { return m_UV; };
	float GetMass() { return m_Mass; };
	ClothNode* GetConnection(Side _side);
	void SetConnection(Side _side, ClothNode* _node);
private:
	glm::vec3 m_Position;
	glm::vec3 m_PreviousPos;
	glm::vec3 m_Acceleration;
	glm::vec2 m_UV;

	bool m_Static;
	float m_Mass;
	float m_Damping;
	float m_Stiffness = 2.0f;
	float m_RestingDistance = 0.2f;

	ClothNode* m_Top;
	ClothNode* m_Left;
	ClothNode* m_Bottom;
	ClothNode* m_Right;
};

#endif