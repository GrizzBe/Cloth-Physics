#ifndef __CLOTH_H__
#define __CLOTH_H__

#include "ClothQuad.h"
#include "ClothNode.h"
#include "Camera.h"
#include "InputHandle.h"
#include <vector>

struct GrabInfo {
	int i;
	int j;
	float distance;
	bool grabbing = false;
};

class Cloth
{
public:
	Cloth(int _width, int _height);
	~Cloth();
	void Render(CCamera* _camera);
	void Update(float _dT, CCamera* _camera);
	void DropCloth();

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