#ifndef __TEXT_H__
#define __TEXT_H__
// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   Text.h
//  Description :   Text object.
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // Dependency Includes
#include <glew.h>
#include <freeglut.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
 // Library Includes
#include <string>
#include <iostream>
 // Local Includes
#include "ShaderLoader.h"
 // Implementation 
class CText
{
public:
	CText(
		std::string TextStr,
		std::string Font,
		glm::vec2 PixelSize,
		glm::vec2 Position,
		glm::vec3 Color = glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec2 Scale = glm::vec2(1.0f, 1.0f));
	~CText() {};

	void Render();
	void SetColor(glm::vec3 Color)			{ this->Color = Color; };
	void SetPosition(glm::vec2 Position)	{ this->Position = Position; };
	void SetScale(glm::vec2 Scale)			{ this->Scale = Scale; };
	void SetText(std::string Text)			{ this->TextStr = Text; };

private:
	struct FontCharacter
	{
		GLuint TextureID = 0;	//Texture ID
		glm::ivec2 Size;		// Size of the glyph
		glm::ivec2 Bearing;		// Offset of the glyph (top left) from the baseline
		GLuint Advance = 0;		// How far to move for the next character
	};

	CText() {};
	GLuint GenerateTexture(FT_Face* Face);

	static const int FontCharacterLimit = 128;
	bool InitialisedCorrectly = false;

	std::string TextStr;
	glm::vec2 Position;
	glm::vec2 Scale;
	glm::vec3 Color;

	glm::mat4 ProjectionMat;
	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint Program_Text = 0;
	FontCharacter FontCharacters[FontCharacterLimit];
};
#endif
