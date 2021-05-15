#define STB_IMAGE_IMPLEMENTATION
// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2021 Media Design School 
// 
//  File Name   :   ShaderLoader.cpp
//  Description :   Handles rendering
//  Author      :   William de Beer, Callan Moore 
//  Mail        :   William.Beer@mds.ac.nz
// 
 // Library Includes 
#include <stb_image.h>
#include<iostream>
#include<fstream>
#include<vector>
 // This Include
#include "ShaderLoader.h" 
 // Static Variables 
ShaderLoader* ShaderLoader::sm_Instance = 0;
 // Implementation 
ShaderLoader::ShaderLoader(void) {}
ShaderLoader::~ShaderLoader(void) {}
/***********************
* GetInstance: Returns instance of ShaderLoader or creates one if it does not exist
* @author: William de Beer
********************/
ShaderLoader& ShaderLoader::GetInstance()
{
	if (sm_Instance == 0)
	{
		sm_Instance = new ShaderLoader();
	}
	return *sm_Instance;
}

/***********************
* RemoveInstance: Deletes the Instance
* @author: William de Beer
********************/
void ShaderLoader::RemoveInstance()
{
	if (sm_Instance != 0)
	{
		delete sm_Instance;
		sm_Instance = 0;
	}
}

/***********************
* GetProgram: Returns the program
* @author: William de Beer
* @parameter: File names
* @return: Program
********************/
GLuint ShaderLoader::GetProgram(const char* VertexShaderFilename, const char* FragmentShaderFilename)
{
	// Create key
	std::string key = "<";
	key += VertexShaderFilename;
	key += "><";
	key += FragmentShaderFilename;
	key += ">";

	std::map<std::string, GLuint>::iterator it = m_Map.find(key);

	if (it == m_Map.end())
	{
		return 0;
	}

	return (*it).second;
}

/***********************
* CreateProgram: Creates a program from shader files
* @author: William de Beer
* @parameter: File names
* @return: Returns the program
********************/
GLuint ShaderLoader::CreateProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename)
{
	std::string vString = "Resources/Shaders/";
	vString += vertexShaderFilename;
	std::string fString = "Resources/Shaders/";
	fString += fragmentShaderFilename;

	GLuint RetrievedProgram = GetProgram(vertexShaderFilename, fragmentShaderFilename);

	if (RetrievedProgram != 0)
	{
		return RetrievedProgram;
	}

	// Create the shaders from the filepath
	GLuint vertexSF(CreateShader(GL_VERTEX_SHADER, vString.c_str()));
	GLuint fragmentSF(CreateShader(GL_FRAGMENT_SHADER, fString.c_str()));

	// Create the program handle, attach the shaders and link it
	GLuint program = glCreateProgram();
	// ...
	glAttachShader(program, vertexSF);
	glAttachShader(program, fragmentSF);

	glLinkProgram(program);

	// Check for link errors
	int link_result = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	if (link_result == GL_FALSE)
	{
		std::string programName = vertexShaderFilename + *fragmentShaderFilename;
		PrintErrorDetails(false, program, programName.c_str());
		return 0;
	}


	std::string key = "<";
	key += vertexShaderFilename;
	key += "><";
	key += fragmentShaderFilename;
	key += ">";
	m_Map.insert(std::pair<std::string, GLuint>(key, program));

	return program;
}


/***********************
* CreateVAO: Creates VAO for object
* @author: William de Beer
* @parameter: vertices, indices, array sizes
* @return: VAO
********************/
GLuint ShaderLoader::CreateVAO(GLfloat* _vertices, GLuint* _indices, unsigned int _uiVertArraySize, unsigned int _uiIndiArraySize)
{
	GLuint VAO;
	GLuint EBO;
	GLuint VBO;

	// VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _uiIndiArraySize, _indices, GL_STATIC_DRAW);

	// VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, _uiVertArraySize, _vertices, GL_STATIC_DRAW);

	// Vertex Attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	return VAO;
}

/***********************
* CreateTexture: Creates texture from file
* @author: William de Beer
* @parameter: File name
* @return: Texture
********************/
GLuint ShaderLoader::CreateTexture(const char* _textureFilename)
{
	// Form file path
	std::string vString = "Resources/Textures/";
	vString += _textureFilename;

	// Create texture
	GLuint newTexture = 0;

	int ImageHeight;
	int ImageWidth;
	int ImageComponents;
	unsigned char* ImageData = stbi_load(vString.c_str(),
		&ImageWidth, &ImageHeight, &ImageComponents, 0); 

	if (ImageData)
	{
		glGenTextures(1, &newTexture);
		glBindTexture(GL_TEXTURE_2D, newTexture);

		// Textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		GLint LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, LoadedComponents, ImageWidth, ImageHeight, 0,
			LoadedComponents, GL_UNSIGNED_BYTE, ImageData);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Image not load" << std::endl;
	}
	stbi_image_free(ImageData); 

	return newTexture;
}

/***********************
* CreateShader: Creates a shader
* @author: William de Beer
* @parameter: type of shader and shader name
* @return: Shader ID
********************/
GLuint ShaderLoader::CreateShader(GLenum shaderType, const char* shaderName)
{
	// Read the shader files and save the source code as strings

	std::string string(ReadShaderFile(shaderName));
	const char* cstring = string.c_str();
	const int size = string.size();

	// Create the shader ID and create pointers for source code string and length
	GLuint shaderID = glCreateShader(shaderType);
	// ...

	// Populate the Shader Object (ID) and compile
	glShaderSource(shaderID, 1, &cstring, &size);
	glCompileShader(shaderID);


	// Check for errors
	int compile_result = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compile_result);
	if (compile_result == GL_FALSE)
	{
		PrintErrorDetails(true, shaderID, shaderName);
		return 0;
	}
	return shaderID;
}

/***********************
* ReadShaderFile: Reads the shader file
* @author: William de Beer
* @parameter: File name
* @return: Shadercode
********************/
std::string ShaderLoader::ReadShaderFile(const char* filename)
{
	// Open the file for reading
	std::ifstream file(filename, std::ios::in);
	std::string shaderCode;

	// Ensure the file is open and readable
	if (!file.good()) {
		std::cout << "Cannot read file:  " << filename << std::endl;
		return "";
	}

	// Determine the size of of the file in characters and resize the string variable to accomodate
	file.seekg(0, std::ios::end);
	shaderCode.resize((unsigned int)file.tellg());

	// Set the position of the next character to be read back to the beginning
	file.seekg(0, std::ios::beg);
	// Extract the contents of the file and store in the string variable
	file.read(&shaderCode[0], shaderCode.size());
	file.close();
	return shaderCode;
}

void ShaderLoader::PrintErrorDetails(bool isShader, GLuint id, const char* name)
{
	int infoLogLength = 0;
	// Retrieve the length of characters needed to contain the info log
	(isShader == true) ? glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength) : glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> log(infoLogLength);

	// Retrieve the log info and populate log variable
	(isShader == true) ? glGetShaderInfoLog(id, infoLogLength, NULL, &log[0]) : glGetProgramInfoLog(id, infoLogLength, NULL, &log[0]);
	std::cout << "Error compiling " << ((isShader == true) ? "shader" : "program") << ": " << name << std::endl;
	std::cout << &log[0] << std::endl;
}