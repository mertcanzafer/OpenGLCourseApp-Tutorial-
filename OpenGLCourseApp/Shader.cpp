#include "Shader.h"
using namespace SNS;

Shader::Shader()
	:shaderID{0},uniformModel{0},uniformProjection{0},uniformView{0},
	unformEyePosition{ 0 }, uniformShininess{ 0 }, uniformSpecularIntensity{ 0 },
	pointLightCount{ 0 }, spotLightCount{ 0 } {}

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

void Shader:: CreateFromFiles
(
	const char* vertexLocation, const char* fragmentLocation, const char* geometryLocation
)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	std::string geometryString = ReadFile(geometryLocation);

	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();
	const char* geometryCode = geometryString.c_str();

	CompileShader(vertexCode, fragmentCode, geometryCode);
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

	BindUniformIDs();
}

void Shader::CompileShader
(
	const char* vertexCode, const char* fragmentCode, const char* geometryCode
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
	AddShader(&shaderID, geometryCode, GL_GEOMETRY_SHADER);

	BindUniformIDs();
}

void Shader::BindUniformIDs()
{
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

	uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");
	HandleSpotLights();

	uniformTexture = glGetUniformLocation(shaderID, "theTexture");
	uniformDirectionalLightTransform = glGetUniformLocation(shaderID, "directionalLightTransform");
	uniformDirectionalShadowMap = glGetUniformLocation(shaderID, "directionalShadowMap");

	uniformOmniLightPos = glGetUniformLocation(shaderID, "ligthPos");
	uniformFarPlane = glGetUniformLocation(shaderID, "farPlane");
	HandleCubeMapFaces();
}

void Shader::HandleCubeMapFaces()
{
	for (size_t i = 0; i < NUM_OF_FACES; ++i)
	{
		char locBuff[50] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "ligthMatrices[%d]", i);
		uniformLightMatrices[i] = glGetUniformLocation(shaderID, locBuff);
	}
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

void Shader::HandleSpotLights()
{
	for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
	{
		char locBuff[50] = { '\0' };

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.colour", i);
		uniformSpotLight[i].uniformColour = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i);
		uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i);
		uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i);
		uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i);
		uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i);
		uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", i);
		uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
		uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
		uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID, locBuff);
	}
}

void Shader::SetTexture(GLuint textureUnit)
{
	glUniform1i(uniformTexture, textureUnit);
}

void Shader::SetDirectionalShadowMap(GLuint textureUnit)
{
	glUniform1i(uniformDirectionalShadowMap, textureUnit);
}

void Shader::SetDirectionalLightTransform(glm::mat4* lTransform)
{
	glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*lTransform));
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

void Shader::SetSpotLights
(
	LNS::SpotLight* sLight, unsigned int sLightCount
)
{
	// Check if The number of spot lights is greater than Max spot lights, if it is don't implement it!!!
	// Reset lightcount to Max Spot Light
	if (sLightCount > MAX_SPOT_LIGHTS) sLightCount = MAX_SPOT_LIGHTS;

	glUniform1i(uniformSpotLightCount, sLightCount);

	for (size_t i{ 0 }; i < sLightCount; i++)
	{
		sLight[i].useLight(uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformColour,
			uniformSpotLight[i].uniformDiffuseIntensity, uniformSpotLight[i].uniformPosition, uniformSpotLight[i].uniformDirection,
			uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear,
			uniformSpotLight[i].uniformExponent, uniformSpotLight[i].uniformEdge);
	}
}

void Shader::setLightMatrices(std::vector<glm::mat4> lightMatrices)
{
	for (size_t i = 0; i < NUM_OF_FACES; i++)
	{
		glUniformMatrix4fv(uniformLightMatrices[i], 1, GL_FALSE, glm::value_ptr(lightMatrices[i]));
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
inline GLuint Shader::GetModelLocation() const{ return uniformModel; }
inline GLuint Shader::GetProjectionLocation()const { return uniformProjection; }
inline GLuint Shader::GetViewLocation()const { return uniformView; }
inline GLuint Shader::GetAmbientIntensityLocation()const { return uniformDirectionalLight.uniformAmbientIntensity; }
inline GLuint Shader::GetAmbientColourLocation()const { return uniformDirectionalLight.uniformColour; }
inline GLuint Shader::GetDirectionLocation()const { return uniformDirectionalLight.uniformDirection; }
inline GLuint Shader::GetDiffuseIntensityLocation()const { return uniformDirectionalLight.uniformDiffuseIntensity; }
inline GLuint Shader::GetShininessLocation()const { return uniformShininess; }
inline GLuint Shader::GetSpecularIntensityLocation()const { return uniformSpecularIntensity; }
inline GLuint Shader::GetEyePositionLocation()const { return unformEyePosition; }
inline GLuint Shader::GetFarPlaneLoc()const { return uniformFarPlane; }
inline GLuint Shader::GetOmniLightPos()const { return uniformOmniLightPos; }