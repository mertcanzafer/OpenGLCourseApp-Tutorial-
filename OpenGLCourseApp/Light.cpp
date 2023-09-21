#include "Light.h"
using namespace LNS;

Light::Light():
	colour{ glm::vec3(1.0f,1.0f,1.0f) }, ambientIntensity{ 1.0f }, diffuseIntensity{ 0.0f }, shadowMap{ nullptr } {}

// Overloaded Constructor
Light::Light
(
	GLfloat shadowWidth, GLfloat shadowHeight,
	GLfloat red, GLfloat green, GLfloat blue, 
	GLfloat aIntensity, GLfloat dIntensity
)
	:colour{ glm::vec3(red,green,blue)},ambientIntensity{aIntensity},diffuseIntensity{dIntensity}
{
	shadowMap = new SMN::ShadowMap();
	shadowMap->Init(shadowWidth, shadowHeight); 
}

Light::Light(const Light& copy)
{
	shadowMap = new SMN::ShadowMap();
	shadowMap = copy.shadowMap;
	colour = copy.colour;
	ambientIntensity = copy.ambientIntensity;
	diffuseIntensity = copy.diffuseIntensity;
	LightProj = copy.LightProj;
}

void LNS::Light::operator=(const Light& copy)
{
	shadowMap = new SMN::ShadowMap();
	shadowMap = copy.shadowMap;
	colour = copy.colour;
	ambientIntensity = copy.ambientIntensity;
	diffuseIntensity = copy.diffuseIntensity;
	LightProj = copy.LightProj;
}

Light::~Light() { delete shadowMap; }