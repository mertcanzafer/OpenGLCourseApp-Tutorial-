#include "Light.h"
using namespace LNS;

Light::Light():
colour{ glm::vec3(1.0f,1.0f,1.0f) }, ambientIntensity{ 1.0f }, 
direction{glm::vec3(0.0f,-1.0f,0.0f)}, diffuseIntensity{0.0f}{}

// Overloaded Constructor
Light::Light
(
	GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
	GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity
)
	:colour{ glm::vec3(red,green,blue)},ambientIntensity{aIntensity},
	direction{glm::vec3(xDir,yDir,zDir)},diffuseIntensity{dIntensity}{}

void Light::UseLight
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

Light::~Light(){}