#include "Material.h"

using namespace MNS;

Material::Material() :specularIntensity{ 0.0f }, shininess{0.0f} {}

Material::Material(GLfloat sIntensity, GLfloat shine)
	:specularIntensity{sIntensity},shininess{shine}{}

void Material::useMaterial
(
	GLuint specularIntensityLoc, GLuint shininessLoc
)
{
	glUniform1f(specularIntensityLoc, specularIntensity);
	glUniform1f(shininessLoc, shininess);
}

Material::~Material()
{

}