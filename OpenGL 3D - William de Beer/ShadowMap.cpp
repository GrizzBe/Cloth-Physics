#include "ShadowMap.h"
#include "Utilities.h"

ShadowMap::ShadowMap(CCamera* _camera, CLightSource* _light)
{
	m_Camera = _camera;
	m_Light = _light;

	// depth texture
	GLuint depthMapTexture;
	glGenTextures(1, &depthMapTexture);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);

	glTexImage2D(GL_TEXTURE_2D,
		0, //mipmap level
		GL_DEPTH_COMPONENT, //internal format
		CUtilities::GetInstance().GetResolution().x, //screen width
		CUtilities::GetInstance().GetResolution().y, //screen height
		0, //border
		GL_DEPTH_COMPONENT, //color format
		GL_FLOAT, //data type
		NULL);

	glGenFramebuffers(1, &m_depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);

	// attach depth texture as FBO's depth buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);

	//disable writes to color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind buffer

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) 
		printf("FB error, status: 0x%x\n", Status);

	Program_ShadowMap = ShaderLoader::GetInstance().CreateProgram("ShadowMap.vs", "ShadowMap.fs");
}

ShadowMap::~ShadowMap()
{
}

void ShadowMap::Start()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowMap::End()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
