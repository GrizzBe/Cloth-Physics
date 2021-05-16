#ifndef __CLOTH_H__
#define __CLOTH_H__

#include "ClothNode.h"
#include "Camera.h"
#include "InputHandle.h"
#include <vector>

class Cloth
{
public:
	Cloth(int _width, int _height);
	~Cloth();
	void Render(CCamera* _camera);
	void Update(float _dT, CCamera* _camera);
	void DropCloth();
private:

	float m_Gravity;
	ClothNode** m_Nodes;
	glm::vec2 m_Size;
	float m_Spacing;
	int m_Width;
	int m_Height;

	std::vector<ClothNode*> m_vRings;

	float PrevMousePosX;
	float PrevMousePosY;
};

#endif 