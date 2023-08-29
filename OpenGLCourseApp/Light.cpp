#include "Light.h"
using namespace LNS;

Light::Light():
colour{ glm::vec3(1.0f,1.0f,1.0f) }, ambientIntensity{ 1.0f }, diffuseIntensity{0.0f}{}

// Overloaded Constructor
Light::Light
(
	GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity
)
	:colour{ glm::vec3(red,green,blue)},ambientIntensity{aIntensity},diffuseIntensity{dIntensity}{}

Light::~Light(){}