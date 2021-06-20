#ifndef __CLOTH_NODE_H__
#define __CLOTH_NODE_H__

#include "Camera.h"
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
	TR,
	BR,
	TL,
	BL,
};

class ClothNode
{
public:
	ClothNode(glm::vec3 _pos, glm::vec2 _uv);
	~ClothNode();
	void Update(float _dT);

	void ApplyForce(glm::vec3 _force);
	void ApplyConstraint(ClothNode* _other, float _spacing);

	void SetStatic(bool _static) { m_Static = _static; };
	bool GetStatic() { return m_Static; };

	void CatchFire() { m_OnFire = true; };
	void CalculateFire(float _dT);
	bool IsOnFire() { return m_OnFire; };
	float GetFireLevel() { return m_FireLevel; };
	void SetFireLevel(float _fireLevel) { m_FireLevel = _fireLevel; };
	float GetHeatResistance() { return m_fHeatResistance; };

	bool IsEdge();

	void SetPos(glm::vec3 _pos) { m_Position = _pos; };
	glm::vec3 GetPos() { return m_Position; };
	glm::vec2 GetUV() { return m_UV; };
	glm::vec3 GetColor() { return m_Color; };
	float GetMass() { return m_Mass; };
	ClothNode* GetConnection(Side _side);
	void SetConnection(Side _side, ClothNode* _node);

	void ClearConnections();
	void RenderConnectionLines(CCamera* _camera, Side _side);
	bool GetConnectionStatus() { return m_Connected; };
	bool GetToBeDestroyed() { return m_ToBeDestroyed; };
private:
	glm::vec3 m_Position;
	glm::vec3 m_PreviousPos;
	glm::vec3 m_Acceleration;
	glm::vec2 m_UV;
	glm::vec3 m_Color;

	bool m_Static;
	bool m_Connected;
	float m_Mass;
	float m_Damping;
	float m_Stiffness = 1.5f;
	float m_RestingDistance = 1.0f;
	float m_BreakingDistance = 5.0f;
	float m_MaxSpeed = 20.0f;
	float m_fHeatResistance;

	bool m_ToBeDestroyed;

	float m_FireLevel;
	bool m_OnFire;

	ClothNode* m_Top;
	ClothNode* m_Left;
	ClothNode* m_Bottom;
	ClothNode* m_Right;
	ClothNode* m_TR;
	ClothNode* m_BR;
	ClothNode* m_TL;
	ClothNode* m_BL;
};

#endif