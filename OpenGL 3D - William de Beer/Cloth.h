#ifndef __CLOTH_H__
#define __CLOTH_H__
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
 // Library Includes 
#include <vector>
 // Local Includes 
#include "ClothQuad.h"
#include "ClothNode.h"
#include "Camera.h"
#include "InputHandle.h"

struct GrabInfo {
	int i;
	int j;
	float distance;
	bool grabbing = false;
};

enum class CollisionType {
	CUBE,
	SPHERE,
	CLOTHNODE,
	PYRAMID,
	CAPSULE,
};

class Cloth
{
public:
	Cloth(int _width, int _height);
	~Cloth();
	void Render(CCamera* _camera);
	void Update(float _dT, CCamera* _camera);
	void DropCloth();

	void CalculateCollision(CollisionType _type, glm::vec3 _pos, glm::vec3 _scale);
	void Untangle();
	void ApplyWind(glm::vec3 _windOrigin);
	void TearCloth(int _i, int _j);
	void IgniteCloth(int _i, int _j);

private: 

	float m_Gravity;
	ClothNode** m_Nodes;
	ClothQuad* m_Quad;
	GLuint m_Program;

	glm::vec2 m_Size;
	float m_Spacing;
	int m_Width;
	int m_Height;

	std::vector<ClothNode*> m_vRings;

	GrabInfo grab;

	float PrevMousePosX;
	float PrevMousePosY;
};

#endif 