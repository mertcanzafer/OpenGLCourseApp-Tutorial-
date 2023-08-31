#include "PointLight.h"
using namespace LNS;

PointLight::PointLight():Light(),position{glm::vec3(0.0f,0.0f,0.0f)},
  constant{1.0f},linear{0.0f},exponent{0.0f}{} // L = 1 / ax^2 + bx + c if a,b and c is equal to zero. 1 / 0!! which is not proper!

PointLight::PointLight
(
    GLfloat red, GLfloat green, GLfloat blue,
    GLfloat aIntensity, GLfloat dIntensity,
    GLfloat xPos, GLfloat yPos, GLfloat zPos,
    GLfloat Constant, GLfloat Linear, GLfloat Exponent
) :Light(red, green, blue, aIntensity, dIntensity)
{
    position = glm::vec3(xPos, yPos, zPos);
    constant = Constant;
    linear = Linear;
    exponent = Exponent;
}

void PointLight::useLight
(
    GLfloat ambientIntensityLoc, GLfloat ambientColorLoc,
    GLfloat diffuseIntensityLoc, GLfloat positionLoc,
    GLfloat constantLoc, GLfloat linearLoc, GLfloat exponentLoc
)
{

    glUniform3f(ambientColorLoc, colour.x, colour.y, colour.z);
    glUniform1f(ambientIntensityLoc, ambientIntensity);
    glUniform1f(diffuseIntensityLoc, diffuseIntensity);

    glUniform3f(positionLoc, position.x, position.y, position.z);
    glUniform1f(constantLoc, constant);
    glUniform1f(linearLoc, linear);
    glUniform1f(exponentLoc, exponent);
}

PointLight::~PointLight(){}