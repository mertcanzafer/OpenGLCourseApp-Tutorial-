#include "DirectionalLight.h"
using namespace LNS;

DirectionalLight::DirectionalLight():Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	LightProj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f);
}

DirectionalLight::DirectionalLight
(
	GLfloat shadowWidth, GLfloat shadowHeight,
	GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
	GLfloat dIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir
)
	:Light(shadowWidth,shadowHeight,red, green, blue, aIntensity, dIntensity)
{
	direction = glm::vec3(xDir, yDir, zDir);
	LightProj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 20.0f);
}

void DirectionalLight::UseLight
(
	GLfloat ambientIntensityLoc, GLfloat ambientColorLoc,
	GLfloat diffuseIntensityLoc, GLfloat directionLoc
)
{
	glUniform3f(ambientColorLoc, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLoc, ambientIntensity);

	glUniform3f(directionLoc, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLoc, diffuseIntensity);
}

glm::mat4 DirectionalLight::CalcLightTransform()
{
	//    projection * view
	return LightProj * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

DirectionalLight::DirectionalLight(const DirectionalLight& copy)
	:Light(copy), direction{copy.direction} {}

void DirectionalLight::operator=(const DirectionalLight& copy)
{
	Light::operator=(copy);
	direction = copy.direction;
}

DirectionalLight::~DirectionalLight(){}