#include "DirectionalLight.h"
using namespace LNS;

DirectionalLight::DirectionalLight():Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight
(
	GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
	GLfloat dIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir
)
	:Light(red, green, blue, aIntensity, dIntensity)
{
	direction = glm::vec3(xDir, yDir, zDir);
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


DirectionalLight::~DirectionalLight(){}