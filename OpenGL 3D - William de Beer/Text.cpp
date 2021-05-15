// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   Text.cpp
//  Description :   Text objects
//  Author      :   William de Beer
//  Mail        :   William.Beer@mds.ac.nz
// 
 // This Include 
#include "Text.h"
 // Implementation 
CText::CText(std::string TextStr, std::string Font, glm::vec2 PixelSize, glm::vec2 Position, glm::vec3 Color, glm::vec2 Scale)
{
	SetText(TextStr);
	SetPosition(Position);
	SetColor(Color);
	SetScale(Scale);

	ProjectionMat = glm::ortho(0.0f, 1200.0f, 0.0f, 800.0f);
	Program_Text = ShaderLoader::GetInstance().CreateProgram("Text.vs", "Text.fs");

	FT_Library Library;
	FT_Face Face;

	//Initialise the Font Library
	if (FT_Init_FreeType(&Library) != FT_Err_Ok)
	{
		std::cout << "ERROR::FREETYPE: Could not initialise FreeType Library" << std::endl;
		return;
	}
	if (FT_New_Face(Library, Font.c_str(), 0, &Face) != FT_Err_Ok)
	{
		std::cout << "ERROR::FREETYPE: Failed to Load font - " << Font << std::endl;
		return;
	}

	FT_Set_Pixel_Sizes(Face, (FT_UInt)PixelSize.x, (FT_UInt)PixelSize.y);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load the first 128 (FontCharacterLimit) characters of the ASCII set
	for (GLubyte CharIndex = 0; CharIndex < FontCharacterLimit; CharIndex++)
	{
		// Load the individual character glyph from the face ready to retrieve info from
		if (FT_Load_Char(Face, CharIndex, FT_LOAD_RENDER) != FT_Err_Ok)
		{
			std::cout << "ERROR::FREETYPE: Failed to load Glyph Ascii Index-" << CharIndex << std::endl;
			continue;
		}
		// Store the current character in an array for use in Rendering
		FontCharacters[CharIndex] = {
			GenerateTexture(&Face),
			glm::ivec2(Face->glyph->bitmap.width, Face->glyph->bitmap.rows),
			glm::ivec2(Face->glyph->bitmap_left, Face->glyph->bitmap_top),
			(GLuint)Face->glyph->advance.x >> 6,
		};
	}

	// Release the Freetype objects now they are no longer needed
	FT_Done_Face(Face);
	FT_Done_FreeType(Library);

	// Create an Index array for a quad
	GLuint Indices[] = {
		0, 1, 2,
		0, 2, 3,
	};


	// VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// EBO
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	// VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Using Dynamic_Draw so that the VBO can be edited to better fit each character during Render()
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 4, NULL, GL_DYNAMIC_DRAW);

	// Vertex Attributes
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	InitialisedCorrectly = true;
}

/***********************
* Render: Renders text.
* @author: William de Beer
********************/
void CText::Render()
{
	if (!InitialisedCorrectly)
	{
		return;
	}

	// Setup the shader with variables used with each character
	glUseProgram(Program_Text);
	glUniform3fv(glGetUniformLocation(Program_Text, "TextColor"), 1, glm::value_ptr(Color));
	glUniformMatrix4fv(glGetUniformLocation(Program_Text, "ProjectionMat"), 1, GL_FALSE, glm::value_ptr(ProjectionMat));

	glBindVertexArray(VAO);

	glm::vec2 LocalPosition = Position; // Save a local copy of the Position to edit during this render cycle
	// Iterate through the stirng and render ach character seperately
	for (std::string::const_iterator CharIter = TextStr.begin(); CharIter != TextStr.end(); CharIter++)
	{
		// Pull the relevant info from the FontChar for the current character
		FontCharacter FontChar = FontCharacters[*CharIter];
		GLfloat XPos = LocalPosition.x + (FontChar.Bearing.x * Scale.x);
		GLfloat YPos = LocalPosition.y - (FontChar.Size.y - FontChar.Bearing.y) * Scale.y;
		GLfloat Width = FontChar.Size.x * Scale.x;
		GLfloat Height = FontChar.Size.y * Scale.y;
		// Create a new Vertex array based on the size and local position of the current character
		GLfloat Vertices[4][4] = {
			{XPos, YPos + Height, 0.0f, 0.0f},
			{XPos, YPos, 0.0f, 1.0f},
			{XPos + Width, YPos, 1.0f, 1.0f},
			{XPos + Width, YPos + Height, 1.0f, 0.0f},
		};
		// Update the memory location wtih the new vertices
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertices), Vertices);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, FontChar.TextureID);
		glUniform1i(glGetUniformLocation(Program_Text, "TextTexture"), 0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// Move the current local position along the X axis to be the start point of the next character.
		LocalPosition.x += FontChar.Advance * Scale.x;
	}

	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

/***********************
* GenerateTexture: Creates texture from Face.
* @author: William de Beer
* @parameter: Face
* @return: GLuint Texture
********************/
GLuint CText::GenerateTexture(FT_Face* Face)
{
	GLuint TextureID;
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, (*Face)->glyph->bitmap.width, (*Face)->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, (*Face)->glyph->bitmap.buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return TextureID;
}
