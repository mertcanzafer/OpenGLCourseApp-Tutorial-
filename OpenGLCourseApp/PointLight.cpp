#include "PointLight.h"
using namespace LNS;

PointLight::PointLight():Light(),position{glm::vec3(0.0f,0.0f,0.0f)},
  constant{1.0f},linear{0.0f},exponent{0.0f}{} // L = 1 / ax^2 + bx + c if a,b and c is equal to zero. 1 / 0!! which is not proper!

PointLight::PointLight
(
    GLuint shadowWidth, GLuint shadowHeight,
    GLfloat near, GLfloat far,
    GLfloat red, GLfloat green, GLfloat blue,
    GLfloat aIntensity, GLfloat dIntensity,
    GLfloat xPos, GLfloat yPos, GLfloat zPos,
    GLfloat Constant, GLfloat Linear, GLfloat Exponent
) :Light(shadowWidth,shadowHeight,red, green, blue, aIntensity, dIntensity)
{
    position = glm::vec3(xPos, yPos, zPos);
    constant = Constant;
    linear = Linear;
    exponent = Exponent;
    farPlane = far;

    float aspect = static_cast<float>(shadowWidth) / static_cast<float>(shadowHeight);
    LightProj = glm::perspective(glm::radians(90.0f), aspect, near, farPlane);

    delete shadowMap;

    shadowMap = new SMN::OmniShadowMap();
    shadowMap->Init(shadowWidth, shadowHeight);
}

void PointLight::useLight
(
    GLuint ambientIntensityLoc, GLuint ambientColorLoc,
    GLuint diffuseIntensityLoc, GLuint positionLoc,
    GLuint constantLoc, GLuint linearLoc, GLuint exponentLoc
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

std::vector<glm::mat4> PointLight::calcLightTransform()
{
    std::vector<glm::mat4> lightMatrices;
    // +x -x
    lightMatrices.emplace_back(LightProj * glm::lookAt(position, position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    lightMatrices.emplace_back(LightProj * glm::lookAt(position, position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    // +y -y
    lightMatrices.emplace_back(LightProj * glm::lookAt(position, position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    lightMatrices.emplace_back(LightProj * glm::lookAt(position, position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
    // +z -z
    lightMatrices.emplace_back(LightProj * glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    lightMatrices.emplace_back(LightProj * glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
     
    return lightMatrices;
}

PointLight::~PointLight() {}