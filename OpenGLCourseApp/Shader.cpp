#include "Shader.h"
using namespace SNS;

Shader::Shader()
	:
	shaderID{0},uniformModel{0},uniformProjection{0},uniformView{0},
	uniformAmbientIntensity{0},uniformAmbientColour{0},uniformDiffuseIntensity{0},uniformDirection{0}{}

// Implement the all methods in shader.h class

std::string Shader::ReadFile(const char* filePath)
{
	std::string content;
	std::fstream file;
	file.open(filePath, std::ios::in);

	if (!file) { std::cout << "No such a file."; return ""; }
    
	std::string line = "";

	while (!file.eof())
	{
		std::getline(file, line);
		content.append(line + "\n");
	}
	file.close();
	return content;
}

void Shader::CreateFromFiles
(
	const char* vertexLocation, const char* fragmentLocation
)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);

	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, fragmentCode);

}

void Shader::CreateFromString
(
	const char* vertexCode, const char* fragmentCode
)
{
	CompileShader(vertexCode,fragmentCode);
}

void Shader::CompileShader
(
	const char* vertexCode, const char* fragmentCode
)
{
	shaderID = glCreateProgram();

	if (!shaderID)
	{
		std::cout << "Error creating shader program" << std::endl;
		return;
	}
	AddShader(&shaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(&shaderID, fragmentCode, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar elog[1024] = { 0 };

	glLinkProgram(shaderID);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(elog), NULL, elog);
		std::cout << "Error linking program  " << elog << std::endl;
		return;
	}
	// Validating our program. Remember That's really important in OpenGL Apps
	glValidateProgram(shaderID);
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);

	if (!result)
	{
		glGetProgramInfoLog(shaderID, sizeof(elog), NULL, elog);
		std::cout << "Error validating program  " << elog << std::endl;
		return;
	}

	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformView = glGetUniformLocation(shaderID, "view");
	uniformAmbientColour = glGetUniformLocation(shaderID, "directionalLight.colour");
	uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.ambientIntensity");
	uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
	uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.diffuseIntensity");
}

// Implement Shaders or Apply them
void Shader::AddShader
(
	GLuint* theProgram, const char* shaderCode, GLenum shaderType
)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1]{};
	theCode[0] = shaderCode;

	GLint codeLength[1]{};
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);

	glCompileShader(theShader);

	GLint result = 0;
	GLchar elog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(elog), NULL, elog);
		std::cout << "Error compiling the " << shaderType << " shader: " << elog << std::endl;
		return;
	}
	glAttachShader(*theProgram, theShader);
}


void Shader::UseShader()
{
	glUseProgram(shaderID);
}

void Shader::ClearShader()
{
	// Just checking possible errors
	// Remember! You should check the IDs which may cause garbage usage from your graphic card!!!!
	// So in ClearShader func. You have to delete the program if ShaderID is not equal to zero.
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	uniformModel = uniformProjection = uniformView = uniformAmbientIntensity = uniformAmbientColour = uniformDiffuseIntensity = uniformDirection = 0;
}

Shader::~Shader()
{
	ClearShader();
}

// Getters
GLuint Shader::GetModelLocation() { return uniformModel; }
GLuint Shader::GetProjectionLocation() { return uniformProjection; }
GLuint Shader::GetViewLocation() { return uniformView; }
GLuint Shader::GetAmbientIntensityLocation() { return uniformAmbientIntensity; }
GLuint Shader::GetAmbientColourLocation() { return uniformAmbientColour; }
GLuint Shader::GetDirectionLocation() { return uniformDirection; }
GLuint Shader::GetDiffuseIntensityLocation() { return uniformDiffuseIntensity; }