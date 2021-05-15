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
	ClothNode(glm::vec3 _pos);
	~ClothNode();
	void Update(float _dT);

	void ApplyForce(glm::vec3 _force);
	void ApplyConstraint(ClothNode* _other);

	void SetStatic(bool _static) { m_Static = _static; };
	bool GetStatic() { return m_Static; };

	void SetPos(glm::vec3 _pos) { m_Position = _pos; };
	glm::vec3 GetPos() { return m_Position; };
	float GetMass() { return m_Mass; };
	ClothNode* GetConnection(Side _side);
	void SetConnection(Side _side, ClothNode* _node);
private:
	glm::vec3 m_Position;
	glm::vec3 m_PreviousPos;
	glm::vec3 m_Acceleration;

	bool m_Static;
	float m_Mass;
	float m_Damping;
	float m_Stiffness = 0.4;
	float m_RestingDistance = 0.5f;

	ClothNode* m_Top;
	ClothNode* m_Left;
	ClothNode* m_Bottom;
	ClothNode* m_Right;
};

#endif