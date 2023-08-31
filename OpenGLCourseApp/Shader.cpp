#include "Shader.h"
using namespace SNS;

Shader::Shader()
	:shaderID{0},uniformModel{0},uniformProjection{0},uniformView{0},
	unformEyePosition{ 0 }, uniformShininess{ 0 }, uniformSpecularIntensity{ 0 }, pointLightCount{0} {}

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
	uniformDirectionalLight.uniformColour = glGetUniformLocation(shaderID, "directionalLight.base.colour");
	uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
	uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
	uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
	unformEyePosition = glGetUniformLocation(shaderID, "eyePosition");

	uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");
	HandlePointLights();
}

void Shader::HandlePointLights()
{
	for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
	{
		char locBuff[50] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.colour", i);
		uniformPointLight[i].uniformColour = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
		uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
		uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
		uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
		uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);
		
		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
		uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
		uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);
	}
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

void Shader::SetDirectionalLight(LNS::Light* mLight)
{   
	LNS::DirectionalLight *dLight = dynamic_cast<LNS::DirectionalLight*>(mLight);

	dLight->UseLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColour,
		uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void Shader::SetPointLights
(
	LNS::PointLight* pLight, unsigned int LightCount
)
{
	// Check if The number of point lights is greater than Max point lights, if it is don't implement it!!!
	// Reset lightcount to Max Point Light
	if (LightCount > MAX_POINT_LIGHTS) LightCount = MAX_POINT_LIGHTS;

	glUniform1i(uniformPointLightCount, LightCount);

	for (size_t i{ 0 }; i < LightCount; i++)
	{
		pLight[i].useLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColour,
			              uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition,
			              uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear,
			              uniformPointLight[i].uniformExponent);
	}
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

	uniformModel = uniformProjection = uniformView = 0;
	uniformShininess = unformEyePosition = uniformSpecularIntensity = 0;
}

Shader::~Shader()
{
	ClearShader();
}

// Getters
GLuint Shader::GetModelLocation() { return uniformModel; }
GLuint Shader::GetProjectionLocation() { return uniformProjection; }
GLuint Shader::GetViewLocation() { return uniformView; }
GLuint Shader::GetAmbientIntensityLocation() { return uniformDirectionalLight.uniformAmbientIntensity; }
GLuint Shader::GetAmbientColourLocation() { return uniformDirectionalLight.uniformColour; }
GLuint Shader::GetDirectionLocation() { return uniformDirectionalLight.uniformDirection; }
GLuint Shader::GetDiffuseIntensityLocation() { return uniformDirectionalLight.uniformDiffuseIntensity; }
GLuint Shader::GetShininessLocation() { return uniformShininess; }
GLuint Shader::GetSpecularIntensityLocation() { return uniformSpecularIntensity; }
GLuint Shader::GetEyePositionLocation() { return unformEyePosition; }