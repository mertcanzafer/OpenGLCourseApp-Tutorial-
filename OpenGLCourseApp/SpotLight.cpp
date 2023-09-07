#include "SpotLight.h"
using namespace LNS;

SpotLight::SpotLight() :PointLight()
{
	direction = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));	
	edge = 0.0f;
	procEdge = cosf(glm::radians(edge));
}

SpotLight::SpotLight
(
	GLfloat red, GLfloat green, GLfloat blue,
	GLfloat aIntensity, GLfloat dIntensity,
	GLfloat xPos, GLfloat yPos, GLfloat zPos,
	GLfloat xDir, GLfloat yDir, GLfloat zDir,
	GLfloat Constant, GLfloat Linear, GLfloat Exponent,
	GLfloat Edge
):PointLight(red,green,blue,aIntensity,dIntensity,
	xPos,yPos,zPos,Constant,Linear,Exponent)
{
	direction = glm::normalize(glm::vec3(xDir, yDir, zDir));
	edge = Edge;
	procEdge = cosf(glm::radians(edge));
}

void SpotLight::useLight
(
	GLuint ambientIntensityLoc, GLuint ambientColorLoc,
	GLuint diffuseIntensityLoc, GLuint positionLoc, GLuint directionLoc,
	GLuint constantLoc, GLuint linearLoc, GLuint exponentLoc,
	GLuint edgeLoc
)
{
	glUniform3f(ambientColorLoc, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLoc, ambientIntensity);
	glUniform1f(diffuseIntensityLoc, diffuseIntensity);

	glUniform3f(positionLoc, position.x, position.y, position.z);
	glUniform1f(constantLoc, constant);
	glUniform1f(linearLoc, linear);
	glUniform1f(exponentLoc, exponent);

	glUniform3f(directionLoc, direction.x, direction.y, direction.z);
	glUniform1f(edgeLoc, procEdge);
}

void SpotLight::SetFlash
(
	glm::vec3 pos, glm::vec3 dir
)
{
	position = pos;
	direction = dir;
}

SpotLight::~SpotLight(){}