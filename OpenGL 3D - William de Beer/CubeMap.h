#ifndef __CUBEMAP_H__
#define __CUBEMAP_H__
// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   CubeMap.h
//  Description :   Object that holds the skybox.
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // Library Includes 
#include <vector>
#include <string>
 // Local Includes 
#include "Camera.h"
 // Implementation 
class CCubeMap
{
public:
	CCubeMap(CCamera* _cam, std::vector<std::string> _textures);
	~CCubeMap();

	void Update();
	void Render();
	GLuint GetTextureID();
private:
	glm::mat4 PVM;
	GLuint VAO;
	GLuint Program_Cubemap;
	GLuint TextureID;
	CCamera* m_Cam;
};

#endif