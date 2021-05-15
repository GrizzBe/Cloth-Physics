#ifndef __SHADOW_MAP_H__
#define __SHADOW_MAP_H__

#include "Camera.h"
#include "LightSource.h"
#include "ShaderLoader.h"

class ShadowMap
{
public:
	ShadowMap(CCamera* _camera, CLightSource* _light);
	virtual ~ShadowMap();

	void Start();
	void End();

	CCamera* GetCamera() { return m_Camera; };
	CLightSource* GetLightSource() { return m_Light; };
	GLuint GetProgram() { return Program_ShadowMap; };
	GLuint GetTexture() { return m_depthMapFBO; };
private:
	GLuint Program_ShadowMap;
	GLuint m_depthMapFBO;

	CCamera* m_Camera;
	CLightSource* m_Light;
};

#endif